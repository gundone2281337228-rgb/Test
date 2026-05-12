# Полный реверс PelengPC v1.2 + дефектоскоп УД2-102 / УД2-103 (вагонная серия)

Документ собран по результатам многосессионного статического реверса и
кодирования Python-клона `peleng_clone.py`. Цель — дать другой ИИ-системе
готовую «карту» бинарей, протоколов, форматов БД и алгоритмов, чтобы её можно
было использовать для дальнейшего реверса, расширения клона или независимой
реализации.

Все RVA и формулы получены статически (Ghidra/radare2/strings/IDA-style
сравнение, без живого UART-дампа). Где есть сомнения — помечено `[??]`.

---

## 0. Карта бинарей и связанной экосистемы

| Файл | Тип | Что внутри |
|------|-----|------------|
| `PelengPC_ver1_2.exe` | Inno Setup installer | Распаковщик. После установки кладёт `PelengPC.exe`, `102_203dll.dll`, `zapis2.exe`, `*.dal`, `PelengPC.fdb`, `fbembed.dll` + ICU |
| `PelengPC.exe` | Borland C++Builder 2009, PE32 | Главный GUI: COM-порт, импорт `.fdb`, экспорт `.xls`, отчёты `.fr3` |
| `102_203dll.dll` | Delphi 2007 (внутр. имя `dbdll.dll`) | Декодер TLV-кадров от прибора. Экспортирует `_Form_View`, `_SortBufData`, `_FreeBuffer` |
| `zapis2.exe` | Delphi | Хранит словарь `TypeVar` (типовых вариантов деталей) и алгоритмы построения отчётов |
| `PelengPC.fdb` | Firebird 2.5 embedded | Локальная БД ПК (см. § 5) |
| `PelengPCtest.dal` | Custom binary schema | Описание схем декодирования RESULTS2/SHORTPROT2 (поля TLV-блоков) |
| `fbembed.dll`, `libicu*.dll` | Firebird Embedded 2.5.9 + ICU | Нужны для чтения `.fdb` |
| `*.fr3` | FastReport v3 template | Шаблоны отчётов («ОТД» / «Вагонная» / «общая») |

**Версии прибора:**
- Версия 1 («Базовая») — толщиномер, листы металла, сварные швы
- Версия 2 («Вагонная» / «Железнодорожная») — оси РУ1/РУ1Ш, колёсные пары
- Версия 3 («Рельсовая» / «ОТД») — рельсы Р50/Р65/Р75/Р43, стыки, стрелочные переводы

Каждой версии в FDB соответствует своя таблица (`NASTR1/2/3`, `RESULTS1/2/3`, `SHORTPROT1/2/3`).

---

## 1. Низкоуровневый UART-протокол PelengPC ↔ Прибор

### 1.1. Физика

- RS-232, 8N1, без flow-control
- Скорость подбирается из `(1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200)`
  через 4× handshake `0x55`
- Между байтами команды — `Sleep(10 ms)` (busy-wait через `GetTickCount`,
  RVA `0x00423a58` в `PelengPC.exe`)

### 1.2. Команды (РЕВЕРС `fcn.00423f0c` — единственный SendByte-wrapper)

В `PelengPC.exe` ровно 5 caller'ов SendByte-wrapper'а. Все опкоды:

| op | имя | формат запроса | ответ | RVA caller |
|----|-----|----------------|-------|------------|
| `0x55` | `HANDSHAKE` | `0x55` | 16 байт (serial+fw+date+payload_len) | `0x004249c0` |
| `0x42` | `BLOCK_REQ` | `0x42` `LL` `HH` | 16 байт заголовка + TLV-поток | `0x00424cc0` |
| `0x74` | `STATUS_T` | `0x74` | байты статуса/идентификатора | `0x00422828` |
| `0x9a` | `FLASH_DUMP` | `0x9a` | 0x10c5 или 0x1485 байт `.fla` | `0x0041cbf4` |

**Других опкодов в бинаре НЕТ.** Никаких `CRC`, `RTC sync` (SetLocalTime),
`erase`, `lock` — все эти импорты отсутствуют (`rabin2 -i` → пусто).

### 1.3. Заголовок ответа (16 байт)

```
offset  поле                            тип
+0      record_id                       LE16
+2      sub_type                        u8
+3      flags                           u8
+4      time_min, time_h                u8, u8
+6      payload_len                     LE16
+8      version_byte, version_byte2     u8, u8 (firmware vMAJ.MIN)
+10     date_d, date_m                  u8, u8
+12     ...                             4 байта (zero/pad)
```

### 1.4. TLV-формат (тело после заголовка)

Каждая TLV-запись: `tag (LE16)` `len (LE16)` `body (len байт)`.
Тег `0xFFFF` — padding (пропускается).

### 1.5. Классификатор тегов — `_SortBufData` @ `0x00401454` в DLL

DLL делит входные теги на 5 категорий через LUT из 30 байт @ `0x00401670`:

```
                  0   1   2   3   4   5   6   7   8   9
LUT[0..29] = [ 00, 05, 00, 00, 03, 04, 03, 00, 00, 00,    # 0..9
               02, 02, 02, 02, 02, 02, 02, 02, 02, 02,    # 10..19
               01, 01, 01, 01, 01, 01, 01, 01, 01, 01 ]   # 20..29
```

Алгоритм:
```python
tcode = (tag & 0xFFFF) // 1000   # idiv 1000 в дизасме
if 0 <= tcode < 30:
    case_idx = LUT[tcode]
    # case_idx → (категория, body[0] writeback)
```

| case_idx | категория | DLL пишет body[0] | RVA handler в DLL |
|----------|-----------|-------------------|-------------------|
| 1 | A-развёртка | 4 | `0x00403420` |
| 2 | B-развёртка | 3 | `0x004031dc` |
| 3 | Generic v3 (отчёт-значение) | 1 | `0x00402c8c` |
| 4 | Generic v4 | 1 | `0x00402f34` |
| 5 | Settings | 2 | `0x00402980` |
| 0 | unknown — пропустить | — | — |

Семейства тегов:
- `tag/1000 == 1` → Settings (`1xxx`)
- `tag/1000 ∈ {4,5,6}` → Generic (`4xxx`, `5xxx`, `6xxx`)
- `tag/1000 ∈ {10..19}` → B-scan (`10xxx..19xxx`)
- `tag/1000 ∈ {20..29}` → A-scan (`20xxx..29xxx`)

### 1.6. Адреса ключевых функций

**`PelengPC.exe`:**
```
0x00423e58  SendByte_lowlevel      — WriteFile + busy-wait timeout
0x00423f0c  SendByte_wrapper       — обёртка
0x00423f64  ReadFile               — ReadFile с тайм-аутом
0x00423a58  Sleep_busy_10ms        — GetTickCount-loop (inter-byte delay)
0x004249c0  TestPort_handshake     — 'U' → 16 байт
0x00424bb0  SendOpcode_recv        — 1-byte opcode + receive
0x00424cc0  SendBlockReq_recv      — 'B' LL HH + receive
0x0041cbf4  Cmd_0x9a_FlashDump     — caller of 0x9a
0x00422828  Cmd_0x74_StatusT       — caller of 0x74
```

**`102_203dll.dll`:**
```
0x0040131c  _Form_View             — export #1
0x00401454  _SortBufData           — export #2 (классификатор TLV)
0x0040193c  _FreeBuffer            — export #3
0x00401670  TLV_LUT_30             — 30-байт LUT
0x00403420  AScan_handler          — body[0]=4
0x004031dc  BScan_handler          — body[0]=3
0x00402c8c  Generic3_handler       — body[0]=1
0x00402f34  Generic4_handler       — body[0]=1
0x00402980  Settings_handler       — body[0]=2
```

---

## 2. Протокол УД2-102 (рельсовый дефектоскоп — отдельный слой)

Это **второй, независимый** протокол поверх того же RS-232. УД2-102 не отдаёт
TLV-блоки — он отдаёт фиксированные 86-байт записи по адресам.

### 2.1. Физика (отличается от PelengPC!)

| Параметр | Значение |
|----------|----------|
| Скорость | 19200 8N1 |
| Inter-byte gap | 4.8 мс |
| RX response timeout | 28 мс |
| RX body timeout | 150 мс |
| Handshake | 4× `0x55`, пауза 100 мс, response timeout 1500 мс |

### 2.2. Команды

| op | имя | формат | ответ |
|----|-----|--------|-------|
| `0x55` | `UD2_HANDSHAKE` | `0x55` (×4) | 16 байт; `bytes[0..1] LE = device_no` |
| `0x42` | `UD2_READ_RECORD` | `0x42` `Addr_LE16` | 2 байта (header) + 84 байта (BCD) |
| `0x4B` | `UD2_SEND_KEY` | `0x4B` `key_code` | — (виртуальная клавиатура) |
| `0x49` | `UD2_SCREEN_DUMP` | `0x49` | ~15 КБ видеобуфера |
| `0x54` | `UD2_RTC_SYNC` | `0x54` `YY MM DD hh mm ss` | — |

Виртуальная клавиатура: `0x01=ENTER`, `0x02=CANCEL`, `0x03=LEFT`, `0x04=RIGHT`,
`0x05=UP`, `0x06=DOWN`.

### 2.3. Карта адресов (sweep)

```
UD2_BASES = (10100, 10200, 10300, 10400, 10500, 10600, 13300, 13400)
UD2_INDICES_PER_BASE = 100        # i = 0..99 в каждой базе
addr = base + i
```

База + индекс кодируются в LE16.

### 2.4. Streak-эвристика (когда останавливать sweep базы)

- Пустые блоки (`0xFD 0xFF` или `0xFF 0xFF` в первых 2 байтах) считаются.
- **До первого хита**: если 15 пустых подряд — следующая база.
- **После первого хита**: если 2 пустых подряд — следующая база.

### 2.5. Формат BCD-паспорта (84 байта)

Поиск «плавающего якоря» — 5 байт даты с разумным диапазоном:
```python
def is_valid_date_anchor(b: bytes) -> bool:
    return (1  <= b[0] <= 31  and    # day
            1  <= b[1] <= 12  and    # month
            15 <= b[2] <= 40  and    # year (20YY)
            0  <= b[3] <= 23  and    # hour
            0  <= b[4] <= 59)        # minute
```

От якоря `i` парсятся поля по фиксированным смещениям. См.
`parse_ud2102_record()` в `peleng_clone.py` (строки 935–988). Ключевые поля:

| смещение | поле | формат |
|----------|------|--------|
| `i+0..4` | дата/время | BCD bytes |
| `i+5..6` | type-code | LE16 |
| `i+7` | sub-byte (вариант) | u8 |
| `i+10..18` | плавка (Plavka) | Reverse-BCD, terminator `0x0A` |
| `i+20..28` | № объекта | Reverse-BCD |
| `i+30` | сторона колеса | 0=Левая, 1=Правая |
| `i+31` | шейка | 0=С кольцами, 1=Без колец, 2=С втулкой |
| `i+32..33` | толщина обода | LE16 в десятых мм |
| `i+34` | категория обточки | u8 |
| `i+35..36` | гребень | LE16 (десятые мм) |
| `i+37` | наплавка | 0=Нет, 1=Восстановлена наплавкой |

(Точные смещения см. в `parse_ud2102_record`; они подобраны эмпирически
под фиктивные сэмплы и реальные дампы.)

### 2.6. Reverse-BCD

Каждый байт = 2 десятичные цифры (high|low nibble), порядок байт обратный,
терминатор `0x0A`, ведущие нули отсекаются:

```python
def reverse_bcd(buf: bytes) -> str:
    digits = []
    for b in reversed(buf):
        if b == 0x0A: break
        hi, lo = (b >> 4) & 0xF, b & 0xF
        if hi <= 9: digits.append(str(hi))
        if lo <= 9: digits.append(str(lo))
    return "".join(digits).lstrip("0")
```

---

## 3. Словарь TypeVar (типовых вариантов деталей)

Взят из `zapis2.exe` (Delphi-строковый сегмент) + ресурсов в DFM.
Ключ — `(tcode, sub_byte)` или `(tcode, None)`.

### 3.1. Железнодорожная / вагонная (наиболее частая)

| tcode | sub | имя |
|-------|-----|-----|
| 24667 | 1 | Ось РУ1 |
| 24667 | 0 | Ось РУ1Ш |
| 57435 | 1 | Ось РУ1 |
| 57435 | 0 | Ось РУ1Ш |
| 24668 | 1 | Ось РВ2Ш |
| 24668 | 0 | Ось РУ1Ш-957 |
| 24669 | — | Ось колёсной пары грузового вагона |
| 24670 | — | Ось локомотива ТЭП70 |
| 24671 | — | Ось ВЛ80 |
| 24672 | — | Ось ЭП1М |
| 24750 | — | Колёсная пара (РУ1) |
| 24751 | — | Колёсная пара (РУ1Ш) |
| 24800 | — | Бандаж локомотива |
| 24801 | — | Бандаж пассажирского вагона |
| 24802 | — | Бандаж грузового вагона |
| 24803 | — | Обод колёсной пары |
| 24850 | — | Гребень колёсной пары |
| 24851 | — | Гребень бандажа |
| 24900 | — | Шейка оси (зеркальный канал) |
| 24901 | — | Подступичная часть |
| 24902 | — | Средняя часть оси |

### 3.2. Базовая

| tcode | имя |
|-------|-----|
| 10001 | Толщиномер |
| 10002 | Стенка трубы |
| 10003 | Лист металла |
| 10004 | Сварной шов |

### 3.3. Рельсовая (ОТД)

| tcode | имя |
|-------|-----|
| 40001 | Рельс Р50 |
| 40002 | Рельс Р65 |
| 40003 | Рельс Р75 |
| 40004 | Рельс Р43 |
| 40010 | Стрелочный перевод |
| 40020 | Сварной стык рельса |
| 40030 | Болтовое отверстие в рельсе |
| 40040 | Подошва рельса |
| 40050 | Шейка рельса |
| 40060 | Головка рельса |

### 3.4. Алгоритм поиска

```python
def decode_type_name(tcode, sub_b):
    name = TYPEVAR_DICT.get((tcode, sub_b))    or \
           TYPEVAR_DICT.get((tcode, None))
    return name or "Ось колесной пары"          # legacy fallback
```

### 3.5. FDB `TYPEVAR` — отдельная нумерация

В таблицах FDB (`NASTR2`/`RESULTS2`/`SHORTPROT2`) колонка `TYPEVAR` —
это **не tcode**, а малый индекс (0..851) во внутренний справочник
PelengPC (хранится в DFM-ресурсах `zapis2.exe`). Соглашение по диапазонам:

- `0` — не задан / общий
- `1xx` — Базовая (толщина / стенка / лист)
- `2xx` — Железнодорожная (ось колёсной пары)
- `3xx` — Бандажи / обода
- `5xx` — Конфигурации настройки прибора
- `7xx` — Вагонная: оси РУ1/РУ1Ш/РВ2Ш
- `8xx` — Вагонная: колёсные пары / бандаж / гребень

Полный словарь — `FDB_TYPEVAR_DICT` в `peleng_clone.py` (строки 771–820).

---

## 4. Физика прибора (нужно для A-/B-развёртки)

### 4.1. dB-байт

```python
def byte_to_db(b): return b / 2.0 - 64.0
```
(8-bit unsigned, 0.5 dB на шаг, диапазон −64..+63.5 dB)

### 4.2. Sample → микросекунды

```python
def sample_to_us(idx, sample_period_ns=200):
    return idx * sample_period_ns / 1000.0
```

### 4.3. μs → глубина (mm)

```python
def us_to_depth_mm(us, sound_speed_mm_per_us=2.95):
    # Двукратный путь (туда-обратно)
    return us * sound_speed_mm_per_us / 2.0
```

### 4.4. ARD verdict (АРД-диаграмма)

```python
def ard_verdict(amp_db, threshold_db, hysteresis_db=2.0):
    delta = amp_db - threshold_db
    if   delta >  hysteresis_db: return ("БРАК",   delta)
    elif delta < -hysteresis_db: return ("ГОДЕН",  delta)
    else:                        return ("СОМНИТ", delta)
```

### 4.5. V-тракт коррекция (для эхо-сигнала с гребня)

`vtract_correction(x_mm, axle_radius_mm=65.0)` — геометрическая коррекция
эхо-времени на радиус оси. Формула:
```
y = x_mm * (1 - x_mm / (4*R))
```

### 4.6. Эталонные параметры

```python
UD2_DEFAULT_RAXLE_MM = 65.0   # типовой радиус оси РУ1
SAMPLE_PERIOD_NS     = 200    # 5 МГц АЦП (по умолчанию)
SOUND_SPEED_MM_PER_US = 2.95  # сталь, поперечная волна
```

---

## 5. Схема `PelengPC.fdb` (Firebird Embedded 2.5)

### 5.1. Таблицы

| Таблица | Назначение |
|---------|-----------|
| `NASTR1/2/3` | Настройки прибора (один кадр на запись) |
| `RESULTS1/2/3` | Протоколы (А-развёртки + полная сессия TLV) |
| `SHORTPROT1/2/3` | Короткие отчёты (наплавка/сторона/толщины) |
| `BLOCKZAP` | Сырые TLV-блоки (по одному на каждую `Number`/`NumKod`) |
| Справочники: TYPEVAR_DICT, NUMOBJ_HISTORY и т.д. |

Цифра в имени таблицы — версия комплектации (1=базовая, 2=вагонная, 3=рельсовая).

### 5.2. Ключевые столбцы (на примере SHORTPROT2)

```
NUMBER       INT       PK (порядковый № в таблице)
NUMPRIB      INT       заводской № прибора
NUMKOD       INT       FK на BLOCKZAP.Number
DATEFORM     CHAR(10)  ISO дата ('YYYY-MM-DD')
TIMEFORM     CHAR(8)   время ('HH:MM:SS')
NAMEOPERA    VARCHAR   оператор (CP1251)
INDMAKER     VARCHAR   индекс завода
MAKETIME     INT       дата изготовления (год 4 цифры)
DEFEKT       VARCHAR   описание дефекта
CODEDEF      VARCHAR   код дефекта
TYPEVAR      INT       индекс типового варианта (см. § 3.5)
TYPEZAP      VARCHAR   тип записи (текст)
NUMOBJ       VARCHAR   № объекта
SMELTING     VARCHAR   плавка
PROTOCOL     BLOB      84-байт BCD-тело (тот же формат, что UART-кадр)
```

**Важно:** в `SHORTPROT2` поле `PROTOCOL` — это **BLOB с BCD-записью того же
формата, что приходит по UART** (см. § 2.5). Внутри лежат
сторона/шейка/обод/гребень/наплавка/плавка/№объекта. Чтобы их вытащить —
парсить `_try_parse_ud2_bcd()`.

### 5.3. BLOCKZAP — сырые TLV-блоки

```
NUMBER  INT      PK (NUMKOD)
BLOCK   BLOB     16-байт заголовок + TLV-поток (то же тело, что UART-блок)
```

Тело — стандартный PelengPC-блок (см. § 1.3–1.4). В TLV-записи могут лежать
A-/B-развёртки, settings и (для RESULTS-строк) BCD-паспорт УД2-102 завёрнутый
в TLV-обёртку. Стратегия извлечения:
```python
def _try_extract_ud2_bcd_from_tlv(blockzap_block):
    body = blockzap_block[16:]            # после заголовка
    for tlv in parse_tlv(body):
        rec = _try_parse_ud2_bcd(tlv.body)
        if rec is not None: return rec
    # fallback — побайтовый сканер всего блока
    return _try_parse_ud2_bcd(blockzap_block)
```

---

## 6. Архитектура Python-клона `peleng_clone.py`

Один файл, ≈ 6150 строк, без внешних модулей кроме `pyserial`, `PyQt6` и
(опц.) `openpyxl`, `fdb`.

### 6.1. Слои

```
┌─────────────────────────────────────────────────────────────┐
│ GUI (PyQt6)                                                 │
│  - MainWindow с вкладками:                                  │
│    «Поля» / «A-развёртка» / «B-развёртка» / «Отчёт»         │
│    «Записи (УД2-102)» ── 3 под-вкладки:                     │
│       ├ Настройки    (Source='FDB-NASTR')                   │
│       ├ Протоколы    (Source='FDB-RESULTS')                 │
│       └ Отчёты       (Source IN ('FDB-SHORTPROT','UD2'))    │
│  - Двойной клик: protocol → A-scan; report → «Отчёт»        │
├─────────────────────────────────────────────────────────────┤
│ Декодеры                                                    │
│  - parse_tlv / decode_all (PelengPC TLV)                    │
│  - parse_ud2102_record (BCD-паспорт)                        │
│  - decode_ud2102_acoustic (настройки УЗ-тракта)             │
│  - _try_parse_ud2_bcd / _try_extract_ud2_bcd_from_tlv       │
├─────────────────────────────────────────────────────────────┤
│ Транспорты                                                  │
│  - PelengPort   (PelengPC TLV-блоки)                        │
│  - UD2_102Port  (УД2-102 BCD-записи)                        │
├─────────────────────────────────────────────────────────────┤
│ Хранилище                                                   │
│  - BlockZapDB (SQLite ~/.peleng_clone/blockzap.sqlite3)     │
│    таблицы: Measurements, BlockZap, FdbRecord, FdbBlock     │
│  - fdb_import_to_db (Firebird → SQLite, dual-write)         │
└─────────────────────────────────────────────────────────────┘
```

### 6.2. Таблица `Measurements` (24 столбца — единая для UD2 и FDB)

```sql
CREATE TABLE Measurements(
  DeviceNo INT, Addr INT,
  Date     TEXT,                   -- 'YYYY-MM-DD HH:MM'
  TypeName TEXT, NumObj TEXT, Plavka TEXT, Stamp INT, God INT,
  Side     TEXT, Sheika TEXT, Obod TEXT, Obtochka TEXT,
  Greben   TEXT, Naplavka TEXT,
  Raw      BLOB,                   -- 84-байт BCD
  TimeStr  TEXT, Operator TEXT, IndMaker TEXT,
  MakeTime INT, Defekt TEXT, CodeDef TEXT,
  TypeVar  INT, NumKod INT,
  Source   TEXT,                   -- 'UD2' | 'FDB-NASTR' | 'FDB-RESULTS' | 'FDB-SHORTPROT'
  Saved    TEXT DEFAULT (datetime('now')),
  PRIMARY KEY(DeviceNo, Addr)
);
```

### 6.3. Адресация FDB-записей

Чтобы FDB-строки не пересекались с UART-адресами УД2-102 (которые лежат в
диапазоне 0x2774..0x34AC), для FDB используется маска:
```python
FDB_ADDR_OFFSET = 0xF000_0000
addr = FDB_ADDR_OFFSET | number          # number из FDB-таблицы
device_no = NUMPRIB                       # заводской № прибора
```

### 6.4. Двойная запись при импорте FDB

```python
# fdb_import_to_db (упрощённо):
for row in fetch_all(f"SELECT * FROM {tbl}"):
    db.upsert_fdb_record(row, kind, variant)
    db.upsert_measurement_from_fdb(kind, variant, row, raw_blob=blockzap_block)

# После импорта BLOCKZAP — второй проход «обогащения»:
db.enrich_measurements_from_blocks()      # вытаскивает BCD из TLV-тел
```

### 6.5. Очистка «квадратиков» в строках FDB

```python
def _strip_unprintable(s: str) -> str:
    """Срезает U+0000..U+001F (кроме \\t \\n \\r), U+007F и U+0080..U+009F."""
    out = []
    for ch in s:
        o = ord(ch)
        if o < 0x20 and o not in (0x09, 0x0A, 0x0D): continue
        if o == 0x7F: continue
        if 0x80 <= o <= 0x9F: continue
        out.append(ch)
    return "".join(out).strip()
```

Применяется в `_opt_str()` и в `upsert_measurement_from_fdb()` к датам и
именам типов. Без неё ~10 строк FDB отображаются в Qt-таблице как
«квадратик» (Qt рисует control chars как U+FFFD-glyph).

### 6.6. Двойной клик в под-вкладках «Записи (УД2-102)»

```python
def _on_ud2_open_protocol(row, settings=False):
    meta  = self._ud2_row_meta(table, row)        # достаём dict из UserRole
    block, fw = self._load_blockzap_for_row(meta)
    recs = decode_all(list(parse_tlv(block)), fw)
    self._show_records(recs, raw_block=block)
    # выбираем вкладку:
    if any(isinstance(r, AScanRecord) for r in recs):
        self.tabs.setCurrentWidget(self.ascan)
    elif any(isinstance(r, BScanRecord) for r in recs):
        self.tabs.setCurrentWidget(self.bscan)
    else:
        self.tabs.setCurrentWidget(self.fields_view)

def _on_ud2_open_report(row):
    meta = self._ud2_row_meta(self.ud2_table_reports, row)
    block, fw = self._load_blockzap_for_row(meta)
    recs = decode_all(...) if block else []
    # собираем HTML-отчёт: 19 полей шапки из Measurements + поля из TLV
    self.report_view.setHtml(html_parts)
    self.tabs.setCurrentWidget(self.report_view)
```

---

## 7. Что **не покрыто** и где остались пробелы

### 7.1. Покрыто статически, не валидировано на живом приборе

- **Скорость подбирается перебором** — `4× 0x55` без подтверждения,
  что прибор пришлёт ответ на конкретной из 8 скоростей. Нужен дамп
  живой сессии для подтверждения порядка.
- **Streak-эвристика sweep'а** (15/2 пустых) — взята из gemini-наработок,
  не проверена на реальной памяти.

### 7.2. Покрыто частично

- **Команда `0x9a` (FLASH_DUMP)** — известно, что отдаёт `0x10c5` или
  `0x1485` байт `.fla`-файла прошивки. Парсер `.fla` не реализован.
- **Команда `0x74` (STATUS_T)** — известно, что отдаёт байты статуса,
  но смысл байтов не реверсирован.
- **TYPEVAR-индексы 1..851 в FDB** — известны только диапазоны,
  не реверсированы точные имена из DFM-ресурсов `zapis2.exe`
  (нужен `wrestool` / Resource Hacker).

### 7.3. Не покрыто

- **Шаблоны FastReport `.fr3`** — клон пишет собственный текстовый
  формат с magic `"PELENG-CLONE-FR3"`, оригинальные шаблоны не
  парсятся. Нужен парсер FastReport v3 (опубликован, формат
  бинарный + property-stream).
- **AРД-таблицы и ВРЧ-кривые** в FDB — есть BLOB-колонки, не
  распарсены.

### 7.4. Возможные расширения

- B-scan визуализация с реальной геометрией (сейчас цветной растр без
  привязки к мм)
- Полный парсер `.dal` (PelengPCtest.dal) — описывает схемы декодирования
  всех TLV-кадров, можно автоматически генерировать поля
- Live-протокольный sniffer для записи реальных UART-дампов

---

## 8. Запуск и проверка клона

### 8.1. Зависимости

```bash
python3 -m pip install PyQt6 pyserial openpyxl fdb
# fbembed (для импорта PelengPC.fdb):
# Linux:   sudo apt install firebird2.5-classic-server  ИЛИ ручной деплой
#          libfbembed.so.2.5 в ~/.peleng_clone/fb25/
# Windows: положить fbembed.dll рядом с .exe / в %PATH%
```

### 8.2. CLI

```bash
python3 peleng_clone.py --demo                # GUI с фейковыми данными
python3 peleng_clone.py proto-test --port COM1
python3 peleng_clone.py ud2102-scan --port /dev/ttyUSB0
python3 peleng_clone.py fdb-info  PelengPC.fdb
python3 peleng_clone.py fdb-import PelengPC.fdb
python3 peleng_clone.py rev-info                # все RVA + LUT
python3 peleng_clone.py fbembed-install         # авто-загрузка fbembed
```

### 8.3. Тесты

```bash
python3 -m py_compile peleng_clone.py
python3 peleng_sanity.py                       # 12/12 unit-tests
QT_QPA_PLATFORM=offscreen timeout 10 python3 peleng_clone.py --demo
```

---

## 9. Краткая шпаргалка по reverse-инструментам

| Файл | Инструмент | Что искать |
|------|-----------|------------|
| `*.exe` (BCB) | radare2 / Ghidra | `axt` для caller'ов SendByte/ReadFile |
| `*.dll` (Delphi 2007) | IDR + Ghidra | Delphi RTTI, exports, _SortBufData LUT |
| `zapis2.exe` (Delphi) | IDR + strings | DFM-ресурсы, TypeVar dict |
| `*.fdb` (Firebird 2.5) | isql / Python `fdb` | Схема таблиц, BLOB-поля |
| `*.dal` (custom) | hex + strings + структурный анализ | Описание TLV-схем |
| `*.fr3` (FastReport) | FastReport viewer / fr3-parser | Шаблоны отчётов |

Импорты, которые **НЕ найдены** (важно для гипотез):
- `Crc*` — отсутствует и в DLL, и в EXE → **CRC в протоколе нет**
- `SetLocalTime` / `SetSystemTime` — отсутствует → **синхронизации часов с ПК нет**
- `Get/SetTimeZone` — отсутствует
- `WMI`, `RegSetValue*` — отсутствует (нет писания в реестр от имени прибора)

---

## 10. Лицензия и атрибуция

Это клон создан в образовательных целях по официально распространяемому
бесплатному инсталлятору PelengPC v1.2 от ОАО «Пеленг». Формат `.fdb`
реверсирован читая только schema и данные, без модификации firmware
прибора. Реверс сделан статически — без подключения к реальному
дефектоскопу, без декомпиляции защищённых ресурсов и без обхода каких-либо
защитных механизмов (их в бинаре нет).
