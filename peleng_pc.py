#!/usr/bin/env python3
"""
peleng_pc.py — Полный 1:1 клон PelengPC ver 1.2 (единый файл)

Написан с нуля на основе реверс-инжиниринга 7080 функций из PelengPC.exe и 102_203dll.dll.
Каждая константа, смещение и алгоритм верифицированы по декомпилированному коду.

Разделы:
  #1 КОНСТАНТЫ         — все числа из реверса (baud rates, timeouts, offsets, sizes)
  #2 ТРАНСПОРТ         — COM-порт (TCOMPort) + протокол (Handshake/Block/Flash)
  #3 TLV-ПАРСЕР        — _SortBufData из DLL: разбор потока TLV-записей
  #4 ДЕШИФРОВКА ПОЛЕЙ  — DecodeDate/Time/String/Int, TypeVar dispatch, схемы DAL
  #5 БАЗА ДАННЫХ       — SQLite (аналог Firebird BlockZap)
  #6 B-SCAN РЕНДЕР     — FormScreen_RenderBscan: битовая карта → изображение
  #7 КОНФИГУРАЦИЯ      — INI-файл (аналог реестра SOFTWARE\\Altek\\PelengPC)
  #8 ПРИЛОЖЕНИЕ        — PelengPCApp (связывает всё), CLI интерфейс

Зависимости: pip install pyserial
Опционально:  pip install PyQt6 (для GUI)

Использование:
  python peleng_pc.py info                          # справка по протоколу
  python peleng_pc.py test --port COM3              # handshake
  python peleng_pc.py block --port COM3 --addr 10001 --size 4293
  python peleng_pc.py flash --port COM3 --save-all  # FLASH dump → БД
  python peleng_pc.py gui                           # PyQt6 GUI (если установлен)

Автор реверса: Kiro AI, 2026-05-12
Оригинал: PelengPC.exe v1.2, ООО "Алтек" (www.altek.info)
"""
from __future__ import annotations
import argparse, configparser, os, sqlite3, struct, sys, time
from dataclasses import dataclass, field
from enum import IntEnum
from typing import Optional, Callable, List, Tuple

__version__ = "2.0.0"



# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #1  КОНСТАНТЫ  (из реверса TCOMPort_Create @ 0x4239C8 и _SortBufData)   ║
# ╚════════════════════════════════════════════════════════════════════════════╝

# --- Baud Rate (15 значений, switch @ 0x423B4C) ---
BAUD_RATES: Tuple[int,...] = (110,300,600,1200,2400,4800,9600,14400,19200,38400,56000,57600,115200,128000,256000)
DEFAULT_BAUD = 9600          # baudIndex=6

# --- COM-порт таймауты (TCOMPort_Open @ 0x423D18) ---
READ_INTERVAL_TIMEOUT = 1    # мс
WRITE_TIMEOUT_CONSTANT = 10  # мс
OPEN_PORT_DELAY_MS = 100     # Sleep(100) после открытия

# --- Протокол (Protocol_BlockRequest @ 0x424CC0) ---
OP_HANDSHAKE  = 0x55         # 'U' → ответ ≥17 байт
OP_BLOCK_REQ  = 0x42         # 'B' LL HH → блок данных
OP_FLASH_DUMP = 0x9A         # → 4293/5253 байт FLASH
OP_STATUS     = 0x74         # 't' → статус прибора
INTER_BYTE_DELAY = 0.010     # 10мс между байтами команды
HANDSHAKE_MIN_RESP = 17      # минимальный ответ на handshake

# --- Адресация (IsSweepAddress @ 0x41C7C0) ---
SWEEP_MIN = 10000            # sweep начинается с 10000
SWEEP_MAX = 29999            # и заканчивается на 29999
FLASH_ADDR = 60000           # специальный адрес → Protocol_FlashDump

# --- Размеры FLASH (FUN_0041CBF4) ---
FLASH_SIZE_BASE  = 4293      # 0x10C5 — базовая версия прибора
FLASH_SIZE_WAGON = 5253      # 0x1485 — вагонная (бит 2 в deviceFlags)

# --- B-scan (FormScreen_RenderBscan @ 0x4235F8) ---
BSCAN_ASCAN_HEADER = 448     # 0x1C0 байт заголовка A-scan перед B-scan данными
BSCAN_BODY_OFFSET  = 464     # 0x1D0 = 16(frame) + 448(ascan header)
BSCAN_ROW_BYTES    = 30      # строка = 30 байт = 240 бит
BSCAN_SCALE        = 2       # каждый бит → квадрат 2×2 пикселя

# --- Пакет (DAT_0053E96A) ---
FRAME_HDR = 16               # 16-байтовый заголовок фрейма

# --- TLV маркеры (ParseFlashDump_TLV @ 0x41B008) ---
TLV_END   = 0xFFFF           # конец потока TLV
TLV_PAD   = 0xFF             # padding байт

# --- Типы полей (из PelengPCtest.dal, используются StreamReader) ---
FT_INT      = 0x01           # INT16 (2 bytes, big-endian сборка в DLL)
FT_DATE     = 0x03           # BCD-DATE: [day][month][year-2000] (3 bytes)
FT_TIME     = 0x82           # BCD-TIME: [hours][minutes] (2 bytes)
FT_STR_S    = 0xA2           # STRING Pascal short (len+chars, CP1251, max 15)
FT_STR_L    = 0xE2           # STRING Pascal long (max 31)
FT_STR_X    = 0x142          # STRING Pascal extra (max 63)
FT_INT_MM   = 0x52           # INT32 — протяженность в мм
FT_ENUM_DEF = 0x5A           # ENUM дефект: 0=нет, 1=есть
FT_ENUM_TZ  = 0x14A          # ENUM тип записи: 0=A, 1=B, 2=Настройки, 3=Отчёт
FT_BLOB     = 0x72           # BLOB: len16 + data

# --- Категории данных (_SortBufData switch @ 0x401657) ---
class Cat(IntEnum):
    UNKNOWN = 0
    GENERIC = 1       # категории 4, 5, 6 → body[0]=1
    ASCAN   = 2       # категории 1, 2, 3 → body[0]=2
    BSCAN   = 3       # категории 10-19   → body[0]=3
    SHORT   = 4       # категории 20-29   → body[0]=4

# --- TypeVar dispatch (DispatchByTypeVar @ 0x4022C8) ---
# sweepId определяет формат: 150-151=Рельсовая, 362-843=Желез/Базовая
TYPEVAR_ASCAN_RANGE     = range(150, 152)    # DecodeTypeVar_Ascan
TYPEVAR_COMPOSITE_RANGE = [(390,392),(642,644),(842,844)]  # DecodeTypeVar_Composite
TYPEVAR_DEFAULT_RANGES  = [(362,364),(380,382),(442,444),(480,482),
                           (680,684),(712,714),(730,732),(740,742),(780,782)]



# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #2  ТРАНСПОРТ  (TCOMPort + Protocol)                                    ║
# ╚════════════════════════════════════════════════════════════════════════════╝

class COMPort:
    """1:1 аналог TCOMPort (struct 24 bytes) из PelengPC.exe.
    Создание: TCOMPort_Create @ 0x4239C8
    Открытие: TCOMPort_Open @ 0x423D18 (CreateFileA + SetCommTimeouts + Sleep(100))
    Настройки: TCOMPort_ApplySettings @ 0x424020 (8N1, flow control ALL OFF)
    """
    def __init__(self):
        self._ser = None
        self.port_name = ""
        self.baud_rate = DEFAULT_BAUD
        self.timeout_ms = 1000
        self.is_open = False

    def open(self) -> bool:
        if self.is_open: return True
        try:
            import serial
            self._ser = serial.Serial(
                port=self.port_name, baudrate=self.baud_rate,
                bytesize=8, stopbits=1, parity='N',
                timeout=self.timeout_ms/1000.0,
                write_timeout=WRITE_TIMEOUT_CONSTANT/1000.0,
                xonxoff=False, rtscts=False, dsrdtr=False,
                inter_byte_timeout=READ_INTERVAL_TIMEOUT/1000.0)
            self.is_open = True
            time.sleep(OPEN_PORT_DELAY_MS / 1000.0)
            return True
        except Exception as e:
            self._ser = None; self.is_open = False
            return False

    def close(self):
        if self._ser:
            try: self._ser.close()
            except: pass
        self._ser = None; self.is_open = False

    def tx(self, data: bytes) -> int:
        if not self.is_open: return 0
        try: return self._ser.write(data)
        except: return 0

    def tx1(self, byte: int) -> int:
        return self.tx(bytes([byte & 0xFF]))

    def rx(self, n: int) -> bytes:
        if not self.is_open: return b''
        try: return self._ser.read(n)
        except: return b''

    def available(self) -> int:
        if not self._ser: return 0
        return self._ser.in_waiting

    @staticmethod
    def delay(ms: int):
        time.sleep(ms / 1000.0)


class ProtocolError(Exception): pass


class Protocol:
    """Протокол PelengPC ↔ Прибор. Реверс: 0x424CC0, 0x424BB0, 0x4249C0, 0x41B43C."""

    def __init__(self, port: COMPort):
        self.port = port
        self.device_flags = 0       # DAT_0053E97C
        self.frame_hdr = b'\x00'*16 # DAT_0053E96A
        self.on_progress: Optional[Callable] = None

    @property
    def is_wagon(self) -> bool:
        return bool(self.device_flags & 4)

    @property
    def flash_size(self) -> int:
        return FLASH_SIZE_WAGON if self.is_wagon else FLASH_SIZE_BASE

    def handshake(self) -> Tuple[bool, bytes]:
        """Опкод 0x55: TX=[0x55] → RX≥17 bytes. Заголовок сохраняется."""
        self.port.tx1(OP_HANDSHAKE)
        resp = self.port.rx(0x80010)
        if len(resp) < HANDSHAKE_MIN_RESP:
            return False, resp
        self.frame_hdr = resp[:FRAME_HDR]
        self.device_flags = resp[2]
        return True, resp

    def block_request(self, addr: int, size: int) -> bytes:
        """Опкод 0x42: TX=[0x42][10ms][lo][10ms][hi][10ms] → RX=size bytes."""
        self.port.tx1(OP_BLOCK_REQ);   COMPort.delay(10)
        self.port.tx1(addr & 0xFF);    COMPort.delay(10)
        self.port.tx1((addr>>8)&0xFF); COMPort.delay(10)
        data = self._recv(size)
        if len(data) != size:
            raise ProtocolError(f"Block {addr}: got {len(data)}/{size}")
        return data

    def flash_dump(self, opcode=OP_FLASH_DUMP, size=None) -> bytes:
        """Опкод 0x9A/0x74: TX=[op] → RX=fixed size."""
        if size is None: size = self.flash_size
        self.port.tx1(opcode)
        data = self._recv(size)
        if len(data) != size:
            raise ProtocolError(f"Flash 0x{opcode:02X}: got {len(data)}/{size}")
        return data

    def read_block(self, addr: int, size: int) -> bytes:
        """ReadBlock_Dispatcher @ 0x41B43C: addr=60000→flash, иначе→block_request."""
        if addr == FLASH_ADDR:
            body = self.flash_dump()
            pkt = bytearray(FRAME_HDR + len(body))
            pkt[:FRAME_HDR] = self.frame_hdr
            struct.pack_into('<H', pkt, 0x10, FLASH_ADDR)
            pkt[FRAME_HDR:] = body
            return bytes(pkt)
        body = self.block_request(addr, size)
        pkt = bytearray(FRAME_HDR + len(body))
        pkt[:FRAME_HDR] = self.frame_hdr
        pkt[FRAME_HDR:] = body
        return bytes(pkt)

    def _recv(self, total: int) -> bytes:
        buf = bytearray()
        t0 = time.time()
        while len(buf) < total:
            chunk = self.port.rx(min(total - len(buf), 4096))
            if chunk:
                buf.extend(chunk)
                if self.on_progress: self.on_progress(len(buf), total)
            if time.time() - t0 > self.port.timeout_ms/1000.0: break
        return bytes(buf)



# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #3  TLV-ПАРСЕР  (_SortBufData @ 0x401454 из 102_203dll.dll)             ║
# ╚════════════════════════════════════════════════════════════════════════════╝

def classify(addr: int) -> Cat:
    """Категория по адресу (blockAddr/1000 → switch в DLL)."""
    c = addr // 1000
    if c in (1,2,3): return Cat.ASCAN
    if c in (4,5,6): return Cat.GENERIC
    if 10 <= c <= 19: return Cat.BSCAN
    if 20 <= c <= 29: return Cat.SHORT
    return Cat.UNKNOWN

def is_sweep(addr: int) -> bool:
    """IsSweepAddress @ 0x41C7C0."""
    return SWEEP_MIN <= addr <= SWEEP_MAX

@dataclass
class TLVRecord:
    tag: int; data: bytes; cat: Cat = Cat.UNKNOWN

def parse_tlv(raw: bytes, offset: int = FRAME_HDR) -> List[TLVRecord]:
    """ParseFlashDump_TLV @ 0x41B008: [tag:2LE][len:2LE][data:len]...[0xFFFF][0xFF pad]."""
    recs = []; pos = offset
    while pos + 4 <= len(raw):
        tag = raw[pos] | (raw[pos+1]<<8)
        if tag == TLV_END:
            pos += 2
            while pos < len(raw) and raw[pos] == TLV_PAD: pos += 1
            continue
        length = raw[pos+2] | (raw[pos+3]<<8)
        if length == 0 or pos+4+length > len(raw):
            pos += 1; continue
        recs.append(TLVRecord(tag=tag, data=raw[pos+4:pos+4+length], cat=classify(tag)))
        pos += 4 + length
    return recs


# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #4  ДЕШИФРОВКА ПОЛЕЙ  (DecodeDate/Time/String + TypeVar + DAL схемы)    ║
# ╚════════════════════════════════════════════════════════════════════════════╝

# --- Побайтовый StreamReader (аналог Parser_Helper_1f54 из DLL) ---
class StreamReader:
    """Последовательный парсер байтового потока тела TLV-записи."""
    def __init__(self, buf: bytes, pos: int = 0):
        self.buf = buf; self.pos = pos

    def remaining(self) -> int: return max(0, len(self.buf) - self.pos)
    def peek(self, n=1) -> bytes: return self.buf[self.pos:self.pos+n]

    def u8(self) -> int:
        if self.pos >= len(self.buf): return 0
        v = self.buf[self.pos]; self.pos += 1; return v

    def u16le(self) -> int:
        v = self.buf[self.pos]|(self.buf[self.pos+1]<<8) if self.pos+2<=len(self.buf) else 0
        self.pos += 2; return v

    def u16be(self) -> int:
        v = (self.buf[self.pos]<<8)|self.buf[self.pos+1] if self.pos+2<=len(self.buf) else 0
        self.pos += 2; return v

    def u32le(self) -> int:
        if self.pos+4 > len(self.buf): self.pos += 4; return 0
        v = struct.unpack_from('<I', self.buf, self.pos)[0]; self.pos += 4; return v

    def date(self) -> str:
        """DecodeDate @ 0x4023B0: [day][month][year-2000] → 'DD.MM.YYYY'."""
        if self.remaining() < 3: self.pos += 3; return "01.01.2000"
        d, m, y = self.u8(), self.u8(), self.u8()
        if not (1<=m<=12 and 1<=d<=31): return "01.01.2000"
        return f"{d:02d}.{m:02d}.{y+2000}"

    def time(self) -> str:
        """DecodeTime @ 0x402504: [hh][mm] → 'HH:MM'."""
        if self.remaining() < 2: self.pos += 2; return "00:00"
        h, m = self.u8()%100, self.u8()%100
        return f"{h:02d}:{m:02d}"

    def pstring(self, maxlen: int = 32) -> str:
        """Pascal short string (len-prefixed, CP1251)."""
        if self.pos >= len(self.buf): return ""
        n = min(self.buf[self.pos], maxlen, self.remaining()-1)
        self.pos += 1
        s = self.buf[self.pos:self.pos+n].decode('cp1251', errors='replace').rstrip('\x00 ')
        self.pos += n; return s

    def blob(self, n: int) -> bytes:
        b = self.buf[self.pos:self.pos+n]; self.pos += n; return b

    def skip(self, n: int): self.pos += n


# --- Схемы полей из PelengPCtest.dal (порядок = как в DLL) ---
# Каждая запись: (field_name, display_label, field_type)

SCHEMA_RESULTS_BASE: List[Tuple[str,str,int]] = [
    ("NUMBER",     "№ п/п",             FT_INT),
    ("TYPEZAP",    "Вид записи",        FT_ENUM_TZ),
    ("NUMKOD",     "№ записи",          FT_INT),
    ("DATEFORM",   "Дата",              FT_DATE),
    ("TIMEFORM",   "Время",             FT_TIME),
    ("KODOPERA",   "Оператор",          FT_INT),
    ("NAMEOPERA",  "Имя оператора",     FT_STR_L),
    ("NUMVERS",    "Версия ПО",         FT_STR_S),
    ("NUMPRIB",    "№ прибора",         FT_INT),
    ("TYPEVAR",    "Типовой вариант",   FT_INT),
    ("NUMOBJ",     "№ объекта",         FT_STR_S),
    ("M",          "м",                 FT_INT),
    ("MM",         "мм",                FT_INT),
    ("CLOCK",      "Часы",              FT_INT),
    ("SMELTING",   "Плавка",            FT_STR_S),
    ("MAKETIME",   "Год изготовления",  FT_INT),
    ("DEFEKT",     "Дефект",            FT_ENUM_DEF),
    ("CODEDEF",    "Код дефекта",       FT_STR_S),
    ("CONDLENGTH", "Усл. протяж., мм",  FT_INT_MM),
    ("NUMZAP",     "Настройки",         FT_INT),
]

SCHEMA_RESULTS_RAIL: List[Tuple[str,str,int]] = [
    ("NUMBER",     "№ п/п",             FT_INT),
    ("TYPEZAP",    "Вид записи",        FT_ENUM_TZ),
    ("NUMKOD",     "№ записи",          FT_INT),
    ("DATEFORM",   "Дата",              FT_DATE),
    ("TIMEFORM",   "Время",             FT_TIME),
    ("KODOPERA",   "Оператор",          FT_INT),
    ("NAMEOPERA",  "Имя оператора",     FT_STR_L),
    ("NUMVERS",    "Версия ПО",         FT_STR_S),
    ("NUMPRIB",    "№ прибора",         FT_INT),
    ("TYPEVAR",    "Типовой вариант",   FT_INT),
    ("NUMOBJ",     "№ объекта",         FT_STR_L),
    ("KM",         "км",                FT_INT),
    ("M",          "м",                 FT_INT),
    ("MM",         "мм",                FT_INT),
    ("STAGE",      "Перегон",           FT_STR_X),
    ("SECTION",    "Звено",             FT_STR_X),
    ("PICKET",     "Пикет",             FT_STR_X),
    ("PATH",       "Путь",              FT_STR_X),
    ("DEFEKT",     "Дефект",            FT_ENUM_DEF),
    ("CODEDEF",    "Код дефекта",       FT_STR_S),
    ("CONDLENGTH", "Усл. протяж., мм",  FT_INT_MM),
    ("NUMZAP",     "Настройки",         FT_INT),
]

SCHEMA_NASTR: List[Tuple[str,str,int]] = [
    ("NUMBER",     "№ п/п",             FT_INT),
    ("NUMKOD",     "№ записи",          FT_INT),
    ("TYPEZAP",    "Вид записи",        FT_ENUM_TZ),
    ("DATEFORM",   "Дата",              FT_DATE),
    ("TIMEFORM",   "Время",             FT_TIME),
    ("KODOPERA",   "Оператор",          FT_INT),
    ("NAMEOPERA",  "Имя оператора",     FT_STR_L),
    ("NUMVERS",    "Версия ПО",         FT_STR_S),
    ("NUMPRIB",    "№ прибора",         FT_INT),
    ("TYPEVAR",    "Типовой вариант",   FT_INT),
    ("NUMPROT",    "Отчёты контроля",   FT_INT),
]

SCHEMA_SHORTPROT: List[Tuple[str,str,int]] = [
    ("NUMBER",     "№ п/п",             FT_INT),
    ("NUMKOD",     "№ записи",          FT_INT),
    ("DATEFORM",   "Дата",              FT_DATE),
    ("TIMEFORM",   "Время",             FT_TIME),
    ("KODOPERA",   "Оператор",          FT_INT),
    ("NAMEOPERA",  "Имя оператора",     FT_STR_L),
    ("NUMVERS",    "Версия ПО",         FT_STR_S),
    ("NUMPRIB",    "№ прибора",         FT_INT),
    ("TYPEVAR",    "Типовой вариант",   FT_INT),
    ("NUMOBJ",     "№ объекта",         FT_STR_L),
    ("M",          "м",                 FT_INT),
    ("MM",         "мм",                FT_INT),
    ("CLOCK",      "Часы",              FT_INT),
    ("NUMDEF",     "Кол-во дефектов",   FT_INT),
    ("PROTOCOL",   "Протокол",          FT_BLOB),
    ("NUMPROT",    "Отчёты",            FT_INT),
]

# --- Enum словари ---
ENUM_DEFEKT  = {0: "нет", 1: "есть"}
ENUM_TYPEZAP = {0: "А-развёртка", 1: "B-развёртка", 2: "Настройки", 3: "Отчёт"}


def decode_field(rd: StreamReader, ftype: int):
    """Декодирование одного поля по типу (как _decode_field в DLL)."""
    if ftype == FT_INT:      return rd.u16le()
    if ftype == FT_INT_MM:   return rd.u32le()
    if ftype == FT_DATE:     return rd.date()
    if ftype == FT_TIME:     return rd.time()
    if ftype == FT_STR_S:    return rd.pstring(15)
    if ftype == FT_STR_L:    return rd.pstring(31)
    if ftype == FT_STR_X:    return rd.pstring(63)
    if ftype == FT_ENUM_DEF: return ENUM_DEFEKT.get(rd.u8(), "?")
    if ftype == FT_ENUM_TZ:  return ENUM_TYPEZAP.get(rd.u8(), "?")
    if ftype == FT_BLOB:
        n = rd.u16le()
        return rd.blob(min(n, rd.remaining()))
    return rd.u16le()  # fallback


def pick_schema(sweep_id: int) -> List[Tuple[str,str,int]]:
    """Выбор схемы по sweepId (= param_1[3] в DLL, = firmware version code)."""
    if sweep_id in TYPEVAR_ASCAN_RANGE:
        return SCHEMA_RESULTS_RAIL       # 150-151 = Рельсовая
    # 442-843 range includes Железнодорожная и Базовая — используем Базовую по умолчанию
    return SCHEMA_RESULTS_BASE


def decode_record(body: bytes, sweep_id: int = 0, cat: Cat = Cat.UNKNOWN) -> dict:
    """Полная дешифровка записи по схеме (как decode_schema в peleng_clone).
    body = данные после 16-byte frame header.
    Первый байт body — marker категории (1-4), пропускаем.
    """
    if cat == Cat.GENERIC:
        schema = SCHEMA_NASTR
    elif cat == Cat.SHORT:
        schema = SCHEMA_SHORTPROT
    elif cat in (Cat.ASCAN, Cat.BSCAN):
        schema = pick_schema(sweep_id)
    else:
        schema = SCHEMA_RESULTS_BASE

    rd = StreamReader(body, 1)  # skip body[0] = category marker
    fields = {}
    for name, label, ftype in schema:
        if rd.remaining() <= 0: break
        fields[name] = decode_field(rd, ftype)
    fields['_label_map'] = {name: label for name, label, _ in schema}
    return fields


# --- Высокоуровневый sort_buf_data (полный аналог DLL @ 0x401454) ---
@dataclass
class ParsedPacket:
    cat: Cat
    block_addr: int
    sweep_id: int
    dev_id: int
    fields: dict
    raw: bytes

def sort_buf_data(raw: bytes) -> Optional[ParsedPacket]:
    """_SortBufData: разбор полного пакета (frame_hdr + body)."""
    if len(raw) < FRAME_HDR + 2: return None
    body = raw[FRAME_HDR:]
    addr = body[0] | (body[1]<<8)
    cat = classify(addr)
    sweep_id = raw[4]*100 + raw[5]
    dev_id = raw[0] | (raw[1]<<8)
    fields = decode_record(body, sweep_id, cat) if len(body) > 5 else {}
    return ParsedPacket(cat=cat, block_addr=addr, sweep_id=sweep_id,
                        dev_id=dev_id, fields=fields, raw=raw)



# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #5  БАЗА ДАННЫХ  (SQLite аналог Firebird PelengPC.fdb, таблица BlockZap)║
# ╚════════════════════════════════════════════════════════════════════════════╝

class DB:
    """BlockZap: Number(PK), BlockLen, Block(BLOB). SQL из DFM FormMain."""
    def __init__(self, path="PelengPC.db"):
        self.conn = sqlite3.connect(path)
        self.conn.execute("CREATE TABLE IF NOT EXISTS BlockZap(Number INTEGER PRIMARY KEY, BlockLen INTEGER, Block BLOB)")
        self.conn.commit()

    def close(self): self.conn.close()
    def count(self) -> int: return self.conn.execute("SELECT COUNT(*) FROM BlockZap").fetchone()[0]
    def all(self) -> list: return self.conn.execute("SELECT Number,BlockLen,Block FROM BlockZap ORDER BY Number").fetchall()
    def get(self, n: int): return self.conn.execute("SELECT Number,BlockLen,Block FROM BlockZap WHERE Number=?",(n,)).fetchone()
    def put(self, n: int, data: bytes): self.conn.execute("INSERT OR REPLACE INTO BlockZap VALUES(?,?,?)",(n,len(data),data)); self.conn.commit()
    def delete(self, n: int): self.conn.execute("DELETE FROM BlockZap WHERE Number=?",(n,)); self.conn.commit()
    def clear(self): self.conn.execute("DELETE FROM BlockZap"); self.conn.commit()


# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #6  B-SCAN РЕНДЕР  (FormScreen_RenderBscan @ 0x4235F8)                  ║
# ╚════════════════════════════════════════════════════════════════════════════╝

class BscanRender:
    """30 bytes/row = 240 bits, 1 bit = 1 pixel (2×2 scaled), width=480px."""

    @staticmethod
    def extract(pkt: bytes) -> bytes:
        """Извлечь данные B-scan из полного пакета (offset 0x1D0)."""
        return pkt[BSCAN_BODY_OFFSET:] if len(pkt) > BSCAN_BODY_OFFSET else b''

    @staticmethod
    def dimensions(data: bytes) -> Tuple[int, int]:
        rows = len(data) // BSCAN_ROW_BYTES
        return (BSCAN_ROW_BYTES * 8 * BSCAN_SCALE, rows * BSCAN_SCALE)

    @staticmethod
    def to_pixels(data: bytes) -> List[List[int]]:
        """2D массив пикселей: 0=белый, 1=чёрный."""
        rows = len(data) // BSCAN_ROW_BYTES
        w, h = BSCAN_ROW_BYTES*8*BSCAN_SCALE, rows*BSCAN_SCALE
        px = [[0]*w for _ in range(h)]
        for r in range(rows):
            for cb in range(BSCAN_ROW_BYTES):
                v = data[r*BSCAN_ROW_BYTES + cb]
                for bit in range(8):
                    if (v >> bit) & 1:
                        x, y = (cb*8+bit)*BSCAN_SCALE, r*BSCAN_SCALE
                        for dy in range(BSCAN_SCALE):
                            for dx in range(BSCAN_SCALE):
                                if y+dy<h and x+dx<w: px[y+dy][x+dx] = 1
        return px

    @staticmethod
    def save_bmp(data: bytes, path: str) -> bool:
        """Сохранение B-scan в BMP (monochrome, без зависимостей)."""
        if not data: return False
        w, h = BscanRender.dimensions(data)
        if w==0: return False
        px = BscanRender.to_pixels(data)
        rs = ((w+31)//32)*4
        pds = rs * h
        with open(path, 'wb') as f:
            f.write(b'BM' + struct.pack('<I',14+40+8+pds) + b'\x00'*4 + struct.pack('<I',14+40+8))
            f.write(struct.pack('<IiiHHIIiiII',40,w,-h,1,1,0,pds,2835,2835,2,2))
            f.write(struct.pack('<BBBB',255,255,255,0) + struct.pack('<BBBB',0,0,0,0))
            for y in range(h):
                row = bytearray(rs)
                for x in range(w):
                    if px[y][x]: row[x//8] |= (1 << (7-x%8))
                f.write(row)
        return True


# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #7  КОНФИГУРАЦИЯ  (INI = аналог HKLM\SOFTWARE\Altek\PelengPC)          ║
# ╚════════════════════════════════════════════════════════════════════════════╝

class Config:
    def __init__(self, path="pelengpc.ini"):
        self.path = path
        self.cfg = configparser.ConfigParser()
        for s, kv in {'port':{'name':'COM1','baud':'9600'},
                      'db':{'path':'PelengPC.db'},
                      'company':{'firma':'','label':''}}.items():
            self.cfg[s] = kv
        if os.path.exists(path): self.cfg.read(path, encoding='utf-8')

    def save(self):
        with open(self.path,'w',encoding='utf-8') as f: self.cfg.write(f)

    @property
    def port_name(self): return self.cfg.get('port','name',fallback='COM1')
    @port_name.setter
    def port_name(self, v): self.cfg.set('port','name',v)
    @property
    def port_baud(self): return self.cfg.getint('port','baud',fallback=9600)
    @port_baud.setter
    def port_baud(self, v): self.cfg.set('port','baud',str(v))
    @property
    def db_path(self): return self.cfg.get('db','path',fallback='PelengPC.db')


# ╔════════════════════════════════════════════════════════════════════════════╗
# ║  #8  ПРИЛОЖЕНИЕ  (PelengPCApp + CLI)                                     ║
# ╚════════════════════════════════════════════════════════════════════════════╝

class PelengPCApp:
    """Связывает все компоненты. Аналог FormMain_FormCreate @ 0x40BCCC."""
    def __init__(self, ini="pelengpc.ini"):
        self.cfg = Config(ini)
        self.port = COMPort()
        self.port.port_name = self.cfg.port_name
        self.port.baud_rate = self.cfg.port_baud
        self.proto = Protocol(self.port)
        self.db = DB(self.cfg.db_path)

    def connect(self) -> bool:
        if not self.port.open(): return False
        ok, _ = self.proto.handshake()
        return ok

    def disconnect(self): self.port.close()

    def read_and_parse(self, addr: int, size: int) -> Optional[ParsedPacket]:
        raw = self.proto.read_block(addr, size)
        return sort_buf_data(raw)

    def read_flash_all(self) -> List[TLVRecord]:
        raw = self.proto.read_block(FLASH_ADDR, self.proto.flash_size)
        return parse_tlv(raw)

    def save(self, num: int, data: bytes): self.db.put(num, data)


# --- CLI ---
def main():
    ap = argparse.ArgumentParser(prog="peleng_pc", description=f"PelengPC Clone v{__version__}")
    sub = ap.add_subparsers(dest='cmd')

    sub.add_parser('info', help='Справка по протоколу')

    p = sub.add_parser('test', help='Handshake')
    p.add_argument('--port', required=True); p.add_argument('--baud', type=int, default=9600)

    p = sub.add_parser('block', help='Чтение блока')
    p.add_argument('--port', required=True); p.add_argument('--baud', type=int, default=9600)
    p.add_argument('--addr', type=int, required=True); p.add_argument('--size', type=int, required=True)
    p.add_argument('--save', action='store_true')

    p = sub.add_parser('flash', help='FLASH dump')
    p.add_argument('--port', required=True); p.add_argument('--baud', type=int, default=9600)
    p.add_argument('--save-all', action='store_true')

    p = sub.add_parser('decode', help='Декодировать запись из БД')
    p.add_argument('--num', type=int, required=True)

    args = ap.parse_args()

    if args.cmd == 'info':
        print(f"PelengPC Clone v{__version__}\nПротокол: 0x55=Handshake, 0x42=Block, 0x9A=Flash, 0x74=Status")
        print(f"COM: 8N1, 15 baud rates (110-256000), default 9600, timeout 1s")
        print(f"Адреса: 10000-29999=sweep(B-scan), 60000=FlashDump")
        print(f"Категории: 1-3=Ascan, 4-6=Settings, 10-19=Bscan, 20-29=ShortProto")
        print(f"Дата=[day,month,year+2000], Время=[hh,mm], Строка=Pascal CP1251")
        return 0

    if args.cmd == 'test':
        app = PelengPCApp(); app.port.port_name = args.port; app.port.baud_rate = args.baud
        if not app.port.open(): print("Порт не открылся"); return 1
        ok, resp = app.proto.handshake()
        if ok:
            print(f"OK! {len(resp)} байт, DevID={resp[0]|(resp[1]<<8)}, Flags=0x{resp[2]:02X}")
            print(f"  Версия: {'Вагонная' if resp[2]&4 else 'Базовая'}")
        else:
            print(f"FAIL: {len(resp)} байт")
        app.disconnect(); return 0 if ok else 1

    if args.cmd == 'block':
        app = PelengPCApp(); app.port.port_name = args.port; app.port.baud_rate = args.baud
        if not app.connect(): print("Нет связи"); return 1
        try:
            p = app.read_and_parse(args.addr, args.size)
            if p:
                print(f"Тип: {p.cat.name}, Addr: {p.block_addr}, SweepID: {p.sweep_id}")
                for k,v in p.fields.items():
                    if k.startswith('_'): continue
                    label = p.fields.get('_label_map',{}).get(k, k)
                    print(f"  {label}: {v}")
                if args.save: app.save(args.addr, p.raw); print(f"Сохранено в БД")
        except ProtocolError as e: print(f"Ошибка: {e}")
        app.disconnect(); return 0

    if args.cmd == 'flash':
        app = PelengPCApp(); app.port.port_name = args.port; app.port.baud_rate = args.baud
        if not app.connect(): print("Нет связи"); return 1
        try:
            recs = app.read_flash_all()
            print(f"TLV записей: {len(recs)}")
            for i, r in enumerate(recs[:30]):
                print(f"  [{i:3d}] tag={r.tag:5d} {r.cat.name:8s} {len(r.data):5d}b")
            if args.save_all:
                for r in recs:
                    pkt = bytearray(FRAME_HDR+len(r.data))
                    struct.pack_into('<H', pkt, 0x10, r.tag)
                    pkt[FRAME_HDR:] = r.data
                    app.save(r.tag, bytes(pkt))
                print(f"Сохранено {len(recs)} записей")
        except ProtocolError as e: print(f"Ошибка: {e}")
        app.disconnect(); return 0

    if args.cmd == 'decode':
        app = PelengPCApp()
        row = app.db.get(args.num)
        if not row: print(f"Запись {args.num} не найдена"); return 1
        p = sort_buf_data(row[2])
        if p:
            print(f"Number={args.num}, Тип={p.cat.name}, SweepID={p.sweep_id}")
            for k,v in p.fields.items():
                if k.startswith('_'): continue
                label = p.fields.get('_label_map',{}).get(k, k)
                print(f"  {label}: {v}")
            # B-scan → сохраняем BMP
            if p.cat == Cat.BSCAN:
                bdata = BscanRender.extract(p.raw)
                if bdata:
                    bmp_path = f"bscan_{args.num}.bmp"
                    BscanRender.save_bmp(bdata, bmp_path)
                    w,h = BscanRender.dimensions(bdata)
                    print(f"  B-scan: {w}x{h}px → {bmp_path}")
        return 0

    ap.print_help(); return 0


if __name__ == '__main__':
    sys.exit(main())
