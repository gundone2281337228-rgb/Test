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
    """Парсит TLV-поток (после 16-byte header)"""
    records = []
    pos = 0
    while pos + 4 <= len(data):
        tag = struct.unpack_from('<H', data, pos)[0]
        if tag == 0xFFFF:
            break
        length = struct.unpack_from('<H', data, pos + 2)[0]
        if length == 0 or pos + 4 + length > len(data):
            pos += 1
            continue
        body = data[pos + 4:pos + 4 + length]
        category = tag // 1000
        records.append(TLVRecord(
            tag=tag, offset=pos, size=length,
            body=body, category=category
        ))
        pos += 4 + length
        # Skip padding 0xFF
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
    """Протокол связи с дефектоскопом УД2-102 через COM-порт"""
    
    def __init__(self, port: str = "COM1", baudrate: int = 9600):
        self.port = port
        self.baudrate = baudrate
        self.serial = None
        self.header = b""
        self.is_wagon = False
    
    def connect(self) -> bool:
        """Открывает COM-порт и настраивает параметры"""
        try:
            import serial
            self.serial = serial.Serial(
                port=self.port,
                baudrate=self.baudrate,
                bytesize=8, parity='N', stopbits=1,
                timeout=1.0,
                write_timeout=0.01
            )
            time.sleep(0.1)
            return True
        except Exception as e:
            print(f"Ошибка подключения: {e}")
            return False
    
    def disconnect(self):
        """Закрывает COM-порт"""
        if self.serial and self.serial.is_open:
            self.serial.close()
    
    def handshake(self) -> Optional[bytes]:
        """Отправляет 0x55, принимает заголовок + данные"""
        if not self.serial:
            return None
        self.serial.write(b'\x55')
        data = self.serial.read(0x80010)
        if len(data) < 17:
            return None
        self.header = data[:16]
        self.is_wagon = bool(data[2] & 4)
        return data
    
    def read_block(self, address: int) -> Optional[bytes]:
        """Запрос блока по адресу (0x42 + addr_lo + addr_hi)"""
        if not self.serial:
            return None
        # Send command
        self.serial.write(b'\x42')
        time.sleep(0.01)
        self.serial.write(bytes([address & 0xFF]))
        time.sleep(0.01)
        self.serial.write(bytes([(address >> 8) & 0xFF]))
        time.sleep(0.01)
        # Read response
        data = self.serial.read(0x10000)
        return data if len(data) > 0 else None
    
    def read_flash(self) -> Optional[bytes]:
        """Чтение FLASH-дампа (0x9A)"""
        if not self.serial:
            return None
        expected_size = 5253 if self.is_wagon else 4293
        self.serial.write(b'\x9A')
        data = self.serial.read(expected_size)
        return data if len(data) >= expected_size - 5 else None


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
    print("PelengPC Reader v1.0")
    print("Сбор данных с дефектоскопа УД2-102")
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
    else:
        print("Использование:")
        print("  python peleng_reader.py --demo-xml     Дешифровка XML протокола")
        print("  python peleng_reader.py --demo-decode  Демо декодирования пакета")
        print("  python peleng_reader.py --port COM3    Подключение к прибору")
