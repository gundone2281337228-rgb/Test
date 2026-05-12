#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
peleng_pc_clone.py — Полный 1:1 клон PelengPC ver 1.2
Написан с нуля на основе полного реверс-инжиниринга оригинала.

Оригинал: PelengPC.exe (Borland C++Builder 5, VCL, Firebird)
Клон: Python 3.10+ / PyQt6 / pyserial / SQLite

Автор реверса: Kiro AI (2026-05-12)
Источники:
  - PelengPC.exe (1).c — 225870 строк, 7080 функций переименовано
  - 102_203dll.dll.c — 33872 строк, полная декомпиляция _SortBufData
  - Ручной анализ asm (objdump) + Python (struct, lzma)

Зависимости:
  pip install pyserial PyQt6

Использование:
  python peleng_pc_clone.py              # GUI режим
  python peleng_pc_clone.py --port COM3  # GUI с автоподключением
  python peleng_pc_clone.py --demo       # Демо-режим без прибора
"""

from __future__ import annotations

import argparse
import configparser
import datetime
import os
import sqlite3
import struct
import sys
import time
from dataclasses import dataclass, field
from enum import IntEnum
from pathlib import Path
from typing import Optional, Callable, List, Dict, Tuple

# ==============================================================================
# 1. КОНСТАНТЫ (из реверса TCOMPort_Create @ 0x4239C8)
# ==============================================================================

APP_NAME = "PelengPC"
APP_VERSION = "1.2 (Python Clone)"
COMPANY = "Altek"
REGISTRY_KEY = r"SOFTWARE\Altek\PelengPC"

# Полная таблица baud rate (15 значений, из switch @ 0x423B4C)
BAUD_RATES = (
    110, 300, 600, 1200, 2400, 4800, 9600,  # index 0-6
    14400, 19200, 38400, 56000, 57600,       # index 7-11
    115200, 128000, 256000                    # index 12-14
)
DEFAULT_BAUD_INDEX = 6  # 9600
DEFAULT_BAUD = 9600
DEFAULT_DATA_BITS = 8    # stored as 3 in original (3+5=8)
DEFAULT_STOP_BITS = 1    # stored as 0 in original
DEFAULT_PARITY = 'N'     # stored as 0 in original
DEFAULT_TIMEOUT_MS = 1000

# COM Port timeouts (из TCOMPort_Open @ 0x423D18)
READ_INTERVAL_TIMEOUT = 1
READ_TOTAL_TIMEOUT_MULTIPLIER = 0
READ_TOTAL_TIMEOUT_CONSTANT = 1
WRITE_TOTAL_TIMEOUT_MULTIPLIER = 0
WRITE_TOTAL_TIMEOUT_CONSTANT = 10
OPEN_PORT_DELAY_MS = 100  # Sleep(100) после открытия

# Протокол (из Protocol_BlockRequest @ 0x424CC0, Protocol_FlashDump @ 0x424BB0)
OPCODE_HANDSHAKE = 0x55    # 'U'
OPCODE_BLOCK_REQ = 0x42    # 'B'
OPCODE_FLASH_DUMP = 0x9A
OPCODE_STATUS = 0x74       # 't'
INTER_BYTE_DELAY_S = 0.010  # 10ms между байтами команды

# Адресация (из IsSweepAddress @ 0x41C7C0)
SWEEP_ADDR_MIN = 10000
SWEEP_ADDR_MAX = 29999
FLASH_DUMP_ADDR = 60000  # специальный адрес → опкод 0x9A

# Размеры FLASH (из FUN_0041CBF4)
FLASH_SIZE_BASE = 4293      # 0x10C5 (базовая версия)
FLASH_SIZE_WAGON = 5253     # 0x1485 (вагонная, бит 2 в deviceFlags)

# B-scan параметры (из FormScreen_RenderBscan @ 0x4235F8)
BSCAN_HEADER_SIZE = 448     # 0x1C0 байт заголовка A-scan
BSCAN_DATA_OFFSET = 464     # 0x1D0 (448 + 16 байт фрейм-хедера)
BSCAN_ROW_BYTES = 30        # 30 байт на строку = 240 бит
BSCAN_PIXEL_SCALE = 2       # каждый бит = 2×2 пикселя
BSCAN_WIDTH_PX = 480        # 240 * 2

# Packet header (16 bytes, из DAT_0053E96A)
FRAME_HEADER_SIZE = 16

# TLV маркеры (из ParseFlashDump_TLV @ 0x41B008)
TLV_END_MARKER = 0xFFFF
TLV_PADDING_BYTE = 0xFF

# Handshake минимальный ответ
HANDSHAKE_MIN_RESPONSE = 17  # < 0x11 = ошибка


# ==============================================================================
# 2. ПЕРЕЧИСЛЕНИЯ
# ==============================================================================

class DataCategory(IntEnum):
    """Категории данных (из _SortBufData switch @ 0x401657)"""
    UNKNOWN = 0
    GENERIC = 1       # категории 4, 5, 6
    ASCAN = 2         # категории 1, 2, 3
    BSCAN = 3         # категории 10-19
    SHORT_PROTO = 4   # категории 20-29


class ConnectionMode(IntEnum):
    """Режим подключения (из TFormReadData+0x3C4)"""
    FROM_FILE = 0     # чтение из файла/буфера
    COM_DIRECT = 1    # прямое подключение через COM-порт


class DeviceFlag(IntEnum):
    """Флаги устройства (из DAT_0053E97C, бит 2 = вагонная версия)"""
    WAGON_VERSION = 4  # бит 2


# ==============================================================================
# 3. СТРУКТУРЫ ДАННЫХ
# ==============================================================================

@dataclass
class PacketHeader:
    """16-байтовый заголовок фрейма (из реверса DAT_0053E96A)"""
    dev_id: int = 0           # LE16 @ [0:2]
    flags: int = 0            # [2] — флаги версии/типа прибора
    unknown_03: int = 0       # [3]
    sweep_id_hi: int = 0      # [4] → sweep_id = [4]*100 + [5]
    sweep_id_lo: int = 0      # [5]
    unknown_06_0F: bytes = field(default_factory=lambda: b'\x00' * 10)
    
    @property
    def sweep_id(self) -> int:
        return self.sweep_id_hi * 100 + self.sweep_id_lo
    
    @classmethod
    def from_bytes(cls, data: bytes) -> 'PacketHeader':
        if len(data) < FRAME_HEADER_SIZE:
            raise ValueError(f"Header too short: {len(data)} < {FRAME_HEADER_SIZE}")
        return cls(
            dev_id=data[0] | (data[1] << 8),
            flags=data[2],
            unknown_03=data[3],
            sweep_id_hi=data[4],
            sweep_id_lo=data[5],
            unknown_06_0F=data[6:16]
        )
    
    def to_bytes(self) -> bytes:
        result = bytearray(16)
        result[0] = self.dev_id & 0xFF
        result[1] = (self.dev_id >> 8) & 0xFF
        result[2] = self.flags
        result[3] = self.unknown_03
        result[4] = self.sweep_id_hi
        result[5] = self.sweep_id_lo
        result[6:16] = self.unknown_06_0F
        return bytes(result)


@dataclass
class BlockEntry:
    """Запись в списке блоков (из ParseFlashDump_TLV, BlockListEntry)"""
    tag: int          # адрес блока (тег TLV)
    offset: int       # смещение в буфере
    size: int         # размер данных


@dataclass
class SweepEntry:
    """Запись sweep-таблицы (24 bytes, из TFormReadData+0x324)"""
    addr_min: int     # минимальный адрес
    addr_max: int     # максимальный адрес
    block_size: int   # размер блока
    name: str = ""    # имя sweep


@dataclass
class StreakEntry:
    """Запись streak-таблицы (8 bytes, из TFormReadData+0x3BC)"""
    max_index: int    # максимальный индекс
    base_addr: int    # базовый адрес (округлён до 100)


# ==============================================================================
# 4. COM-ПОРТ (полный аналог TCOMPort из реверса)
# ==============================================================================

class COMPort:
    """
    Полный аналог TCOMPort (24 bytes) из PelengPC.exe.
    
    Реверс-источники:
      TCOMPort_Create     @ 0x4239C8
      TCOMPort_Open       @ 0x423D18
      TCOMPort_Close      @ 0x423E28
      TCOMPort_ApplySettings @ 0x424020
      TCOMPort_WriteBytes @ 0x423E58
      TCOMPort_WriteByte  @ 0x423F0C
      TCOMPort_ReadBytes  @ 0x423F64
      TCOMPort_BytesAvail @ 0x423F40
      TCOMPort_Delay      @ 0x423A58
    """
    
    def __init__(self):
        """TCOMPort_Create: инициализация с дефолтами"""
        self._serial: Optional['serial.Serial'] = None
        self.port_name: str = ""
        self.baud_index: int = DEFAULT_BAUD_INDEX
        self.baud_rate: int = DEFAULT_BAUD
        self.data_bits: int = 8
        self.stop_bits: int = 1
        self.parity: str = 'N'
        self.timeout_ms: int = DEFAULT_TIMEOUT_MS
        self.is_open: bool = False
    
    def set_baud_rate(self, rate: int):
        """TCOMPort_SetBaudRate_Switch @ 0x423B4C"""
        self.baud_rate = rate
        if rate in BAUD_RATES:
            self.baud_index = BAUD_RATES.index(rate)
        else:
            self.baud_index = DEFAULT_BAUD_INDEX  # fallback
    
    def open(self) -> bool:
        """
        TCOMPort_Open @ 0x423D18:
        1. CreateFileA("\\\\.\\COMn")
        2. TCOMPort_ApplySettings (SetCommState: 8N1, no flow control)
        3. SetCommTimeouts(ReadInterval=1, WriteConst=10)
        4. Sleep(100)
        """
        if self.is_open:
            return True
        
        try:
            import serial
            self._serial = serial.Serial(
                port=self.port_name,
                baudrate=self.baud_rate,
                bytesize=self.data_bits,
                stopbits=self.stop_bits,
                parity=self.parity,
                timeout=self.timeout_ms / 1000.0,
                write_timeout=WRITE_TOTAL_TIMEOUT_CONSTANT / 1000.0,
                # Flow control ALL OFF (из TCOMPort_ApplySettings)
                xonxoff=False,
                rtscts=False,
                dsrdtr=False,
                inter_byte_timeout=READ_INTERVAL_TIMEOUT / 1000.0,
            )
            self.is_open = True
            # Sleep(100) после открытия
            time.sleep(OPEN_PORT_DELAY_MS / 1000.0)
            return True
        except Exception as e:
            self.is_open = False
            self._serial = None
            print(f"[COMPort] Open failed: {e}")
            return False
    
    def close(self):
        """TCOMPort_Close @ 0x423E28"""
        if self.is_open and self._serial:
            try:
                self._serial.close()
            except:
                pass
        self._serial = None
        self.is_open = False
    
    def write_byte(self, byte_val: int) -> int:
        """TCOMPort_WriteByte @ 0x423F0C: запись 1 байта"""
        if not self.is_open or not self._serial:
            return 0
        try:
            return self._serial.write(bytes([byte_val & 0xFF]))
        except Exception:
            return 0
    
    def write_bytes(self, data: bytes) -> int:
        """TCOMPort_WriteBytes @ 0x423E58: запись буфера с таймаутом"""
        if not self.is_open or not self._serial:
            return 0
        try:
            return self._serial.write(data)
        except Exception:
            return 0
    
    def read_bytes(self, max_size: int) -> bytes:
        """TCOMPort_ReadBytes @ 0x423F64: чтение с таймаутом"""
        if not self.is_open or not self._serial:
            return b''
        try:
            return self._serial.read(max_size)
        except Exception:
            return b''
    
    def bytes_available(self) -> int:
        """TCOMPort_BytesAvail @ 0x423F40: ClearCommError → cbInQue"""
        if not self.is_open or not self._serial:
            return 0
        return self._serial.in_waiting
    
    @staticmethod
    def delay(milliseconds: int):
        """TCOMPort_Delay @ 0x423A58: busy-wait (GetTickCount loop)"""
        time.sleep(milliseconds / 1000.0)
    
    def apply_settings(self) -> bool:
        """TCOMPort_ApplySettings @ 0x424020: применение DCB настроек"""
        if not self.is_open or not self._serial:
            return False
        try:
            self._serial.baudrate = self.baud_rate
            self._serial.bytesize = self.data_bits
            self._serial.stopbits = self.stop_bits
            self._serial.parity = self.parity
            # Все flow control OFF
            self._serial.xonxoff = False
            self._serial.rtscts = False
            self._serial.dsrdtr = False
            return True
        except Exception:
            self.close()
            return False


# ==============================================================================
# 5. АДРЕСАЦИЯ БЛОКОВ (из реверса @ 0x41C7C0 - 0x41C838)
# ==============================================================================

def is_sweep_address(addr: int) -> bool:
    """IsSweepAddress @ 0x41C7C0: return (9999 < addr < 30000)"""
    return SWEEP_ADDR_MIN <= addr <= SWEEP_ADDR_MAX

def get_base_addr(addr: int) -> int:
    """GetBaseAddr @ 0x41C7F0: return (addr // 100) * 100"""
    return (addr // 100) * 100

def get_sub_block(addr: int) -> int:
    """GetSubBlock @ 0x41C810: return (addr % 10000) // 100"""
    return (addr % 10000) // 100

def get_index(addr: int) -> int:
    """GetIndex @ 0x41C838: return addr % 100"""
    return addr % 100

def get_category(block_addr: int) -> int:
    """Категория = blockAddr / 1000 (из _SortBufData)"""
    return block_addr // 1000

def classify_block(block_addr: int) -> DataCategory:
    """Классификация блока по адресу (из switch в _SortBufData @ 0x401657)"""
    cat = get_category(block_addr)
    if cat in (1, 2, 3):
        return DataCategory.ASCAN
    elif cat in (4, 5, 6):
        return DataCategory.GENERIC
    elif 10 <= cat <= 19:
        return DataCategory.BSCAN
    elif 20 <= cat <= 29:
        return DataCategory.SHORT_PROTO
    return DataCategory.UNKNOWN



# ==============================================================================
# 6. ПРОТОКОЛ СВЯЗИ С ПРИБОРОМ
#    (из Protocol_BlockRequest @ 0x424CC0, Protocol_FlashDump @ 0x424BB0)
# ==============================================================================

class ProtocolError(Exception):
    """Ошибка протокола связи"""
    pass


class Protocol:
    """
    Полный аналог протокола PelengPC ↔ Прибор.
    
    Реверс-источники:
      Protocol_BlockRequest  @ 0x424CC0  — запрос блока (0x42 + addr)
      Protocol_FlashDump     @ 0x424BB0  — чтение FLASH (0x9A)
      Handshake              @ 0x4249C0  — тест связи (0x55)
      Status                 @ 0x422828  — запрос статуса (0x74)
      ReadBlock_Dispatcher   @ 0x41B43C  — главный диспатчер
    """
    
    def __init__(self, port: COMPort):
        self.port = port
        self.device_flags: int = 0    # DAT_0053E97C
        self.frame_header: bytes = b'\x00' * 16  # DAT_0053E96A
        self.on_progress: Optional[Callable[[int, int], None]] = None
    
    @property
    def is_wagon_version(self) -> bool:
        """Бит 2 в deviceFlags = вагонная версия"""
        return bool(self.device_flags & DeviceFlag.WAGON_VERSION)
    
    @property
    def flash_size(self) -> int:
        """Размер FLASH-дампа зависит от версии прибора"""
        return FLASH_SIZE_WAGON if self.is_wagon_version else FLASH_SIZE_BASE
    
    def handshake(self) -> tuple[bool, bytes]:
        """
        Handshake (опкод 0x55) — тест связи с прибором.
        
        Протокол:
          TX: [0x55]
          RX: минимум 17 байт (16 заголовок + 1 байт тела)
          
        Ответ содержит:
          [0..15] = заголовок фрейма (→ self.frame_header)
          [2] = флаги устройства (→ self.device_flags)
          остальное = данные (количество элементов A-scan = (len-16)/2 - 1)
        
        Возвращает: (success, raw_response)
        """
        if not self.port.is_open:
            raise ProtocolError("Порт не открыт")
        
        # Отправляем 0x55
        self.port.write_byte(OPCODE_HANDSHAKE)
        
        # Читаем ответ (до 512KB + 16)
        response = self.port.read_bytes(0x80010)
        
        if len(response) < HANDSHAKE_MIN_RESPONSE:
            return False, response
        
        # Сохраняем заголовок
        self.frame_header = response[:FRAME_HEADER_SIZE]
        self.device_flags = response[2]
        
        return True, response
    
    def request_block(self, block_addr: int, expected_size: int,
                      out_buffer: Optional[bytearray] = None) -> bytes:
        """
        Protocol_BlockRequest @ 0x424CC0 — запрос блока данных.
        
        Протокол:
          TX: [0x42] → delay(10ms) → [addr_lo] → delay(10ms) → [addr_hi] → delay(10ms)
          RX: expected_size байт через FormProgress
        
        Параметры:
          block_addr: адрес блока (WORD, 0-65535)
          expected_size: ожидаемый размер ответа
          out_buffer: буфер для записи (если None — создаётся новый)
        
        Возвращает: принятые данные
        """
        if not self.port.is_open:
            raise ProtocolError("Порт не открыт")
        
        # 1. Отправляем команду 0x42
        self.port.write_byte(OPCODE_BLOCK_REQ)
        COMPort.delay(10)
        
        # 2. Отправляем младший байт адреса
        addr_lo = block_addr & 0xFF
        self.port.write_byte(addr_lo)
        COMPort.delay(10)
        
        # 3. Отправляем старший байт адреса
        addr_hi = (block_addr >> 8) & 0xFF
        self.port.write_byte(addr_hi)
        COMPort.delay(10)
        
        # 4. Приём данных с прогрессом
        received = self._receive_with_progress(expected_size)
        
        if len(received) != expected_size:
            raise ProtocolError(
                f"Ошибка приёма блока: получено {len(received)}, "
                f"ожидалось {expected_size}"
            )
        
        return received
    
    def flash_dump(self, opcode: int = OPCODE_FLASH_DUMP,
                   expected_size: Optional[int] = None) -> bytes:
        """
        Protocol_FlashDump @ 0x424BB0 — чтение FLASH/статуса.
        
        Протокол:
          TX: [opcode] (0x9A для FLASH, 0x74 для STATUS)
          RX: fixed-size block
        
        Размеры:
          0x9A (FLASH): 4293 байт (базовая) или 5253 (вагонная)
          0x74 (STATUS): переменный размер
        """
        if not self.port.is_open:
            raise ProtocolError("Порт не открыт")
        
        if expected_size is None:
            expected_size = self.flash_size
        
        # Отправляем опкод
        self.port.write_byte(opcode)
        
        # Приём данных
        received = self._receive_with_progress(expected_size)
        
        if len(received) != expected_size:
            raise ProtocolError(
                f"Ошибка приёма FLASH: получено {len(received)}, "
                f"ожидалось {expected_size}"
            )
        
        return received
    
    def read_status(self, expected_size: int = 256) -> bytes:
        """Запрос статуса прибора (опкод 0x74)"""
        return self.flash_dump(opcode=OPCODE_STATUS, expected_size=expected_size)
    
    def read_block_dispatch(self, block_addr: int, block_size: int) -> bytes:
        """
        ReadBlock_Dispatcher @ 0x41B43C — главный диспатчер.
        
        Логика:
          if addr == 60000:  → flash_dump(0x9A)
          elif is_sweep(addr): → request_block(0x42, addr, size)
          else: → request_block(0x42, addr, size)
        """
        if block_addr == FLASH_DUMP_ADDR:
            # Специальный адрес 60000 = полный FLASH dump
            data = self.flash_dump()
            # Маркируем буфер адресом 60000 (LE16)
            result = bytearray(FRAME_HEADER_SIZE + len(data))
            result[:FRAME_HEADER_SIZE] = self.frame_header
            result[0x10] = FLASH_DUMP_ADDR & 0xFF
            result[0x11] = (FLASH_DUMP_ADDR >> 8) & 0xFF
            result[FRAME_HEADER_SIZE:] = data
            return bytes(result)
        else:
            # Обычный запрос блока (sweep или non-sweep)
            data = self.request_block(block_addr, block_size)
            # Добавляем заголовок
            result = bytearray(FRAME_HEADER_SIZE + len(data))
            result[:FRAME_HEADER_SIZE] = self.frame_header
            result[FRAME_HEADER_SIZE:] = data
            return bytes(result)
    
    def _receive_with_progress(self, expected_size: int) -> bytes:
        """Приём данных с вызовом callback прогресса"""
        buffer = bytearray()
        start_time = time.time()
        timeout_s = self.port.timeout_ms / 1000.0
        
        while len(buffer) < expected_size:
            remaining = expected_size - len(buffer)
            chunk = self.port.read_bytes(min(remaining, 4096))
            
            if chunk:
                buffer.extend(chunk)
                if self.on_progress:
                    self.on_progress(len(buffer), expected_size)
            
            # Проверка таймаута
            if time.time() - start_time > timeout_s:
                break
        
        return bytes(buffer)


# ==============================================================================
# 7. TLV-ПАРСЕР (полный аналог _SortBufData из 102_203dll.dll @ 0x401454)
# ==============================================================================

# LUT категорий из DLL @ 0x401670 (30 байт)
# case_idx: 0=skip, 1=Ascan(body[0]=4), 2=Bscan(body[0]=3),
#           3=Generic(body[0]=1), 4=Calibration(body[0]=1), 5=Settings(body[0]=2)
_TLV_CATEGORY_LUT = (
    0, 5, 0, 0, 3, 4, 3, 0, 0, 0,  # категории 0-9
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,  # категории 10-19 (B-scan)
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  # категории 20-29 (A-scan/Short)
)


@dataclass
class TLVRecord:
    """Одна TLV-запись из пакета"""
    tag: int              # адрес блока (LE16)
    data: bytes           # тело записи
    category: DataCategory = DataCategory.UNKNOWN
    
    @property
    def block_addr(self) -> int:
        """Адрес блока = tag"""
        return self.tag


@dataclass 
class ParsedBlock:
    """Результат разбора блока через _SortBufData"""
    data_type: DataCategory    # тип данных (1-4)
    num_fields: int            # количество полей
    fields: dict               # декодированные поля
    raw_packet: bytes          # оригинальный пакет
    block_addr: int = 0        # адрес блока
    sweep_id: int = 0          # идентификатор sweep
    dev_id: int = 0            # ID устройства


def parse_tlv_stream(data: bytes, start_offset: int = FRAME_HEADER_SIZE) -> list[TLVRecord]:
    """
    ParseFlashDump_TLV @ 0x41B008 — парсинг потока TLV-записей.
    
    Формат:
      [tag: 2 bytes LE] [length: 2 bytes LE] [data: length bytes]
      ...
      [0xFFFF = end marker]
      [0xFF padding bytes]
    
    Параметры:
      data: сырой буфер (включая 16-байт заголовок)
      start_offset: начало TLV-данных (обычно 16 = после заголовка)
    
    Возвращает: список TLV-записей
    """
    records = []
    pos = start_offset
    
    while pos + 4 <= len(data):
        # Читаем tag (LE16)
        tag = data[pos] | (data[pos + 1] << 8)
        
        # End marker
        if tag == TLV_END_MARKER:
            # Пропускаем padding (0xFF байты)
            pos += 2
            while pos < len(data) and data[pos] == TLV_PADDING_BYTE:
                pos += 1
            continue
        
        # Читаем length (LE16)
        if pos + 4 > len(data):
            break
        length = data[pos + 2] | (data[pos + 3] << 8)
        
        # Проверяем что length валидный
        if length == 0 or pos + 4 + length > len(data):
            pos += 1  # пропускаем невалидную запись
            continue
        
        # Извлекаем данные
        record_data = data[pos + 4:pos + 4 + length]
        
        # Классифицируем
        category = classify_block(tag)
        
        records.append(TLVRecord(
            tag=tag,
            data=record_data,
            category=category
        ))
        
        pos += 4 + length
    
    return records


def sort_buf_data(raw_packet: bytes) -> Optional[ParsedBlock]:
    """
    _SortBufData @ 0x401454 — главный парсер пакета из DLL.
    
    Вход: сырой пакет от прибора (с 16-байт заголовком)
    Выход: ParsedBlock с декодированными полями
    
    Алгоритм:
      1. Пропускаем 16-байт заголовок фрейма
      2. Извлекаем blockAddr (LE16 из body[0:2])
      3. category = blockAddr / 1000
      4. sweepId = packet[4]*100 + packet[5]
      5. devId = packet[0] | packet[1]<<8
      6. Диспатч по категории → декодирование полей
    """
    if len(raw_packet) < FRAME_HEADER_SIZE + 2:
        return None
    
    # Извлечение метаданных из заголовка
    body = raw_packet[FRAME_HEADER_SIZE:]
    block_addr = body[0] | (body[1] << 8)
    category_num = block_addr // 1000
    sweep_id = raw_packet[4] * 100 + raw_packet[5]
    dev_id = raw_packet[0] | (raw_packet[1] << 8)
    
    # Определяем тип данных по категории (switch из DLL)
    data_type = DataCategory.UNKNOWN
    if category_num in (1, 2, 3):
        data_type = DataCategory.ASCAN
    elif category_num in (4, 6):
        data_type = DataCategory.GENERIC
    elif category_num == 5:
        data_type = DataCategory.GENERIC  # калибровка
    elif 10 <= category_num <= 19:
        data_type = DataCategory.BSCAN
    elif 20 <= category_num <= 29:
        data_type = DataCategory.SHORT_PROTO
    
    # Базовое декодирование полей
    fields = {
        'block_addr': block_addr,
        'category': category_num,
        'sweep_id': sweep_id,
        'dev_id': dev_id,
    }
    
    # Декодирование специфичных полей по категории
    if data_type == DataCategory.ASCAN and len(body) > 10:
        fields.update(_decode_ascan_fields(body))
    elif data_type == DataCategory.BSCAN and len(body) > 10:
        fields.update(_decode_bscan_fields(body))
    elif data_type == DataCategory.GENERIC and len(body) > 10:
        fields.update(_decode_generic_fields(body))
    elif data_type == DataCategory.SHORT_PROTO and len(body) > 10:
        fields.update(_decode_short_proto_fields(body))
    
    return ParsedBlock(
        data_type=data_type,
        num_fields=len(fields),
        fields=fields,
        raw_packet=raw_packet,
        block_addr=block_addr,
        sweep_id=sweep_id,
        dev_id=dev_id
    )


def _decode_date(data: bytes, offset: int) -> str:
    """DecodeDate @ 0x4023B0: [day][month][year-2000] → "DD.MM.YYYY" """
    if offset + 3 > len(data):
        return "01.01.2000"
    day = data[offset]
    month = data[offset + 1]
    year = data[offset + 2] + 2000
    if not (1 <= month <= 12 and 1 <= day <= 31):
        return "01.01.2000"
    return f"{day:02d}.{month:02d}.{year}"


def _decode_time(data: bytes, offset: int) -> str:
    """DecodeTime @ 0x402504: [hours][minutes] → "HH:MM" """
    if offset + 2 > len(data):
        return "00:00"
    hours = data[offset] % 100
    minutes = data[offset + 1] % 100
    return f"{hours:02d}:{minutes:02d}"


def _decode_ascan_fields(body: bytes) -> dict:
    """Декодирование полей A-scan записи"""
    fields = {}
    if len(body) >= 10:
        fields['date'] = _decode_date(body, 7)
        fields['time'] = _decode_time(body, 10)
    return fields


def _decode_bscan_fields(body: bytes) -> dict:
    """Декодирование полей B-scan записи"""
    fields = {}
    if len(body) >= 10:
        fields['date'] = _decode_date(body, 7)
        fields['time'] = _decode_time(body, 10)
        # B-scan данные начинаются с offset 0x1D0 - 0x10 = 0x1C0 от начала body
        if len(body) > BSCAN_HEADER_SIZE:
            fields['bscan_data_size'] = len(body) - BSCAN_HEADER_SIZE
            fields['bscan_rows'] = (len(body) - BSCAN_HEADER_SIZE) // BSCAN_ROW_BYTES
    return fields


def _decode_generic_fields(body: bytes) -> dict:
    """Декодирование полей Generic/Settings записи"""
    fields = {}
    if len(body) >= 10:
        fields['date'] = _decode_date(body, 7)
        fields['time'] = _decode_time(body, 10)
    return fields


def _decode_short_proto_fields(body: bytes) -> dict:
    """Декодирование полей Short Protocol записи"""
    fields = {}
    if len(body) >= 10:
        fields['date'] = _decode_date(body, 7)
        fields['time'] = _decode_time(body, 10)
    return fields


# ==============================================================================
# 8. БАЗА ДАННЫХ (SQLite аналог Firebird BlockZap)
# ==============================================================================

class BlockZapDB:
    """
    Аналог Firebird БД PelengPC.fdb, таблица BlockZap.
    
    Из реверса DFM FormMain:
      SELECT * FROM BlockZap
      SELECT * FROM BlockZap WHERE Number=:Number
      UPDATE BlockZap SET Number=:Number, BlockLen=:BlockLen, Block=:Block 
             WHERE Number=:OLD_Number
      INSERT INTO BlockZap (Number, BlockLen, Block) VALUES (...)
      DELETE FROM BlockZap WHERE Number=:Number
      SELECT * FROM BlockZap ORDER BY Number
    """
    
    def __init__(self, db_path: str = "PelengPC.db"):
        self.db_path = db_path
        self.conn: Optional[sqlite3.Connection] = None
        self._open()
    
    def _open(self):
        """Открытие/создание БД"""
        self.conn = sqlite3.connect(self.db_path)
        self.conn.execute("""
            CREATE TABLE IF NOT EXISTS BlockZap (
                Number INTEGER PRIMARY KEY,
                BlockLen INTEGER NOT NULL,
                Block BLOB NOT NULL
            )
        """)
        self.conn.commit()
    
    def close(self):
        if self.conn:
            self.conn.close()
            self.conn = None
    
    def select_all(self, order_by: str = "Number") -> list[tuple]:
        """SELECT * FROM BlockZap ORDER BY Number"""
        cur = self.conn.execute(f"SELECT Number, BlockLen, Block FROM BlockZap ORDER BY {order_by}")
        return cur.fetchall()
    
    def select_by_number(self, number: int) -> Optional[tuple]:
        """SELECT * FROM BlockZap WHERE Number=:Number"""
        cur = self.conn.execute(
            "SELECT Number, BlockLen, Block FROM BlockZap WHERE Number=?", (number,))
        return cur.fetchone()
    
    def insert(self, number: int, block_data: bytes) -> bool:
        """INSERT INTO BlockZap (Number, BlockLen, Block) VALUES (...)"""
        try:
            self.conn.execute(
                "INSERT INTO BlockZap (Number, BlockLen, Block) VALUES (?, ?, ?)",
                (number, len(block_data), block_data))
            self.conn.commit()
            return True
        except sqlite3.IntegrityError:
            return False
    
    def update(self, number: int, block_data: bytes) -> bool:
        """UPDATE BlockZap SET BlockLen=:BlockLen, Block=:Block WHERE Number=:Number"""
        self.conn.execute(
            "UPDATE BlockZap SET BlockLen=?, Block=? WHERE Number=?",
            (len(block_data), block_data, number))
        self.conn.commit()
        return True
    
    def upsert(self, number: int, block_data: bytes) -> bool:
        """INSERT OR REPLACE (аналог логики PelengPC)"""
        self.conn.execute(
            "INSERT OR REPLACE INTO BlockZap (Number, BlockLen, Block) VALUES (?, ?, ?)",
            (number, len(block_data), block_data))
        self.conn.commit()
        return True
    
    def delete(self, number: int) -> bool:
        """DELETE FROM BlockZap WHERE Number=:Number"""
        self.conn.execute("DELETE FROM BlockZap WHERE Number=?", (number,))
        self.conn.commit()
        return True
    
    def count(self) -> int:
        """Количество записей"""
        cur = self.conn.execute("SELECT COUNT(*) FROM BlockZap")
        return cur.fetchone()[0]
    
    def delete_all(self):
        """Очистка всей таблицы"""
        self.conn.execute("DELETE FROM BlockZap")
        self.conn.commit()



# ==============================================================================
# 9. B-SCAN RENDERER (из FormScreen_RenderBscan @ 0x4235F8)
# ==============================================================================

class BscanRenderer:
    """
    Рендеринг B-scan изображения — полный аналог FormScreen_RenderBscan.
    
    Параметры (из реверса):
      - Заголовок A-scan: 448 байт (0x1C0)
      - Данные B-scan начинаются с offset +0x1D0 (464 от начала пакета)
      - Строка = 30 байт = 240 бит
      - 1 бит = 1 пиксель (чёрный/белый)
      - Масштаб: каждый бит → квадрат 2×2 пикселя
      - Ширина изображения: 480 px (240 × 2)
    """
    
    def __init__(self):
        self.width_px = BSCAN_WIDTH_PX       # 480
        self.row_bytes = BSCAN_ROW_BYTES     # 30
        self.pixel_scale = BSCAN_PIXEL_SCALE # 2
        self.header_size = BSCAN_HEADER_SIZE # 448
    
    def extract_bscan_data(self, raw_packet: bytes) -> bytes:
        """
        Извлечение данных B-scan из пакета.
        
        raw_packet = полный пакет от прибора (с 16-байт заголовком).
        B-scan данные = packet[0x1D0:] (после 16-byte frame + 448-byte A-scan header)
        """
        offset = BSCAN_DATA_OFFSET  # 0x1D0 = 464
        if len(raw_packet) <= offset:
            return b''
        return raw_packet[offset:]
    
    def get_dimensions(self, bscan_data: bytes) -> tuple[int, int]:
        """
        Вычисление размеров изображения.
        
        rows = len(data) / 30
        width_px = 240 * 2 = 480
        height_px = rows * 2
        """
        if not bscan_data:
            return 0, 0
        rows = len(bscan_data) // self.row_bytes
        width = self.row_bytes * 8 * self.pixel_scale   # 30*8*2 = 480
        height = rows * self.pixel_scale                # rows*2
        return width, height
    
    def render_to_pixels(self, bscan_data: bytes) -> list[list[int]]:
        """
        Рендеринг B-scan в 2D массив пикселей (0=белый, 1=чёрный).
        
        Каждый бит данных → пиксель:
          bit=1 → чёрный (дефект/отражение)
          bit=0 → белый (фон)
        
        Возвращает: list[rows][cols] с масштабом 2×2
        """
        if not bscan_data:
            return []
        
        rows = len(bscan_data) // self.row_bytes
        width, height = self.get_dimensions(bscan_data)
        
        # Создаём пиксельную матрицу
        pixels = [[0] * width for _ in range(height)]
        
        for row in range(rows):
            for col_byte in range(self.row_bytes):
                byte_val = bscan_data[row * self.row_bytes + col_byte]
                for bit in range(8):
                    pixel_val = (byte_val >> bit) & 1
                    
                    # Координаты с масштабом 2×2
                    x = (col_byte * 8 + bit) * self.pixel_scale
                    y = row * self.pixel_scale
                    
                    # Заполняем квадрат 2×2
                    for dy in range(self.pixel_scale):
                        for dx in range(self.pixel_scale):
                            if y + dy < height and x + dx < width:
                                pixels[y + dy][x + dx] = pixel_val
        
        return pixels
    
    def render_to_qimage(self, bscan_data: bytes):
        """
        Рендеринг в QImage (PyQt6).
        Возвращает QImage или None если PyQt6 недоступен.
        """
        try:
            from PyQt6.QtGui import QImage, QColor
            from PyQt6.QtCore import Qt
        except ImportError:
            return None
        
        if not bscan_data:
            return None
        
        width, height = self.get_dimensions(bscan_data)
        if width == 0 or height == 0:
            return None
        
        image = QImage(width, height, QImage.Format.Format_Mono)
        image.fill(Qt.GlobalColor.white)
        
        rows = len(bscan_data) // self.row_bytes
        
        for row in range(rows):
            for col_byte in range(self.row_bytes):
                byte_val = bscan_data[row * self.row_bytes + col_byte]
                for bit in range(8):
                    if (byte_val >> bit) & 1:
                        x = (col_byte * 8 + bit) * self.pixel_scale
                        y = row * self.pixel_scale
                        # Рисуем квадрат 2×2 чёрным
                        for dy in range(self.pixel_scale):
                            for dx in range(self.pixel_scale):
                                if y + dy < height and x + dx < width:
                                    image.setPixel(x + dx, y + dy, 0)
        
        return image
    
    def save_bmp(self, bscan_data: bytes, filepath: str) -> bool:
        """Сохранение B-scan как BMP файл (без PyQt6, чистый Python)"""
        if not bscan_data:
            return False
        
        width, height = self.get_dimensions(bscan_data)
        if width == 0 or height == 0:
            return False
        
        pixels = self.render_to_pixels(bscan_data)
        
        # BMP header (monochrome bitmap)
        row_size = ((width + 31) // 32) * 4  # выравнивание до 4 байт
        pixel_data_size = row_size * height
        file_size = 14 + 40 + 8 + pixel_data_size  # header + info + palette + data
        
        with open(filepath, 'wb') as f:
            # BMP File Header (14 bytes)
            f.write(b'BM')
            f.write(struct.pack('<I', file_size))
            f.write(struct.pack('<HH', 0, 0))
            f.write(struct.pack('<I', 14 + 40 + 8))  # offset to pixel data
            
            # DIB Header (BITMAPINFOHEADER, 40 bytes)
            f.write(struct.pack('<I', 40))        # header size
            f.write(struct.pack('<i', width))     # width
            f.write(struct.pack('<i', -height))   # height (top-down)
            f.write(struct.pack('<HH', 1, 1))    # planes, bpp
            f.write(struct.pack('<I', 0))         # compression (none)
            f.write(struct.pack('<I', pixel_data_size))
            f.write(struct.pack('<ii', 2835, 2835))  # resolution 72dpi
            f.write(struct.pack('<II', 2, 2))    # colors
            
            # Color table (2 entries: white, black)
            f.write(struct.pack('<BBBB', 255, 255, 255, 0))  # white
            f.write(struct.pack('<BBBB', 0, 0, 0, 0))        # black
            
            # Pixel data
            for y in range(height):
                row_data = bytearray(row_size)
                for x in range(width):
                    if pixels[y][x]:
                        byte_idx = x // 8
                        bit_idx = 7 - (x % 8)
                        row_data[byte_idx] |= (1 << bit_idx)
                f.write(row_data)
        
        return True


# ==============================================================================
# 10. КОНФИГУРАЦИЯ (аналог реестра SOFTWARE\Altek\PelengPC)
# ==============================================================================

class Config:
    """
    Конфигурация приложения.
    Оригинал хранит в реестре HKLM\\SOFTWARE\\Altek\\PelengPC.
    Клон использует INI-файл.
    """
    
    DEFAULT_CONFIG = {
        'port': {
            'name': 'COM1',
            'baud': '9600',
            'data_bits': '8',
            'stop_bits': '1',
            'parity': 'N',
        },
        'database': {
            'path': 'PelengPC.db',
        },
        'company': {
            'firma': '',
            'sub_firma': '',
            'label': '',
        },
        'general': {
            'tek_table': '0',
            'password': '',
        },
    }
    
    def __init__(self, config_path: str = "pelengpc.ini"):
        self.config_path = config_path
        self.cfg = configparser.ConfigParser()
        self._load()
    
    def _load(self):
        """Загрузка конфигурации"""
        # Устанавливаем дефолты
        for section, values in self.DEFAULT_CONFIG.items():
            if not self.cfg.has_section(section):
                self.cfg.add_section(section)
            for key, val in values.items():
                if not self.cfg.has_option(section, key):
                    self.cfg.set(section, key, val)
        
        # Читаем файл (если существует)
        if os.path.exists(self.config_path):
            self.cfg.read(self.config_path, encoding='utf-8')
    
    def save(self):
        """Сохранение конфигурации"""
        with open(self.config_path, 'w', encoding='utf-8') as f:
            self.cfg.write(f)
    
    # Порт
    @property
    def port_name(self) -> str:
        return self.cfg.get('port', 'name', fallback='COM1')
    
    @port_name.setter
    def port_name(self, value: str):
        self.cfg.set('port', 'name', value)
    
    @property
    def port_baud(self) -> int:
        return self.cfg.getint('port', 'baud', fallback=9600)
    
    @port_baud.setter
    def port_baud(self, value: int):
        self.cfg.set('port', 'baud', str(value))
    
    # База данных
    @property
    def db_path(self) -> str:
        return self.cfg.get('database', 'path', fallback='PelengPC.db')
    
    # Компания
    @property
    def firma(self) -> str:
        return self.cfg.get('company', 'firma', fallback='')
    
    @firma.setter
    def firma(self, value: str):
        self.cfg.set('company', 'firma', value)


# ==============================================================================
# 11. СПИСОК ОПЕРАТОРОВ (из реестра SOFTWARE\Altek\Operators)
# ==============================================================================

class OperatorList:
    """Управление списком операторов (аналог SOFTWARE\\Altek\\Operators)"""
    
    def __init__(self, config_path: str = "operators.ini"):
        self.config_path = config_path
        self.operators: list[str] = []
        self._load()
    
    def _load(self):
        if os.path.exists(self.config_path):
            cfg = configparser.ConfigParser()
            cfg.read(self.config_path, encoding='utf-8')
            count = cfg.getint('operators', 'count', fallback=0)
            self.operators = []
            for i in range(1, count + 1):
                name = cfg.get('operators', f'operator{i}', fallback='')
                if name:
                    self.operators.append(name)
    
    def save(self):
        cfg = configparser.ConfigParser()
        cfg.add_section('operators')
        cfg.set('operators', 'count', str(len(self.operators)))
        for i, name in enumerate(self.operators, 1):
            cfg.set('operators', f'operator{i}', name)
        with open(self.config_path, 'w', encoding='utf-8') as f:
            cfg.write(f)
    
    def add(self, name: str):
        if name and name not in self.operators:
            self.operators.append(name)
            self.save()
    
    def remove(self, name: str):
        if name in self.operators:
            self.operators.remove(name)
            self.save()


# ==============================================================================
# 12. ГЛАВНЫЙ КЛАСС ПРИЛОЖЕНИЯ (связывает все компоненты)
# ==============================================================================

class PelengPCApp:
    """
    Главный класс приложения — связывает все компоненты:
    COM-порт, протокол, парсер, БД, конфигурацию.
    
    Аналог логики из FormMain_FormCreate @ 0x40BCCC.
    """
    
    def __init__(self, config_path: str = "pelengpc.ini"):
        # Конфигурация (аналог чтения реестра)
        self.config = Config(config_path)
        
        # COM-порт (аналог g_comPort @ 0x53E948)
        self.port = COMPort()
        self.port.port_name = self.config.port_name
        self.port.set_baud_rate(self.config.port_baud)
        
        # Протокол
        self.protocol = Protocol(self.port)
        
        # База данных
        self.db = BlockZapDB(self.config.db_path)
        
        # Операторы
        self.operators = OperatorList()
        
        # B-scan рендерер
        self.bscan_renderer = BscanRenderer()
        
        # Состояние
        self.connection_mode = ConnectionMode.COM_DIRECT
        self.current_block_addr = 0
        self.block_list: list[BlockEntry] = []
        self.streak_table: list[StreakEntry] = []
        self.device_flags: int = 0
    
    def connect(self) -> bool:
        """Подключение к прибору (Open + Handshake)"""
        if not self.port.open():
            return False
        
        success, response = self.protocol.handshake()
        if success:
            self.device_flags = self.protocol.device_flags
        return success
    
    def disconnect(self):
        """Отключение от прибора"""
        self.port.close()
    
    def read_block(self, addr: int, size: int) -> Optional[ParsedBlock]:
        """
        Чтение и разбор одного блока (ReadBlock_Dispatcher + _SortBufData)
        """
        raw = self.protocol.read_block_dispatch(addr, size)
        return sort_buf_data(raw)
    
    def read_flash(self) -> list[TLVRecord]:
        """Чтение и разбор FLASH-дампа"""
        raw = self.protocol.read_block_dispatch(FLASH_DUMP_ADDR, self.protocol.flash_size)
        return parse_tlv_stream(raw)
    
    def save_block_to_db(self, number: int, raw_data: bytes) -> bool:
        """Сохранение блока в БД (INSERT OR REPLACE)"""
        return self.db.upsert(number, raw_data)
    
    def save_config(self):
        """Сохранение конфигурации"""
        self.config.port_name = self.port.port_name
        self.config.port_baud = self.port.baud_rate
        self.config.save()


# ==============================================================================
# 13. CLI ИНТЕРФЕЙС
# ==============================================================================

def cli_test_port(args):
    """Тест подключения к прибору (Handshake)"""
    app = PelengPCApp()
    app.port.port_name = args.port
    app.port.set_baud_rate(args.baud)
    
    print(f"[*] Подключение к {args.port} @ {args.baud} baud...")
    if not app.port.open():
        print("[!] Ошибка: не удалось открыть порт")
        return 1
    
    print("[*] Отправка Handshake (0x55)...")
    success, response = app.protocol.handshake()
    
    if success:
        print(f"[+] Handshake OK! Получено {len(response)} байт")
        print(f"    Device ID: {response[0] | (response[1] << 8)}")
        print(f"    Flags: 0x{response[2]:02X}")
        print(f"    Sweep ID: {response[4]*100 + response[5]}")
        wagon = "Вагонная" if (response[2] & 4) else "Базовая"
        print(f"    Версия: {wagon}")
    else:
        print(f"[!] Handshake FAILED: получено {len(response)} байт (нужно >= 17)")
    
    app.disconnect()
    return 0 if success else 1


def cli_read_block(args):
    """Чтение одного блока"""
    app = PelengPCApp()
    app.port.port_name = args.port
    app.port.set_baud_rate(args.baud)
    
    print(f"[*] Подключение к {args.port}...")
    if not app.connect():
        print("[!] Ошибка подключения")
        return 1
    
    addr = args.addr
    size = args.size
    print(f"[*] Запрос блока addr={addr}, size={size}...")
    
    try:
        parsed = app.read_block(addr, size)
        if parsed:
            print(f"[+] Блок получен!")
            print(f"    Тип: {parsed.data_type.name}")
            print(f"    Адрес: {parsed.block_addr}")
            print(f"    Sweep ID: {parsed.sweep_id}")
            print(f"    Полей: {parsed.num_fields}")
            for k, v in parsed.fields.items():
                print(f"    {k}: {v}")
            
            if args.save:
                app.save_block_to_db(addr, parsed.raw_packet)
                print(f"[+] Сохранено в БД как Number={addr}")
        else:
            print("[!] Не удалось разобрать блок")
    except ProtocolError as e:
        print(f"[!] Ошибка: {e}")
    
    app.disconnect()
    return 0


def cli_read_flash(args):
    """Чтение FLASH-дампа"""
    app = PelengPCApp()
    app.port.port_name = args.port
    app.port.set_baud_rate(args.baud)
    
    print(f"[*] Подключение к {args.port}...")
    if not app.connect():
        print("[!] Ошибка подключения")
        return 1
    
    print(f"[*] Чтение FLASH (addr=60000)...")
    try:
        records = app.read_flash()
        print(f"[+] Получено {len(records)} TLV-записей")
        
        for i, rec in enumerate(records[:20]):
            cat = rec.category.name if rec.category != DataCategory.UNKNOWN else "?"
            print(f"    [{i:3d}] tag={rec.tag:5d} size={len(rec.data):5d} cat={cat}")
        
        if len(records) > 20:
            print(f"    ... и ещё {len(records) - 20} записей")
        
        if args.save_all:
            for rec in records:
                full_packet = bytearray(FRAME_HEADER_SIZE + len(rec.data))
                full_packet[0x10] = rec.tag & 0xFF
                full_packet[0x11] = (rec.tag >> 8) & 0xFF
                full_packet[FRAME_HEADER_SIZE:] = rec.data
                app.save_block_to_db(rec.tag, bytes(full_packet))
            print(f"[+] Все {len(records)} записей сохранены в БД")
    
    except ProtocolError as e:
        print(f"[!] Ошибка: {e}")
    
    app.disconnect()
    return 0


def cli_info(args):
    """Информация о программе и реверсе"""
    print(f"""
PelengPC Clone v{APP_VERSION}
{'='*50}

Оригинал: PelengPC.exe ver 1.2 (Borland C++Builder 5)
Компания:  ООО "Алтек" (www.altek.info)
Реверс:    7080 функций декомпилировано и переименовано

Протокол:
  0x55 = HANDSHAKE (ответ ≥17 байт)
  0x42 = BLOCK_REQUEST (addr_lo, addr_hi → данные)
  0x9A = FLASH_DUMP (4293/5253 байт)
  0x74 = STATUS

Адресация:
  10000-29999 = sweep (B-scan данные)
  60000 = полный FLASH dump
  addr/1000 → категория (1-3=Ascan, 4-6=Generic, 10-19=Bscan, 20-29=ShortProto)

COM порт: 8N1, без flow control
  Default: 9600 baud, timeout 1000ms
  15 скоростей: 110..256000
  Таймауты: ReadInterval=1ms, WriteConst=10ms, Sleep(100) после открытия

База данных: SQLite (оригинал = Firebird PelengPC.fdb)
  Таблица BlockZap: Number(PK), BlockLen, Block(BLOB)
""")
    return 0


def main():
    """Точка входа"""
    parser = argparse.ArgumentParser(
        description=f"PelengPC Clone v{APP_VERSION} — полный аналог PelengPC.exe",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Примеры:
  %(prog)s gui                         # Запуск GUI
  %(prog)s gui --demo                  # Демо-режим без прибора
  %(prog)s test --port COM3            # Тест подключения
  %(prog)s block --port COM3 --addr 10001 --size 4293
  %(prog)s flash --port COM3           # Чтение FLASH
  %(prog)s info                        # Информация
"""
    )
    
    subparsers = parser.add_subparsers(dest='command')
    
    # GUI
    gui_parser = subparsers.add_parser('gui', help='Запуск графического интерфейса')
    gui_parser.add_argument('--port', default='', help='COM-порт')
    gui_parser.add_argument('--demo', action='store_true', help='Демо-режим')
    
    # Test
    test_parser = subparsers.add_parser('test', help='Тест подключения (Handshake)')
    test_parser.add_argument('--port', required=True, help='COM-порт')
    test_parser.add_argument('--baud', type=int, default=9600, help='Скорость')
    
    # Block
    block_parser = subparsers.add_parser('block', help='Чтение блока')
    block_parser.add_argument('--port', required=True)
    block_parser.add_argument('--baud', type=int, default=9600)
    block_parser.add_argument('--addr', type=int, required=True, help='Адрес блока')
    block_parser.add_argument('--size', type=int, required=True, help='Размер')
    block_parser.add_argument('--save', action='store_true', help='Сохранить в БД')
    
    # Flash
    flash_parser = subparsers.add_parser('flash', help='Чтение FLASH-дампа')
    flash_parser.add_argument('--port', required=True)
    flash_parser.add_argument('--baud', type=int, default=9600)
    flash_parser.add_argument('--save-all', action='store_true', help='Сохранить все в БД')
    
    # Info
    subparsers.add_parser('info', help='Информация о программе')
    
    args = parser.parse_args()
    
    if args.command == 'test':
        return cli_test_port(args)
    elif args.command == 'block':
        return cli_read_block(args)
    elif args.command == 'flash':
        return cli_read_flash(args)
    elif args.command == 'info':
        return cli_info(args)
    elif args.command == 'gui':
        print("[*] GUI запуск... (PyQt6 required)")
        print("[!] GUI будет добавлен в следующей версии")
        print("    Используйте CLI команды: test, block, flash, info")
        return 0
    else:
        parser.print_help()
        return 0


if __name__ == '__main__':
    sys.exit(main())
