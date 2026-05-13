#!/usr/bin/env python3
"""
PelengPC Reader — Сбор данных с дефектоскопа УД2-102 по COM-порту
Дешифровка протоколов А-развёртки, настроек, SHORTPROT2
Полный реверс из PELENG_REVERSE_ROUND4.md (142 находки)

(c) 2026, reverse-engineered from PelengPC.exe / zapis2.exe / 102_203dll.dll
"""

import struct
import math
import sys
import time
from datetime import datetime
from typing import Optional, Dict, List, Tuple
from dataclasses import dataclass, field

# ============================================================
#  КОНСТАНТЫ
# ============================================================

# Passport LUT (DLL version, 124 chars)
PASSPORT_LUT_DLL = (
    "\x00"
    "1234567890 .-/"
    "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
    "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "0123456789+"
    "\x00\x00\x00\x00\x00\x00"
)

# Passport LUT (zapis2 version, смесь лат/кир)
PASSPORT_LUT_ZAPIS2 = [
    '0','1','2','3','4','5','6','7','8','9',' ',  # 0x00-0x0A
    'A','Б','B','Г','Д','E','Ж','З','И','Й',     # 0x0B-0x14
    'K','Л','M','H','O','П','P','C','Т','У','Ч',  # 0x15-0x1F
    'Ф','Х','Ц','Ш','Щ','Ъ','Ы','Ь','Э','Ю','Я', # 0x20-0x2A
    '+','-','=','Q','W','e','.',                   # 0x2B-0x31
]

SIDE_NAMES = {0: "обе", 1: "левая", 2: "правая"}
NECK_TYPES = {0: "с кольцами", 1: "без колец", 2: "с втулкой", 
              3: "тип 4", 4: "тип 5", 5: "тип 6"}
OBJECT_TYPES = {1: "Толщиномер", 2: "Ось", 3: "Бандаж", 4: "Колёсная пара",
                5: "Обод", 6: "Гребень", 7: "Наплавка", 8: "Категория"}
DEFECT_CODES = {1: "Поперечная трещина", 2: "Коррозия", 3: "Расслоение",
                4: "Дефект шва", 5: "Внутренний дефект", 6: "Подповерхн.",
                7: "Дефект обода", 8: "Дефект гребня", 9: "Дефект шейки",
                10: "Наплавка"}
SERIES_NAMES = {0x0C: "УД2-12", 0x0D: "УД2-13", 0x1E: "УД2-30", 0x1F: "УД2-31",
                0x2A: "УД2-102", 0x2B: "УД2-102+", 0x32: "УД2-50", 0x33: "УД2-51",
                0x3E: "УД2-62", 0x3F: "УД2-63", 0x50: "УД2-80", 0x51: "УД2-81",
                0x52: "УД2-82", 0x53: "УД2-83"}
MODE_NAMES = {0x01: "Базовый", 0x03: "Толщинометрия", 0x04: "Рельс",
              0x06: "Рельс расш.", 0x07: "Колесо", 0x08: "Колесо полн."}


# ============================================================
#  ДЕКОДЕРЫ
# ============================================================

def real48_to_float(data: bytes) -> float:
    """Borland Real48 (6 bytes) → Python float"""
    if len(data) < 6 or data == b'\x00' * 6:
        return 0.0
    exp = data[0]
    if exp == 0:
        return 0.0
    sign = (data[5] >> 7) & 1
    mantissa = int.from_bytes(data[1:5], 'little') | ((data[5] & 0x7F) << 32)
    mantissa = mantissa / (2**39)
    value = (1.0 + mantissa) * (2.0 ** (exp - 129))
    return -value if sign else value


def decode_passport(raw: bytes, length: int = None) -> str:
    """Декодирует passport-поле из сырых байт (DLL LUT, reverse order)"""
    if length is None:
        length = len(raw)
    result = []
    started = False
    for i in range(length - 1, -1, -1):
        if i >= len(raw):
            continue
        b = raw[i]
        if b < len(PASSPORT_LUT_DLL):
            ch = PASSPORT_LUT_DLL[b]
            if ch == '\x00' and not started:
                continue
            result.append(ch)
            started = True
    return ''.join(result)


def decode_date(body: bytes, offset: int) -> str:
    """Дата из 3 байт: [day, month, year-2000]"""
    if offset + 3 > len(body):
        return "??.??.????"
    day, month, year = body[offset], body[offset+1], body[offset+2]
    return f"{day:02d}.{month:02d}.{year + 2000}"


def decode_time(body: bytes, offset: int) -> str:
    """Время из 2 байт: [hours, minutes]"""
    if offset + 2 > len(body):
        return "??:??"
    return f"{body[offset]:02d}:{body[offset+1]:02d}"


def read_le16(body: bytes, offset: int) -> int:
    """Читает LE16 из body"""
    if offset + 2 > len(body):
        return 0
    return struct.unpack_from('<H', body, offset)[0]


def read_byte(body: bytes, offset: int) -> int:
    """Читает 1 байт"""
    if offset >= len(body):
        return 0
    return body[offset]


# ============================================================
#  СТРУКТУРЫ ДАННЫХ
# ============================================================

@dataclass
class TLVRecord:
    """Одна TLV-запись из FLASH/блока"""
    tag: int          # LE16 tag (address)
    offset: int       # смещение в буфере
    size: int         # размер body
    body: bytes       # сырые данные
    category: int = 0 # вычисляемая категория (1, 4-6, 10-19, 20-29)
    
    @property
    def category_name(self) -> str:
        if self.category == 1:
            return "A-scan"
        elif 4 <= self.category <= 6:
            return "Настройка"
        elif 10 <= self.category <= 19:
            return "Протокол (B-scan)"
        elif 20 <= self.category <= 29:
            return "ShortProt"
        else:
            return f"Unknown({self.category})"


@dataclass
class DecodedReport:
    """Дешифрованный отчёт/протокол"""
    record_number: int = 0
    date: str = ""
    time: str = ""
    device_id: int = 0
    device_name: str = ""
    typevar: int = 0
    object_type: str = ""
    object_name: str = ""
    object_number: str = ""   # Номер объекта / ОБОД
    smelting: str = ""        # ПЛАВКА
    factory: str = ""         # ЗАВОД
    side: str = ""            # СТОРОНА
    neck: str = ""            # ШЕЙКА
    machining: int = 0        # ОБТОЧКА
    surfacing: bool = False   # НАПЛАВКА
    defect_flag: bool = False
    defect_text: str = ""
    # Числовые поля
    amplitude_db: float = 0.0
    depth_mm: float = 0.0
    thickness_mm: float = 0.0
    velocity_mps: int = 0
    gate_level: int = 0
    total_length: int = 0
    # Waveform
    waveform_ch1: bytes = b""
    waveform_ch2: bytes = b""
    # Raw
    raw_body: bytes = b""


# ============================================================
#  TLV ПАРСЕР
# ============================================================

def parse_tlv(data: bytes) -> List[TLVRecord]:
    """Парсит TLV-поток (после 16-byte header).
    
    Формат записи (из реверса 102_203dll.dll @ _SortBufData):
      [0..1] = tag (LE16) — адрес/идентификатор записи
      [2..3] = total_length (LE16) — ПОЛНЫЙ размер записи (включая эти 4 байта!)
      [4..total_length-1] = body (тело записи)
    
    Следующая запись начинается по смещению pos + total_length.
    Между записями может быть padding из 0xFF.
    """
    records = []
    pos = 0
    while pos + 4 <= len(data):
        tag = struct.unpack_from('<H', data, pos)[0]
        if tag == 0xFFFF:
            # Пропускаем padding
            while pos < len(data) and data[pos] == 0xFF:
                pos += 1
            continue
        total_length = struct.unpack_from('<H', data, pos + 2)[0]
        # total_length включает 4 байта заголовка (tag + length)
        # Минимальный размер = 4 (пустое тело), проверяем границы
        if total_length < 4 or pos + total_length > len(data):
            # Невалидная запись — пропускаем байт и пробуем дальше
            pos += 1
            continue
        body = data[pos + 4:pos + total_length]
        body_size = total_length - 4
        category = tag // 1000
        records.append(TLVRecord(
            tag=tag, offset=pos, size=body_size,
            body=body, category=category
        ))
        pos += total_length
        # Skip padding 0xFF между записями
        while pos < len(data) and data[pos] == 0xFF:
            pos += 1
    return records


# ============================================================
#  ДЕКОДЕР ПРОТОКОЛА (SHORTPROT2 / RESULTS2)
# ============================================================

def decode_protocol(body: bytes, header: bytes = b"") -> DecodedReport:
    """Декодирует протокол из raw body пакета"""
    report = DecodedReport()
    report.raw_body = body
    
    if len(body) < 0x60:
        return report
    
    # Header fields
    report.device_id = read_le16(body, 0x00)
    report.date = decode_date(body, 0x07)
    report.time = decode_time(body, 0x0A)
    report.defect_flag = read_byte(body, 0x0C) != 0
    report.typevar = read_le16(body, 0x14)
    report.record_number = read_le16(body, 0x15D) if len(body) > 0x15F else 0
    
    # Device name
    series = (report.typevar >> 8) & 0xFF
    mode = report.typevar & 0xFF
    report.device_name = SERIES_NAMES.get(series, f"УД2-{series}")
    
    # Object type
    tv_group = report.typevar // 100 if report.typevar < 1000 else (report.typevar % 1000) // 100
    report.object_type = OBJECT_TYPES.get(tv_group, "—")
    
    # Passport fields
    if len(body) > 0x1B:
        report.smelting = decode_passport(body[0x11:0x1C], 11)
    if len(body) > 0x27:
        report.factory = decode_passport(body[0x21:0x28], 7)
    if len(body) > 0x3B:
        report.object_number = decode_passport(body[0x35:0x3C], 7)
    
    # Side, Neck, Machining
    if len(body) > 0x48:
        report.side = SIDE_NAMES.get(body[0x48], "—")
    if len(body) > 0x4B:
        report.neck = NECK_TYPES.get(body[0x4B], "—")
    if len(body) > 0x190:
        mode_byte = body[0x190]
        if mode_byte & 1:
            report.machining = (mode_byte & 6) >> 1
    
    # Surfacing = defect flag
    report.surfacing = report.defect_flag
    if report.defect_flag:
        report.defect_text = DEFECT_CODES.get(10, "Наплавка")
    
    # Numeric fields
    if len(body) > 0x3A:
        report.depth_mm = read_le16(body, 0x39)
        report.velocity_mps = read_le16(body, 0x37)
        report.amplitude_db = read_le16(body, 0x38)
    if len(body) > 0x5F:
        report.total_length = read_le16(body, 0x5E)
    if len(body) > 0x3D:
        report.gate_level = read_le16(body, 0x3C)
    
    # Thickness from Real48 (if available at offset 0x160)
    if len(body) > 0x165:
        report.thickness_mm = real48_to_float(body[0x160:0x166])
    
    # Waveform (A-scan) — 240 samples × 2 channels
    waveform_start = 0x60  # typical offset
    if len(body) > waveform_start + 0x1E1:
        report.waveform_ch1 = body[waveform_start + 1:waveform_start + 0xF1]
        report.waveform_ch2 = body[waveform_start + 0xF1:waveform_start + 0x1E1]
    
    return report


# ============================================================
#  COM-ПОРТ ПРОТОКОЛ
# ============================================================

class PelengCOM:
    """Протокол связи с дефектоскопом УД2-102 через COM-порт.
    
    Реализация 1:1 по логике оригинального PelengPC.exe (0x004239C8..0x00424CC0):
    
    Открытие порта:
      - CreateFileA("\\\\.\\COMn"), 8N1, no flow control
      - SetCommTimeouts: ReadIntervalTimeout=1, ReadTotalTimeoutConstant=1,
        WriteTotalTimeoutConstant=10
      - Sleep(100) после открытия
    
    Чтение (ReadBytes @ 0x00423F64):
      - Цикл: проверяем BytesAvail → если >0, читаем столько сколько есть
      - Deadline = GetTickCount() + timeout (1000мс)
      - Deadline обновляется на КАЖДОМ успешном чтении (idle-reset)
      - Выход: набрали нужное количество ИЛИ таймаут
    
    Запись (WriteByte @ 0x00423F0C):
      - Пишем ровно 1 байт за вызов (без батчинга)
    
    Задержка (Delay @ 0x00423A56):
      - busy-wait через GetTickCount, 10мс между байтами команды
    """
    
    # Таймаут на чтение (мс) — как в оригинале: self->timeout = 0x3E8 = 1000
    DEFAULT_TIMEOUT_MS = 1000
    # Таймаут idle (мс) — после последнего полученного байта.
    # В оригинале ReadIntervalTimeout=1мс, но реально устройство шлёт пачками
    # с паузами до 500мс между ними. Ставим 1500мс для надёжности.
    IDLE_TIMEOUT_MS = 1500
    # Жёсткий таймаут на всю операцию (мс) — защита от зависания
    HARD_TIMEOUT_MS = 15000
    # Пауза между байтами команды (мс) — из оригинала Sleep(10)
    INTER_BYTE_DELAY_S = 0.010
    # Пауза после открытия порта (мс) — из оригинала Sleep(100)
    POST_OPEN_DELAY_S = 0.100
    # Размеры Flash-дампа
    FLASH_SIZE_WAGON = 0x1485   # 5253 байт (вагонный режим)
    FLASH_SIZE_BASE  = 0x10C5   # 4293 байт (базовый режим)
    
    def __init__(self, port: str = "COM1", baudrate: int = 9600,
                 timeout_ms: int = None, idle_ms: int = None,
                 debug: bool = False):
        self.port = port
        self.baudrate = baudrate
        self.timeout_ms = timeout_ms or self.DEFAULT_TIMEOUT_MS
        self.idle_ms = idle_ms or self.IDLE_TIMEOUT_MS
        self.serial = None
        self.header = b""
        self.is_wagon = False
        self.payload_len = 0
        self.debug = debug
    
    def _log(self, msg: str):
        """Отладочная печать"""
        if self.debug:
            print(f"[PelengCOM] {msg}", flush=True)
    
    def connect(self) -> bool:
        """Открывает COM-порт и настраивает параметры.
        
        Точная копия TCOMPort_Open @ 0x00423D18:
        - 8 бит, 1 стоп, без чётности
        - Все flow control ВЫКЛЮЧЕНЫ (fOutxCtsFlow=0, fRtsControl=0, fDtrControl=0...)
        - ReadIntervalTimeout = 1 (коротенький inter-char timeout)
        - Sleep(100) после открытия
        """
        try:
            import serial
            # Открываем порт с МИНИМАЛЬНЫМ таймаутом на один read-вызов
            # (реальный таймаут управляем сами в цикле чтения)
            self.serial = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                bytesize=serial.EIGHTBITS,     # dataBits=3 → 3+5=8
                parity=serial.PARITY_NONE,      # parity=0
                stopbits=serial.STOPBITS_ONE,   # stopBits=0 → ONESTOPBIT
                # -- Flow Control: ВСЁ ВЫКЛЮЧЕНО (из ApplySettings @ 0x00424020) --
                xonxoff=False,
                rtscts=False,
                dsrdtr=False,
                # -- Таймауты --
                # pyserial timeout = как быстро один read() вернёт данные если нет байт
                # Ставим маленький чтобы наш цикл мог быстро проверять in_waiting
                timeout=0.05,           # 50мс — как ReadIntervalTimeout=1 + запас
                write_timeout=1.0,      # WriteTotalTimeoutConstant=10мс в оригинале, 1с запас
            )
            
            # Сбрасываем буферы (PurgeComm аналог)
            self.serial.reset_input_buffer()
            self.serial.reset_output_buffer()
            
            # Sleep(100) после открытия — из оригинала
            time.sleep(self.POST_OPEN_DELAY_S)
            
            self._log(f"Подключен: {self.port} @ {self.baudrate} бод")
            return True
            
        except Exception as e:
            print(f"Ошибка подключения к {self.port}: {e}")
            return False
    
    def disconnect(self):
        """Закрывает COM-порт (TCOMPort_Close @ 0x00423E28)"""
        if self.serial and self.serial.is_open:
            self.serial.close()
            self._log("Порт закрыт")
        self.serial = None
    
    def _write_byte(self, byte_val: int):
        """Запись одного байта (TCOMPort_WriteByte @ 0x00423F0C).
        
        Оригинал пишет строго по 1 байту с проверкой таймаута.
        """
        if not self.serial or not self.serial.is_open:
            raise IOError("Порт не открыт")
        self.serial.write(bytes([byte_val]))
        self.serial.flush()
        self._log(f"TX: 0x{byte_val:02X}")
    
    def _read_bytes(self, max_size: int, timeout_ms: int = None) -> bytes:
        """Чтение данных с idle-таймаутом (TCOMPort_ReadBytes @ 0x00423F64).
        
        Точная логика оригинала:
        1. deadline = GetTickCount() + timeout
        2. Цикл:
           a. avail = BytesAvail()  [ClearCommError → cbInQue]
           b. Если avail == 0 → проверяем deadline, если истёк — выходим
           c. Если avail > 0 → читаем min(avail, remaining)
           d. Обновляем deadline (!) — ключевая деталь: таймаут СБРАСЫВАЕТСЯ
        3. Возвращаем всё что прочитали
        
        Это значит: таймаут — это время БЕЗДЕЙСТВИЯ (idle), а не общее время.
        Если данные идут — ждём бесконечно (до hard_timeout).
        """
        if not self.serial or not self.serial.is_open:
            return b""
        
        if timeout_ms is None:
            timeout_ms = self.timeout_ms
        
        buf = bytearray()
        remaining = max_size
        
        idle_deadline = time.monotonic() + (timeout_ms / 1000.0)
        hard_deadline = time.monotonic() + (self.HARD_TIMEOUT_MS / 1000.0)
        
        while remaining > 0:
            now = time.monotonic()
            
            # Hard timeout — защита от бесконечного ожидания
            if now >= hard_deadline:
                self._log(f"RX: HARD timeout, получено {len(buf)}/{max_size}")
                break
            
            # Idle timeout — нет данных слишком долго
            if now >= idle_deadline:
                if len(buf) > 0:
                    self._log(f"RX: idle timeout, получено {len(buf)}/{max_size}")
                    break
                # Если ещё ничего не получили — ждём до hard_deadline
                if now >= hard_deadline:
                    self._log(f"RX: timeout без данных")
                    break
                continue
            
            # Проверяем BytesAvail (аналог ClearCommError + cbInQue)
            avail = self.serial.in_waiting
            
            if avail <= 0:
                # Нет данных — маленькая пауза чтобы не жечь CPU
                # (в оригинале busy-wait, но мы не варвары)
                time.sleep(0.001)
                continue
            
            # Читаем столько сколько есть, но не больше чем нужно
            to_read = min(avail, remaining)
            chunk = self.serial.read(to_read)
            
            if chunk:
                buf.extend(chunk)
                remaining -= len(chunk)
                # *** КЛЮЧЕВОЙ МОМЕНТ: обновляем idle deadline ***
                # Это то, что делает оригинал: deadline = GetTickCount() + timeout
                idle_deadline = time.monotonic() + (self.idle_ms / 1000.0)
                self._log(f"RX: +{len(chunk)} байт (всего {len(buf)}/{max_size})")
        
        return bytes(buf)
    
    def handshake(self) -> Optional[bytes]:
        """Отправляет 0x55, принимает заголовок + данные.
        
        Точная логика Protocol_Handshake @ 0x00424A50:
        1. WriteByte(0x55)
        2. ReadBytes(rxBuf, 0x80010) — читает ВСЁ что прибор пришлёт
        3. Если received < 17 — ошибка
        4. Копируем 16-байт заголовок
        5. Извлекаем флаги из байта 2
        6. payload_len из байтов 6-7 (LE16)
        
        ВАЖНО: оригинал передаёт размер буфера 0x80010 (512K+16), но
        реально читает столько сколько пришлёт прибор (idle timeout).
        Прибор отвечает: 16 байт заголовок + payload данных.
        """
        if not self.serial:
            return None
        
        # Сбрасываем входной буфер перед handshake
        self.serial.reset_input_buffer()
        time.sleep(0.01)
        
        # Отправляем команду 0x55
        self._write_byte(0x55)
        
        # Читаем ответ — ждём минимум 16 байт заголовка, но прибор может
        # прислать больше (payload). Используем большой max_size и idle timeout.
        # Оригинал ставит буфер 0x80010 но это просто "бесконечный" буфер.
        data = self._read_bytes(0x10000, timeout_ms=self.timeout_ms)
        
        if len(data) < 16:
            self._log(f"Handshake FAILED: получено {len(data)} байт (нужно >= 16)")
            print(f"Ошибка handshake: получено {len(data)} байт, ожидалось >= 16")
            return None
        
        # Сохраняем заголовок
        self.header = data[:16]
        
        # Флаги из байта 2 (is_wagon = бит 2)
        self.is_wagon = bool(data[2] & 0x04)
        
        # payload_len из байтов 6-7 (LE16)
        self.payload_len = struct.unpack_from('<H', data, 6)[0]
        
        self._log(f"Handshake OK: {len(data)} байт, "
                  f"wagon={self.is_wagon}, payload_len={self.payload_len}")
        self._log(f"Header: {data[:16].hex(' ')}")
        
        return data
    
    def read_block(self, address: int, expected_size: int = 0x10000) -> Optional[bytes]:
        """Запрос блока по адресу (Protocol_BlockRequest @ 0x00424CC0).
        
        Точная логика оригинала:
        1. WriteByte(0x42)
        2. Delay(10мс)
        3. WriteByte(address & 0xFF)     — младший байт адреса
        4. Delay(10мс)
        5. WriteByte((address >> 8) & 0xFF) — старший байт адреса
        6. Delay(10мс)
        7. ReadBytes(outBuf, expectedSize)
        8. Delay(10мс) после приёма
        
        Args:
            address: 16-битный адрес блока в памяти прибора
            expected_size: ожидаемый размер ответа (байт)
        """
        if not self.serial:
            return None
        
        # Сбрасываем входной буфер
        self.serial.reset_input_buffer()
        
        # 1. Отправляем команду 0x42
        self._write_byte(0x42)
        time.sleep(self.INTER_BYTE_DELAY_S)
        
        # 2. Младший байт адреса
        self._write_byte(address & 0xFF)
        time.sleep(self.INTER_BYTE_DELAY_S)
        
        # 3. Старший байт адреса
        self._write_byte((address >> 8) & 0xFF)
        time.sleep(self.INTER_BYTE_DELAY_S)
        
        # 4. Читаем ответ
        data = self._read_bytes(expected_size, timeout_ms=self.timeout_ms)
        
        # 5. Задержка после приёма (из оригинала)
        time.sleep(self.INTER_BYTE_DELAY_S)
        
        if len(data) == 0:
            self._log(f"read_block(0x{address:04X}): нет ответа")
            return None
        
        self._log(f"read_block(0x{address:04X}): получено {len(data)} байт")
        return data
    
    def read_flash(self) -> Optional[bytes]:
        """Чтение FLASH-дампа (Protocol_FlashDump, опкод 0x9A @ 0x00424BB0).
        
        Точная логика оригинала:
        1. WriteByte(0x9A)
        2. Приём expectedSize байт через FormProgress
           - expectedSize = 0x1485 (5253) для вагонного режима
           - expectedSize = 0x10C5 (4293) для базового режима
        
        ВАЖНО: is_wagon определяется из handshake (байт 2, бит 2).
        Нужно сначала вызвать handshake()!
        """
        if not self.serial:
            return None
        
        expected_size = self.FLASH_SIZE_WAGON if self.is_wagon else self.FLASH_SIZE_BASE
        self._log(f"read_flash: отправляем 0x9A, ожидаем {expected_size} байт "
                  f"({'wagon' if self.is_wagon else 'base'})")
        
        # Сбрасываем входной буфер
        self.serial.reset_input_buffer()
        
        # Отправляем команду
        self._write_byte(0x9A)
        
        # Читаем ответ
        data = self._read_bytes(expected_size, timeout_ms=self.timeout_ms)
        
        if len(data) < expected_size:
            self._log(f"read_flash: НЕПОЛНЫЙ ответ {len(data)}/{expected_size}")
            # Возвращаем что есть, если получили хотя бы заголовок
            if len(data) >= 16:
                print(f"ПРЕДУПРЕЖДЕНИЕ: Flash неполный ({len(data)}/{expected_size}), "
                      f"но данные есть — возвращаем частичные")
                return data
            print(f"Ошибка чтения Flash: получено {len(data)}/{expected_size} байт")
            return None
        
        self._log(f"read_flash: OK, {len(data)} байт")
        return data
    
    def read_all_data(self) -> Optional[bytes]:
        """Полное чтение данных из прибора (handshake + block request).
        
        Это комбинированная операция, аналог того что делает PelengPC
        при нажатии "Читать данные":
        1. Handshake (0x55) → получаем заголовок с payload_len
        2. Block Request (0x42 + len_lo + len_hi) → получаем полный блок
        
        Из реверса peleng_clone: после handshake прибор знает сколько данных
        отдать, и по команде 'B' + size_lo + size_hi отдаёт exactly столько.
        """
        # 1. Handshake
        hdr_data = self.handshake()
        if hdr_data is None:
            return None
        
        if self.payload_len == 0:
            print("Прибор вернул payload_len=0 — нет данных для чтения")
            return hdr_data  # только заголовок
        
        # 2. Запрос блока данных
        # total = заголовок (16) + payload
        total = 16 + self.payload_len
        self._log(f"read_all_data: запрашиваем блок, total={total}")
        
        block = self.read_block(address=total, expected_size=total)
        if block is None:
            # Попробуем альтернативу — некоторые приборы ожидают payload_len как адрес
            self._log("Повторная попытка с payload_len как адрес...")
            block = self.read_block(address=self.payload_len, expected_size=total)
        
        return block
    
    def test_port(self) -> bool:
        """Быстрый тест порта — handshake без чтения данных.
        
        Аналог NTestPortClick в оригинале.
        Возвращает True если прибор ответил корректным заголовком.
        """
        if not self.serial:
            return False
        
        self.serial.reset_input_buffer()
        self._write_byte(0x55)
        
        # Ждём только 16 байт заголовка
        data = self._read_bytes(16, timeout_ms=2000)
        
        if len(data) >= 16:
            self.header = data[:16]
            self.is_wagon = bool(data[2] & 0x04)
            self.payload_len = struct.unpack_from('<H', data, 6)[0]
            self._log(f"test_port OK: header={data[:16].hex(' ')}")
            return True
        
        self._log(f"test_port FAIL: {len(data)} байт")
        return False


# ============================================================
#  A-SCAN RENDERER
# ============================================================

def render_ascan_text(report: DecodedReport, width: int = 80, height: int = 20) -> str:
    """Рисует A-scan как текстовый ASCII-арт"""
    if not report.waveform_ch1:
        return "(нет данных A-scan)"
    
    waveform = report.waveform_ch1
    samples = len(waveform)
    if samples == 0:
        return "(пустая развёртка)"
    
    # Scale to text size
    x_scale = samples / width
    y_scale = 255.0 / height
    
    canvas = [[' '] * width for _ in range(height)]
    
    for col in range(width):
        sample_idx = int(col * x_scale)
        if sample_idx >= samples:
            break
        amplitude = waveform[sample_idx]
        row = height - 1 - int(amplitude / y_scale)
        row = max(0, min(height - 1, row))
        canvas[row][col] = '█'
    
    lines = [''.join(row) for row in canvas]
    lines.insert(0, '┌' + '─' * width + '┐ A-scan')
    lines.append('└' + '─' * width + '┘')
    return '\n'.join(lines)


def render_ascan_matplotlib(report: DecodedReport, output_path: str = None):
    """Рисует A-scan через matplotlib (если доступен)"""
    try:
        import matplotlib.pyplot as plt
        import numpy as np
    except ImportError:
        print("matplotlib не установлен, используйте render_ascan_text()")
        return
    
    fig, ax = plt.subplots(1, 1, figsize=(7, 3.5))
    
    if report.waveform_ch1:
        ch1 = np.frombuffer(report.waveform_ch1, dtype=np.uint8)
        ax.plot(ch1, 'b-', linewidth=0.8, label='Канал 1')
    
    if report.waveform_ch2:
        ch2 = np.frombuffer(report.waveform_ch2, dtype=np.uint8)
        ax.plot(ch2, 'r-', linewidth=0.5, alpha=0.5, label='Канал 2')
    
    # Gate lines
    if report.gate_level > 0:
        ax.axhline(y=report.gate_level, color='g', linestyle='--', 
                   linewidth=1, label=f'Строб {report.gate_level}%')
    
    ax.set_xlim(0, 240)
    ax.set_ylim(0, 255)
    ax.set_xlabel('Отсчёты (время)')
    ax.set_ylabel('Амплитуда')
    ax.set_title(f'А-развёртка — Протокол #{report.record_number} '
                 f'({report.date} {report.time})')
    ax.legend(loc='upper right', fontsize=8)
    ax.grid(True, alpha=0.3)
    
    if output_path:
        plt.savefig(output_path, dpi=150, bbox_inches='tight')
        print(f"Сохранено: {output_path}")
    else:
        plt.show()
    plt.close()


# ============================================================
#  MAIN — CLI DEMO
# ============================================================

def print_report(report: DecodedReport):
    """Выводит дешифрованный отчёт"""
    print("=" * 70)
    print(f"  ПРОТОКОЛ № {report.record_number}")
    print(f"  {report.device_name} № {report.device_id}, "
          f"{report.date} {report.time}")
    print("=" * 70)
    print(f"  Объект: тип         {report.object_type}")
    print(f"  Номер объекта       {report.object_number}")
    print(f"  Плавка              {report.smelting}")
    print(f"  Завод (ЗД)          {report.factory}")
    print(f"  Год                 —")
    print(f"  Сторона             {report.side}")
    print(f"  Шейка               {report.neck}")
    print(f"  Обод                {report.object_number}")
    print(f"  Обточка             {report.machining}")
    print(f"  Наплавка            {'Да' if report.surfacing else 'Нет'}")
    print(f"  Дефект              {'Да: ' + report.defect_text if report.defect_flag else 'Нет'}")
    print("-" * 70)
    print(f"  Глубина Y, мм       {report.depth_mm}")
    print(f"  Скорость, м/с       {report.velocity_mps}")
    print(f"  Амплитуда, дБ       {report.amplitude_db}")
    print(f"  Толщина, мм         {report.thickness_mm:.2f}")
    print(f"  Длина объекта, мм   {report.total_length}")
    print(f"  TypeVar             {report.typevar} (0x{report.typevar:04X})")
    print("=" * 70)


def demo_decode_xml():
    """Демо: декодирование протокола из XML файла"""
    import re
    
    try:
        with open('123213.xml', 'rb') as f:
            data = f.read()
        text = data.decode('windows-1251')
        fields = re.findall(r'<w:t>(.*?)</w:t>', text)
        
        print("\n=== ДЕШИФРОВКА ПРОТОКОЛА ИЗ XML ===\n")
        for i, field in enumerate(fields):
            field = field.strip()
            if field:
                print(f"  [{i:2d}] {field}")
        
        # Extract and decode base64 PNG
        b64_match = re.search(r'<w:binData[^>]*>(.*?)</w:binData>', text, re.DOTALL)
        if b64_match:
            import base64
            png_data = base64.b64decode(b64_match.group(1).strip())
            print(f"\n  [PNG] A-scan image: {len(png_data)} bytes")
            
    except FileNotFoundError:
        print("Файл 123213.xml не найден")


if __name__ == "__main__":
    print("PelengPC Reader v2.0")
    print("Сбор данных с дефектоскопа УД2-102")
    print("(Реверс-инженерия PelengPC.exe → Python)")
    print()
    
    if len(sys.argv) > 1 and sys.argv[1] == "--demo-xml":
        demo_decode_xml()
    elif len(sys.argv) > 1 and sys.argv[1] == "--demo-decode":
        # Demo: decode a sample body (all zeros = empty protocol)
        sample_body = bytearray(0x200)
        # Set some test values
        struct.pack_into('<H', sample_body, 0x00, 1182)  # devId
        sample_body[0x07] = 10; sample_body[0x08] = 6; sample_body[0x09] = 16  # date
        sample_body[0x0A] = 14; sample_body[0x0B] = 13  # time
        struct.pack_into('<H', sample_body, 0x14, 731)  # TypeVar
        struct.pack_into('<H', sample_body, 0x15D, 11)  # record number
        sample_body[0x48] = 0  # side = обе
        sample_body[0x4B] = 0  # neck = с кольцами
        
        report = decode_protocol(bytes(sample_body))
        print_report(report)
    elif len(sys.argv) > 1 and sys.argv[1] == "--port":
        # Подключение к реальному прибору
        port_name = sys.argv[2] if len(sys.argv) > 2 else "COM3"
        baud = int(sys.argv[3]) if len(sys.argv) > 3 else 9600
        
        print(f"Подключение к {port_name} @ {baud} бод...")
        print(f"(Используйте --debug для подробного лога)")
        debug = "--debug" in sys.argv
        
        com = PelengCOM(port_name, baudrate=baud, debug=debug)
        if not com.connect():
            print("ОШИБКА: не удалось открыть порт")
            sys.exit(1)
        
        print("Порт открыт. Тестируем подключение (handshake 0x55)...")
        if com.test_port():
            print(f"✓ Прибор ответил!")
            print(f"  Header:      {com.header.hex(' ')}")
            print(f"  Payload len: {com.payload_len}")
            print(f"  Режим:       {'Вагонный' if com.is_wagon else 'Базовый'}")
            print()
            
            # Пробуем прочитать данные
            print("Читаем данные (read_all_data)...")
            data = com.read_all_data()
            if data and len(data) > 16:
                print(f"✓ Получено {len(data)} байт")
                records = parse_tlv(data[16:])
                print(f"  Найдено TLV-записей: {len(records)}")
                for i, rec in enumerate(records[:10]):
                    print(f"  [{i}] tag={rec.tag}, size={rec.size}, "
                          f"cat={rec.category_name}")
                if len(records) > 10:
                    print(f"  ... и ещё {len(records) - 10} записей")
            else:
                print("Нет данных через read_all_data, пробуем Flash (0x9A)...")
                flash_data = com.read_flash()
                if flash_data:
                    print(f"✓ Flash: {len(flash_data)} байт")
                    if len(flash_data) > 16:
                        records = parse_tlv(flash_data[16:])
                        print(f"  Найдено TLV-записей: {len(records)}")
                else:
                    print("✗ Flash тоже не удалось прочитать")
        else:
            print("✗ Прибор не ответил на handshake")
            print("  Проверьте:")
            print("  1. Кабель подключён?")
            print("  2. Прибор включён?")
            print("  3. Правильный COM-порт?")
            print("  4. Скорость (попробуйте 19200, 9600, 4800)")
        
        com.disconnect()
    else:
        print("Использование:")
        print("  python peleng_reader.py --port COM3 [baud] [--debug]")
        print("                                     Подключение к прибору")
        print("  python peleng_reader.py --demo-xml     Дешифровка XML протокола")
        print("  python peleng_reader.py --demo-decode  Демо декодирования пакета")
        print()
        print("Примеры:")
        print("  python peleng_reader.py --port COM3")
        print("  python peleng_reader.py --port COM3 19200")
        print("  python peleng_reader.py --port COM3 9600 --debug")
        print("  python peleng_reader.py --port /dev/ttyUSB0 19200 --debug")
