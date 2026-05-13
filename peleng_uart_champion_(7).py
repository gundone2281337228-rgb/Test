# -*- coding: utf-8 -*-
"""
peleng_uart_champion.py — выгрузка отчётов УД2-102 через USB-COM в один файл.

Эталонная логика юзера (даёт 218 записей за ~57 c) сохранена 1:1:
- send_precision: побайтовая отправка с паузой 4.8 мс;
- read_precision: 2 байта головы → если `\\xFD\\xFF` → пусто, иначе +84 байта tail;
- streak-обрыв: 15 пустых подряд без хитов / 2 пустых после первого хита;
- бaзы 10100..13400 шагом 100, по 100 оффсетов на каждую;
- smart_decode: ищет дату «1..31, 1..12, 15..40, 0..23, 0..59», парсит относительно неё.

Доработки (минимум, чтобы не сломать логику):
1) handshake `\\x55` ОДИН раз перед сканом → `device_no = handshake[0:2] LE16`;
2) classify_type: 24667 / 57435 → «ось РУ-1Ш» (по дампу прибора 4428);
3) COM-порт выбирается в GUI (раньше был hardcoded);
4) Экспорт CSV (как у юзера) + SQL (как просили в предыдущих фазах).

ДОБАВЛЕНО (после полного реверса PelengPC_ver1_2.exe — см. PROTOCOL.md):
5) Кнопка «🛰 Протокол» открывает отдельное окно для ПРЯМОЙ работы с
   протоколом УД2-102: команды 0x55 / 0x42 LL HH / 0x9a с показом сырого
   hex-ответа, парсингом 16-байтного device-id заголовка, поиском индексов
   записей в дампе по сигнатуре `<idx_le> 56 00 <type>`. Не мешает
   эталонному скану — отдельный путь.
6) Опциональный Win32-backend (через ctypes на kernel32) — точное
   воспроизведение того, что делает PelengPC.exe: \\\\.\\COMx,
   dwShareMode=0, DCB.flags=0, COMMTIMEOUTS={1,0,1,0,10}, busy-wait
   GetTickCount-делей между байтами. По умолчанию выключен — основной
   скан остаётся на pyserial (он у вас даёт 218 записей).

Внешняя зависимость: только pyserial (без неё работа с COM физически невозможна).
"""

from __future__ import annotations

import csv
import json
import os
import struct
import sys
import threading
import time
import tkinter as tk
from tkinter import ttk, messagebox, filedialog

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    sys.stderr.write("Требуется pyserial: pip install pyserial\n")
    sys.exit(2)


# ============================================================================
#                              КОНФИГУРАЦИЯ
# ============================================================================
BAUD = 19200
DEFAULT_CSV = "DEFECTO_CHAMPION_STABLE.csv"
DEFAULT_SQL = "DEFECTO_CHAMPION_STABLE.sql"

# ЭТАЛОННЫЕ тайминги УД2-102: трогать НЕЛЬЗЯ — иначе теряются записи.
# Проверено на реальном приборе: даже -10 мс на empty_timeout даёт пропуски.
GAP_S = 0.0048
EMPTY_TIMEOUT_DEFAULT = 0.028
TAIL_TIMEOUT = 0.150
HANDSHAKE_TIMEOUT = 1.5

# Inter-byte gap, который PelengPC.exe реально использует (fcn.00424cc0):
# busy-wait через GetTickCount, 10 мс между байтами 0x42 LL HH.
# В нашем pyserial-пути GAP_S = 4.8 мс работает быстрее без потерь.
EXE_INTER_BYTE_MS = 10

# Перебирающий флаг для бенчмарка: если True, send_precision() использует
# busy-wait через perf_counter() вместо time.sleep(). На Windows time.sleep
# имеет дефолтную гранулярность 15.6 мс — то есть time.sleep(0.0048)
# фактически спит ~15.6 мс, не 4.8 мс. Busy-wait даёт точные тайминги
# (как exe через GetTickCount), но грузит CPU. По умолчанию выключен —
# включается только в режиме «🏁 БЕНЧМАРК» / «Win32-точные тайминги».
BUSY_WAIT_GAP = False
# Текущий gap (сек) — переменная чтобы бенчмарк мог переключать без
# изменения GAP_S. По умолчанию = GAP_S.
ACTIVE_GAP_S = GAP_S


def _busy_wait_until(deadline_s: float):
    """Активное ожидание до `time.perf_counter() == deadline_s`.

    Эквивалент `do { } while (GetTickCount() < deadline)` который exe
    использует в `fcn.00423a58` для inter-byte gap'а 0x42 LL HH.
    На Windows гранулярность time.sleep по умолчанию 15.6 мс — для
    наших 4.8/10 мс это слишком много. perf_counter работает с
    гранулярностью QueryPerformanceCounter (микросекунды).
    """
    while time.perf_counter() < deadline_s:
        pass


def _gap_wait(gap_s: float):
    """Универсальный inter-byte gap. Уважает глобальный BUSY_WAIT_GAP."""
    if BUSY_WAIT_GAP:
        _busy_wait_until(time.perf_counter() + gap_s)
    else:
        time.sleep(gap_s)


# ============================================================================
#       WIN32 BACKEND — точное воспроизведение PelengPC_ver1_2.exe
#       (использует kernel32 через ctypes, БЕЗ pyserial; только на Windows)
#       См. PROTOCOL.md, разделы 3..7.
# ============================================================================
import ctypes
import ctypes.wintypes as wt
import platform

_IS_WINDOWS = sys.platform.startswith("win") or platform.system() == "Windows"

GENERIC_READ  = 0x80000000
GENERIC_WRITE = 0x40000000
OPEN_EXISTING = 3
INVALID_HANDLE_VALUE_INT = wt.HANDLE(-1).value & 0xFFFFFFFF


class _DCB(ctypes.Structure):
    _fields_ = [
        ("DCBlength", wt.DWORD), ("BaudRate", wt.DWORD),
        ("flags",     wt.DWORD), ("wReserved", wt.WORD),
        ("XonLim",    wt.WORD),  ("XoffLim",   wt.WORD),
        ("ByteSize",  wt.BYTE),  ("Parity",    wt.BYTE),
        ("StopBits",  wt.BYTE),
        ("XonChar",   ctypes.c_char), ("XoffChar", ctypes.c_char),
        ("ErrorChar", ctypes.c_char), ("EofChar",  ctypes.c_char),
        ("EvtChar",   ctypes.c_char), ("wReserved1", wt.WORD),
    ]


class _COMMTIMEOUTS(ctypes.Structure):
    _fields_ = [
        ("ReadIntervalTimeout",         wt.DWORD),
        ("ReadTotalTimeoutMultiplier",  wt.DWORD),
        ("ReadTotalTimeoutConstant",    wt.DWORD),
        ("WriteTotalTimeoutMultiplier", wt.DWORD),
        ("WriteTotalTimeoutConstant",   wt.DWORD),
    ]


class _COMSTAT(ctypes.Structure):
    _fields_ = [("flags", wt.DWORD),
                ("cbInQue", wt.DWORD), ("cbOutQue", wt.DWORD)]


def _bind_kernel32():
    if not _IS_WINDOWS:
        return None
    k = ctypes.WinDLL("kernel32", use_last_error=True)
    k.CreateFileA.argtypes = [wt.LPCSTR, wt.DWORD, wt.DWORD,
                              ctypes.c_void_p, wt.DWORD, wt.DWORD, wt.HANDLE]
    k.CreateFileA.restype  = wt.HANDLE
    k.CloseHandle.argtypes = [wt.HANDLE]
    k.CloseHandle.restype  = wt.BOOL
    k.GetCommState.argtypes = [wt.HANDLE, ctypes.POINTER(_DCB)]
    k.GetCommState.restype  = wt.BOOL
    k.SetCommState.argtypes = [wt.HANDLE, ctypes.POINTER(_DCB)]
    k.SetCommState.restype  = wt.BOOL
    k.SetCommTimeouts.argtypes = [wt.HANDLE, ctypes.POINTER(_COMMTIMEOUTS)]
    k.SetCommTimeouts.restype  = wt.BOOL
    k.ClearCommError.argtypes = [wt.HANDLE,
                                 ctypes.POINTER(wt.DWORD),
                                 ctypes.POINTER(_COMSTAT)]
    k.ClearCommError.restype  = wt.BOOL
    k.ReadFile.argtypes  = [wt.HANDLE, ctypes.c_void_p, wt.DWORD,
                            ctypes.POINTER(wt.DWORD), ctypes.c_void_p]
    k.ReadFile.restype   = wt.BOOL
    k.WriteFile.argtypes = [wt.HANDLE, ctypes.c_void_p, wt.DWORD,
                            ctypes.POINTER(wt.DWORD), ctypes.c_void_p]
    k.WriteFile.restype  = wt.BOOL
    k.GetTickCount.argtypes = []
    k.GetTickCount.restype  = wt.DWORD
    k.Sleep.argtypes = [wt.DWORD]
    k.Sleep.restype  = None
    return k


_K32 = _bind_kernel32()


def _busy_wait_ms(ms: int) -> None:
    """fcn.00423a58 — busy-wait через GetTickCount.
    На Linux сваливается в time.sleep (только для отладки)."""
    if _IS_WINDOWS and _K32 is not None:
        start = _K32.GetTickCount()
        while (_K32.GetTickCount() - start) < ms:
            pass
    else:
        time.sleep(ms / 1000.0)


class Win32ComPort:
    """Точное воспроизведение TCOMPort из PelengPC.exe (см. PROTOCOL.md §3..7).

    Только на Windows. На других платформах open() кидает RuntimeError.
    """

    def __init__(self, port: str, baud: int = 19200, timeout_ms: int = 1500,
                 inter_byte_ms: int = EXE_INTER_BYTE_MS):
        self.port_name   = port
        self.baud        = baud
        self.timeout_ms  = timeout_ms
        self.inter_byte_ms = inter_byte_ms
        self.handle      = INVALID_HANDLE_VALUE_INT
        self.is_open     = False

    # ------ ctxmgr ------
    def __enter__(self):
        self.open()
        return self

    def __exit__(self, *a):
        self.close()

    # ------ Open / Close ------
    def open(self) -> None:
        if self.is_open:
            return
        if not _IS_WINDOWS or _K32 is None:
            raise RuntimeError("Win32 backend работает только под Windows. "
                               "Переключись на 'PySerial' в окне Протокол.")
        path = rf"\\.\{self.port_name}".encode("ascii")
        h = _K32.CreateFileA(path,
                             GENERIC_READ | GENERIC_WRITE,
                             0,            # exclusive (как в exe)
                             None,
                             OPEN_EXISTING,
                             0,            # НЕ overlapped
                             None)
        if h in (0, INVALID_HANDLE_VALUE_INT):
            raise OSError(f"CreateFileA(\\\\.\\{self.port_name}) failed: "
                          f"GetLastError={ctypes.get_last_error()}")
        self.handle = h
        # GetCommState → patch → SetCommState
        dcb = _DCB(); dcb.DCBlength = ctypes.sizeof(_DCB)
        if not _K32.GetCommState(self.handle, ctypes.byref(dcb)):
            err = ctypes.get_last_error(); self.close()
            raise OSError(f"GetCommState failed: {err}")
        dcb.BaudRate = self.baud
        dcb.ByteSize = 8
        dcb.Parity   = 0
        dcb.StopBits = 0
        dcb.flags    = 0   # *** ВСЕ control-биты в 0, как в fcn.00424020 ***
        if not _K32.SetCommState(self.handle, ctypes.byref(dcb)):
            err = ctypes.get_last_error(); self.close()
            raise OSError(f"SetCommState failed: {err}")
        # COMMTIMEOUTS = {1, 0, 1, 0, 10} — точно как в fcn.00423d18
        to = _COMMTIMEOUTS(1, 0, 1, 0, 10)
        if not _K32.SetCommTimeouts(self.handle, ctypes.byref(to)):
            err = ctypes.get_last_error(); self.close()
            raise OSError(f"SetCommTimeouts failed: {err}")
        _K32.Sleep(100)            # как в exe
        self.is_open = True

    def close(self) -> None:
        if self.handle not in (0, INVALID_HANDLE_VALUE_INT) and _K32 is not None:
            _K32.CloseHandle(self.handle)
        self.handle  = INVALID_HANDLE_VALUE_INT
        self.is_open = False

    # ------ Примитивы ------
    def _avail(self) -> int:
        if not self.is_open: return 0
        err = wt.DWORD(0); st = _COMSTAT()
        if not _K32.ClearCommError(self.handle, ctypes.byref(err), ctypes.byref(st)):
            return 0
        return int(st.cbInQue)

    def _write_one(self, b: int) -> int:
        if not self.is_open: return 0
        buf = (ctypes.c_ubyte * 1)(b & 0xFF); n = wt.DWORD(0)
        deadline = _K32.GetTickCount() + self.timeout_ms
        while True:
            if _K32.WriteFile(self.handle, buf, 1, ctypes.byref(n), None) and n.value > 0:
                return n.value
            if _K32.GetTickCount() > deadline: return 0

    def write_bytewise(self, data: bytes) -> int:
        """Точно как fcn.00424cc0: побайтовый WriteFile + busy-wait inter_byte_ms."""
        n = 0
        for b in data:
            n += self._write_one(b)
            _busy_wait_ms(self.inter_byte_ms)
        return n

    def read_idle(self, want: int, idle_ms: int = None) -> bytes:
        """fcn.00423f64: ClearCommError + ReadFile циклом, дедлайн сбрасывается
        после каждого успешного куска (idle-таймаут)."""
        if not self.is_open or want <= 0:
            return b""
        if idle_ms is None:
            idle_ms = self.timeout_ms
        out = bytearray()
        remaining = want
        deadline = _K32.GetTickCount() + idle_ms
        chunk_buf = (ctypes.c_ubyte * 4096)()
        n = wt.DWORD(0)
        while remaining > 0:
            avail = self._avail()
            if avail > 0:
                ask = min(remaining, avail, 4096)
                if (_K32.ReadFile(self.handle, chunk_buf, ask, ctypes.byref(n), None)
                        and n.value > 0):
                    out.extend(bytes(chunk_buf[:n.value]))
                    remaining -= n.value
                    deadline = _K32.GetTickCount() + idle_ms
                    continue
            if _K32.GetTickCount() > deadline:
                break
        return bytes(out)


# ============================================================================
#       PROTOCOL HELPER — общие методы поверх любого backend'а
#       (pyserial.Serial / Win32ComPort).
# ============================================================================

def proto_write_bytewise(transport, data: bytes, gap_s: float) -> None:
    """Универсальная побайтовая отправка: pyserial использует .write+flush+sleep,
    Win32ComPort — write_bytewise() с встроенным busy-wait."""
    if isinstance(transport, Win32ComPort):
        # Конвертируем gap_s → ms; если 0/None — оставляем дефолт inter_byte_ms.
        if gap_s and gap_s > 0:
            transport.inter_byte_ms = max(1, int(round(gap_s * 1000)))
        transport.write_bytewise(data)
    else:
        for b in data:
            transport.write(bytes([b]))
            transport.flush()
            time.sleep(gap_s)


def proto_read(transport, want: int, timeout_s: float) -> bytes:
    """Универсальное чтение «всё что прибор успел прислать за timeout_s».
    На pyserial — установка ser.timeout + read(want). На Win32 — read_idle()."""
    if isinstance(transport, Win32ComPort):
        return transport.read_idle(want, idle_ms=int(round(timeout_s * 1000)))
    transport.timeout = timeout_s
    return transport.read(want)


def parse_dump_indices(body: bytes,
                       lo: int = 0x2700,
                       hi: int = 0x3aff) -> list:
    """В теле дампа от 0x55 (всё после 16-байтного header'а) ищем
    16-битные индексы записей в правдоподобном диапазоне (LE).

    ВАЖНО (проверено на реальном дампе прибора 4428, 218 записей):
    тело handshake'а — это НЕ массив записей с маркером `56 00 <type>`
    (как я сначала предположил по disassembly fcn.004249c0), а смесь
    служебных слов и LE16-индексов в произвольных байтовых позициях.
    Маркер `56 00` появляется только в ОТВЕТАХ на 0x42 LL HH (т.е. в
    конкретных записях), но не в handshake-теле.

    Поэтому функция сканирует body шифтом по 1 байту и ищет все LE16,
    попадающие в `[lo..hi]`. На реальном дампе (4428) находит 175
    кандидатов, из которых ~133 действительно валидны.

    Чтобы получить полный список 218 записей — нужен полный 0x42-скан
    (как делает основная кнопка «🚀 СТАРТ»). Этот результат — лишь
    «hint set» для быстрого превью в окне «🛰 Протокол».
    """
    if not body:
        return []
    out = []
    seen = set()
    for i in range(len(body) - 1):
        v = body[i] | (body[i + 1] << 8)
        if lo <= v <= hi and v not in seen:
            out.append(v)
            seen.add(v)
    return out


def parse_handshake_header(hs: bytes) -> dict:
    """Парсит 16-байтный header от 0x55 (см. PROTOCOL.md §8).
        bytes[0..1] = device_no (LE16), у нашего прибора 4428 = 4c 11
        bytes[2]    = info_byte (нечто типа версии прошивки)
        bytes[3..5] = ?
        bytes[6..15]= S/N + дата + тип прибора
    """
    if not hs or len(hs) < 16:
        return {"raw_len": len(hs) if hs else 0, "ok": False}
    return {
        "ok":          True,
        "raw_len":     len(hs),
        "device_no":   hs[0] | (hs[1] << 8),
        "info_byte":   hs[2],
        "tail_3_5":    hs[3:6].hex(" "),
        "id_block_10": hs[6:16].hex(" "),
        "header_full": hs[:16].hex(" "),
    }


# ============================================================================
#                       КЛАССИФИКАЦИЯ И ДЕКОДЕРЫ
# ============================================================================
AXLE_SUBTYPES = {
    700: "ось (общая)", 710: "ось локомотивная", 720: "ось моторвагонная",
    731: "ось РУ-1Ш", 732: "ось РУ-1Ш", 733: "ось РУ-1Ш",
    735: "ось РУ-1Ш", 737: "ось РУ-1Ш", 738: "ось РУ-1Ш-957",
    751: "ось РВ2Ш",  752: "ось РВ2Ш",  753: "ось РВ2Ш", 758: "ось РВ2Ш-957",
}


def classify_type(code, sub_byte: int = 0):
    """Классификатор детали по основному tcode (i+34..35) и под-маркеру (i+33).

    На реальном УД2-102 (прибор 4428) байты на i+34..35 у осей всегда
    дают «маркер-разделитель» `5b 60` (24667) или `5b e0` (57435) — это
    «ось без подкатегории». Конкретный подтип (РУ1 / РУ1Ш) лежит в байте
    i+33 ПЕРЕД маркером:
        01 → «ось РУ1»
        02 → «ось РУ1Ш»
        иначе → просто «ось»
    """
    if not code:
        return ""
    if code in (24667, 57435):
        if sub_byte == 1:
            return "ось РУ1"
        if sub_byte == 2:
            return "ось РУ1Ш"
        return "ось"
    if 300 <= code <= 399:
        return "ось"
    if 700 <= code <= 799:
        if code in AXLE_SUBTYPES:
            return AXLE_SUBTYPES[code]
        if 700 <= code <= 749:
            return "ось РУ-1Ш"
        return "ось РВ2Ш"
    if 800 <= code <= 899 or code == 595:
        return "колесо"
    if 900 <= code <= 999:
        return "бандаж"
    return "неизвестно"


def _decode_side(v):
    return "Лев" if v == 1 else "Прав" if v == 2 else "Обе" if v == 5 else "Нет"


def _decode_sheika(v):
    return "С кольцами" if v == 1 else "С буксой" if v == 2 else "Открытая"


def _decode_obod(v):
    return ("Выше 50мм" if v == 3
            else "Более 40мм" if v == 1
            else "Менее 40мм" if v == 2
            else "Нет")


def _decode_obtochka(v):
    return "Есть" if v == 1 else "Нет"


def _decode_greben(v):
    # 0 и 6 → «Нет», остальные значения — как есть (число).
    return "Нет" if v in (0, 6) else str(v)


def _is_axle(type_name: str) -> bool:
    return "ось" in type_name


def _is_wheel_or_band(type_name: str) -> bool:
    return "колесо" in type_name or "бандаж" in type_name


def _bcd_reverse(buf, start, max_len):
    """Прочитать BCD-строку «как у эталонной выгрузки»: 
    байты в ОБРАТНОМ порядке до первого 0x0A (терминатор), 
    каждый байт = одна десятичная цифра. Ведущие нули убираются.
    Например, `06 02 06 01 05 0a 0a` → «51626».
    """
    end = start
    limit = min(start + max_len, len(buf))
    while end < limit and buf[end] != 0x0A:
        end += 1
    chunk = buf[start:end]
    s = "".join(str(b) for b in reversed(chunk))
    return s.lstrip('0') or ("0" if s else "")


def safe_get(buf, idx, default=0):
    """Возвращает байт, если он есть, иначе default. Исключает падение скрипта."""
    if 0 <= idx < len(buf):
        return buf[idx]
    return default


# ============================================================================
#                          ENGINE — 1:1 с эталоном
# ============================================================================
def open_port(port: str) -> serial.Serial:
    """Открывает COM-порт с DCB-флагами как в PelengPC.exe (fcn.00424020):
      - bytesize=8, parity=N, stopbits=1, baud=19200,
      - rtscts=False, dsrdtr=False, xonxoff=False (DCB.flags = 0 в exe),
      - exclusive=True (dwShareMode=0 в exe — fcn.00423d18).
    После открытия — Sleep(100) для устаканивания CH340 (как exe).
    Эти параметры взяты из реверса; они не меняют скорость, но делают
    handshake стабильнее на «холодном» CH340.
    """
    ser = serial.Serial(
        port, BAUD,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=EMPTY_TIMEOUT_DEFAULT,
        xonxoff=False, rtscts=False, dsrdtr=False,
    )
    # Sleep(100) после открытия — exe делает это сразу после
    # SetCommTimeouts(). На «холодном» CH340 без этой паузы первый 0x55
    # иногда корруптится.
    time.sleep(0.1)
    # Сброс возможного «застрявшего» мусора в буферах прибора и драйвера.
    try:
        ser.reset_input_buffer()
        ser.reset_output_buffer()
    except Exception:
        pass
    return ser


def send_precision(ser, data, gap_s: float = None):
    """Побайтовая отправка с inter-byte gap'ом + flush после каждого байта.
    Эталонная процедура — без неё прибор теряет команды.

    Использует `_gap_wait()` который уважает глобальный `BUSY_WAIT_GAP`:
    - False (default): time.sleep(gap_s) — быстрее в плане CPU, но на
      Windows фактический сон может быть 15.6 мс из-за гранулярности.
    - True: busy-wait через perf_counter — точные 4.8/10 мс как exe,
      но грузит CPU.

    `gap_s=None` → берётся текущий `ACTIVE_GAP_S` (по умолчанию = GAP_S).
    """
    if gap_s is None:
        gap_s = ACTIVE_GAP_S
    for b in data:
        ser.write(bytes([b]))
        ser.flush()
        _gap_wait(gap_s)


def read_precision(ser, empty_timeout: float = EMPTY_TIMEOUT_DEFAULT,
                   tail_timeout: float = TAIL_TIMEOUT):
    """Голова 2 байта → если `\\xFD\\xFF` → b'EMPTY'. Иначе +84 байта tail.

    КРИТИЧНО (находка из v2-диагностики):
    Не сбрасываем ser.timeout на empty_timeout! Эталонный код юзера
    оставляет timeout=150мс после первого хита, и это позволяет ловить
    «медленные» ответы прибора, которые иначе теряются.

    Поведение:
    - Первый запрос: timeout=28мс (выставлен в open_port).
    - После первого хита: timeout=150мс на head НАВСЕГДА → не теряем
      записи с латентностью >28мс на этом приборе.
    - Эталон-sticky дал 217/218, наша «правильная» версия — 191/218.
    Таймаут пользователя на head после первого хита может оставаться
    150мс (или передан явно через empty_timeout).
    """
    if empty_timeout != EMPTY_TIMEOUT_DEFAULT:
        # Если параметр передан явно — выставляем (для тестов/сравнения).
        ser.timeout = empty_timeout
    head = ser.read(2)
    if not head or head == b"\xFD\xFF":
        return b"EMPTY"
    ser.timeout = tail_timeout
    tail = ser.read(84)
    return head + tail


def request_handshake(ser, total_timeout: float = HANDSHAKE_TIMEOUT) -> bytes:
    """TX `\\x55` → RX полное тело оглавления прибора через idle-timeout.

    Использует idle-timeout паттерн (как exe): читает пока не пройдёт
    80 мс тишины после последнего полученного байта, или не сработает
    общий потолок (`total_timeout`). Это даёт ПОЛНЫЙ ответ — на УД2-102
    первый 0x55 возвращает 3212 B (а не 600 как `read(700)` обрезал).

    Номер прибора УД2-102 = handshake[0:2] LE16 (например 4428 = `4c 11`).
    Cost ≈ 0 мс относительно старого `read(700)` — idle-timeout закроется
    на тех же ~600мс если прибор замолкает.
    """
    send_precision(ser, b"\x55")
    deadline = time.perf_counter() + total_timeout
    chunks = []
    saved_to = ser.timeout
    try:
        ser.timeout = 0.080  # idle-timeout 80 мс
        last_got = time.perf_counter()
        total = 0
        while time.perf_counter() < deadline and total < 65536:
            in_w = getattr(ser, "in_waiting", None)
            if in_w:
                data = ser.read(in_w)
            else:
                data = ser.read(1)
            if data:
                chunks.append(data)
                total += len(data)
                last_got = time.perf_counter()
            else:
                if total > 0 and (time.perf_counter() - last_got) >= 0.080:
                    break
    finally:
        ser.timeout = saved_to
    return b"".join(chunks)


def parse_device_no(handshake: bytes) -> int:
    if not handshake or len(handshake) < 2:
        return 0
    return handshake[0] | (handshake[1] << 8)


def parse_handshake_hints(hs: bytes,
                          lo: int = 0x2700, hi: int = 0x3aff) -> list:
    """Извлекает из handshake-блоба все возможные кандидаты индексов записей
    (LE16 в правдоподобном диапазоне 0x2700..0x3aff).

    Используется только для диагностики/доскана — никаких гарантий, что
    список «чистый», возможны как ложные срабатывания, так и пропуски.
    """
    if not hs:
        return []
    cands: set = set()
    for i in range(len(hs) - 1):
        v = hs[i] | (hs[i + 1] << 8)
        if lo <= v <= hi:
            cands.add(v)
    return sorted(cands)


def request_handshake_full(ser, idle_ms: int = 80,
                           hard_ms: int = 2500,
                           max_bytes: int = 65536) -> bytes:
    """Шлёт 0x55 и читает ВЕСЬ ответ через idle-timeout (как PelengPC.exe).

    Эталонный `request_handshake()` читает 700 байт жёстким таймаутом и
    обрывает чтение на 601 байт даже когда прибор шлёт 3212. Здесь
    используется паттерн как в exe: ждём `idle_ms` миллисекунд тишины
    после последнего полученного байта, абсолютный потолок `hard_ms`.

    Возвращает все полученные байты (header + тело с LE16-индексами).
    """
    send_precision(ser, b"\x55")

    deadline = time.perf_counter() + hard_ms / 1000.0
    chunks = []
    total = 0
    saved_to = ser.timeout
    try:
        ser.timeout = idle_ms / 1000.0
        last_got_t = time.perf_counter()
        while time.perf_counter() < deadline and total < max_bytes:
            in_w = getattr(ser, "in_waiting", None)
            if in_w:
                data = ser.read(in_w)
            else:
                data = ser.read(1)
            if data:
                chunks.append(data)
                total += len(data)
                last_got_t = time.perf_counter()
            else:
                if total > 0 and (time.perf_counter() - last_got_t) >= idle_ms / 1000.0:
                    break
    finally:
        ser.timeout = saved_to
    return b"".join(chunks)


def multi_batch_handshake(ser, max_batches: int = 4,
                          lo: int = 10000, hi: int = 20000,
                          gap_between_batches_s: float = 0.05,
                          progress_cb=None) -> tuple:
    """Шлёт 0x55 несколько раз (как exe — мульти-батч) и собирает все
    LE16-индексы в диапазоне `[lo..hi]` десятичных (по умолчанию
    10000..20000 — где живут все записи + отчёты УД2-102).

    Останавливается когда:
      - набрано `max_batches` батчей, ИЛИ
      - очередной батч не добавил новых индексов и батчей уже >= 2.

    Диапазон 10000..20000 покрывает:
      - классические записи (≈10100..10700, 13300..13400),
      - отчёты (≈14000..15000),
      - служебные блоки (≈15000..20000).
    Узкий диапазон уменьшает шум (LE16-сдвиги в массиве ловят меньше
    мусорных слов вне разумного для УД2-102 диапазона)."

    Returns:
      (sorted_hints, batches_info)
        batches_info: list of dicts {size, new, total, parsed}
    """
    all_hints: set = set()
    batches_info = []
    last_batch_new = -1
    for k in range(max_batches):
        hs = request_handshake_full(ser)
        body = hs[16:] if len(hs) >= 16 else b""
        new_hints = []
        for i in range(0, len(body) - 1, 2):
            v = body[i] | (body[i + 1] << 8)
            if lo <= v <= hi:
                new_hints.append(v)
        for i in range(1, len(body) - 1, 2):
            v = body[i] | (body[i + 1] << 8)
            if lo <= v <= hi:
                new_hints.append(v)
        added = 0
        for v in new_hints:
            if v not in all_hints:
                all_hints.add(v)
                added += 1
        info = {"size": len(hs), "new": added,
                "parsed": len(new_hints), "total": len(all_hints)}
        batches_info.append(info)
        if progress_cb:
            try:
                progress_cb(k + 1, info)
            except Exception:
                pass
        if k >= 1 and added == 0:
            break
        last_batch_new = added
        time.sleep(gap_between_batches_s)
    return sorted(all_hints), batches_info


def simulate_streak(raw: dict, bases: list, pre_break: int, post_break: int):
    """Симулирует, какие индексы дала бы streak-стратегия на собранных
    raw-ответах. Не делает запросов к прибору.

    Args:
        raw: {idx: bytes_response}, b"EMPTY" для пустых.
        bases: список scan-баз.
        pre_break: сколько подряд пустых до первого хита терпим.
        post_break: сколько подряд пустых после хита терпим.

    Returns:
        set найденных idx.
    """
    found_set = set()
    for base in bases:
        found, streak = False, 0
        for offset in range(100):
            idx = base + offset
            resp = raw.get(idx)
            if resp is None:
                # Не было запроса — считаем как пусто (как при реальном
                # обрыве по streak — мы дальше не сканировали).
                streak += 1
            elif resp == b"EMPTY":
                streak += 1
            else:
                # Хит. Декодим, чтобы убедиться что это реальная запись.
                d = smart_decode(idx, resp)
                if d:
                    found_set.add(idx)
                    found = True
                    streak = 0
                else:
                    streak += 1
            limit = post_break if found else pre_break
            if streak >= limit:
                break
    return found_set


def smart_decode(requested_id, buf):
    """ЭТАЛОННЫЙ парсер юзера (дал 218 записей). Не трогать!"""
    if len(buf) < 60:
        return None
    for i in range(len(buf) - 4):
        d, m, y, hh, mm = buf[i], buf[i + 1], buf[i + 2], buf[i + 3], buf[i + 4]
        if (1 <= d <= 31) and (1 <= m <= 12) and (15 <= y <= 40) \
                and (0 <= hh <= 23) and (0 <= mm <= 59):
            try:
                date_str = f"20{y:02d}-{m:02d}-{d:02d} {hh:02d}:{mm:02d}"

                # «Объект» и «Плавка» в эталонной выгрузке читаются
                # в ОБРАТНОМ порядке до первого 0x0A. См. _bcd_reverse.
                # Раньше брали прямой порядок и хвостовой `00` подмешивался
                # как лишний «0» в конце.
                part = _bcd_reverse(buf, i + 10, 13)
                melt = _bcd_reverse(buf, i + 46, 10)

                tcode = safe_get(buf, i + 34) | (safe_get(buf, i + 35) << 8)
                sub_b = safe_get(buf, i + 33)   # под-маркер для осей (РУ1/РУ1Ш)
                type_name = classify_type(tcode, sub_b)

                # Layout полей у осей и колёс отличается на 1 байт:
                #   Колесо: stamp@i+53..54, year@i+55..56, side@i+57, obod@i+59
                #   Ось:    stamp@i+52..53, year@i+54..55, side@i+56, sheika@i+57
                # Поэтому считаем оба варианта, ниже выберем по типу детали.
                if _is_axle(type_name):
                    stamp = safe_get(buf, i + 52) | (safe_get(buf, i + 53) << 8)
                    y_val = safe_get(buf, i + 54) | (safe_get(buf, i + 55) << 8)
                    year_valid = 1950 <= y_val <= 2050
                    year = y_val if year_valid else "??"
                    # У эталона ВСЕ оси показаны как «обе» (физически
                    # дефектоскопируются на обе стороны как одна заготовка).
                    # В сыром ответе байт i+56 иногда даёт мусор → жёстко «Обе».
                    side = "Обе"
                    # Эвристика для шейки:
                    #   если год валидный → шейка лежит на i+57;
                    #   если год пустой/мусор → field сдвинут влево и шейка
                    #   читается с i+56, либо с i+57 как fallback.
                    sh1 = safe_get(buf, i + 56)
                    sh2 = safe_get(buf, i + 57)
                    if year_valid:
                        sheika = _decode_sheika(sh2)
                    else:
                        chosen = sh1 if sh1 in (1, 2) else (sh2 if sh2 in (1, 2) else 0)
                        sheika = _decode_sheika(chosen)
                    obod = obtochka = greben = ""
                else:
                    # Колёса/бандажи: классический поиск года в окне i+48..i+67
                    # (стабильно даёт год+З-д у колёс).
                    year, stamp = "??", 0
                    for k in range(i + 48, min(i + 68, len(buf) - 1)):
                        y_val = buf[k] | (buf[k + 1] << 8)
                        if 1950 <= y_val <= 2050:
                            year = y_val
                            stamp = buf[k - 2] | (buf[k - 1] << 8)
                            break
                    side = _decode_side(safe_get(buf, i + 57))
                    if _is_wheel_or_band(type_name):
                        sheika = ""
                        obod = _decode_obod(safe_get(buf, i + 59))
                        obtochka = _decode_obtochka(safe_get(buf, i + 60))
                        greben = _decode_greben(safe_get(buf, i + 61))
                    else:
                        sheika = obod = obtochka = greben = ""

                return {
                    "id": requested_id, "date": date_str, "type": type_name,
                    "part": part, "melt": melt, "stamp": stamp, "year": year,
                    "side": side, "sheika": sheika, "obod": obod,
                    "obtochka": obtochka, "greben": greben,
                }
            except Exception:
                continue
    return None


# ============================================================================
#                       НАСТРОЙКИ ОТЧЁТА (локальные)
# ============================================================================
# По результатам реверса PelengPC_v12.exe + анализа трассы общения с УД2-102:
# текстовые поля шапки отчёта (Предприятие, Подразделение, Оператор, НТД,
# Номер настройки) НЕ хранятся в приборе и НЕ передаются по UART. В трассе
# нет ни одной кириллической строки в ответах прибора. В самом exe этих
# строк тоже нет ни в одной из кодировок (CP1251/UTF-16/UTF-8/CP866/KOI-8R).
# Значит PelengPC хранит их локально (в реестре или зашифрованном overlay'е).
# Мы повторяем эту логику простым JSON-файлом рядом со скриптом.

REPORT_FIELDS = [
    ("enterprise",       "Предприятие"),
    ("division",         "Подразделение"),
    ("operator_code",    "Оператор шифр"),
    ("operator_surname", "Фамилия"),
    ("ntd",              "НТД на контроль"),
    ("setup_no",         "Номер настройки"),
]
DEFAULT_REPORT_SETTINGS = {k: "" for k, _ in REPORT_FIELDS}


def _settings_path() -> str:
    """Файл настроек рядом со скриптом: peleng_settings.json."""
    here = os.path.dirname(os.path.abspath(__file__)) if "__file__" in globals() \
        else os.getcwd()
    return os.path.join(here, "peleng_settings.json")


def load_report_settings() -> dict:
    p = _settings_path()
    if not os.path.exists(p):
        return dict(DEFAULT_REPORT_SETTINGS)
    try:
        with open(p, "r", encoding="utf-8") as f:
            data = json.load(f)
    except Exception:
        return dict(DEFAULT_REPORT_SETTINGS)
    out = dict(DEFAULT_REPORT_SETTINGS)
    if isinstance(data, dict):
        for k in DEFAULT_REPORT_SETTINGS:
            v = data.get(k)
            if isinstance(v, str):
                out[k] = v
    return out


def save_report_settings(settings: dict) -> None:
    p = _settings_path()
    try:
        with open(p, "w", encoding="utf-8") as f:
            json.dump(settings, f, ensure_ascii=False, indent=2)
    except Exception as e:
        try:
            messagebox.showwarning("peleng_settings.json",
                                   f"Не удалось сохранить настройки:\n{e}")
        except Exception:
            pass


# ============================================================================
#                                  GUI
# ============================================================================
class DefectoApp:
    def __init__(self, root):
        self.root = root
        self.root.title("DefectoScan УД2-102 — Champion")
        self.root.geometry("1280x650")
        self.records = {}        # base -> [data dict, ...]
        self.is_scanning = False
        self.device_no = 0       # из handshake
        self.device_info_byte = 0    # hs[2] — типа версии прошивки
        self.device_header_hex = ""  # hs[:16] для отчёта
        self.start_t = 0.0
        self.total_records = 0
        # Локальные настройки шапки отчёта (Предприятие/НТД/...).
        # Аналог того что PelengPC.exe хранит в реестре (см. реверс).
        self.report_settings = load_report_settings()
        self.setup_ui()

    # ---------- UI ----------
    def setup_ui(self):
        top = tk.Frame(self.root, pady=10)
        top.pack(fill=tk.X)

        tk.Label(top, text="Порт:", font=("Arial", 10)).pack(side=tk.LEFT, padx=(15, 4))
        self.cmb_port = ttk.Combobox(top, width=12, state="readonly")
        self.cmb_port.pack(side=tk.LEFT)
        self.refresh_ports()
        tk.Button(top, text="↻", width=2, command=self.refresh_ports).pack(side=tk.LEFT, padx=4)

        tk.Button(top, text="🚀 СТАРТ", bg="#27ae60", fg="white",
                  font=("Arial", 11, "bold"), command=self.start_scan
                  ).pack(side=tk.LEFT, padx=15)

        self.lbl_timer = tk.Label(top, text="⏱ 00.00", font=("Consolas", 14, "bold"),
                                  fg="#e74c3c", width=10)
        self.lbl_timer.pack(side=tk.LEFT, padx=10)

        self.lbl_n = tk.Label(top, text="Найдено: 0", font=("Arial", 12, "bold"))
        self.lbl_n.pack(side=tk.LEFT, padx=20)

        self.lbl_dev = tk.Label(top, text="Прибор: —", font=("Arial", 11))
        self.lbl_dev.pack(side=tk.LEFT, padx=10)

        # Профайл нужен внутренней логике 🚀 СТАРТ (`if self.var_profile.get():`).
        # Не выводим как UI-элемент — оставляем как невидимый BooleanVar,
        # чтобы не сломать существующий код скана.
        self.var_profile = tk.BooleanVar(value=False)

        self.prog = ttk.Progressbar(self.root, orient=tk.HORIZONTAL, mode='determinate')
        self.prog.pack(fill=tk.X, padx=15, pady=5)

        paned = ttk.PanedWindow(self.root, orient=tk.HORIZONTAL)
        paned.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        self.tree_c = ttk.Treeview(paned, show="tree", selectmode="browse")

        self.cols = ("id", "date", "type", "part", "melt",
                     "stamp", "year", "side", "sheika", "obod",
                     "obtochka", "greben")
        self.tree_r = ttk.Treeview(paned, columns=self.cols, show="headings")
        heads = ["ID", "Дата", "Тип детали", "Объект", "Плавка",
                 "З-д", "Год", "Стр", "Шейка", "Обод",
                 "Обт. колеса", "Нап. гребня"]
        for i, c in enumerate(self.cols):
            self.tree_r.heading(c, text=heads[i])
            self.tree_r.column(c, width=90, anchor="center")

        sb = ttk.Scrollbar(self.tree_r, orient="vertical",
                           command=self.tree_r.yview)
        self.tree_r.configure(yscrollcommand=sb.set)
        sb.pack(side=tk.RIGHT, fill=tk.Y)

        paned.add(self.tree_c, weight=1)
        paned.add(self.tree_r, weight=5)
        self.tree_c.bind("<<TreeviewSelect>>", self.on_sel)
        # Двойной клик на бакете в дереве — открываем окно «Отчёт»
        # с шапкой (Прибор №/Версия + реквизиты) и таблицей записей
        # внутри этого бакета. Аналог двойного клика по «Отчёту №X» в PelengPC.
        self.tree_c.bind("<Double-Button-1>", self._on_bucket_double)

    def refresh_ports(self):
        ports = [p.device for p in list_ports.comports()]
        if not ports:
            ports = [f"COM{i}" for i in range(1, 21)]
        self.cmb_port["values"] = ports
        if ports and not self.cmb_port.get():
            self.cmb_port.set(ports[0])

    # ---------- скан ----------
    def start_scan(self):
        if self.is_scanning:
            return
        port = self.cmb_port.get().strip()
        if not port:
            messagebox.showwarning("Порт", "Выберите COM-порт")
            return

        self.is_scanning = True
        self.records.clear()
        self.total_records = 0
        self.device_no = 0
        self.lbl_dev.config(text="Прибор: —")

        for i in self.tree_c.get_children():
            self.tree_c.delete(i)
        for i in self.tree_r.get_children():
            self.tree_r.delete(i)

        self.start_t = time.perf_counter()
        self.update_clock()
        threading.Thread(target=self.run_logic, args=(port,), daemon=True).start()

    def update_clock(self):
        if self.is_scanning:
            self.lbl_timer.config(
                text=f"⏱ {time.perf_counter() - self.start_t:.2f}")
            self.root.after(50, self.update_clock)

    # ---------- БЕНЧМАРК ТАЙМИНГОВ (sleep × busy_wait × 4.8/10 мс) ----------
    def start_benchmark(self):
        """Прогоняет 4 варианта тайминга на одном приборе и пишет отчёт.

        Варианты:
          V1: time.sleep gap=4.8 мс — текущий эталон
          V2: time.sleep gap=10 мс  — exe-значение, но через sleep
          V3: busy-wait gap=4.8 мс  — наше значение, но точное (как exe)
          V4: busy-wait gap=10 мс   — exe-значение точное (`fcn.00424cc0`)

        Каждый прогон = полный 8×100 streak 15/2 эталонный скан.
        Между прогонами пауза 2 секунды.

        Отчёт `peleng_uart_bench.txt` сравнивает:
          - время скана
          - количество найденных записей
          - объявляет «победителя» (минимум времени при максимуме записей)
        """
        if self.is_scanning:
            return
        port = self.cmb_port.get().strip()
        if not port:
            messagebox.showwarning("Порт", "Выберите COM-порт")
            return

        self.is_scanning = True
        self.records.clear()
        self.total_records = 0
        self.device_no = 0
        self.lbl_dev.config(text="Прибор: —")
        for i in self.tree_c.get_children():
            self.tree_c.delete(i)
        for i in self.tree_r.get_children():
            self.tree_r.delete(i)

        self.start_t = time.perf_counter()
        self.update_clock()
        threading.Thread(target=self.run_benchmark_logic, args=(port,),
                         daemon=True).start()

    def _bench_run_once(self, port: str, label: str,
                        busy_wait: bool, gap_s: float) -> dict:
        """Один прогон бенчмарка: открывает порт, handshake, 8×100 скан.
        Не трогает self.records/tree — записывает в локальный словарь.

        Returns: dict {label, busy, gap_ms, t_open, t_handshake, t_scan,
                       n_records, device_no}
        """
        global BUSY_WAIT_GAP, ACTIVE_GAP_S
        BUSY_WAIT_GAP = busy_wait
        ACTIVE_GAP_S = gap_s

        result = {
            "label": label, "busy": busy_wait, "gap_ms": gap_s * 1000.0,
            "t_open": 0.0, "t_handshake": 0.0, "t_scan": 0.0,
            "n_records": 0, "device_no": 0, "error": None,
        }

        t_open0 = time.perf_counter()
        try:
            ser = open_port(port)
        except Exception as e:
            result["error"] = f"Не открыт порт: {e}"
            return result
        result["t_open"] = time.perf_counter() - t_open0

        # handshake
        t_hs0 = time.perf_counter()
        try:
            hs = request_handshake(ser)
            result["device_no"] = parse_device_no(hs)
        except Exception:
            pass
        result["t_handshake"] = time.perf_counter() - t_hs0

        ser.timeout = EMPTY_TIMEOUT_DEFAULT
        bases = [10100, 10200, 10300, 10400, 10500, 10600, 13300, 13400]
        records_local = {}
        n_records = 0

        scan_t0 = time.perf_counter()
        for base in bases:
            records_local[base] = []
            found, streak = False, 0
            for offset in range(100):
                if not self.is_scanning:
                    break
                idx = base + offset
                send_precision(ser, bytes([0x42]) + struct.pack("<H", idx))
                resp = read_precision(ser)
                if resp == b"EMPTY":
                    streak += 1
                    if (not found and streak >= 15) or (found and streak >= 2):
                        break
                    continue
                d = smart_decode(idx, resp)
                if d:
                    found, streak = True, 0
                    records_local[base].append(d)
                    n_records += 1
                else:
                    streak += 1
            # пакетный апдейт UI после прохода базы
            self.root.after(0, lambda lbl=label, n=n_records:
                self.lbl_n.config(text=f"{lbl}: {n} записей"))
        result["t_scan"] = time.perf_counter() - scan_t0
        result["n_records"] = n_records

        try: ser.close()
        except Exception: pass
        return result

    def run_benchmark_logic(self, port: str):
        variants = [
            ("V1 sleep 4.8мс",  False, 0.0048),
            ("V2 sleep 10мс",   False, 0.010),
            ("V3 busy 4.8мс",   True,  0.0048),
            ("V4 busy 10мс",    True,  0.010),
        ]
        results = []
        for i, (label, busy, gap) in enumerate(variants, 1):
            if not self.is_scanning:
                break
            self.root.after(0, lambda lbl=label:
                self.lbl_dev.config(text=f"Бенч: {lbl}"))
            try:
                r = self._bench_run_once(port, label, busy, gap)
                results.append(r)
            except Exception as e:
                results.append({"label": label, "busy": busy,
                                "gap_ms": gap * 1000, "error": str(e),
                                "t_open": 0.0, "t_handshake": 0.0,
                                "t_scan": 0.0, "n_records": 0,
                                "device_no": 0})
            # пауза между прогонами — прибор отдохнёт
            if i < len(variants):
                time.sleep(2.0)

        # ---- Восстанавливаем дефолты ----
        global BUSY_WAIT_GAP, ACTIVE_GAP_S
        BUSY_WAIT_GAP = False
        ACTIVE_GAP_S = GAP_S

        # ---- Анализ + отчёт ----
        self._write_bench_report(results)

        # ---- Победитель ----
        ok_results = [r for r in results if not r.get("error")]
        if ok_results:
            max_n = max(r["n_records"] for r in ok_results)
            full_results = [r for r in ok_results if r["n_records"] == max_n]
            winner = min(full_results, key=lambda r: r["t_scan"])
            msg = (f"БЕНЧМАРК ГОТОВ\n\n"
                   f"Победитель: {winner['label']}\n"
                   f"Время скана: {winner['t_scan']:.1f} c\n"
                   f"Записей: {winner['n_records']}\n\n"
                   f"Подробный отчёт: peleng_uart_bench.txt")
        else:
            msg = "Бенчмарк завершился с ошибкой во всех вариантах"

        self.is_scanning = False
        self.root.after(0, lambda m=msg: messagebox.showinfo("🏁 БЕНЧМАРК", m))

    def _write_bench_report(self, results: list):
        path = "peleng_uart_bench.txt"
        try:
            with open(path, "w", encoding="utf-8") as f:
                f.write("════════════════════════════════════════════════════════\n")
                f.write("  БЕНЧМАРК ТАЙМИНГОВ УД2-102\n")
                f.write("════════════════════════════════════════════════════════\n\n")
                f.write("Цель: проверить скорость 4-х комбинаций (sleep/busy-wait\n")
                f.write("× 4.8мс/10мс) на реальном приборе одним прогоном.\n")
                f.write("Источник тайминга — реверс PelengPC.exe (fcn.00424cc0,\n")
                f.write("inter-byte gap через busy-wait GetTickCount = 10мс).\n\n")
                # сводка
                f.write("┌─────────────────────┬──────────┬──────────┬──────────┐\n")
                f.write("│ Вариант             │ Скан, с  │ Записей  │ Прибор   │\n")
                f.write("├─────────────────────┼──────────┼──────────┼──────────┤\n")
                for r in results:
                    if r.get("error"):
                        line = (f"│ {r['label']:<19} │   ERROR  │     —    │ "
                                f"{(r.get('device_no') or '—'):>8} │\n")
                    else:
                        line = (f"│ {r['label']:<19} │ "
                                f"{r['t_scan']:7.2f}  │ "
                                f"{r['n_records']:>6}   │ "
                                f"{(r.get('device_no') or '—'):>8} │\n")
                    f.write(line)
                f.write("└─────────────────────┴──────────┴──────────┴──────────┘\n\n")

                ok_results = [r for r in results if not r.get("error")]
                if ok_results:
                    max_n = max(r["n_records"] for r in ok_results)
                    full = [r for r in ok_results if r["n_records"] == max_n]
                    winner = min(full, key=lambda r: r["t_scan"])
                    f.write(f"ПОБЕДИТЕЛЬ: {winner['label']}\n")
                    f.write(f"  Скан:    {winner['t_scan']:.2f} с\n")
                    f.write(f"  Записей: {winner['n_records']}\n")
                    f.write(f"  Open:    {winner['t_open']*1000:.0f} мс\n")
                    f.write(f"  Hshake:  {winner['t_handshake']*1000:.0f} мс\n\n")

                    # анализ
                    f.write("АНАЛИЗ:\n")
                    sleep_4 = next((r for r in ok_results if r['label'].startswith('V1')), None)
                    busy_4  = next((r for r in ok_results if r['label'].startswith('V3')), None)
                    if sleep_4 and busy_4:
                        diff = sleep_4["t_scan"] - busy_4["t_scan"]
                        if abs(diff) < 1.0:
                            f.write(f"  • V1 (sleep 4.8) и V3 (busy 4.8) почти равны "
                                    f"(Δ={diff:+.2f} с) → Windows-граничность не\n"
                                    f"    мешает, можно оставлять `time.sleep`.\n")
                        elif diff > 0:
                            f.write(f"  • V3 (busy 4.8) быстрее V1 (sleep 4.8) на "
                                    f"{diff:.2f} с — это {diff/sleep_4['t_scan']*100:.0f}%\n"
                                    f"    выигрыша. ВКЛЮЧАЙТЕ busy-wait по умолчанию.\n")
                        else:
                            f.write(f"  • V1 (sleep) быстрее V3 (busy) на {-diff:.2f} с\n"
                                    f"    → busy-wait здесь не помогает (CPU overhead).\n")
                else:
                    f.write("ВСЕ ВАРИАНТЫ ОШИБКА — проверь подключение прибора.\n")

                f.write("\nПодробности по каждому прогону:\n")
                for r in results:
                    f.write(f"\n— {r['label']} —\n")
                    if r.get("error"):
                        f.write(f"   ОШИБКА: {r['error']}\n")
                    else:
                        f.write(f"   gap={r['gap_ms']:.1f} мс, busy_wait={r['busy']}\n")
                        f.write(f"   open_port:    {r['t_open']*1000:7.0f} мс\n")
                        f.write(f"   handshake:    {r['t_handshake']*1000:7.0f} мс\n")
                        f.write(f"   8×100 scan:   {r['t_scan']:7.2f} с\n")
                        f.write(f"   записей:      {r['n_records']:>4}\n")
                        f.write(f"   device_no:    {r['device_no'] or '—'}\n")
        except Exception as e:
            print(f"Ошибка записи отчёта: {e}")

    # ---------- ГИБРИДНЫЙ СКАН (handshake → широкий диапазон → densify) ----------
    def start_hybrid(self):
        """Гибридный скан: handshake-якоря + densify + опрос 0x42.

        Стратегия (точно как делает PelengPC.exe v1.2 + наша эвристика):
          1) Мульти-батч `0x55`: посылаем до 4-х раз с idle-timeout
             чтением, накапливаем все LE16-кандидаты в широком диапазоне
             `[0x2700..0xfeff]`. Это ловит и классические записи
             (0x2700..0x3aff), и отчёты в области ~0x5200 (21000+).
          2) Densify ±5 вокруг каждого якоря (соседние индексы которые
             не объявлены в handshake'е, но физически могут существовать).
          3) Опрос всех кандидатов через `0x42 LL HH` с обрывом по
             5 пустым `\\xfd\\xff` подряд внутри сегмента.

        Эталонный «🚀 СТАРТ» (8×100, streak 15/2) НЕ ТРОГАЕТ — гибрид
        ставится отдельной кнопкой, использует те же tree_c/tree_r.
        """
        if self.is_scanning:
            return
        port = self.cmb_port.get().strip()
        if not port:
            messagebox.showwarning("Порт", "Выберите COM-порт")
            return

        self.is_scanning = True
        self.records.clear()
        self.total_records = 0
        self.device_no = 0
        self.lbl_dev.config(text="Прибор: —")

        for i in self.tree_c.get_children():
            self.tree_c.delete(i)
        for i in self.tree_r.get_children():
            self.tree_r.delete(i)

        self.start_t = time.perf_counter()
        self.update_clock()
        threading.Thread(target=self.run_hybrid_logic, args=(port,),
                         daemon=True).start()

    def run_hybrid_logic(self, port: str):
        try:
            ser = open_port(port)
        except Exception as e:
            self.root.after(0, lambda: messagebox.showerror(
                "Порт", f"Не удалось открыть {port}:\n{e}"))
            self.is_scanning = False
            return

        # ---- ШАГ 1: МУЛЬТИ-БАТЧ HANDSHAKE ----
        # Шлём 0x55 до 4 раз. Останавливаемся когда очередной батч не
        # добавил новых индексов (как exe — там было 4 батча для 218 рек).
        # Диапазон поиска индексов в теле handshake'а: 10000..20000
        # десятичных (= 0x2710..0x4e20). Покрывает классические записи,
        # отчёты и служебные блоки УД2-102. Узкий диапазон убирает шум.
        def hs_progress(k, info):
            self.root.after(0, lambda k=k, info=info:
                self.lbl_n.config(
                    text=f"HS {k}: {info['size']}B → "
                         f"+{info['new']} (всего {info['total']})"))

        HINT_LO = 10000
        HINT_HI = 20000
        try:
            hints, batches = multi_batch_handshake(
                ser, max_batches=4, lo=HINT_LO, hi=HINT_HI,
                progress_cb=hs_progress)
        except Exception as e:
            try: ser.close()
            except Exception: pass
            self.root.after(0, lambda: messagebox.showerror(
                "Handshake", f"Ошибка handshake'а:\n{e}"))
            self.is_scanning = False
            return

        # device_no из заголовка первого батча (лежит в первых 2 байтах)
        # — не критично, просто для отображения.
        try:
            send_precision(ser, b"\x55")
            ser.timeout = 0.3
            hs_head = ser.read(16)
            ser.timeout = EMPTY_TIMEOUT_DEFAULT
            self.device_no = parse_device_no(hs_head)
            self._capture_device_info(hs_head)
            if self.device_no:
                self.root.after(0, lambda v=self.device_no:
                    self.lbl_dev.config(text=f"Прибор: {v}"))
        except Exception:
            pass

        if not hints:
            try: ser.close()
            except Exception: pass
            self.root.after(0, lambda: messagebox.showwarning(
                "Гибрид", "Handshake не вернул ни одного индекса. "
                "Проверь связь с прибором."))
            self.is_scanning = False
            return

        # ---- ШАГ 2: DENSIFY ----
        # Вокруг каждого якоря добавляем ±RADIUS в общую корзину.
        # Это ловит индексы которые прибор не объявил в handshake'е,
        # но физически имеет в flash (соседние записи, ±5 от якоря).
        # Densify ограничен тем же диапазоном [HINT_LO..HINT_HI] что и
        # handshake-парсер — не выпрыгиваем за разумные пределы УД2-102.
        RADIUS = 5
        candidates: set = set(hints)
        for h in hints:
            for off in range(-RADIUS, RADIUS + 1):
                c = h + off
                if HINT_LO <= c <= HINT_HI:
                    candidates.add(c)

        ordered = sorted(candidates)
        # Сегментируем (последовательные диапазоны без пропусков).
        segments = []
        cur = [ordered[0]]
        for x in ordered[1:]:
            if x == cur[-1] + 1:
                cur.append(x)
            else:
                segments.append(cur)
                cur = [x]
        segments.append(cur)

        # ---- ШАГ 3: ОПРОС 0x42 LL HH ПО СЕГМЕНТАМ ----
        # Группируем сегменты по 100-кам для отображения в дереве (как
        # эталонный скан, чтобы UI был совместим).
        ser.timeout = EMPTY_TIMEOUT_DEFAULT

        scan_t0 = time.perf_counter()
        STREAK_LIMIT = 5    # обрыв сегмента после 5 пустых подряд
        anchor_set = set(hints)

        # Собираем используемые «базы» (десятки сотен) для tree_c.
        used_bases = set()
        for seg in segments:
            for idx in seg:
                used_bases.add((idx // 100) * 100)
        used_bases = sorted(used_bases)
        for base in used_bases:
            self.records[base] = []
            p_id = f"hb_{base}"
            self.root.after(0, lambda p=p_id, b=base:
                self.tree_c.insert("", "end", iid=p,
                                   text=f"📁 Блок {b}"))

        total_to_query = sum(len(s) for s in segments)
        queried = 0

        for seg_i, seg in enumerate(segments):
            if not self.is_scanning:
                break
            seg_anchor_seen = any(x in anchor_set for x in seg)
            streak = 0
            for idx in seg:
                if not self.is_scanning:
                    break
                queried += 1
                send_precision(ser, bytes([0x42]) + struct.pack("<H", idx))
                resp = read_precision(ser)

                if resp == b"EMPTY":
                    streak += 1
                    if seg_anchor_seen and streak >= STREAK_LIMIT:
                        break
                    if not seg_anchor_seen and streak >= 15:
                        break
                    continue

                d = smart_decode(idx, resp)
                if d:
                    streak = 0
                    seg_anchor_seen = True
                    base = (idx // 100) * 100
                    if base not in self.records:
                        self.records[base] = []
                        p_id = f"hb_{base}"
                        # вставляем «лениво», если объявилась новая база
                        # (например после densify попали в незаявленную сотню)
                        self.root.after(0, lambda p=p_id, b=base:
                            self.tree_c.insert("", "end", iid=p,
                                               text=f"📁 Блок {b}"))
                    if not any(r.get("id") == idx for r in self.records[base]):
                        self.records[base].append(d)
                        self.total_records += 1
                else:
                    streak += 1

            # пакетный апдейт UI после сегмента
            base = (seg[0] // 100) * 100
            self.root.after(0, lambda b=base: self.upd_block(f"hb_{b}", b))
            # прогресс
            self.root.after(0, lambda v=(queried / max(total_to_query, 1)) * 100:
                self.prog.config(value=v))

        try:
            ser.close()
        except Exception:
            pass

        dt = time.perf_counter() - scan_t0
        # Сохраняем полную информацию для отчёта (опционально).
        try:
            with open("peleng_uart_hybrid.txt", "w", encoding="utf-8") as f:
                f.write("=== ГИБРИДНЫЙ СКАН ===\n")
                f.write(f"Прибор:          {self.device_no or '—'}\n")
                f.write(f"Найдено записей: {self.total_records}\n")
                f.write(f"Hint-якорей:     {len(hints)}\n")
                f.write(f"Densify ±:       {RADIUS}\n")
                f.write(f"Кандидатов:      {len(ordered)}\n")
                f.write(f"Сегментов:       {len(segments)}\n")
                f.write(f"Опрошено 0x42:   {queried}\n")
                f.write(f"Время скана:     {dt:.1f} c\n\n")
                f.write("Батчи handshake'а:\n")
                for k, b in enumerate(batches, 1):
                    f.write(f"  Батч {k}: {b['size']} B → "
                            f"распознано {b['parsed']}, новых +{b['new']}, "
                            f"всего {b['total']}\n")
        except Exception:
            pass

        self.root.after(0, self.finish)


    def run_logic(self, port: str):
        # Профайл-аккумуляторы (заполняются всегда — overhead < 0.01 с).
        prof = {
            "open_port": 0.0,
            "handshake": 0.0,
            "tx_total": 0.0,
            "rx_empty": 0.0, "rx_empty_n": 0,
            "rx_hit":   0.0, "rx_hit_n":   0,
            "decode":   0.0, "decode_n":   0,
            "ui_after": 0.0, "ui_after_n": 0,
            "scan_total": 0.0,
        }
        t0 = time.perf_counter()

        try:
            ser = open_port(port)
        except Exception as e:
            self.root.after(0, lambda: messagebox.showerror(
                "Порт", f"Не удалось открыть {port}:\n{e}"))
            self.is_scanning = False
            return
        prof["open_port"] = time.perf_counter() - t0

        # handshake → device_no
        ts = time.perf_counter()
        try:
            hs = request_handshake(ser)
            self.device_no = parse_device_no(hs)
            self._capture_device_info(hs)
            if self.device_no:
                self.root.after(0, lambda v=self.device_no:
                                self.lbl_dev.config(text=f"Прибор: {v}"))
        except Exception:
            pass
        prof["handshake"] = time.perf_counter() - ts

        bases = [10100, 10200, 10300, 10400, 10500, 10600, 13300, 13400]
        total = len(bases) * 100
        curr = 0

        # Перед сканом возвращаем timeout к 28мс — handshake оставил 1.5с.
        # ВАЖНО: read_precision больше НЕ сбрасывает timeout каждый раз,
        # после первого хита он становится 150мс и остаётся (sticky-режим
        # эталонного кода). Это ловит «медленные» ответы прибора, которые
        # иначе теряются. Диагностика подтвердила: sticky=217, reset=191.
        ser.timeout = EMPTY_TIMEOUT_DEFAULT

        scan_t0 = time.perf_counter()

        for base in bases:
            self.records[base] = []
            p_id = f"b_{base}"
            ts = time.perf_counter()
            self.root.after(0, lambda p=p_id, b=base:
                            self.tree_c.insert("", "end", iid=p,
                                               text=f"📁 Блок {b}"))
            prof["ui_after"] += time.perf_counter() - ts
            prof["ui_after_n"] += 1
            found, streak = False, 0

            for offset in range(100):
                idx = base + offset
                curr += 1
                # Прогресс: раз в 100 шагов (вместо каждого) — экономит
                # лишние Tcl/Tk-вызовы из горячего цикла.
                if curr % 100 == 0:
                    ts = time.perf_counter()
                    self.root.after(0, lambda v=(curr / total) * 100:
                                    self.prog.config(value=v))
                    prof["ui_after"] += time.perf_counter() - ts
                    prof["ui_after_n"] += 1

                ts = time.perf_counter()
                send_precision(ser, bytes([0x42]) + struct.pack("<H", idx))
                prof["tx_total"] += time.perf_counter() - ts

                ts = time.perf_counter()
                resp = read_precision(ser)
                rx_dt = time.perf_counter() - ts

                if resp == b"EMPTY":
                    prof["rx_empty"] += rx_dt
                    prof["rx_empty_n"] += 1
                    streak += 1
                    # Эталонный break: до 1-го хита — 15 пустых, после — 2.
                    # На USB-Serial эти пороги работают благодаря sticky
                    # timeout (после хита head читается 150мс — нет ложных
                    # пустых из-за латентности прибора).
                    if (not found and streak >= 15) or (found and streak >= 2):
                        break
                    continue

                prof["rx_hit"] += rx_dt
                prof["rx_hit_n"] += 1

                ts = time.perf_counter()
                data = smart_decode(idx, resp)
                prof["decode"] += time.perf_counter() - ts
                prof["decode_n"] += 1

                if data:
                    found, streak = True, 0
                    self.records[base].append(data)
                    self.total_records += 1
                else:
                    streak += 1

            # Пакетное обновление UI после прохода каждой базы.
            ts = time.perf_counter()
            self.root.after(0, self.upd_block, p_id, base)
            prof["ui_after"] += time.perf_counter() - ts
            prof["ui_after_n"] += 1

        prof["scan_total"] = time.perf_counter() - scan_t0

        try:
            ser.close()
        except Exception:
            pass

        # Сохранить отчёт, если включён чекбокс «Профайл».
        if self.var_profile.get():
            try:
                self._write_profile_report(prof)
            except Exception:
                pass

        self.root.after(0, self.finish)

    def _write_profile_report(self, prof: dict):
        total = (prof["open_port"] + prof["handshake"] + prof["scan_total"])
        accounted = (prof["tx_total"] + prof["rx_empty"] + prof["rx_hit"]
                     + prof["decode"] + prof["ui_after"])
        unaccounted = prof["scan_total"] - accounted
        path = "peleng_uart_profile.txt"
        rows = [
            "═══════════════════════════════════════════════════════════",
            "  ПРОФАЙЛ СКАНА УД2-102",
            "═══════════════════════════════════════════════════════════",
            f"  Прибор:                {self.device_no or '—'}",
            f"  Найдено записей:       {self.total_records}",
            "",
            "  ─── ОБЩЕЕ ВРЕМЯ ───────────────────────────────────",
            f"    Открытие порта:      {prof['open_port']:7.3f} с",
            f"    Handshake:           {prof['handshake']:7.3f} с",
            f"    Сам скан:            {prof['scan_total']:7.3f} с",
            f"    ИТОГО:               {total:7.3f} с",
            "",
            "  ─── ВНУТРИ СКАНА ──────────────────────────────────",
            f"    TX (отправка 0x42):  {prof['tx_total']:7.3f} с",
            f"    RX EMPTY (FD FF):    {prof['rx_empty']:7.3f} с "
            f"× {prof['rx_empty_n']:>4}  ({prof['rx_empty']*1000/max(prof['rx_empty_n'],1):.1f} мс/шт)",
            f"    RX HIT  (запись):    {prof['rx_hit']:7.3f} с "
            f"× {prof['rx_hit_n']:>4}  ({prof['rx_hit']*1000/max(prof['rx_hit_n'],1):.1f} мс/шт)",
            f"    smart_decode:        {prof['decode']:7.3f} с "
            f"× {prof['decode_n']:>4}",
            f"    UI (root.after):     {prof['ui_after']:7.3f} с "
            f"× {prof['ui_after_n']:>4}",
            f"    ── итого учтено:     {accounted:7.3f} с",
            f"    ── НЕучтено:         {unaccounted:7.3f} с  "
            "(Python/Tk overhead, GIL, кооперативный планировщик)",
            "",
            "  ─── ЧТО ОЗНАЧАЕТ ─────────────────────────────────",
            "  ✓ TX = время отправки 3 байт с паузой 4.8 мс (≈14.4 мс)",
            "  ✓ RX EMPTY = ожидание FD FF (timeout 28 мс — эталон)",
            "  ✓ RX HIT  = чтение 86 байт (теор. ≈49 мс на 19200)",
            "  ✓ Decode  = парсинг записи в Python (должно быть < 0.5 мс)",
            "  ✓ UI      = постановка задачи в Tk-thread (root.after)",
            "  ✓ НЕучтено = всё остальное: GC, переключения, loop overhead",
            "═══════════════════════════════════════════════════════════",
        ]
        with open(path, "w", encoding="utf-8") as f:
            f.write("\n".join(rows))

    def _bucket_label(self, base: int, records: list) -> str:
        """Формирует подпись бакета для дерева:
            "📋 Отчёт №N (10100) — 12.04.2026 — 30 зап."
        Где N = порядковый номер бакета (1, 2, 3...).
        Дата — из «заголовочной» (самой ранней) записи бакета.
        """
        bases = sorted(self.records.keys())
        try:
            n = bases.index(base) + 1
        except ValueError:
            n = 0
        cnt = len(records)
        if records:
            try:
                ds = sorted(records, key=lambda r: r.get("date") or "")
                d10 = (ds[0].get("date") or "")[:10]  # YYYY-MM-DD
                if len(d10) == 10:
                    y, m, d = d10.split("-")
                    date_pretty = f"{d}.{m}.{y}"
                else:
                    date_pretty = d10 or "—"
            except Exception:
                date_pretty = "—"
        else:
            date_pretty = "—"
        return f"📋 Отчёт №{n} ({base}) — {date_pretty} — {cnt} зап."

    def _on_bucket_double(self, _evt):
        """Двойной клик по бакету в дереве → открыть окно отчёта."""
        sel = self.tree_c.selection()
        if not sel:
            return
        iid = sel[0]
        # iid формируется как "b_{base}" или "hb_{base}".
        try:
            base = int(iid.split("_")[-1])
        except Exception:
            return
        if base not in self.records:
            return
        self.open_report_window(only_base=base)

    def upd_block(self, p_id, base):
        """Пакетное обновление по окончании сканирования одного блока.
        Вызывается один раз на блок (а не на каждую запись) — экономит
        Tk/Treeview-overhead.
        """
        cnt = len(self.records[base])
        self.lbl_n.config(text=f"Найдено: {self.total_records}")
        self.tree_c.item(p_id, text=self._bucket_label(base, self.records[base]))
        sel = self.tree_c.selection()
        if sel and sel[0] == p_id:
            for i in self.tree_r.get_children():
                self.tree_r.delete(i)
            for data in self.records.get(base, []):
                vals = tuple(data[col] for col in self.cols)
                self.tree_r.insert("", "end", values=vals)

    def finish(self):
        self.is_scanning = False
        messagebox.showinfo("ГОТОВО",
                            f"Сбор завершён.\n"
                            f"Найдено: {self.total_records} записей.\n"
                            f"Прибор: {self.device_no or '—'}")

    # ---------- ДИАГНОСТИКА ----------
    def start_diag(self):
        if self.is_scanning:
            return
        port = self.cmb_port.get().strip()
        if not port:
            messagebox.showwarning("Порт", "Выберите COM-порт")
            return
        self.is_scanning = True
        self.records.clear()
        self.total_records = 0
        self.device_no = 0
        self.lbl_dev.config(text="Прибор: —  [ДИАГНОСТИКА]")
        for i in self.tree_c.get_children():
            self.tree_c.delete(i)
        for i in self.tree_r.get_children():
            self.tree_r.delete(i)
        self.start_t = time.perf_counter()
        self.update_clock()
        threading.Thread(target=self.run_diag, args=(port,), daemon=True).start()

    def run_diag(self, port: str):
        """Полный диагностический проход:
        1) handshake (с замером времени и hex-дампом),
        2) полный скан 8 баз × 100 оффсетов БЕЗ break (~50 c),
        3) доскан handshake-кандидатов вне баз,
        4) симуляция всех streak-стратегий по собранным raw-ответам,
        5) сохранение peleng_uart_diag.txt + peleng_uart_dump.hex.
        """
        diag = {
            "t_open": 0.0,
            "t_handshake": 0.0,
            "t_scan": 0.0,
            "t_extra": 0.0,
            "n_queries": 0,
            "n_empty": 0,
            "n_hit": 0,
        }
        t0 = time.perf_counter()
        try:
            ser = open_port(port)
        except Exception as e:
            self.root.after(0, lambda: messagebox.showerror(
                "Порт", f"Не удалось открыть {port}:\n{e}"))
            self.is_scanning = False
            return
        diag["t_open"] = time.perf_counter() - t0

        # 1) handshake
        ts = time.perf_counter()
        try:
            hs_raw = request_handshake(ser)
            self.device_no = parse_device_no(hs_raw)
            self._capture_device_info(hs_raw)
            if self.device_no:
                self.root.after(0, lambda v=self.device_no:
                                self.lbl_dev.config(
                                    text=f"Прибор: {v}  [ДИАГНОСТИКА]"))
        except Exception:
            hs_raw = b""
        diag["t_handshake"] = time.perf_counter() - ts

        hints = parse_handshake_hints(hs_raw)

        # 2) полный скан
        bases = [10100, 10200, 10300, 10400, 10500, 10600, 13300, 13400]
        raw: dict = {}
        ts = time.perf_counter()
        total_q = len(bases) * 100
        done_q = 0
        for base in bases:
            for offset in range(100):
                idx = base + offset
                send_precision(ser, bytes([0x42]) + struct.pack("<H", idx))
                resp = read_precision(ser)
                raw[idx] = resp
                done_q += 1
                diag["n_queries"] += 1
                if resp == b"EMPTY":
                    diag["n_empty"] += 1
                else:
                    diag["n_hit"] += 1
                if done_q % 50 == 0:
                    self.root.after(0, lambda v=done_q / total_q * 100:
                                    self.prog.config(value=v))
                # Живая статистика в шапке
                if done_q % 25 == 0:
                    cur_hits = diag["n_hit"]
                    self.root.after(0, lambda h=cur_hits:
                                    self.lbl_n.config(
                                        text=f"Найдено: {h} (диаг)"))
        diag["t_scan"] = time.perf_counter() - ts

        # 3) доскан handshake-кандидатов, не покрытых базами
        extra_idx = [h for h in hints if h not in raw]
        ts = time.perf_counter()
        extra_hits = 0
        for idx in extra_idx:
            send_precision(ser, bytes([0x42]) + struct.pack("<H", idx))
            resp = read_precision(ser)
            raw[idx] = resp
            diag["n_queries"] += 1
            if resp == b"EMPTY":
                diag["n_empty"] += 1
            else:
                diag["n_hit"] += 1
                if smart_decode(idx, resp):
                    extra_hits += 1
        diag["t_extra"] = time.perf_counter() - ts

        # 3.5) ЭТАЛОН-СТИКИ: повторный скан с поведением вашего оригинального
        # эталона (timeout НЕ сбрасывается между запросами; после первого
        # хита остаётся 150 мс на head). Цель — проверить, не теряем ли
        # «медленные» ответы прибора из-за слишком короткого 28 мс.
        raw_sticky: dict = {}
        diag_sticky = {"n_q": 0, "n_e": 0, "n_h": 0, "t": 0.0}
        ts = time.perf_counter()
        ser.timeout = EMPTY_TIMEOUT_DEFAULT
        for base in bases:
            found_in_base, streak = False, 0
            for offset in range(100):
                idx = base + offset
                send_precision(ser, bytes([0x42]) + struct.pack("<H", idx))
                # ЭТАЛОН: НЕ сбрасываем timeout перед head
                head = ser.read(2)
                if not head or head == b"\xFD\xFF":
                    raw_sticky[idx] = b"EMPTY"
                    streak += 1
                    diag_sticky["n_e"] += 1
                else:
                    ser.timeout = TAIL_TIMEOUT
                    tail = ser.read(84)
                    raw_sticky[idx] = head + tail
                    diag_sticky["n_h"] += 1
                    found_in_base = True
                    streak = 0
                    # ВАЖНО: не возвращаем timeout обратно — sticky 150 мс!
                diag_sticky["n_q"] += 1
                if (not found_in_base and streak >= 15) \
                        or (found_in_base and streak >= 2):
                    break
        diag_sticky["t"] = time.perf_counter() - ts

        # 3.6) ЭКСПЕРИМЕНТЫ:
        # B) Тест опкодов 0x40, 0x41, 0x43, 0x44, 0x45 на выборке индексов,
        #    у которых эталон знает год, а у нас «??».
        # C) Чтение длинного (до 256 байт) ответа по 0x42.
        # Цель: выяснить, лежит ли год где-то еще, доступном через USB.
        sample_idx = [0x277f, 0x2781, 0x2783, 0x2788, 0x2789,
                      0x278a, 0x278f, 0x2790, 0x2791, 0x2792]
        # Оставляем только те, что реально есть в приборе (раз нашли в raw)
        sample_idx = [i for i in sample_idx if raw.get(i, b"EMPTY") != b"EMPTY"]
        experiments: dict = {"opcodes": {}, "long_read": {}}
        ts = time.perf_counter()
        try:
            for opcode in (0x40, 0x41, 0x43, 0x44, 0x45):
                experiments["opcodes"][opcode] = {}
                for idx in sample_idx:
                    try:
                        ser.reset_input_buffer()
                        ser.timeout = TAIL_TIMEOUT
                        send_precision(ser,
                                       bytes([opcode]) + struct.pack("<H", idx))
                        # Читаем до 256 байт; используем 300 мс, чтобы поймать
                        # потенциально более длинный ответ от другого опкода.
                        ser.timeout = 0.3
                        resp = ser.read(256)
                    except Exception:
                        resp = b""
                    experiments["opcodes"][opcode][idx] = resp
            # C) Длинное чтение по 0x42
            for idx in sample_idx:
                try:
                    ser.reset_input_buffer()
                    ser.timeout = TAIL_TIMEOUT
                    send_precision(ser,
                                   bytes([0x42]) + struct.pack("<H", idx))
                    ser.timeout = 0.3
                    resp = ser.read(256)
                except Exception:
                    resp = b""
                experiments["long_read"][idx] = resp
        except Exception:
            pass
        experiments["t"] = time.perf_counter() - ts
        experiments["sample"] = sample_idx

        try:
            ser.close()
        except Exception:
            pass

        # 4) анализ
        valid_records: dict = {}
        non_decoded: dict = {}    # idx → resp, хит-но-не-распарсилось
        for idx, resp in raw.items():
            if resp != b"EMPTY":
                d = smart_decode(idx, resp)
                if d:
                    valid_records[idx] = (d, resp)
                else:
                    non_decoded[idx] = resp

        # Эталон-sticky: что нашёл повторный скан?
        sticky_records: dict = {}
        for idx, resp in raw_sticky.items():
            if resp != b"EMPTY":
                d = smart_decode(idx, resp)
                if d:
                    sticky_records[idx] = d

        # Симуляции
        sims = {}
        for name, (pre, post) in [
            ("эталон 15/2",   (15, 2)),
            ("выбранный 15/5",(15, 5)),
            ("широкий 15/15", (15, 15)),
            ("широкий 15/30", (15, 30)),
            ("полный 100/100",(100, 100)),
        ]:
            sims[name] = simulate_streak(raw, bases, pre, post)
        # handshake-only: только индексы из handshake
        sims["handshake_only"] = {i for i in hints if i in valid_records}
        # union (полный скан + handshake): максимум что можно достать
        sims["full + handshake"] = set(valid_records.keys())
        # ЭТАЛОН-STICKY (отдельный физический скан, не симуляция)
        sims["ЭТАЛОН-sticky"] = set(sticky_records.keys())

        # 5) запись отчётов
        try:
            self._write_diag_report(diag, hs_raw, hints, raw,
                                    valid_records, sims, bases,
                                    diag_sticky, sticky_records, non_decoded,
                                    experiments)
            self._write_hex_dump(hs_raw, valid_records, non_decoded,
                                 experiments)
        except Exception as e:
            self.root.after(0, lambda err=e: messagebox.showerror(
                "Диагностика", f"Ошибка записи отчёта:\n{err}"))

        # Заполнить GUI результатами полного скана (как при обычной работе)
        for base in bases:
            self.records[base] = []
            p_id = f"b_{base}"
            self.root.after(0, lambda p=p_id, b=base:
                            self.tree_c.insert("", "end", iid=p,
                                               text=f"📁 Блок {b}"))
        for idx in sorted(valid_records.keys()):
            data, _ = valid_records[idx]
            base = (idx // 100) * 100
            # Подобрать ближайшую базу из списка
            base_use = max((b for b in bases if b <= idx), default=bases[0])
            if base_use not in self.records:
                self.records[base_use] = []
            self.records[base_use].append(data)
            self.total_records += 1
        for base in bases:
            self.root.after(0, self.upd_block, f"b_{base}", base)

        self.is_scanning = False
        cnt = len(valid_records)
        self.root.after(0, lambda: messagebox.showinfo(
            "Диагностика готова",
            f"Полный скан завершён.\n\n"
            f"Найдено валидных записей: {cnt}\n"
            f"Запросов: {diag['n_queries']}\n"
            f"Время: {time.perf_counter() - self.start_t:.1f} c\n\n"
            f"Файлы рядом со скриптом:\n"
            f"  • peleng_uart_diag.txt — отчёт по стратегиям\n"
            f"  • peleng_uart_dump.hex — hex всех ответов"))

    def _write_diag_report(self, diag, hs_raw, hints, raw,
                           valid_records, sims, bases,
                           diag_sticky=None, sticky_records=None,
                           non_decoded=None, experiments=None):
        if sticky_records is None:
            sticky_records = {}
        if non_decoded is None:
            non_decoded = {}
        if diag_sticky is None:
            diag_sticky = {"n_q": 0, "n_e": 0, "n_h": 0, "t": 0.0}
        path = "peleng_uart_diag.txt"
        valid_idx = set(valid_records.keys())

        rows = []
        rows.append("═" * 70)
        rows.append("  ДИАГНОСТИКА УД2-102 — полный анализ")
        rows.append("═" * 70)
        rows.append(f"  Прибор:                 {self.device_no or '—'}")
        rows.append(f"  Время старта:           {time.strftime('%Y-%m-%d %H:%M:%S')}")
        rows.append("")
        rows.append("  ─── ТАЙМИНГИ ────────────────────────────────────────")
        rows.append(f"    Открытие порта:       {diag['t_open']:7.3f} c")
        rows.append(f"    Handshake:            {diag['t_handshake']:7.3f} c "
                    f"({len(hs_raw)} байт)")
        rows.append(f"    Полный скан 8×100:    {diag['t_scan']:7.3f} c")
        rows.append(f"    Доскан handshake:     {diag['t_extra']:7.3f} c")
        rows.append(f"    Запросов всего:       {diag['n_queries']}")
        rows.append(f"    Хиты:                 {diag['n_hit']}")
        rows.append(f"    Пустые (FD FF):       {diag['n_empty']}")
        rows.append("")
        rows.append("  ─── HANDSHAKE-HINTS ─────────────────────────────────")
        rows.append(f"    Кандидатов в handshake (LE16, 0x2700..0x3aff): {len(hints)}")
        if hints:
            rows.append(f"    Первые 30: {', '.join(hex(h) for h in hints[:30])}")
            rows.append(f"    Последние 10: {', '.join(hex(h) for h in hints[-10:])}")
        rows.append("")
        rows.append("  ─── РАСПРЕДЕЛЕНИЕ ВАЛИДНЫХ ЗАПИСЕЙ ПО БАЗАМ ────────")
        rows.append(f"    Всего валидных записей в полном скане: {len(valid_idx)}")
        rows.append("")
        rows.append(f"    {'база':>6} {'хитов':>6} {'мин-макс':>14} "
                    f"{'дыры':>5}  валидные оффсеты")
        for base in bases:
            in_base = sorted(i for i in valid_idx if base <= i < base + 100)
            if not in_base:
                rows.append(f"    {base:>6} {0:>6}     —          —    (пусто)")
                continue
            mn, mx = min(in_base) - base, max(in_base) - base
            offsets = [i - base for i in in_base]
            # Считаем дыры: пустые между min и max
            holes = (mx - mn + 1) - len(offsets)
            # Сегменты подряд
            segs = []
            cur = [offsets[0]]
            for o in offsets[1:]:
                if o == cur[-1] + 1:
                    cur.append(o)
                else:
                    segs.append(cur)
                    cur = [o]
            segs.append(cur)
            seg_str = ", ".join(f"{s[0]}–{s[-1]}" if len(s) > 1 else str(s[0])
                                for s in segs[:6])
            if len(segs) > 6:
                seg_str += f", … ({len(segs)} сегментов всего)"
            rows.append(f"    {base:>6} {len(offsets):>6}  "
                        f"{mn:>3}..{mx:<3}    {holes:>4}    {seg_str}")
        rows.append("")
        rows.append("  ─── МАКСИМАЛЬНЫЕ ДЫРЫ ВНУТРИ БЛОКОВ ─────────────────")
        rows.append("    Это покажет, насколько большой post-streak нужен:")
        for base in bases:
            in_base = sorted(i - base for i in valid_idx
                             if base <= i < base + 100)
            if len(in_base) < 2:
                continue
            gaps = [in_base[k + 1] - in_base[k] - 1
                    for k in range(len(in_base) - 1)]
            max_gap = max(gaps) if gaps else 0
            big = [g for g in gaps if g >= 3]
            rows.append(f"    {base}: max-дыра={max_gap}, "
                        f"дыр≥3: {len(big)}, гистограмма={sorted(set(gaps))}")
        rows.append("")
        rows.append("  ─── ЭТАЛОН-STICKY СКАН (повторный, с залипшим 150мс) ─")
        rows.append(f"    Запросов:             {diag_sticky['n_q']}")
        rows.append(f"    Хитов:                {diag_sticky['n_h']}")
        rows.append(f"    Пустых:               {diag_sticky['n_e']}")
        rows.append(f"    Время:                {diag_sticky['t']:6.2f} c")
        rows.append(f"    Найдено валидных:     {len(sticky_records)}")
        # Сравним со списком valid_records
        sticky_set = set(sticky_records.keys())
        valid_set = set(valid_records.keys())
        only_sticky = sticky_set - valid_set
        only_valid = valid_set - sticky_set
        rows.append(f"    Только sticky нашёл:  {len(only_sticky)} "
                    f"({sorted(hex(i) for i in list(only_sticky)[:10])})")
        rows.append(f"    Sticky упустил:       {len(only_valid)} "
                    f"({sorted(hex(i) for i in list(only_valid)[:10])})")
        rows.append("")
        rows.append("  ─── НЕРАСПАРСЕННЫЕ ХИТЫ (smart_decode = None) ───────")
        rows.append(f"    Всего: {len(non_decoded)}")
        for idx in sorted(non_decoded.keys())[:30]:
            resp = non_decoded[idx]
            preview = " ".join(f"{b:02x}" for b in resp[:24])
            rows.append(f"    0x{idx:04x}  len={len(resp):>3}  {preview}...")
        if len(non_decoded) > 30:
            rows.append(f"    ... +ещё {len(non_decoded) - 30}")
        rows.append("")
        rows.append("  ═══ СРАВНЕНИЕ СТРАТЕГИЙ ═════════════════════════════")
        rows.append(f"    {'стратегия':<22} {'найдено':>9} {'упущено':>9}  "
                    f"упущенные (первые 10)")
        # Считаем эталоном full
        ref = sims["полный 100/100"]
        for name, found in sims.items():
            missed = sorted(ref - found)
            miss_str = ", ".join(hex(m) for m in missed[:10])
            if len(missed) > 10:
                miss_str += f" … (+{len(missed) - 10})"
            rows.append(f"    {name:<22} {len(found):>9} {len(missed):>9}  {miss_str}")
        rows.append("")
        rows.append("  ─── ВСЕ ВАЛИДНЫЕ ЗАПИСИ (idx → расшифровка) ─────────")
        for idx in sorted(valid_records.keys()):
            data, _ = valid_records[idx]
            rows.append(f"    0x{idx:04x} {data['date']} {data['type']:<11} "
                        f"part={data['part']:<8} melt={data['melt']:<6} "
                        f"god={str(data['year']):<5} "
                        f"side={data['side']:<5} sheika={data['sheika']:<11} "
                        f"obod={data['obod']:<10} obt={data['obtochka']} "
                        f"greb={data['greben']}")
        rows.append("")
        # === EXPERIMENTAL: опкоды + длинное чтение ===
        if experiments:
            rows.append("  ═══ EXPERIMENTAL — опкоды и длинное чтение ════════")
            rows.append(f"    Время на эксперименты: {experiments.get('t', 0):.2f} c")
            sample_idx = experiments.get('sample', [])
            rows.append(f"    Выборка ({len(sample_idx)} индексов): "
                        f"{', '.join(hex(i) for i in sample_idx)}")
            rows.append("")
            rows.append("    --- B) Альтернативные опкоды (0x40, 0x41, 0x43, 0x44, 0x45) ---")
            for opcode, by_idx in (experiments.get("opcodes") or {}).items():
                rows.append(f"      опкод 0x{opcode:02x}:")
                for idx, resp in by_idx.items():
                    if not resp:
                        rows.append(f"        0x{idx:04x}: пусто")
                        continue
                    preview = " ".join(f"{b:02x}" for b in resp[:24])
                    rows.append(f"        0x{idx:04x}: len={len(resp):3} "
                                f"first24={preview}")
                rows.append("")
            rows.append("    --- C) Длинное чтение по 0x42 (до 256 байт) ---")
            for idx, resp in (experiments.get("long_read") or {}).items():
                if not resp:
                    rows.append(f"      0x{idx:04x}: пусто")
                    continue
                preview = " ".join(f"{b:02x}" for b in resp[:24])
                rows.append(f"      0x{idx:04x}: len={len(resp):3} "
                            f"first24={preview}")
            rows.append("")
            rows.append("    Расшифровку байт см. в peleng_uart_dump.hex,")
            rows.append("    раздел EXPERIMENTAL.")
            rows.append("")
        rows.append("═" * 70)
        rows.append(" Используйте «СРАВНЕНИЕ СТРАТЕГИЙ» — выберите ту, что")
        rows.append(" даёт 218/0 при минимальной длине скана. Также смотрите")
        rows.append(" на «МАКСИМАЛЬНЫЕ ДЫРЫ»: post-streak должен быть больше")
        rows.append(" самой большой дыры в данных.")
        rows.append("═" * 70)

        with open(path, "w", encoding="utf-8") as f:
            f.write("\n".join(rows))

    def _write_hex_dump(self, hs_raw, valid_records, non_decoded=None,
                        experiments=None):
        if non_decoded is None:
            non_decoded = {}
        path = "peleng_uart_dump.hex"
        rows = []
        rows.append("═" * 70)
        rows.append(" HEX-DUMP ВСЕХ ОТВЕТОВ ПРИБОРА УД2-102")
        rows.append("═" * 70)
        rows.append("")
        rows.append(f" HANDSHAKE  ({len(hs_raw)} байт)")
        rows.append("─" * 70)
        for off in range(0, len(hs_raw), 16):
            chunk = hs_raw[off:off + 16]
            hex_part = " ".join(f"{b:02x}" for b in chunk)
            asc_part = "".join(chr(b) if 32 <= b < 127 else "."
                               for b in chunk)
            rows.append(f"  {off:04x}  {hex_part:<48}  {asc_part}")
        rows.append("")
        rows.append(f" RECORDS  ({len(valid_records)} штук)")
        rows.append("─" * 70)
        for idx in sorted(valid_records.keys()):
            data, resp = valid_records[idx]
            rows.append("")
            rows.append(f" idx=0x{idx:04x}  {data['date']}  "
                        f"{data['type']}  part={data['part']}")
            for off in range(0, len(resp), 16):
                chunk = resp[off:off + 16]
                hex_part = " ".join(f"{b:02x}" for b in chunk)
                asc_part = "".join(chr(b) if 32 <= b < 127 else "."
                                   for b in chunk)
                rows.append(f"  +{off:02x}  {hex_part:<48}  {asc_part}")
        rows.append("")
        # Не-распарсенные хиты — те, где прибор отдал ответ, но smart_decode
        # не нашёл датового паттерна. Возможно, это «удалённые» записи или
        # записи специального формата. Дампим их сырые байты для анализа.
        if non_decoded:
            rows.append("")
            rows.append("═" * 70)
            rows.append(f" NON-DECODED HITS  ({len(non_decoded)} штук) — "
                        "хиты без датового паттерна")
            rows.append("═" * 70)
            for idx in sorted(non_decoded.keys()):
                resp = non_decoded[idx]
                rows.append("")
                rows.append(f" idx=0x{idx:04x}  len={len(resp)}  (NO DATE)")
                for off in range(0, len(resp), 16):
                    chunk = resp[off:off + 16]
                    hex_part = " ".join(f"{b:02x}" for b in chunk)
                    asc_part = "".join(chr(b) if 32 <= b < 127 else "."
                                       for b in chunk)
                    rows.append(f"  +{off:02x}  {hex_part:<48}  {asc_part}")
        rows.append("")
        # EXPERIMENTAL hex
        if experiments:
            rows.append("═" * 70)
            rows.append(" EXPERIMENTAL — RAW HEX по альтернативным опкодам и")
            rows.append(" длинному чтению. Ищите year-данные в этих байтах.")
            rows.append("═" * 70)
            for opcode, by_idx in (experiments.get("opcodes") or {}).items():
                rows.append("")
                rows.append(f" === ОПКОД 0x{opcode:02x} ===")
                for idx, resp in by_idx.items():
                    rows.append("")
                    rows.append(f"  idx=0x{idx:04x}  len={len(resp)}")
                    if not resp:
                        continue
                    for off in range(0, len(resp), 16):
                        chunk = resp[off:off + 16]
                        hex_part = " ".join(f"{b:02x}" for b in chunk)
                        asc_part = "".join(chr(b) if 32 <= b < 127 else "."
                                           for b in chunk)
                        rows.append(f"   +{off:02x}  {hex_part:<48}  {asc_part}")
            rows.append("")
            rows.append(" === ОПКОД 0x42 ДЛИННОЕ ЧТЕНИЕ (до 256 байт) ===")
            for idx, resp in (experiments.get("long_read") or {}).items():
                rows.append("")
                rows.append(f"  idx=0x{idx:04x}  len={len(resp)}")
                if not resp:
                    continue
                for off in range(0, len(resp), 16):
                    chunk = resp[off:off + 16]
                    hex_part = " ".join(f"{b:02x}" for b in chunk)
                    asc_part = "".join(chr(b) if 32 <= b < 127 else "."
                                       for b in chunk)
                    rows.append(f"   +{off:02x}  {hex_part:<48}  {asc_part}")
            rows.append("")
        rows.append("═" * 70)

        with open(path, "w", encoding="utf-8") as f:
            f.write("\n".join(rows))

    # ---------- ОКНО «ПРОТОКОЛ» ----------
    def open_protocol_window(self):
        """Открывает Toplevel с прямым доступом к командам протокола УД2-102.

        Сделано по реверсу PelengPC_ver1_2.exe (см. PROTOCOL.md). Не мешает
        обычному скану — отдельный путь, отдельный COM-открытие.

        Что внутри:
          • выбор COM-порта и backend'а (PySerial / Win32 ctypes);
          • кнопка 0x55  — bulk-dump, парсит 16-байтный device-id header
            и ищет индексы записей в теле дампа;
          • кнопка 0x42 LL HH — чтение записи по 16-битному hex-индексу;
          • кнопка 0x9a  — альтернативная info-команда (только в новом
            режиме прибора);
          • поле «raw bytes» — отправить произвольный hex и прочитать ответ;
          • TX/RX лог в hex с возможностью сохранения в файл.
        """
        if hasattr(self, "_proto_win") and self._proto_win.winfo_exists():
            self._proto_win.lift()
            return
        win = tk.Toplevel(self.root)
        self._proto_win = win
        win.title("Протокол УД2-102 — прямой доступ")
        win.geometry("1100x700")

        # ------------- верхняя панель: порт + backend -------------
        top = tk.Frame(win, pady=8); top.pack(fill=tk.X, padx=10)

        tk.Label(top, text="Порт:", font=("Arial", 10)).pack(side=tk.LEFT)
        cmb_port = ttk.Combobox(top, width=10, state="readonly")
        ports = [p.device for p in list_ports.comports()]
        if not ports:
            ports = [f"COM{i}" for i in range(1, 21)]
        cmb_port["values"] = ports
        cmb_port.set(self.cmb_port.get() or ports[0])
        cmb_port.pack(side=tk.LEFT, padx=(4, 12))

        tk.Label(top, text="Baud:", font=("Arial", 10)).pack(side=tk.LEFT)
        ent_baud = tk.Entry(top, width=8)
        ent_baud.insert(0, str(BAUD))
        ent_baud.pack(side=tk.LEFT, padx=(4, 12))

        tk.Label(top, text="Backend:", font=("Arial", 10)).pack(side=tk.LEFT)
        cmb_backend = ttk.Combobox(top, width=22, state="readonly",
                                   values=["pyserial (стандарт)",
                                           "Win32 ctypes (как exe)"])
        cmb_backend.set("pyserial (стандарт)" if not _IS_WINDOWS
                        else "Win32 ctypes (как exe)")
        cmb_backend.pack(side=tk.LEFT, padx=(4, 12))

        tk.Label(top, text="Inter-byte gap (мс):").pack(side=tk.LEFT)
        ent_gap = tk.Entry(top, width=6)
        ent_gap.insert(0, "10")     # как в exe; ваш быстрый скан = 4.8
        ent_gap.pack(side=tk.LEFT, padx=(4, 12))

        tk.Label(top, text="Read timeout (мс):").pack(side=tk.LEFT)
        ent_to = tk.Entry(top, width=6)
        ent_to.insert(0, "1500")
        ent_to.pack(side=tk.LEFT, padx=4)

        # ------------- средняя панель: команды -------------
        mid_outer = tk.Frame(win); mid_outer.pack(fill=tk.X, padx=10)
        mid = tk.Frame(mid_outer, pady=6); mid.pack(fill=tk.X)
        mid2 = tk.Frame(mid_outer, pady=2); mid2.pack(fill=tk.X)

        # 0x55
        f55 = tk.LabelFrame(mid, text="Bulk dump (0x55)", padx=8, pady=6)
        f55.pack(side=tk.LEFT, padx=4, fill=tk.Y)
        tk.Button(f55, text="Отправить 0x55", bg="#2ecc71", fg="white",
                  font=("Arial", 10, "bold"),
                  command=lambda: self._proto_send_55(
                      cmb_port.get(), ent_baud.get(), cmb_backend.get(),
                      ent_gap.get(), ent_to.get())).pack(fill=tk.X)

        # 0x42 LL HH
        f42 = tk.LabelFrame(mid, text="Чтение записи (0x42 LL HH)",
                            padx=8, pady=6)
        f42.pack(side=tk.LEFT, padx=4, fill=tk.Y)
        row42 = tk.Frame(f42); row42.pack()
        tk.Label(row42, text="idx hex:").pack(side=tk.LEFT)
        ent_idx = tk.Entry(row42, width=8); ent_idx.insert(0, "283d")
        ent_idx.pack(side=tk.LEFT, padx=4)
        tk.Label(row42, text="ждать байт:").pack(side=tk.LEFT, padx=(8, 0))
        ent_want = tk.Entry(row42, width=6); ent_want.insert(0, "86")
        ent_want.pack(side=tk.LEFT, padx=4)
        tk.Button(f42, text="Отправить 0x42", bg="#3498db", fg="white",
                  font=("Arial", 10, "bold"),
                  command=lambda: self._proto_send_42(
                      cmb_port.get(), ent_baud.get(), cmb_backend.get(),
                      ent_gap.get(), ent_to.get(),
                      ent_idx.get(), ent_want.get())).pack(fill=tk.X, pady=(4, 0))

        # 0x9a
        f9a = tk.LabelFrame(mid, text="Info (0x9a)", padx=8, pady=6)
        f9a.pack(side=tk.LEFT, padx=4, fill=tk.Y)
        tk.Button(f9a, text="Отправить 0x9a", bg="#9b59b6", fg="white",
                  font=("Arial", 10, "bold"),
                  command=lambda: self._proto_send_9a(
                      cmb_port.get(), ent_baud.get(), cmb_backend.get(),
                      ent_gap.get(), ent_to.get())).pack(fill=tk.X)

        # raw
        fraw = tk.LabelFrame(mid, text="Произвольные байты",
                             padx=8, pady=6)
        fraw.pack(side=tk.LEFT, padx=4, fill=tk.BOTH, expand=True)
        rrow = tk.Frame(fraw); rrow.pack(fill=tk.X)
        tk.Label(rrow, text="hex:").pack(side=tk.LEFT)
        ent_raw = tk.Entry(rrow); ent_raw.insert(0, "55")
        ent_raw.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=4)
        tk.Label(rrow, text="ждать:").pack(side=tk.LEFT, padx=(6, 0))
        ent_rawn = tk.Entry(rrow, width=6); ent_rawn.insert(0, "256")
        ent_rawn.pack(side=tk.LEFT, padx=4)
        tk.Button(fraw, text="Отправить", bg="#e67e22", fg="white",
                  font=("Arial", 10, "bold"),
                  command=lambda: self._proto_send_raw(
                      cmb_port.get(), ent_baud.get(), cmb_backend.get(),
                      ent_gap.get(), ent_to.get(),
                      ent_raw.get(), ent_rawn.get())
                  ).pack(fill=tk.X, pady=(4, 0))

        # -------- вторая средняя панель: «найти по hint'ам» --------
        fhints = tk.LabelFrame(mid2,
            text="🎯 Полный поиск по hint'ам (как PelengPC.exe)",
            padx=8, pady=6)
        fhints.pack(side=tk.LEFT, padx=4, fill=tk.X, expand=True)
        hrow = tk.Frame(fhints); hrow.pack(fill=tk.X)

        tk.Label(hrow, text="радиус ±:").pack(side=tk.LEFT)
        ent_radius = tk.Entry(hrow, width=4); ent_radius.insert(0, "15")
        ent_radius.pack(side=tk.LEFT, padx=(2, 10))

        tk.Label(hrow, text="streak обрыв:").pack(side=tk.LEFT)
        ent_streak = tk.Entry(hrow, width=4); ent_streak.insert(0, "3")
        ent_streak.pack(side=tk.LEFT, padx=(2, 10))

        tk.Label(hrow, text="lo..hi (hex):").pack(side=tk.LEFT)
        ent_lo = tk.Entry(hrow, width=6); ent_lo.insert(0, "2700")
        ent_lo.pack(side=tk.LEFT, padx=(2, 2))
        tk.Label(hrow, text="..").pack(side=tk.LEFT)
        ent_hi = tk.Entry(hrow, width=6); ent_hi.insert(0, "3aff")
        ent_hi.pack(side=tk.LEFT, padx=(2, 10))

        var_decode = tk.BooleanVar(value=True)
        tk.Checkbutton(hrow, text="smart_decode", variable=var_decode
                       ).pack(side=tk.LEFT, padx=(0, 10))

        tk.Button(hrow, text="🎯 НАЙТИ ВСЕ ПО HINT'АМ",
                  bg="#16a085", fg="white",
                  font=("Arial", 10, "bold"),
                  command=lambda: self._proto_find_by_hints(
                      cmb_port.get(), ent_baud.get(), cmb_backend.get(),
                      ent_gap.get(), ent_to.get(),
                      ent_radius.get(), ent_streak.get(),
                      ent_lo.get(), ent_hi.get(),
                      var_decode.get())
                  ).pack(side=tk.LEFT, padx=(8, 0))

        tk.Button(hrow, text="⏹ Стоп",
                  command=lambda: setattr(self, "_proto_stop", True)
                  ).pack(side=tk.LEFT, padx=4)

        # ------------- нижняя панель: лог + парсер -------------
        bot = tk.Frame(win); bot.pack(fill=tk.BOTH, expand=True, padx=10, pady=5)

        bot_top = tk.Frame(bot); bot_top.pack(fill=tk.X)
        tk.Label(bot_top, text="TX/RX лог", font=("Arial", 10, "bold")
                 ).pack(side=tk.LEFT)
        tk.Button(bot_top, text="Очистить", command=lambda: self._proto_clear()
                  ).pack(side=tk.RIGHT, padx=4)
        tk.Button(bot_top, text="Сохранить лог в .txt",
                  command=lambda: self._proto_save_log()
                  ).pack(side=tk.RIGHT, padx=4)

        self._proto_text = tk.Text(bot, height=20, wrap="none",
                                   font=("Consolas", 9), bg="#1e1e1e",
                                   fg="#d4d4d4", insertbackground="#d4d4d4")
        self._proto_text.tag_configure("tx",   foreground="#f1c40f")
        self._proto_text.tag_configure("rx",   foreground="#2ecc71")
        self._proto_text.tag_configure("info", foreground="#3498db")
        self._proto_text.tag_configure("err",  foreground="#e74c3c")
        self._proto_text.tag_configure("ok",   foreground="#27ae60")
        sb = ttk.Scrollbar(bot, orient="vertical",
                           command=self._proto_text.yview)
        self._proto_text.configure(yscrollcommand=sb.set)
        sb.pack(side=tk.RIGHT, fill=tk.Y)
        self._proto_text.pack(fill=tk.BOTH, expand=True, side=tk.LEFT)

        # Treeview для индексов из дампа
        self._proto_idx_tree = None
        self._proto_log_buf = []
        self._proto_busy = False
        self._proto_append("info",
            "Готов. Команды отправляются в фоновом потоке. "
            "Двойной клик на индексе ниже — отправить 0x42 LL HH.\n")

    # ----- Внутренние методы окна Протокол -----

    def _proto_append(self, tag: str, line: str) -> None:
        """Добавить строку в лог. Безопасно из любого потока."""
        if not hasattr(self, "_proto_text"):
            return
        def _do():
            self._proto_text.insert("end", line, tag)
            self._proto_text.see("end")
            self._proto_log_buf.append(line)
        self.root.after(0, _do)

    def _proto_clear(self):
        if hasattr(self, "_proto_text"):
            self._proto_text.delete("1.0", "end")
            self._proto_log_buf = []

    def _proto_save_log(self):
        if not getattr(self, "_proto_log_buf", None):
            messagebox.showinfo("Лог", "Лог пуст.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=".txt",
            initialfile="peleng_proto_log.txt",
            filetypes=[("Text", "*.txt"), ("All", "*.*")])
        if not path:
            return
        with open(path, "w", encoding="utf-8") as f:
            f.write("".join(self._proto_log_buf))
        messagebox.showinfo("OK", f"Сохранено: {path}")

    @staticmethod
    def _hexdump(data: bytes, width: int = 32) -> str:
        """Компактный hex-дамп: 32 байта в строке + ASCII справа."""
        if not data:
            return "  (пусто)"
        out = []
        for off in range(0, len(data), width):
            chunk = data[off : off + width]
            hx = " ".join(f"{b:02x}" for b in chunk)
            asc = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
            out.append(f"  {off:04x}  {hx:<{width*3}}  {asc}")
        return "\n".join(out)

    # ----- Открытие транспорта по выбранному backend'у -----

    def _proto_open_transport(self, port: str, baud: int, backend: str,
                              timeout_ms: int, inter_byte_ms: int):
        """Открывает транспорт. Возвращает (transport, kind) где kind ∈
        {"win32", "pyserial"}. Кидает исключение при ошибке."""
        if backend.startswith("Win32"):
            if not _IS_WINDOWS:
                raise RuntimeError(
                    "Win32 backend работает только под Windows. "
                    "Выбери 'pyserial (стандарт)'.")
            t = Win32ComPort(port, baud=baud, timeout_ms=timeout_ms,
                             inter_byte_ms=inter_byte_ms)
            t.open()
            return t, "win32"
        # pyserial
        ser = serial.Serial(port, baud, timeout=timeout_ms / 1000.0)
        return ser, "pyserial"

    def _proto_close(self, transport):
        try:
            if isinstance(transport, Win32ComPort):
                transport.close()
            else:
                transport.close()
        except Exception:
            pass

    # ----- Команды (в фоновом потоке) -----

    def _proto_run(self, fn):
        if self._proto_busy:
            self._proto_append("err", "Занято — команда уже выполняется.\n")
            return
        self._proto_busy = True
        def _wrap():
            try:
                fn()
            except Exception as e:
                self._proto_append("err", f"ОШИБКА: {e}\n")
            finally:
                self._proto_busy = False
        threading.Thread(target=_wrap, daemon=True).start()

    def _proto_send_55(self, port, baud, backend, gap_ms, to_ms):
        def _job():
            t_baud = int(baud); t_to = int(to_ms); t_gap = int(float(gap_ms))
            self._proto_append("info",
                f"\n[{time.strftime('%H:%M:%S')}] OPEN {port} {t_baud} "
                f"backend={backend} gap={t_gap}ms timeout={t_to}ms\n")
            tr, kind = self._proto_open_transport(port, t_baud, backend,
                                                  t_to, t_gap)
            try:
                self._proto_append("tx", "TX  55\n")
                proto_write_bytewise(tr, b"\x55", t_gap / 1000.0)
                data = proto_read(tr, 65536, t_to / 1000.0)
                self._proto_append("rx",
                    f"RX  {len(data)} байт:\n{self._hexdump(data)}\n")
                if len(data) >= 16:
                    info = parse_handshake_header(data[:16])
                    self._proto_append("ok",
                        f"PARSE header: device_no={info['device_no']} "
                        f"info_byte=0x{info['info_byte']:02x} "
                        f"id_block_10={info['id_block_10']}\n")
                    body = data[16:]
                    indices = parse_dump_indices(body)
                    self._proto_append("ok",
                        f"PARSE body: {len(body)} байт, "
                        f"найдено {len(indices)} индексов записей "
                        f"по сигнатуре <idx_le> 56 00\n")
                    if indices:
                        chunks = ["  "]
                        for i, ix in enumerate(indices, 1):
                            chunks.append(f"{ix:04x} ")
                            if i % 16 == 0:
                                chunks.append("\n  ")
                        self._proto_append("ok",
                            "".join(chunks).rstrip() + "\n")
                else:
                    self._proto_append("err",
                        "Header < 16 байт — связь по COM не работает "
                        "(см. exe-сообщение 'Связь через СОМ-порт не работает!').\n")
            finally:
                self._proto_close(tr)
        self._proto_run(_job)

    def _proto_send_42(self, port, baud, backend, gap_ms, to_ms,
                       idx_hex, want):
        def _job():
            t_baud = int(baud); t_to = int(to_ms); t_gap = int(float(gap_ms))
            try:
                idx = int(idx_hex.strip().replace("0x", ""), 16)
            except ValueError:
                self._proto_append("err",
                    f"Не могу распарсить индекс '{idx_hex}' как hex.\n")
                return
            n_want = int(want)
            self._proto_append("info",
                f"\n[{time.strftime('%H:%M:%S')}] OPEN {port} {t_baud} "
                f"backend={backend} gap={t_gap}ms timeout={t_to}ms\n")
            tr, kind = self._proto_open_transport(port, t_baud, backend,
                                                  t_to, t_gap)
            try:
                payload = bytes([0x42, idx & 0xff, (idx >> 8) & 0xff])
                self._proto_append("tx",
                    f"TX  42 {payload[1]:02x} {payload[2]:02x}  "
                    f"(idx=0x{idx:04x})\n")
                proto_write_bytewise(tr, payload, t_gap / 1000.0)
                data = proto_read(tr, n_want, t_to / 1000.0)
                self._proto_append("rx",
                    f"RX  {len(data)} байт:\n{self._hexdump(data)}\n")
                if len(data) >= 5:
                    echo = data[0] | (data[1] << 8)
                    marker_ok = (data[2] == 0x56 and data[3] == 0x00)
                    rec_type  = data[4]
                    self._proto_append("ok",
                        f"PARSE record: idx_echo=0x{echo:04x} "
                        f"({'OK' if echo == idx else 'MISMATCH'}), "
                        f"marker 56 00 = {'OK' if marker_ok else 'BAD'}, "
                        f"type=0x{rec_type:02x}\n")
                elif data == b"\xfd\xff" or data[:2] == b"\xfd\xff":
                    self._proto_append("err",
                        "EMPTY (FD FF) — записи по этому индексу нет, либо "
                        "проблема с тайминтгом 0x42 LL HH (попробуй другой gap).\n")
            finally:
                self._proto_close(tr)
        self._proto_run(_job)

    def _proto_send_9a(self, port, baud, backend, gap_ms, to_ms):
        def _job():
            t_baud = int(baud); t_to = int(to_ms); t_gap = int(float(gap_ms))
            self._proto_append("info",
                f"\n[{time.strftime('%H:%M:%S')}] OPEN {port} {t_baud} "
                f"backend={backend} gap={t_gap}ms timeout={t_to}ms\n")
            tr, kind = self._proto_open_transport(port, t_baud, backend,
                                                  t_to, t_gap)
            try:
                self._proto_append("tx", "TX  9a\n")
                proto_write_bytewise(tr, b"\x9a", t_gap / 1000.0)
                data = proto_read(tr, 0x1485, t_to / 1000.0)
                self._proto_append("rx",
                    f"RX  {len(data)} байт:\n{self._hexdump(data)}\n")
                self._proto_append("info",
                    "Команда 0x9a используется только в новом режиме (baud=60000) "
                    "— на стандартном УД2-102 ответ ожидаемо пуст.\n")
            finally:
                self._proto_close(tr)
        self._proto_run(_job)

    def _proto_find_by_hints(self, port, baud, backend, gap_ms, to_ms,
                             radius, streak_n, lo_hex, hi_hex, do_decode):
        """Полный поиск записей через handshake-hints + densify (как делает
        PelengPC.exe v1.2 по результатам реверса).

        Алгоритм:
          1) 0x55 → читаем bulk-dump (~600 байт),
          2) парсим тело на LE16-кандидаты в [lo..hi],
          3) для каждого hint'а сканируем idx-radius..idx+radius
             (с пропуском уже опрошенных и обрывом по `streak_n` пустых
             подряд в каждую сторону),
          4) каждый ответ ≥ 5 байт парсим smart_decode'ом.

        Это даёт те же ~218 записей, что и эталонный «🚀 СТАРТ», но через
        handshake-якоря — стратегия PelengPC, а не sequential 8×100.
        """
        def _job():
            t_baud = int(baud); t_to = int(to_ms); t_gap = int(float(gap_ms))
            r = int(radius); sn = int(streak_n)
            try:
                lo = int(lo_hex.strip().replace("0x", ""), 16)
                hi = int(hi_hex.strip().replace("0x", ""), 16)
            except ValueError:
                self._proto_append("err",
                    f"Не могу распарсить lo/hi как hex: {lo_hex!r}/{hi_hex!r}\n")
                return
            self._proto_stop = False

            self._proto_append("info",
                f"\n[{time.strftime('%H:%M:%S')}] HINTS-SCAN: "
                f"port={port} baud={t_baud} backend={backend} gap={t_gap}ms "
                f"timeout={t_to}ms radius=±{r} streak={sn} "
                f"range=0x{lo:04x}..0x{hi:04x}\n")

            tr, kind = self._proto_open_transport(port, t_baud, backend,
                                                  t_to, t_gap)
            try:
                # ---- шаг 1: 0x55 ----
                self._proto_append("tx", "TX  55  (bulk dump)\n")
                proto_write_bytewise(tr, b"\x55", t_gap / 1000.0)
                # для handshake'а timeout пошире
                hs_to = max(t_to, 1500) / 1000.0
                hs = proto_read(tr, 65536, hs_to)
                self._proto_append("rx",
                    f"RX  handshake = {len(hs)} байт\n")
                if len(hs) < 16:
                    self._proto_append("err",
                        "Handshake < 16 байт — связь сорвана. "
                        "Попробуй ещё раз или переключи backend на Win32.\n")
                    return
                info = parse_handshake_header(hs[:16])
                self._proto_append("ok",
                    f"PARSE header: device_no={info['device_no']}  "
                    f"info_byte=0x{info['info_byte']:02x}\n")
                body = hs[16:]
                hints = parse_dump_indices(body, lo=lo, hi=hi)
                self._proto_append("ok",
                    f"PARSE body: {len(body)} байт, найдено {len(hints)} "
                    f"hint-кандидатов в 0x{lo:04x}..0x{hi:04x}\n")
                if not hints:
                    self._proto_append("err",
                        "Hint'ов 0 — поиск невозможен. "
                        "Проверь связь, baud, backend.\n")
                    return

                # ---- шаг 2: densify вокруг каждого hint'а ----
                # Накапливаем кандидатов в один отсортированный set,
                # потом обходим по возрастанию + ведём streak в обе стороны
                # вокруг каждого якоря.
                anchor_set = set(hints)
                candidates = set()
                for h in hints:
                    for off in range(-r, r + 1):
                        c = h + off
                        if lo <= c <= hi:
                            candidates.add(c)
                # сортируем и обходим
                ordered = sorted(candidates)
                self._proto_append("info",
                    f"После densify ±{r}: {len(ordered)} кандидатов "
                    f"(от 0x{ordered[0]:04x} до 0x{ordered[-1]:04x})\n")

                # Группируем кандидатов в сегменты (последовательные диапазоны).
                # В каждом сегменте идём слева направо и считаем streak
                # пустых; обрываем сегмент после `sn` пустых подряд при
                # условии что хотя бы один hint в нём уже встречен.
                segments = []
                cur = [ordered[0]]
                for x in ordered[1:]:
                    if x == cur[-1] + 1:
                        cur.append(x)
                    else:
                        segments.append(cur)
                        cur = [x]
                segments.append(cur)
                self._proto_append("info",
                    f"Сегментов: {len(segments)}\n")

                # ---- шаг 3: сканируем сегмент за сегментом ----
                t0 = time.perf_counter()
                queried = 0
                hits = 0
                decoded_n = 0
                non_decoded = 0
                results = []   # list of (idx, raw_bytes, decoded_dict_or_None)
                empty_total = 0

                for seg_i, seg in enumerate(segments, 1):
                    if self._proto_stop:
                        break
                    seg_anchor_seen = any(x in anchor_set for x in seg)
                    streak = 0
                    seg_hits = 0
                    for idx in seg:
                        if self._proto_stop:
                            break
                        queried += 1
                        # отправляем 0x42 LL HH побайтово
                        payload = bytes([0x42, idx & 0xff, (idx >> 8) & 0xff])
                        proto_write_bytewise(tr, payload, t_gap / 1000.0)
                        # короткий read: голова или 86 байт
                        # 28 мс idle если EMPTY, 150 мс tail если HIT
                        head_to = 0.030
                        data = proto_read(tr, 2, head_to)
                        if len(data) >= 2 and data[:2] == b"\xfd\xff":
                            empty_total += 1
                            streak += 1
                            if seg_anchor_seen and streak >= sn:
                                break
                            if not seg_anchor_seen and streak >= max(sn, 15):
                                break
                            continue
                        if len(data) < 2:
                            empty_total += 1
                            streak += 1
                            if seg_anchor_seen and streak >= sn:
                                break
                            continue
                        # есть голова — читаем хвост 84 байта
                        tail = proto_read(tr, 84, 0.150)
                        raw = data + tail
                        hits += 1
                        seg_hits += 1
                        streak = 0
                        seg_anchor_seen = True   # после первого хита segment уже якорный
                        d = smart_decode(idx, raw) if do_decode else None
                        if d:
                            decoded_n += 1
                            results.append((idx, raw, d))
                        else:
                            non_decoded += 1
                            results.append((idx, raw, None))
                    # короткий live-апдейт по сегменту
                    self._proto_append("info",
                        f"  сегмент {seg_i}/{len(segments)} "
                        f"(0x{seg[0]:04x}..0x{seg[-1]:04x} len={len(seg)}): "
                        f"hits={seg_hits}\n")

                dt = time.perf_counter() - t0
                self._proto_append("ok",
                    f"\n=== ИТОГО за {dt:.1f} c: запросов={queried}, "
                    f"хитов={hits}, пустых={empty_total}, "
                    f"декодировано={decoded_n}, без даты={non_decoded} ===\n")

                # ---- шаг 4: красивый список найденных ----
                if results:
                    self._proto_append("ok",
                        f"\nНайдено индексов (всего {len(results)}):\n")
                    for idx, raw, d in results:
                        if d:
                            line = (f"  0x{idx:04x}  {d['date']}  {d['type']:<10}  "
                                    f"part={d['part']}  melt={d['melt']}\n")
                        else:
                            line = (f"  0x{idx:04x}  (raw {len(raw)} b, без даты)\n")
                        self._proto_append("ok", line)
            finally:
                self._proto_close(tr)
                self._proto_stop = False
        self._proto_run(_job)

    def _proto_send_raw(self, port, baud, backend, gap_ms, to_ms,
                        raw_hex, want):
        def _job():
            t_baud = int(baud); t_to = int(to_ms); t_gap = int(float(gap_ms))
            try:
                cleaned = "".join(c for c in raw_hex if c in "0123456789abcdefABCDEF")
                if len(cleaned) % 2 == 1:
                    raise ValueError("Нечётное число hex-знаков.")
                payload = bytes.fromhex(cleaned)
            except ValueError as e:
                self._proto_append("err", f"Hex-парсинг: {e}\n")
                return
            n_want = int(want)
            self._proto_append("info",
                f"\n[{time.strftime('%H:%M:%S')}] OPEN {port} {t_baud} "
                f"backend={backend} gap={t_gap}ms timeout={t_to}ms\n")
            tr, kind = self._proto_open_transport(port, t_baud, backend,
                                                  t_to, t_gap)
            try:
                self._proto_append("tx",
                    f"TX  {' '.join(f'{b:02x}' for b in payload)}\n")
                proto_write_bytewise(tr, payload, t_gap / 1000.0)
                data = proto_read(tr, n_want, t_to / 1000.0)
                self._proto_append("rx",
                    f"RX  {len(data)} байт:\n{self._hexdump(data)}\n")
            finally:
                self._proto_close(tr)
        self._proto_run(_job)

    def on_sel(self, e):
        sel = self.tree_c.selection()
        if not sel:
            return
        try:
            b = int(sel[0].split('_')[1])
            for i in self.tree_r.get_children():
                self.tree_r.delete(i)
            for data in self.records.get(b, []):
                vals = tuple(data[col] for col in self.cols)
                self.tree_r.insert("", "end", values=vals)
        except Exception:
            pass

    # ---------- ОТЧЁТ (локальные реквизиты + таблица записей) ----------
    def _capture_device_info(self, hs: bytes):
        """Сохраняет 16-байтный header'а handshake'а для шапки отчёта.

        `hs[2]` — байт версии прошивки (например 0x66 = 6.42 у прибора 4428).
        Используется в окне отчёта; на скан/ГИБРИД не влияет.
        """
        if hs and len(hs) >= 16:
            self.device_info_byte = hs[2]
            self.device_header_hex = hs[:16].hex(" ")

    def open_report_settings(self):
        """Диалог редактирования реквизитов (Предприятие, НТД и т.д.).

        Сохраняет в `peleng_settings.json` рядом со скриптом — это аналог того,
        что PelengPC.exe держит в реестре Windows (на самом приборе их нет,
        проверено реверсом и трассой).
        """
        win = tk.Toplevel(self.root)
        win.title("Реквизиты отчёта")
        win.geometry("520x320")
        win.transient(self.root)
        win.grab_set()

        frm = tk.Frame(win, padx=12, pady=12)
        frm.pack(fill=tk.BOTH, expand=True)

        tk.Label(frm, text="Эти поля сохраняются локально (peleng_settings.json)\n"
                           "и подставляются в шапку отчёта.",
                 fg="#555", justify="left").grid(row=0, column=0, columnspan=2,
                                                  sticky="w", pady=(0, 10))

        entries = {}
        for i, (key, label) in enumerate(REPORT_FIELDS, start=1):
            tk.Label(frm, text=label + ":", anchor="w").grid(
                row=i, column=0, sticky="w", padx=(0, 8), pady=2)
            e = tk.Entry(frm, width=48)
            e.insert(0, self.report_settings.get(key, ""))
            e.grid(row=i, column=1, sticky="we", pady=2)
            entries[key] = e

        frm.grid_columnconfigure(1, weight=1)

        def on_save():
            for k, e in entries.items():
                self.report_settings[k] = e.get().strip()
            save_report_settings(self.report_settings)
            win.destroy()

        btns = tk.Frame(frm)
        btns.grid(row=len(REPORT_FIELDS) + 1, column=0, columnspan=2, pady=(14, 0))
        tk.Button(btns, text="Сохранить", bg="#16a085", fg="white",
                  width=14, command=on_save).pack(side=tk.LEFT, padx=4)
        tk.Button(btns, text="Отмена", width=10,
                  command=win.destroy).pack(side=tk.LEFT, padx=4)

    def _filter_records_by_date(self, rows, d_from: str, d_to: str):
        """Фильтр по строке даты `YYYY-MM-DD HH:MM` (поле `date` в record).

        Пустые `d_from`/`d_to` означают «без ограничения». Сравнение посимвольное —
        работает потому что формат даты лексикографически сортируется.
        """
        if not d_from and not d_to:
            return list(rows)
        out = []
        for r in rows:
            d = (r.get("date") or "")[:10]  # YYYY-MM-DD
            if d_from and d < d_from:
                continue
            if d_to and d > d_to:
                continue
            out.append(r)
        return out

    def open_report_window(self, only_base: int | None = None):
        """Окно отчёта: шапка с реквизитами + таблица записей с фильтром по дате.

        Если `only_base` задан — показываются только записи из этого бакета
        (вызов из двойного клика по бакету). Иначе все 218 записей.
        """
        if only_base is not None:
            rows_all = list(self.records.get(only_base, []))
            bases_sorted = sorted(self.records.keys())
            try:
                bucket_no = bases_sorted.index(only_base) + 1
            except ValueError:
                bucket_no = 0
            title_extra = f" — Отчёт №{bucket_no} ({only_base})"
        else:
            rows_all = self._all_rows()
            title_extra = ""

        if not rows_all:
            messagebox.showwarning(
                "Отчёт",
                "Нет записей. Сначала запустите 🚀 СТАРТ.")
            return

        win = tk.Toplevel(self.root)
        win.title(f"📋 Отчёт — Прибор УД2-102 № {self.device_no or '?'}"
                  f"{title_extra}")
        win.geometry("1200x720")

        # ---- Шапка ----
        head = tk.LabelFrame(win, text="Реквизиты отчёта", padx=10, pady=8)
        head.pack(fill=tk.X, padx=10, pady=(10, 4))

        ver_byte = self.device_info_byte
        if ver_byte:
            ver_str = f"{ver_byte >> 4}.{ver_byte & 0x0f:02x}"
        else:
            ver_str = ""

        device_lbl = f"Прибор УД2-102 № {self.device_no or '?'}"
        if ver_str:
            device_lbl += f"   Версия {ver_str}"
        tk.Label(head, text=device_lbl, font=("Arial", 11, "bold"),
                 fg="#16a085").grid(row=0, column=0, columnspan=4, sticky="w")

        for i, (key, label) in enumerate(REPORT_FIELDS):
            r, c = divmod(i, 2)
            r += 1
            tk.Label(head, text=label + ":", anchor="w", fg="#555"
                     ).grid(row=r, column=2 * c, sticky="w", padx=(0, 6), pady=1)
            val = self.report_settings.get(key, "") or "—"
            tk.Label(head, text=val, anchor="w",
                     font=("Arial", 10, "bold")).grid(
                row=r, column=2 * c + 1, sticky="w", padx=(0, 18), pady=1)

        head.grid_columnconfigure(1, weight=1)
        head.grid_columnconfigure(3, weight=1)

        tk.Button(head, text="⚙ Изменить реквизиты", command=lambda:
                  (win.destroy(), self.open_report_settings())
                  ).grid(row=99, column=0, columnspan=4,
                         sticky="e", pady=(8, 0))

        # ---- Фильтр + действия ----
        flt = tk.Frame(win)
        flt.pack(fill=tk.X, padx=10, pady=4)

        tk.Label(flt, text="Дата от:").pack(side=tk.LEFT)
        ent_from = tk.Entry(flt, width=12)
        ent_from.pack(side=tk.LEFT, padx=(2, 12))
        tk.Label(flt, text="до:").pack(side=tk.LEFT)
        ent_to = tk.Entry(flt, width=12)
        ent_to.pack(side=tk.LEFT, padx=(2, 12))
        tk.Label(flt, text="(YYYY-MM-DD, пусто = все)",
                 fg="#888").pack(side=tk.LEFT)

        lbl_count = tk.Label(flt, text="", fg="#16a085",
                             font=("Arial", 10, "bold"))
        lbl_count.pack(side=tk.RIGHT, padx=8)

        # ---- Таблица записей ----
        body = tk.Frame(win)
        body.pack(fill=tk.BOTH, expand=True, padx=10, pady=(4, 8))

        cols = self.cols
        heads = ["ID", "Дата", "Тип детали", "Объект", "Плавка",
                 "З-д", "Год", "Стр", "Шейка", "Обод",
                 "Обт. колеса", "Нап. гребня"]
        tree = ttk.Treeview(body, columns=cols, show="headings",
                            selectmode="browse")
        for i, c in enumerate(cols):
            tree.heading(c, text=heads[i])
            w = 60 if c in ("id", "year", "stamp", "side") else 90
            tree.column(c, width=w, anchor="center")
        sb = ttk.Scrollbar(body, orient="vertical", command=tree.yview)
        tree.configure(yscrollcommand=sb.set)
        sb.pack(side=tk.RIGHT, fill=tk.Y)
        tree.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

        def refill():
            d_from = ent_from.get().strip()
            d_to = ent_to.get().strip()
            sub = self._filter_records_by_date(rows_all, d_from, d_to)
            sub.sort(key=lambda r: (r.get("date") or "", r.get("id") or 0))
            for it in tree.get_children():
                tree.delete(it)
            for r in sub:
                tree.insert("", "end",
                            values=tuple(r.get(c, "") for c in cols))
            lbl_count.config(text=f"Записей в отчёте: {len(sub)}")
            return sub

        current_rows = refill()

        def on_apply():
            nonlocal current_rows
            current_rows = refill()

        def on_save_txt():
            sub = self._filter_records_by_date(
                rows_all, ent_from.get().strip(), ent_to.get().strip())
            sub.sort(key=lambda r: (r.get("date") or "", r.get("id") or 0))
            self._save_report_txt(sub)

        tk.Button(flt, text="Применить", command=on_apply
                  ).pack(side=tk.LEFT, padx=4)
        tk.Button(flt, text="📄 Сохранить .txt", command=on_save_txt,
                  bg="#16a085", fg="white"
                  ).pack(side=tk.LEFT, padx=4)

    def _save_report_txt(self, rows):
        """Сохраняет отчёт в текстовый файл (стиль шапки PelengPC + таблица)."""
        if not rows:
            messagebox.showwarning("Отчёт", "Нет записей по выбранному фильтру.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=".txt",
            initialfile=f"otchet_{self.device_no}.txt",
            filetypes=[("Text", "*.txt"), ("All", "*.*")])
        if not path:
            return
        try:
            with open(path, "w", encoding="utf-8") as f:
                ver_byte = self.device_info_byte
                ver_str = (f"{ver_byte >> 4}.{ver_byte & 0x0f:02x}"
                           if ver_byte else "?")
                f.write(f"Прибор УД2-102 № {self.device_no}, "
                        f"Версия {ver_str}\n")
                for key, label in REPORT_FIELDS:
                    val = self.report_settings.get(key, "") or "—"
                    f.write(f"{label}: {val}\n")
                f.write(f"Записей: {len(rows)}\n")
                f.write("-" * 80 + "\n")
                # Шапка таблицы
                heads = ["ID", "Дата", "Тип детали", "Объект", "Плавка",
                         "З-д", "Год", "Стр", "Шейка", "Обод",
                         "Обт. колеса", "Нап. гребня"]
                widths = [6, 17, 18, 12, 10, 6, 6, 6, 6, 6, 12, 12]
                line_fmt = "  ".join(f"{{:<{w}}}" for w in widths)
                f.write(line_fmt.format(*heads) + "\n")
                f.write("-" * 80 + "\n")
                for r in rows:
                    f.write(line_fmt.format(
                        *[str(r.get(c, ""))[:widths[i]]
                          for i, c in enumerate(self.cols)]) + "\n")
            messagebox.showinfo("Отчёт", f"Отчёт сохранён:\n{path}")
        except Exception as e:
            messagebox.showerror("Отчёт", f"Ошибка сохранения:\n{e}")

    # ---------- экспорт ----------
    def _all_rows(self):
        rows = []
        for b in self.records.values():
            rows.extend(b)
        return rows

    def export_csv(self):
        rows = self._all_rows()
        if not rows:
            messagebox.showwarning("CSV", "Нет данных для экспорта.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=".csv",
            initialfile=DEFAULT_CSV,
            filetypes=[("CSV (Excel)", "*.csv"), ("All", "*.*")])
        if not path:
            return
        with open(path, "w", newline="", encoding="utf-8-sig") as f:
            w = csv.writer(f, delimiter=";")
            w.writerow(["Номер прибора", "ID", "Дата", "Тип детали", "Объект",
                        "Плавка", "З-д", "Год", "Сторона", "Шейка", "Обод",
                        "Обт. колеса", "Нап. гребня"])
            for data in rows:
                w.writerow([self.device_no] +
                           [data[col] for col in self.cols])
        messagebox.showinfo("OK", f"CSV сохранён:\n{path}")

    def export_sql(self):
        rows = self._all_rows()
        if not rows:
            messagebox.showwarning("SQL", "Нет данных для экспорта.")
            return
        path = filedialog.asksaveasfilename(
            defaultextension=".sql",
            initialfile=DEFAULT_SQL,
            filetypes=[("SQL", "*.sql"), ("All", "*.*")])
        if not path:
            return

        def esc(s):
            return str(s).replace("'", "''")

        with open(path, "w", encoding="utf-8") as f:
            f.write("-- УД2-102 → SQL (peleng_uart_champion)\n")
            f.write("-- device_no, id, date, type_name, num_obj, plavka, "
                    "stamp, god, side, sheika, obod, obtochka, greben\n")
            f.write("CREATE TABLE IF NOT EXISTS measurements (\n"
                    "  device_no INTEGER, id INTEGER, date TEXT,\n"
                    "  type_name TEXT, num_obj TEXT, plavka TEXT,\n"
                    "  stamp INTEGER, god TEXT,\n"
                    "  side TEXT, sheika TEXT, obod TEXT,\n"
                    "  obtochka TEXT, greben TEXT);\n\n"
                    "BEGIN;\n")
            for d in rows:
                f.write(
                    f"INSERT INTO measurements VALUES("
                    f"{self.device_no}, {d['id']}, '{esc(d['date'])}', "
                    f"'{esc(d['type'])}', '{esc(d['part'])}', "
                    f"'{esc(d['melt'])}', {d['stamp']}, '{esc(d['year'])}', "
                    f"'{esc(d['side'])}', '{esc(d['sheika'])}', "
                    f"'{esc(d['obod'])}', '{esc(d['obtochka'])}', "
                    f"'{esc(d['greben'])}');\n")
            f.write("COMMIT;\n")
        messagebox.showinfo("OK", f"SQL сохранён:\n{path}")


if __name__ == "__main__":
    root = tk.Tk()
    app = DefectoApp(root)
    root.mainloop()
