#!/usr/bin/env python3
"""
PelengPC Reader v3.0 — Полный реверс протокола УД2-102 (Вагонная версия)

Протокол (из реверса PelengPC.exe + 102_203dll.dll):
  0x55 — Handshake → 16 байт header + каталог адресов (LE16[])
  0x42 — Block Request → addr_lo, addr_hi → 85 байт (или 2 байта если пустой)
  0x9A — Flash Dump → 4293/5253 байт (полный образ памяти)

Формат записи (85 байт от 0x42):
  [0x00-0x01] echo адреса (LE16)
  [0x02-0x03] device_id (LE16)
  [0x04-0x05] typevar (LE16)
  [0x06]      version/flags
  [0x07]      месяц (1-12)
  [0x08]      день (1-31)
  [0x09]      час (0-23)
  [0x0A]      минута (0-59)
  [0x0B]      секунды/reserved
  [0x0C-0x0D] defect_flags (LE16)
  [0x0E-0x0F] year_field (LE16)
  [0x10]      сторона/канал
  [0x11-0x1B] passport_primary (11 байт, LUT-encoded, reverse)
  [0x1C-0x26] reserved
  [0x27-0x29] passport_secondary + typevar2
  [0x2A-0x33] reserved
  [0x34-0x38] measurement values
  [0x39-0x3A] padding (0x0A)
  [0x3B]      probe_angle/velocity
  [0x3C-0x3E] additional params
  [0x3F-0x42] sub-params
  [0x43-0x53] reserved
  [0x54]      checksum/last byte

(c) 2026, reverse-engineered from PelengPC.exe v1.2 / 102_203dll.dll / zapis2.exe
"""

import struct
import sys
import time
from typing import Optional, List, Tuple
from dataclasses import dataclass, field


# ============================================================
#  PASSPORT LUT (из реверса 102_203dll.dll @ FUN_00402708)
# ============================================================

PASSPORT_LUT = (
    '\x00'                              # 0: NUL (terminator/skip)
    '1234567890'                        # 1-10: цифры (1@pos1 .. 9@pos9, 0@pos10)
    ' .-/'                              # 11-14: пробел, точка, тире, слеш
    'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ'  # 15-47: Кириллица А-Я (33)
    'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'  # 48-80: Кириллица а-я (33)
    'ABCDEFGHIJKLMNOPQRSTUVWXYZ'        # 81-106: Латиница A-Z (26)
    '0123456789+'                       # 107-117: доп. цифры + плюс
    '\x00\x00\x00\x00\x00\x00'         # 118-123: reserved
)

# Адресные категории (из LUT 102_203dll.dll @ 0x401670)
CATEGORY_NAMES = {
    1: "Настройка",
    2: "Общие данные",
    3: "Общие данные",
    4: "Протокол",
    5: "Калибровка",
    6: "Протокол",
    10: "Результаты", 11: "Результаты", 12: "Результаты",
    13: "Результаты", 14: "Результаты", 15: "Результаты",
    16: "Результаты", 17: "Результаты", 18: "Результаты", 19: "Результаты",
    20: "A-развёртка", 21: "A-развёртка", 22: "A-развёртка",
    23: "A-развёртка", 24: "A-развёртка", 25: "A-развёртка",
    26: "A-развёртка", 27: "A-развёртка", 28: "A-развёртка", 29: "A-развёртка",
}

# Пустой блок — маркеры (из реверса UD2-102)
EMPTY_MARKERS = (b'\xFD\xFF', b'\xFF\xFF')


# ============================================================
#  ДЕКОДЕРЫ
# ============================================================

def decode_passport(raw: bytes) -> str:
    """Декодирует passport-поле через LUT (обратный порядок байт).
    
    Из реверса FUN_00402708 (102_203dll.dll):
    - Байты обрабатываются в ОБРАТНОМ порядке
    - Каждый байт = индекс в PASSPORT_LUT
    - Ведущие NUL (индекс 0) пропускаются
    - 0x0A = пробел (позиция 10 в таблице = '0', но НЕТ — 0x0A в zapis2 = пробел!)
    """
    result = []
    started = False
    for i in range(len(raw) - 1, -1, -1):
        idx = raw[i]
        if idx == 0 and not started:
            continue  # skip leading NULs
        started = True
        if idx == 0:
            result.append(' ')
        elif idx == 0x0A:
            result.append(' ')  # 0x0A = пробел в zapis2 LUT
        elif 1 <= idx <= 9:
            result.append(str(idx))  # цифры 1-9
        elif idx == 10:
            result.append('0')  # позиция 10 = цифра '0'
        elif idx < len(PASSPORT_LUT):
            ch = PASSPORT_LUT[idx]
            result.append(ch if ch != '\x00' else ' ')
        else:
            result.append('?')
    return ''.join(result)


def category_name(addr: int) -> str:
    """Имя категории по адресу."""
    cat = addr // 1000
    return CATEGORY_NAMES.get(cat, f"Unknown({cat})")


# ============================================================
#  СТРУКТУРЫ ДАННЫХ
# ============================================================

@dataclass
class DeviceInfo:
    """Информация о приборе из handshake header."""
    device_id: int = 0
    version: int = 0
    is_wagon: bool = False
    header_raw: bytes = b""


@dataclass
class MeasurementRecord:
    """Одна запись измерения (декодированная из 85-байтного ответа 0x42)."""
    address: int = 0           # Адрес в памяти прибора
    category: int = 0          # Категория (addr // 1000)
    category_name: str = ""    # Имя категории
    
    # Идентификация
    device_id: int = 0         # ID прибора
    typevar: int = 0           # TypeVar / конфигурация
    
    # Дата и время
    month: int = 0
    day: int = 0
    hour: int = 0
    minute: int = 0
    year_field: int = 0        # Поле года (интерпретация TBD)
    
    # Паспортные данные
    side: int = 0              # Сторона / канал
    passport_primary: str = "" # Номер объекта (LUT-decoded)
    passport_secondary: int = 0
    typevar2: int = 0          # Второй TypeVar (offset 0x28-0x29)
    
    # Измерения
    meas_values: bytes = b""   # Сырые байты измерений (offset 0x34-0x38)
    probe_angle: int = 0       # Угол / скорость
    extra_params: bytes = b""  # Дополнительные параметры
    
    # Defect
    defect_flags: int = 0      # Флаги дефекта
    
    # Raw
    raw: bytes = b""           # Полный сырой ответ
    
    @property
    def date_str(self) -> str:
        """Форматированная дата."""
        if 1 <= self.month <= 12 and 1 <= self.day <= 31:
            return f"{self.day:02d}.{self.month:02d}.{self.year_field}"
        return "??.??.????"
    
    @property
    def time_str(self) -> str:
        """Форматированное время."""
        if self.hour <= 23 and self.minute <= 59:
            return f"{self.hour:02d}:{self.minute:02d}"
        return "??:??"
    
    @property
    def has_defect(self) -> bool:
        return self.defect_flags != 0


def decode_record(addr: int, raw: bytes) -> Optional[MeasurementRecord]:
    """Декодирует 85-байтный ответ блока в MeasurementRecord.
    
    Из реверса (подтверждено реальными данными):
      [0x00-0x01] = echo адреса (LE16)
      [0x02-0x03] = device_id (LE16)
      [0x04-0x05] = typevar (LE16)
      [0x06]      = version/flags
      [0x07]      = месяц
      [0x08]      = день
      [0x09]      = час
      [0x0A]      = минута
      [0x0C-0x0D] = defect_flags (LE16)
      [0x0E-0x0F] = year_field (LE16)
      [0x10]      = сторона/канал
      [0x11-0x1B] = passport_primary (11 байт LUT)
      [0x27]      = passport_secondary
      [0x28-0x29] = typevar2 (LE16)
      [0x34-0x38] = measurement values (5 байт)
      [0x3B]      = probe_angle
      [0x3D-0x42] = extra params
    """
    if len(raw) < 85:
        return None
    
    rec = MeasurementRecord()
    rec.address = addr
    rec.category = addr // 1000
    rec.category_name = category_name(addr)
    rec.raw = raw
    
    # Echo address (для верификации)
    echo = struct.unpack_from('<H', raw, 0)[0]
    if echo != addr:
        # Echo не совпадает — может быть offset или ошибка
        pass
    
    rec.device_id = struct.unpack_from('<H', raw, 0x02)[0]
    rec.typevar = struct.unpack_from('<H', raw, 0x04)[0]
    
    # Дата/время (подтверждено: day=0x06, month=0x07, year=0x08, hour=0x09, min=0x0A)
    rec.day = raw[0x06]
    rec.month = raw[0x07]
    rec.year_field = raw[0x08] + 2000  # year - 2000
    rec.hour = raw[0x09]
    rec.minute = raw[0x0A]
    
    # Defect flags
    rec.defect_flags = struct.unpack_from('<H', raw, 0x0C)[0]
    
    # Сторона
    rec.side = raw[0x10]
    
    # Passport primary (11 байт, LUT-encoded)
    rec.passport_primary = decode_passport(raw[0x11:0x1C])
    
    # Passport secondary + TypeVar2
    rec.passport_secondary = raw[0x27]
    rec.typevar2 = struct.unpack_from('<H', raw, 0x28)[0]
    
    # Measurements
    rec.meas_values = raw[0x34:0x39]
    rec.probe_angle = raw[0x3B]
    rec.extra_params = raw[0x3D:0x43]
    
    return rec


# ============================================================
#  COM-ПОРТ ПРОТОКОЛ
# ============================================================

class PelengCOM:
    """Протокол связи с дефектоскопом УД2-102 (Вагонная версия).
    
    Полный реверс из PelengPC.exe v1.2:
      - 8N1, без flow control
      - 10мс задержка между байтами команды
      - idle-timeout с обновлением deadline на каждом чанке
    """
    
    IDLE_TIMEOUT_MS = 500
    HARD_TIMEOUT_MS = 10000
    INTER_BYTE_DELAY_S = 0.010
    POST_OPEN_DELAY_S = 0.100
    
    def __init__(self, port: str = "COM1", baudrate: int = 19200, debug: bool = False):
        self.port = port
        self.baudrate = baudrate
        self.serial = None
        self.debug = debug
        self.device_info = DeviceInfo()
    
    def _log(self, msg: str):
        if self.debug:
            print(f"[Peleng] {msg}", flush=True)
    
    def connect(self) -> bool:
        """Открывает COM-порт (8N1, no flow control)."""
        try:
            import serial
            self.serial = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                xonxoff=False, rtscts=False, dsrdtr=False,
                timeout=0.05,
                write_timeout=1.0,
            )
            self.serial.reset_input_buffer()
            self.serial.reset_output_buffer()
            time.sleep(self.POST_OPEN_DELAY_S)
            self._log(f"Подключен: {self.port} @ {self.baudrate}")
            return True
        except Exception as e:
            print(f"Ошибка подключения: {e}")
            return False
    
    def disconnect(self):
        if self.serial and self.serial.is_open:
            self.serial.close()
        self.serial = None
    
    def _write_byte(self, val: int):
        if not self.serial:
            raise IOError("Порт не открыт")
        self.serial.write(bytes([val]))
        self.serial.flush()
    
    def _read_bytes(self, max_size: int, timeout_ms: int = None) -> bytes:
        """Чтение с idle-timeout (как в оригинале TCOMPort_ReadBytes)."""
        if not self.serial:
            return b""
        if timeout_ms is None:
            timeout_ms = self.IDLE_TIMEOUT_MS
        
        buf = bytearray()
        idle_deadline = time.monotonic() + (timeout_ms / 1000.0)
        hard_deadline = time.monotonic() + (self.HARD_TIMEOUT_MS / 1000.0)
        
        while len(buf) < max_size:
            now = time.monotonic()
            if now >= hard_deadline:
                break
            if now >= idle_deadline:
                if buf:
                    break
                if now >= hard_deadline:
                    break
                continue
            
            avail = self.serial.in_waiting
            if avail <= 0:
                time.sleep(0.001)
                continue
            
            chunk = self.serial.read(min(avail, max_size - len(buf)))
            if chunk:
                buf.extend(chunk)
                idle_deadline = time.monotonic() + (self.IDLE_TIMEOUT_MS / 1000.0)
        
        return bytes(buf)
    
    # ─── HANDSHAKE ───────────────────────────────────────────
    
    def handshake(self) -> Optional[bytes]:
        """0x55 → header(16) + catalog(LE16[]).
        
        Возвращает полный ответ (header + payload).
        """
        if not self.serial:
            return None
        self.serial.reset_input_buffer()
        time.sleep(0.01)
        self._write_byte(0x55)
        data = self._read_bytes(0x10000)
        if len(data) < 16:
            self._log(f"Handshake: только {len(data)} байт")
            return None
        
        # Parse header
        self.device_info.header_raw = data[:16]
        self.device_info.device_id = struct.unpack_from('<H', data, 0)[0]
        self.device_info.version = data[2]
        self.device_info.is_wagon = bool(data[2] & 0x04)
        
        self._log(f"Handshake OK: {len(data)} байт, device=0x{self.device_info.device_id:04X}")
        return data
    
    def get_catalog(self) -> List[int]:
        """Получает каталог адресов из handshake payload.
        
        Формула из оригинала: numElements = (received - 0x10) / 2 - 1
        Payload = массив LE16 адресов блоков.
        """
        data = self.handshake()
        if data is None or len(data) <= 16:
            return []
        
        payload = data[16:]
        num_words = len(payload) // 2
        
        addrs = []
        for i in range(num_words):
            val = struct.unpack_from('<H', payload, i * 2)[0]
            # Фильтруем: только валидные адреса (1000-29999)
            if 1000 <= val < 30000:
                addrs.append(val)
        
        self._log(f"Каталог: {len(addrs)} валидных адресов из {num_words} слов")
        return addrs
    
    # ─── BLOCK REQUEST ───────────────────────────────────────
    
    def read_block(self, addr: int) -> Optional[bytes]:
        """0x42 + addr_lo + addr_hi → ответ (85 байт или 2 байта если пустой).
        
        Из реверса Protocol_BlockRequest @ 0x00424CC0.
        Оптимизировано: убраны лишние задержки, уменьшен таймаут.
        """
        if not self.serial:
            return None
        
        self.serial.reset_input_buffer()
        self._write_byte(0x42)
        time.sleep(0.005)
        self._write_byte(addr & 0xFF)
        time.sleep(0.005)
        self._write_byte((addr >> 8) & 0xFF)
        time.sleep(0.005)
        
        data = self._read_bytes(256, timeout_ms=500)
        
        if not data:
            return None
        return data
    
    def is_empty_block(self, data: bytes) -> bool:
        """Проверяет является ли ответ пустым блоком."""
        if len(data) == 2 and data in EMPTY_MARKERS:
            return True
        if len(data) >= 2 and data[:2] in EMPTY_MARKERS:
            return True
        return False
    
    # ─── ВЫСОКОУРОВНЕВЫЕ ОПЕРАЦИИ ────────────────────────────
    
    def read_all_records(self, progress_callback=None) -> List[MeasurementRecord]:
        """Читает ВСЕ записи из прибора.
        
        Алгоритм (из реверса PelengPC.exe):
        1. Handshake → каталог адресов
        2. Для каждого адреса из каталога: 0x42 → декодирование
        3. Пропуск пустых блоков (0xFD FF)
        """
        # 1. Получаем каталог
        catalog = self.get_catalog()
        if not catalog:
            print("Каталог пуст или ошибка handshake")
            return []
        
        # Фильтруем — только Results (cat 10-19) и Protocols (cat 4-6)
        target_addrs = [a for a in catalog if 4000 <= a < 30000]
        self._log(f"Целевые адреса: {len(target_addrs)} из {len(catalog)}")
        
        # 2. Запрашиваем каждый блок
        records = []
        empty_count = 0
        
        for i, addr in enumerate(target_addrs):
            if progress_callback:
                progress_callback(i, len(target_addrs), addr)
            
            data = self.read_block(addr)
            
            if data is None:
                self._log(f"  [{i}] addr={addr}: timeout")
                empty_count += 1
                continue
            
            if self.is_empty_block(data):
                empty_count += 1
                continue
            
            # Декодируем запись
            rec = decode_record(addr, data)
            if rec:
                records.append(rec)
                self._log(f"  [{i}] addr={addr}: {rec.date_str} {rec.time_str} "
                         f"passport='{rec.passport_primary}'")
        
        self._log(f"Итого: {len(records)} записей, {empty_count} пустых")
        return records
    
    def test_port(self) -> bool:
        """Быстрый тест — только handshake."""
        if not self.serial:
            return False
        self.serial.reset_input_buffer()
        self._write_byte(0x55)
        data = self._read_bytes(16, timeout_ms=2000)
        if len(data) >= 16:
            self.device_info.header_raw = data[:16]
            self.device_info.device_id = struct.unpack_from('<H', data, 0)[0]
            self.device_info.version = data[2]
            self.device_info.is_wagon = bool(data[2] & 0x04)
            return True
        return False


# ============================================================
#  ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ (для совместимости с peleng_gui.py)
# ============================================================

# Aliases для обратной совместимости
TLVRecord = MeasurementRecord
DecodedReport = MeasurementRecord

def parse_tlv(data: bytes) -> List[MeasurementRecord]:
    """Stub для совместимости — не используется в новом протоколе."""
    return []

def decode_protocol(body: bytes, header: bytes = b"") -> MeasurementRecord:
    """Stub для совместимости."""
    return MeasurementRecord()

def decode_passport_compat(raw: bytes, length: int = None) -> str:
    """Обёртка для совместимости."""
    if length:
        raw = raw[:length]
    return decode_passport(raw)

def render_ascan_text(report, width: int = 80, height: int = 20) -> str:
    """Stub — A-scan не поддерживается в блочном режиме."""
    return "(A-scan недоступен в блочном режиме)"

def print_report(report):
    """Выводит запись."""
    if isinstance(report, MeasurementRecord):
        print(f"  Адрес:    {report.address} ({report.category_name})")
        print(f"  Дата:     {report.date_str} {report.time_str}")
        print(f"  Прибор:   {report.device_id}")
        print(f"  Паспорт:  {report.passport_primary}")
        print(f"  Дефект:   {'Да' if report.has_defect else 'Нет'}")


# Константы для совместимости с peleng_gui.py
SERIES_NAMES = {}
MODE_NAMES = {}
OBJECT_TYPES = {}
SIDE_NAMES = {}
NECK_TYPES = {}


# ============================================================
#  MAIN — CLI
# ============================================================

if __name__ == "__main__":
    print("PelengPC Reader v3.0")
    print("Полный реверс протокола УД2-102")
    print()
    
    if len(sys.argv) < 2:
        print("Использование:")
        print("  python peleng_reader.py COM3 [19200] [--debug]")
        print("  python peleng_reader.py COM3 19200 --catalog   (только каталог)")
        print("  python peleng_reader.py COM3 19200 --all       (все записи)")
        sys.exit(0)
    
    port = sys.argv[1]
    baud = int(sys.argv[2]) if len(sys.argv) > 2 and sys.argv[2].isdigit() else 19200
    debug = "--debug" in sys.argv
    
    com = PelengCOM(port, baudrate=baud, debug=debug)
    if not com.connect():
        print("ОШИБКА: порт не открыт")
        sys.exit(1)
    
    if "--catalog" in sys.argv:
        # Только каталог
        catalog = com.get_catalog()
        print(f"\nКаталог: {len(catalog)} адресов")
        cats = {}
        for a in catalog:
            c = a // 1000
            cats[c] = cats.get(c, 0) + 1
        print("\nРаспределение:")
        for c in sorted(cats):
            print(f"  cat={c:2d} ({category_name(c*1000):15s}): {cats[c]} шт")
        
    elif "--all" in sys.argv:
        # Все записи
        def progress(i, total, addr):
            if i % 10 == 0:
                print(f"  [{i}/{total}] addr={addr}...", flush=True)
        
        records = com.read_all_records(progress_callback=progress)
        print(f"\n{'='*60}")
        print(f"  ПРОЧИТАНО: {len(records)} записей")
        print(f"{'='*60}\n")
        
        for i, rec in enumerate(records[:20]):
            print(f"[{i+1:3d}] addr={rec.address:5d} | {rec.date_str} {rec.time_str} | "
                  f"passport='{rec.passport_primary}' | "
                  f"defect={'Да' if rec.has_defect else 'Нет'}")
        
        if len(records) > 20:
            print(f"  ... и ещё {len(records) - 20}")
    
    else:
        # По умолчанию — тест + каталог
        if com.test_port():
            print(f"✓ Прибор: device=0x{com.device_info.device_id:04X}, "
                  f"wagon={com.device_info.is_wagon}")
            catalog = com.get_catalog()
            print(f"  Каталог: {len(catalog)} адресов")
            results = [a for a in catalog if 10000 <= a < 20000]
            print(f"  Результатов (cat 10-19): {len(results)}")
        else:
            print("✗ Прибор не отвечает")
    
    com.disconnect()
