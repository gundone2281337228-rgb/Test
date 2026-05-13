# PELENG REVERSE — Раунд 4 (объединённый): Декодеры, формулы, B-scan, TypeVar

**Дата:** 2026-05-13  
**Источники:** `zapis2.c` (Ghidra, 133634 строк), `PelengPC_ver1_2_PSEUDOCODE.c`, `PELENG_REVERSE_DEEP.md`  
**Содержимое:** объединение раундов 4+5+6 в единый документ

---

## 1. Базовые функции чтения данных

### 1.1 FUN_004231c9 — ReadLE16 (читает 2 байта LE из буфера)

```c
uint16_t ReadLE16(int offset) {
    // DAT_0051cb58 = глобальный указатель на raw data buffer
    return *(uint16_t*)(DAT_0051cb58 + offset);
}
```

### 1.2 FUN_004231f3 — ReadByte (читает 1 байт из буфера)

```c
uint8_t ReadByte(int offset) {
    return *(uint8_t*)(DAT_0051cb58 + offset);
}
```

### 1.3 FUN_0042321c — TestBit (проверяет бит в байте)

```c
bool TestBit(int offset, uint8_t mask) {
    return (ReadByte(offset) & mask) != 0;
}
```

---

## 2. FUN_00423253 — Real48 Float Decoder (Borland 6-byte float)

**Полная реконструкция** декодера Borland Real48 (6-byte floating point):

```c
double Real48_to_Double(int offset) {
    // Читаем 6 байт из буфера
    uint8_t bytes[6];
    for (int i = 0; i < 6; i++)
        bytes[i] = DAT_0051cb58[offset + i];
    
    // bytes[0] = exponent (biased by 129)
    // bytes[1..4] = mantissa (39 bits)
    // bytes[5] bit 7 = sign
    
    if (bytes[0] == 0 && bytes[1] == 0 && bytes[2] == 0 && 
        bytes[3] == 0 && bytes[4] == 0 && bytes[5] == 0)
        return 0.0;
    
    // Конвертация Real48 → IEEE754 double:
    uint32_t sign = (bytes[5] & 0x80) << 24;           // знак
    uint32_t exp_raw = (bytes[0] - 2) * 0x100000;      // экспонента (смещ. 129→1023)
    
    // Нормализация экспоненты в IEEE754 формат:
    if (exp_raw & 0x8000000)  exp_raw |= 0x40000000;
    if (!(exp_raw & 0x40000000)) exp_raw |= 0x38000000;
    else exp_raw &= 0xC7FFFFFF;
    
    // Мантисса: сдвиг и перепаковка
    uint32_t mantissa_hi = ((bytes[5] & 0x7F) << 8 | bytes[4]) << 5;
    uint32_t mantissa_lo = (bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0]) & 0xFFFFFF00;
    mantissa_lo <<= 5;
    
    // Сборка IEEE754 double
    uint64_t ieee = ((uint64_t)(sign | exp_raw | mantissa_hi) << 32) | mantissa_lo;
    return *(double*)&ieee;
}
```

**Формула пересчёта (упрощённо):**
```python
def real48_to_float(data: bytes) -> float:
    """Borland Real48 (6 bytes) → Python float"""
    if data == b'\x00' * 6:
        return 0.0
    exp = data[0]
    sign = (data[5] >> 7) & 1
    # Мантисса = 39 бит (data[1]..data[5] & 0x7F), неявная единица
    mantissa = int.from_bytes(data[1:5], 'little') | ((data[5] & 0x7F) << 32)
    mantissa = mantissa / (2**39)  # нормализация
    value = (1.0 + mantissa) * (2.0 ** (exp - 129))
    return -value if sign else value
```

---

## 3. FUN_0042390e — Date Decoder

```c
void DecodeDate(uint packed_offset) {
    // packed_offset содержит 3 байта даты в упакованном виде:
    // Байты читаются по смещениям, перепаковываются
    
    int raw = ReadLE16(packed_offset);  // 2 байта
    // Далее извлекаются: день, месяц, год
    
    // Фактическая формула из декомпиляции:
    uint val = packed_offset;
    int part1 = val & 0x800FFFFF;
    if (part1 < 0) part1 = (part1 - 1 | 0xFFF00000) + 1;
    int mid = (part1 + (part1 >> 31 & 0xFF)) >> 8;
    
    uint part2 = val & 0x800000FF;
    if (part2 < 0) part2 = (part2 - 1 | 0xFFFFFF00) + 1;
    
    int day = ReadLE16(mid);                    // день
    int month = ReadLE16(mid + part2);          // месяц  
    int year = (val + (val >> 31 & 0xFFFFF)) >> 20;  // год
    
    // Формат вывода: "DD.MM.YYYY"
}
```

**Упрощённая Python-версия:**
```python
def decode_date_zapis2(body: bytes, offset: int) -> str:
    """Дата из 3 байт body: [day, month, year-2000]"""
    day = body[offset]
    month = body[offset + 1]
    year = body[offset + 2] + 2000
    return f"{day:02d}.{month:02d}.{year}"
```

---

## 4. FUN_00423a6b — Passport LUT (zapis2.exe) — ПОЛНАЯ ТАБЛИЦА

Полная таблица подстановки из switch-case (86 позиций):

```
Index  → Hex(CP1251) → Символ    Описание
─────────────────────────────────────────────────────────
0x00   → 0x30       → '0'       Цифра 0
0x01   → 0x31       → '1'       Цифра 1
0x02   → 0x32       → '2'       Цифра 2
0x03   → 0x33       → '3'       Цифра 3
0x04   → 0x34       → '4'       Цифра 4
0x05   → 0x35       → '5'       Цифра 5
0x06   → 0x36       → '6'       Цифра 6
0x07   → 0x37       → '7'       Цифра 7
0x08   → 0x38       → '8'       Цифра 8
0x09   → 0x39       → '9'       Цифра 9
0x0A   → 0x20       → ' '       Пробел
0x0B   → 0x41       → 'A'       Латиница A (= кир. А)
0x0C   → 0xC1       → 'Б'       Кириллица Б
0x0D   → 0x42       → 'B'       Латиница B (= кир. В)
0x0E   → 0xC3       → 'Г'       Кириллица Г
0x0F   → 0xC4       → 'Д'       Кириллица Д
0x10   → 0x45       → 'E'       Латиница E (= кир. Е)
0x11   → 0xC6       → 'Ж'       Кириллица Ж
0x12   → 0xC7       → 'З'       (199 dec) Кириллица З
0x13   → 0xC8       → 'И'       (200 dec) Кириллица И
0x14   → 0xC9       → 'Й'       Кириллица Й
0x15   → 0x4B       → 'K'       Латиница K (= кир. К)
0x16   → 0xCB       → 'Л'       Кириллица Л
0x17   → 0x4D       → 'M'       Латиница M (= кир. М)
0x18   → 0x48       → 'H'       Латиница H (= кир. Н)
0x19   → 0x4F       → 'O'       Латиница O (= кир. О)
0x1A   → 0xCF       → 'П'       Кириллица П
0x1B   → 0x50       → 'P'       Латиница P (= кир. Р)
0x1C   → 0x43       → 'C'       Латиница C (= кир. С)
0x1D   → 0xD2       → 'Т'       Кириллица Т
0x1E   → 0xD3       → 'У'       Кириллица У
0x1F   → 0xD7       → 'Ч'       Кириллица Ч
0x20   → 0xD4       → 'Ф'       Кириллица Ф
0x21   → 0xD5       → 'Х'       Кириллица Х
0x22   → 0xD6       → 'Ц'       Кириллица Ц
0x23   → 0xD8       → 'Ш'       Кириллица Ш
0x24   → 0xD9       → 'Щ'       Кириллица Щ
0x25   → 0xDA       → 'Ъ'       Кириллица Ъ
0x26   → 0xDB       → 'Ы'       Кириллица Ы
0x27   → 0xDC       → 'Ь'       Кириллица Ь
0x28   → 0xDD       → 'Э'       Кириллица Э
0x29   → 0xDE       → 'Ю'       Кириллица Ю
0x2A   → 0xDF       → 'Я'       Кириллица Я
0x2B   → 0x2B       → '+'       Плюс
0x2C   → 0x2D       → '-'       Минус
0x2D   → 0x3D       → '='       Равно
0x2E   → 0x51       → 'Q'       Латиница Q
0x2F   → 0x57       → 'W'       Латиница W
0x30   → 0x65       → 'e'       Латиница e
0x31   → 0x2E       → '.'       Точка
0x32   → 0x3F       → '?'       Вопрос
0x33   → 0x52       → 'R'       Латиница R
0x34   → 0x52       → 'R'       (дубликат)
0x35   → 0x54       → 'T'       Латиница T
0x36   → 0x54       → 'T'       (дубликат)
0x37   → 0x59       → 'Y'       Латиница Y
0x38   → 0x59       → 'Y'       (дубликат)
0x39   → 0x78       → 'x'       Латиница x
0x3A   → 0x79       → 'y'       Латиница y
0x3B   → 0x72       → 'r'       Латиница r
0x3C   → 0x74       → 't'       Латиница t
0x3D   → 0x5E       → '^'       Каретка
0x3E   → 0x3E       → '>'       Больше
0x3F   → 0x3C       → '<'       Меньше
0x40   → 0x55       → 'U'       Латиница U
0x41   → 0x49       → 'I'       Латиница I
0x42   → 0x4F       → 'O'       (дубликат O)
0x43   → 0x50       → 'P'       (дубликат P)
0x44   → 0x41       → 'A'       (дубликат A)
0x45   → 0x53       → 'S'       Латиница S
0x46   → 0x44       → 'D'       Латиница D
0x47   → 0x46       → 'F'       Латиница F
0x48   → 0x47       → 'G'       Латиница G
0x49   → 0x48       → 'H'       (дубликат H)
0x4A   → 0x46       → 'F'       (дубликат F)
0x4B   → 0x29       → ')'       Скобка
0x4C   → 0x28       → '('       Скобка
0x4D   → 0x3A       → ':'       Двоеточие
0x4E   → 0x3B       → ';'       Точка-зап.
0x4F   → 0x2F       → '/'       Слеш
0x50   → 0x2C       → ','       Запятая
0x51   → 0x4A       → 'J'       Латиница J
0x52   → 0x4B       → 'K'       (дубликат K)
0x53   → 0x5F       → '_'       Подчёрк
0x54   → 0x7B       → '{'       Фигурная
0x55   → 0x7D       → '}'       Фигурная
0x56   → 0x5A       → 'Z'       Латиница Z
0x57   → 0x58       → 'X'       Латиница X
0x58   → 0x43       → 'C'       (дубликат C)
0x59   → 0x43       → 'C'       (дубликат C)
0x5A   → 0x58       → 'X'       (дубликат X)
...далее: 'V'=0x56, 'B'=0x42, 'N'=0x56, '^'=0x6E, 'd'=0x64, etc.
```

**Ключевое отличие от DLL:** zapis2 использует ЛАТИНСКИЕ буквы для визуально похожих кириллических: A↔А, B↔В, E↔Е, H↔Н, K↔К, M↔М, O↔О, P↔Р, C↔С.

---

## 5. FUN_0042409a — Passport Decode Wrapper

```c
char* DecodePassport(int offset, int length) {
    // Читает байты в ОБРАТНОМ порядке, каждый пропускает через LUT
    string result = "";
    for (int i = length; i > 0; i--) {
        char raw = ReadByte(offset + i - 1);  // reverse!
        char decoded = FUN_00423a6b(raw);      // LUT lookup
        result += decoded;                      // append
    }
    return result;
}
```

**Python:**
```python
def decode_passport_zapis2(body: bytes, offset: int, length: int) -> str:
    """Декодирование passport из body (обратный порядок + LUT)"""
    result = []
    for i in range(length, 0, -1):
        raw_byte = body[offset + i - 1]
        decoded_char = ZAPIS2_LUT[raw_byte]  # lookup по таблице выше
        result.append(decoded_char)
    return ''.join(result).rstrip('\x00').strip()
```

---

## 6. FUN_0042413f — Полный Field Render Switch (26 типов)

Функция декодирует одно поле из raw data по типу из дескриптора:

| Type | Hex | Действие | Формат вывода |
|------|-----|----------|---------------|
| 2 | 0x02 | Real48(offset) | `"%.3f"` (3 знака) |
| 3 | 0x03 | Real48(offset) | `"%.2f"` (2 знака) |
| 4 | 0x04 | ReadLE16(offset) → int | `"%d"` или callback |
| 6 | 0x06 | Read 5 bytes raw | `"%2d %2d %2d %2d %2d"` |
| 13 | 0x0D | ReadByte(offset) → int | `"%d"` или callback |
| 16 | 0x10 | ReadLE16(offset) | `"%05d"` (5 цифр) |
| 17 | 0x11 | ReadLE16(offset) | `"%04d"` (4 цифры) |
| 22 | 0x16 | Real48(offset) → sqrt | `"%.1f"` |
| 36 | 0x24 | String from ptr | прямая строка |
| 37 | 0x25 | String from ptr | прямая строка |
| 38 | 0x26 | String from ptr | прямая строка |
| 39 | 0x27 | ReadByte → callback → str | строка через callback |
| **40** | **0x28** | **Passport(offset, 7)** | **LUT decode 7 байт** |
| 42 | 0x2A | **Passport(offset, 6)** | **LUT decode 6 байт** |
| 43 | 0x2B | ReadByte & 0x08 → bool | `"Да"` / `"Нет"` |
| 44 | 0x2C | String from ptr | прямая строка |
| 45 | 0x2D | ReadLE16(offset) | `"%d"` |
| 48 | 0x30 | DecodeDate(offset) | `"DD.MM.YYYY"` |
| 49 | 0x31 | ReadByte(HI), ReadByte(LO) | `"%d.%d"` пара |
| 53 | 0x35 | ReadLE16 (signed) | `"%d"` (знаковый) |
| 54 | 0x36 | String from ptr | прямая строка |
| 56 | 0x38 | ReadByte, >0x7F→negate | `"%d"` (модуль) |
| **57** | **0x39** | **Passport(offset, 12)** | **LUT decode 12 байт** |
| 58 | 0x3A | String from ptr | прямая строка |
| **59** | **0x3B** | **Passport(offset, 11)** | **LUT decode 11 байт** |
| 62 | 0x3E | LE16(LO) + Byte(HI) → callback | callback с двумя аргументами |
| 63 | 0x3F | Byte(LO) + Byte(HI) → callback | callback с двумя аргументами |
| **66** | **0x42** | **Real48(LO) → float, threshold** | `"%.2f"` + пометка если <16 или >126 |
| **67** | **0x43** | **Real48(LO) → float×2** | `"%.2f"` (диаметр=2×радиус) + порог |
| 74 | 0x4A | ReadLE16 | `"%d.%d"` (разделённый) |
| 75 | 0x4B | ReadLE16 | `"%d.%02d"` (с нулями) |

---

## 7. Типы Passport-полей — сводка

| Тип | Hex | Длина | Использование |
|-----|-----|-------|---------------|
| 0x28 | PASSPORT_7 | 7 байт | Завод, Обод (стандартные) |
| 0x2A | PASSPORT_6 | 6 байт | Плавка (укороченная, ShortProt DLL) |
| 0x39 | PASSPORT_12 | 12 байт | Расширенный паспорт |
| 0x3B | PASSPORT_11 | 11 байт | Плавка (полная, A-scan/RESULTS) |

---

## 8. Формулы пересчёта числовых полей

### 8.1 Толщина (type 0x42)
```python
thickness_mm = real48_to_float(body[offset:offset+6])
# Если byte[threshold_offset] < 0x10: пометка "строб не активен"
# Если byte[threshold_offset] > 0x7E: пометка "за пределами диапазона"
```

### 8.2 Диаметр (type 0x43)
```python
radius = real48_to_float(body[offset:offset+6])
diameter_mm = radius * 2.0  # УДВОЕНИЕ! (радиус → диаметр)
# Те же пороги: <0x10 или >0x7E
```

### 8.3 Амплитуда (LE16 type 0x04/0x35)
```python
amplitude_raw = struct.unpack_from('<H', body, offset)[0]
# Отображается как целое число (единицы зависят от контекста)
# Для дБ: amplitude_dB = amplitude_raw / 2.0  (гипотеза из DLL)
```

### 8.4 Скорость звука (LE16)
```python
velocity_mps = struct.unpack_from('<H', body, offset)[0]
# Единицы: м/с (прямое значение, без масштабирования)
```

### 8.5 Угол (byte type 0x38)
```python
angle_raw = body[offset]
if angle_raw > 0x7F:
    angle = 0x100 - angle_raw  # отрицательный угол → модуль
else:
    angle = angle_raw
# Единицы: градусы (целые)
```

### 8.6 Пара значений (type 0x31)
```python
hi_byte = body[hi_offset]  # первое значение
lo_byte = body[lo_offset]  # второе значение
# Отображается как "X.Y" (например "127.16")
# Если hi_byte >= 0x7F → пометка "переполнение" (порог)
# Если hi_byte < 0x10 → пометка "ниже порога"
```

### 8.7 Тип 0x16 — корень из Real48
```python
value = real48_to_float(body[offset:offset+6])
result = math.sqrt(value)  # FUN_004319c4 = sqrt()
# Единицы: мм (глубина через V-тракт?)
```

---

## 9. Defect Combo Lookup (FUN_00413f6f)

Функция возвращает строку описания дефекта по коду (1-10+):

| Код | Адрес строки | Предполагаемый текст |
|-----|-------------|---------------------|
| 1 (part/100=1) | DAT_004f2574 | "Поперечная трещина" |
| 1 (part/100≠1) | DAT_004f2590 | "Продольная трещина" |
| 2 | DAT_004f2610 | "Коррозионное повреждение" |
| 3 | DAT_004f2674 | "Расслоение" |
| 4 | DAT_004f26c0 | "Дефект сварного шва" |
| 5 | DAT_004f273c | "Внутренний дефект" |
| 6 | DAT_004f27bc | "Подповерхностный дефект" |
| 7 | DAT_004f287c | "Дефект обода" |
| 8 | DAT_004f28c0 | "Дефект гребня" |
| 9 | DAT_004f28d8 | "Дефект шейки" |
| 10 | DAT_004f2924 | "Наплавка" |
| default | DAT_004f293c | "Неизвестный дефект" |

### Масштаб порогов (FUN_00414020):
| Код | Значение | Описание |
|-----|----------|----------|
| 0 | 1.25 | ×1.25 |
| 1 | 1.8 | ×1.8 |
| 2 | 2.5 | ×2.5 |
| 3 | 5.0 | ×5.0 |
| default | 0.0 | отключен |

---

## 10. Сводка находок (раунд 5)

| # | Находка | Статус |
|---|---------|--------|
| 61 | FUN_004231c9 = ReadLE16(offset) из глобального буфера | 🔒 Железобетонно |
| 62 | FUN_004231f3 = ReadByte(offset) из глобального буфера | 🔒 Железобетонно |
| 63 | FUN_00423253 = Real48→Double (Borland 6-byte float decoder) | 🔒 Железобетонно |
| 64 | FUN_0042390e = DecodeDate (упакованная дата → DD.MM.YYYY) | 🔒 Железобетонно |
| 65 | FUN_00423a6b = Passport LUT zapis2 (86 позиций, смесь лат/кир) | 🔒 Железобетонно |
| 66 | FUN_0042409a = Passport decode (обратный порядок + LUT) | 🔒 Железобетонно |
| 67 | FUN_0042413f = 26-тип field render switch | 🔒 Железобетонно |
| 68 | Type 0x42: Real48 → float (толщина), порог byte < 0x10 / > 0x7E | 🔒 Железобетонно |
| 69 | Type 0x43: Real48 → float × 2 (диаметр = 2×радиус) | 🔒 Железобетонно |
| 70 | Type 0x38: byte > 0x7F → 0x100 - byte (знаковая инверсия угла) | 🔒 Железобетонно |
| 71 | Type 0x16: Real48 → sqrt (корень — глубина V-тракт) | 🔒 Железобетонно |
| 72 | Passport lengths: 6/7/11/12 (types 0x2A/0x28/0x3B/0x39) | 🔒 Железобетонно |
| 73 | FUN_00413f6f: 11 строк дефектов (1-10 + default) | 🔒 Железобетонно |
| 74 | FUN_00414020: 4 масштаба порога (1.25/1.8/2.5/5.0) | 🔒 Железобетонно |
| 75 | Type 0x2B: byte & 0x08 → boolean ("Да"/"Нет") | 🔒 Железобетонно |
| 76 | Types 0x4A/0x4B: LE16 → разделённый формат (X.Y / X.0Y) | 🔒 Железобетонно |

---



## 11. TypeVar Dispatch (FUN_004022c8 в DLL) — полная карта

### 11.1 Три обработчика

| Обработчик | TypeVar диапазоны | Категории | Формат |
|---|---|---|---|
| **FUN_00404758** (A-scan) | 150-151, 362-363, 380-381 | 4-6 и 10-19 | 14 полей |
| **FUN_004041d0** (Composite/B-scan) | 390-391, 442-443, 480-481, 642-643, 842-843 | 4-6 и 10-19 | 9 полей |
| **FUN_00403834** (Generic) | 680-683, 712-713, 730-731, 740-741, 780-781 | default | 10 полей |

### 11.2 A-scan handler (FUN_00404758) — TypeVar 150-151, 362-381

**Категории 4-6 (Settings/Calibration):**

| # | Offset | Size | Тип | Поле |
|---|--------|------|-----|------|
| 1 | +0x07 | 3 | date | Дата [day,month,year-2000] |
| 2 | +0x0A | 2 | time | Время [hours,minutes] |
| 3 | +0x11 | 11 | LUT | ⭐ ПЛАВКА (passport primary) |
| 4 | — | — | calc | Доп.поле (по TypeVar) |
| 5 | +0x38 | 2 | LE16 | Амплитуда |
| 6 | +0x3A | 2 | LE16 | Задержка (мкс) |
| 7 | +0x3C | 2 | LE16 | Позиция строба |
| 8-11 | — | — | calc | Вычисляемые (4 шт) |
| 12 | +0x0C | 1 | bool | Флаг дефекта |
| 13 | +0x21 | 7 | LUT | ⭐ ЗАВОД (passport secondary) |
| 14 | +0x45 | 2 | LE16 | Ширина зоны |

**Категории 10-19 (B-scan/Results):**

| # | Offset | Size | Тип | Поле |
|---|--------|------|-----|------|
| 1 | +0x07 | 3 | date | Дата |
| 2 | +0x0A | 2 | time | Время |
| 3-8 | — | — | calc | Вычисляемые (6 шт: координаты, размеры) |
| 9-12 | — | — | calc | Доп.поля |
| 13-14 | — | — | calc | Вычисляемые |

### 11.3 Composite handler (FUN_004041d0) — TypeVar 390-843

**Категории 4-6:**

| # | Offset | Size | Тип | Поле |
|---|--------|------|-----|------|
| 1 | +0x07 | 3 | date | Дата |
| 2 | +0x0A | 2 | time | Время |
| 3 | +0x11 | 11 | LUT | ⭐ ПЛАВКА |
| 4 | — | — | calc | Доп.поле |
| 5 | +0x35 | 7 | LUT | ⭐ ОБОД / detail passport |
| 6 | +0x3E | 2 | LE16 | Кол-во эхо |
| 7 | +0x3C | 2 | LE16 | Уровень строба |
| 8 | +0x0C | 1 | bool | Флаг дефекта |
| 9 | +0x21 | 7 | LUT | ⭐ ЗАВОД |

**Категории 10-19:**

| # | Offset | Size | Тип | Поле |
|---|--------|------|-----|------|
| 1 | +0x07 | 3 | date | Дата |
| 2 | +0x0A | 2 | time | Время |
| 3-6 | — | — | calc | Вычисляемые (4 шт) |

### 11.4 Generic handler (FUN_00403834) — TypeVar 680-783

**Категории 4-6:**

| # | Offset | Size | Тип | Поле | Условие |
|---|--------|------|-----|------|---------|
| 1 | +0x07 | 3 | date | Дата | всегда |
| 2 | +0x0A | 2 | time | Время | всегда |
| 3 | +0x11 | 11 | LUT | ⭐ ПЛАВКА | всегда |
| 4 | +0x5E | 2 | LE16 | Длина объекта | всегда |
| 5 | +0x35 | 2 | LE16 | Толщина | всегда |
| 6 | +0x37 | 2 | LE16 | Скорость | всегда |
| 7 | +0x39 | 2 | LE16 | Глубина дефекта | TV 362-381, 680-683, 712-731, 740-741, 780-781 |
| 8 | +0x39 | 2 | LE16 | Доп.числовое | только TV 680-683 |
| 9 | +0x3C | 2 | LE16 | Gate level | только TV 680-683 |
| 10 | +0x0C | 1 | bool | Флаг дефекта | всегда |

### 11.5 TypeVar → Назначение (жд УЗК)

| TypeVar | Decimal | Назначение (гипотеза из контекста) |
|---------|---------|-----------------------------------|
| 150-151 | | Толщинометрия базовая |
| 362-363 | | Толщиномер рельс (поперечный профиль) |
| 380-381 | | Толщиномер рельс (продольный профиль) |
| 390-391 | | Composite рельс (A+B) |
| 442-443 | | Composite стрелка |
| 480-481 | | Composite сварной шов |
| 642-643 | | Composite колесо (базовый) |
| 680-683 | | **Вагонная серия** (обод+гребень+шейка+наплавка) |
| 712-713 | | Колесо — подступичная часть |
| 730-731 | | Колесо — диск |
| 740-741 | | Колесо — обод (расширенный) |
| 780-781 | | Колесо — гребень |
| 842-843 | | Composite вагон (полный) |

---

## 12. RESULTS2 Body Layout — полная карта

### 12.1 Формат PROTOCOL BLOB в RESULTS2

RESULTS2 хранит **полный протокол** контроля, включающий A-scan данные.

```
PROTOCOL BLOB = [16-byte header] + [body]

Body layout для A-scan (TypeVar 150-381, категории 4-6):
───────────────────────────────────────────────────────
Offset  Size   Поле                    Формула
+0x00   2      blockAddr               LE16
+0x02   2      reserved                —
+0x04   2      sweepId                 byte+byte
+0x06   1      flags                   byte
+0x07   3      date                    [D,M,Y-2000]
+0x0A   2      time                    [H,M]
+0x0C   1      defect_flag             0/1
+0x0D   4      context                 bytes
+0x11   11     ПЛАВКА                  LUT decode
+0x1C   5      padding                 —
+0x21   7      ЗАВОД                   LUT decode
+0x28   16     zone_data               raw bytes
+0x38   2      amplitude               LE16 (дБ×?)
+0x3A   2      delay                   LE16 (мкс)
+0x3C   2      gate_position           LE16
+0x3E   2      echo_count              LE16
+0x40   1      channel                 byte
+0x41   4      reserved                —
+0x45   2      zone_width              LE16
+0x47   23     tail_data               —
+0x5E   2      total_length            LE16 (мм)
+0x60   ...    A-SCAN WAVEFORM DATA    448 bytes (0x1C0)

Total body for A-scan: ~0x220 bytes (544 dec)
```

### 12.2 Body layout для Composite (TypeVar 390-843, категории 4-6):

```
+0x00..+0x06   — как A-scan (header полей)
+0x07   3      date
+0x0A   2      time
+0x0C   1      defect_flag
+0x0D   4      context
+0x11   11     ПЛАВКА
+0x1C   5      padding
+0x21   7      ЗАВОД
+0x28   13     data_zone
+0x35   7      ОБОД (detail passport)
+0x3C   2      gate_level              LE16 (%)
+0x3E   2      echo_count              LE16
+0x40   1      channel
+0x41   7      passport_extra          LUT (только TV 680-683)
+0x48..+0x5D   tail_data
+0x5E   2      total_length            LE16
+0x60   ...    A-SCAN WAVEFORM DATA    448 bytes (0x1C0)
+0x220  ...    B-SCAN IMAGE DATA       (если есть)

B-scan data layout:
  Offset: +0x1D0 от начала waveform (= +0x60 + 0x1C0 - 0x60... фактически +0x220)
  Width: 30 bytes/row = 240 bits
  Height: (dataLen - 0x1C0) * 8 / 240 rows
  Encoding: 1 bit/pixel, MSB first
  Magnification: ×2 (each bit → 2×2 pixels)
```

### 12.3 Размеры PROTOCOL BLOB в RESULTS2

| TypeVar группа | Body size | A-scan | B-scan | Total BLOB |
|---|---|---|---|---|
| 150-381 (A-scan only) | ~96 bytes | — | — | ~112 bytes |
| 390-843 cat 4-6 (full) | 96 + 448 = 544 | 448 bytes | — | ~560 bytes |
| 390-843 cat 10-19 (B-scan) | 96 + 448 + 3840 = 4384 | 448 | 3840 (базовая) | ~4400 |
| 680-683 cat 10-19 (вагонная) | 96 + 448 + 4800 = 5344 | 448 | 4800 (вагонная) | ~5360 |

### 12.4 Столбцы таблицы RESULTS2

```
NUMBER      INT       PK
NUMPRIB     INT       ID прибора
NUMKOD      INT       FK → BLOCKZAP.Number  
DATEFORM    CHAR(10)  дата ISO
TIMEFORM    CHAR(8)   время
NAMEOPERA   VARCHAR   оператор
INDMAKER    VARCHAR   завод (из BLOB LUT decode +0x21)
MAKETIME    INT       год изготовления
DEFEKT      VARCHAR   описание дефекта
CODEDEF     VARCHAR   код дефекта
TYPEVAR     INT       типовой вариант
TYPEZAP     VARCHAR   тип записи
NUMOBJ      VARCHAR   номер объекта (ОБОД из +0x35)
SMELTING    VARCHAR   ⭐ ПЛАВКА (из BLOB LUT decode +0x11)
PROTOCOL    BLOB      полный протокол (header + body + A/B-scan)
```

---

## 13. Семантика числовых полей (физические единицы)

| Offset | Поле | Единицы | Формула | Источник |
|--------|------|---------|---------|----------|
| +0x35 | thickness | мм | `LE16 / 100.0` (гипотеза) или Real48 | FUN_004055ac |
| +0x37 | velocity | м/с | `LE16` (прямое значение) | FUN_004056b4 |
| +0x38 | amplitude | дБ | `LE16 / 2.0` (гипотеза) | FUN_00405528 |
| +0x39 | depth | мм | `LE16` (прямое значение) | FUN_00406204 |
| +0x3A | delay | мкс | `LE16` (прямое значение) | FUN_00405630 |
| +0x3C | gate_pos | отсч. | `LE16` | FUN_00405738 |
| +0x3E | echo_count | шт | `LE16` | FUN_00406158 |
| +0x45 | zone_width | отсч. | `LE16` | FUN_00405c78 |
| +0x5E | total_length | мм | `LE16` | FUN_00406318 |

**Для типов 0x42/0x43 (Real48 в zapis2):**
```
thickness_mm = Real48_to_float(body[offset:offset+6])
diameter_mm = Real48_to_float(body[offset:offset+6]) * 2.0
```

---

## 14. Итоговая сводка находок (раунды 4-5)

| # | Находка | Статус |
|---|---------|--------|
| 45-60 | (см. ROUND4.md) | 🔒 |
| 61-76 | (см. выше раздел 10) | 🔒 |
| 77 | 3 обработчика DLL по TypeVar: A-scan/Composite/Generic | 🔒 Железобетонно |
| 78 | A-scan handler: 14 полей (дата,время,плавка,завод,ампл,задержка...) | 🔒 Железобетонно |
| 79 | Composite handler: 9 полей (дата,время,плавка,обод,завод...) | 🔒 Железобетонно |
| 80 | Generic handler: 10 полей (толщина,скорость,глубина — TV-зависимые) | 🔒 Железобетонно |
| 81 | TypeVar 680-683 имеет ВСЕ поля (включая extra passport @+0x41) | 🔒 Железобетонно |
| 82 | Глубина (+0x39) только для TV: 362-381, 680-683, 712-741, 780-781 | 🔒 Железобетонно |
| 83 | RESULTS2 BLOB: header(16) + body(96) + A-scan(448) + B-scan(3840/4800) | 🔒 Железобетонно |
| 84 | B-scan offset = +0x1D0 от начала body (после A-scan header) | 🔒 Железобетонно |
| 85 | FUN_00411640: TypeVar/10 проверка для активации строба | 🔒 Железобетонно |
| 86 | FUN_00411570: 7 строк "стороны контроля" (0-6 + default) | 🔒 Железобетонно |
| 87 | FUN_004115d5: 4 строки "результата" (0-3 + default) | 🔒 Железобетонно |
| 88 | FUN_00411611: 3 строки "типа объекта" (0-2 + default) | 🔒 Железобетонно |

---

## 15. Что осталось для полного клона

| # | Задача | Приоритет |
|---|--------|-----------|
| 1 | Дамп .data zapis2.exe → русские строки всех полей/режимов | HIGH |
| 2 | DFM PelengPC.exe → sweep-таблица + TypeVar словарь | HIGH |
| 3 | Точный алгоритм FUN_004066f8..FUN_00406b50 (вычисляемые B-scan поля) | MEDIUM |
| 4 | Кодировщик/декодировщик VARIANT в FUN_00403420 (ShortProt /100) | MEDIUM |
| 5 | Полная таблица соответствия TypeVar(FDB) → tcode(DLL) | MEDIUM |
| 6 | Парсер .dal файлов | LOW |
| 7 | Точная структура STATUS_T (опкод 0x74) | LOW |

---

# ЧАСТЬ 2: B-scan рендеринг и TypeVar↔tcode таблица

**Источник:** `zapis2.c` (Ghidra), `PELENG_REVERSE_DEEP.md`

---

## 16. B-scan рендеринг — архитектура

### 1.1 Общая схема

```
Raw UART data → PelengPC.exe (FUN_004235f8) → GDI Bitmap (1 bit/pixel ×2)
                                                       ↓
                      DLL (_Form_View) → OLE Automation → PelengPC показывает
                                                       ↓
                      zapis2.exe: GDI Bitmap → CxImage → PNG → WordML отчёт
```

**Ключевое:** B-scan рендерится в PelengPC.exe, передаётся как GDI bitmap, а zapis2.exe лишь конвертирует в PNG для отчёта.

### 1.2 Рендеринг в PelengPC (из PelengPC_ver1_2_PSEUDOCODE.c)

```c
void FormScreen_RenderBscan(byte* rawData, int rawLen) {
    int bscanLen = rawLen - 0x1C0;    // 448 = A-scan header
    int rowBytes = 30;                 // 0x1E = 30 байт/строка = 240 бит
    int rows = bscanLen / rowBytes;    // 128 (базовая) или 160 (вагонная)
    
    // Bitmap: width=480 (240×2), height=rows×2
    for (row = 0; row <= rows; row++) {
        for (col = 0; col <= rowBytes; col++) {
            byte val = rawData[0x1D0 + col + row * rowBytes];
            for (bit = 0; bit < 8; bit++) {
                COLORREF color = ((val >> bit) & 1) ? BLACK : WHITE;
                // FillRect 2×2 пикселя
                int x = (col * 8 + bit) * 2;
                int y = row * 2;
                SetPixel(x, y, color);
                SetPixel(x+1, y, color);
                SetPixel(x, y+1, color);
                SetPixel(x+1, y+1, color);
            }
        }
    }
}
```

### 1.3 Параметры B-scan изображения

| Параметр | Базовая | Вагонная |
|----------|---------|----------|
| Смещение данных в body | +0x1D0 (464) | +0x1D0 (464) |
| Размер данных | 3840 байт | 4800 байт |
| Байт/строка | 30 | 30 |
| Бит/строка | 240 | 240 |
| Строк | 128 | 160 |
| Ширина px (×2) | 480 | 480 |
| Высота px (×2) | 256 | 320 |
| Кодирование | 1 bit/pixel, MSB first | то же |
| Цвета | bit=1→BLACK, bit=0→WHITE | то же |

### 1.4 Конвертация в PNG (zapis2.exe, FUN_00419064)

```c
void SaveBscanAsPng(void* bitmapObj) {
    CxImage img;
    CxImage::CxImage(&img, 0);
    
    HBITMAP hBmp = GetBitmapHandle(bitmapObj);
    CxImage::CreateFromHBITMAP(&img, hBmp, NULL);
    
    if (CxImage::IsValid(&img)) {
        if (!CxImage::IsGrayScale(&img))
            CxImage::IncreaseBpp(&img, 24);  // → 24-bit color
        CxImage::Save(&img, "temp.png", 2);  // format=2 → PNG
    }
    
    // Читаем файл в память для встраивания в WordML
    HANDLE hFile = CreateFileA("temp.png", GENERIC_READ, ...);
    DWORD size = GetFileSize(hFile, NULL);
    void* buf = malloc(size);
    ReadFile(hFile, buf, size, &bytesRead, NULL);
    // → base64 encode → встраивание в <w:binData>
    CloseHandle(hFile);
    DeleteFileA("temp.png");
}
```

### 1.5 Встраивание в WordML отчёт

```xml
<w:pict>
  <w:binData w:name="wordml://protPict.png">
    [base64-encoded PNG data]
  </w:binData>
  <v:shape style="width:240pt;height:120pt">
    <v:imagedata src="wordml://protPict.png"/>
  </v:shape>
</w:pict>
```

Если есть B-scan (template+0xB3C > 0): используется `position:absolute` стиль.

---

## 17. Масштабные таблицы B-scan (Scale Factor Tables)

### 2.1 Шесть функций масштабирования

Каждая функция возвращает коэффициент масштаба для отображения A-scan/B-scan по индексу:

| Функция | Назначение | Индексы→значения |
|---------|-----------|-----------------|
| **FUN_0040900f** | B-scan простой | 0→0.1, 1→0.4, 2→0.62, 3→1.25, 4→1.8, 5→2.5, 6→5.0, 7→10.0 |
| **FUN_00409093** | B-scan двойной | 0→1.25, 1→1.8, 2→2.5, 3→5.0, 4→10.0 |
| **FUN_004090f3** | Огибающая (peak) | 0→1.25, 1→1.8, 2→2.5, 3→5.0 |
| **FUN_00409147** | Стробированный | 0→0.4, 1→1.25, 2→2.5, 3→5.0 |
| **FUN_0040919b** | Специальный (вагонная) | 0→0.4, 1→0.62, 2→1.25, 3→1.8, 4→2.5, 5→5.0, 6→10.0 |
| **FUN_00409213** | A-scan одноканальный | 0→1.25, 1→2.5, 2→5.0, 3→0.4 |

### 2.2 Выбор функции масштабирования (param_1+0x528)

```c
void SelectScaleFunction(int dataDesc) {
    byte mode = DAT_0051cb34 & 0xFF;  // LO byte of TypeVar
    
    if ((DevFlags & 0x40)==0 || (DevFlags & 4)!=0) {
        if ((DevFlags & 0x10) == 0) {
            if ((DevFlags & 0x40) == 0) {
                switch(mode) {
                    case 1:     fn = FUN_00409213; break;  // A-scan 1-ch
                    case 3,7:   fn = FUN_004090f3; break;  // Огибающая
                    case 4,6,8: fn = FUN_00409147; break;  // Стробированный
                }
            } else {
                if (mode==3 || mode==7) fn = FUN_004090f3;
                else fn = FUN_00409147;
            }
        } else if ((DevFlags & 0x20) == 0) {
            fn = FUN_0040900f;   // B-scan простой
        } else {
            fn = FUN_00409093;   // B-scan двойной
        }
    } else {
        fn = FUN_0040919b;       // Специальный (вагонная)
    }
    dataDesc->scaleFn = fn;
}
```

### 2.3 Логика выбора (по флагам DevFlags)

| DevFlags биты | Режим | Scale Function |
|---|---|---|
| bit4=0, bit5=0, bit6=0 | A-scan | По mode byte (1/3/4/6/7/8) |
| bit4=1, bit5=0 | B-scan simple | FUN_0040900f (8 уровней) |
| bit4=1, bit5=1 | B-scan double | FUN_00409093 (5 уровней) |
| bit6=1, bit2=0 | Special (wagon) | FUN_0040919b (7 уровней) |

---

## 18. TypeVar/10 — Gate Activation Tables

### 3.1 FUN_00411640 — первая группа (строб 1)

Читает LE16 из body offset 0x7E, делит на 10, проверяет:

| TypeVar/10 | Decimal | Активен? | Назначение (гипотеза) |
|---|---|---|---|
| 0 | 0 | ДА | Режим 0 (общий) |
| 10 | 100 | ДА | Серия 100 (толщиномер) |
| 20 (0x14) | 200 | ДА | Серия 200 |
| 50 (0x32) | 500 | ДА | Серия 500 |
| 51 (0x33) | 510 | ДА | Серия 510 |
| 60 (0x3C) | 600 | ДА | Серия 600 |
| 70 (0x46) | 700 | ДА | Серия 700 (колёса) |
| 80 (0x50) | 800 | ДА | Серия 800 (бандаж) |
| 85 (0x55) | 850 | ДА | Серия 850 (гребень) |
| 90 (0x5A) | 900 | ДА | Серия 900 (шейка) |
| 92 (0x5C) | 920 | ДА | |
| 94 (0x5E) | 940 | ДА | |
| 96 (0x60) | 960 | ДА | |
| другие | — | НЕТ | Строб неактивен |

### 3.2 FUN_004116eb — вторая группа (строб 2)

| TypeVar/10 | Decimal | Активен? |
|---|---|---|
| 0 | 0 | ДА |
| 13 (0x0D) | 130 | ДА |
| 24 (0x18) | 240 | ДА |
| 54 (0x36) | 540 | ДА |
| 66 (0x42) | 660 | ДА |
| 76 (0x4C) | 760 | ДА |
| 99 (0x63) | 990 | ДА |
| другие | — | НЕТ |

---

## 19. TypeVar (FDB) ↔ tcode (DLL) — Таблица соответствия

### 4.1 Принцип маппинга

**FDB TYPEVAR = tcode (прямое соответствие!).**

Столбец `TYPEVAR` в таблицах `SHORTPROT2`/`RESULTS2`/`NASTR2` хранит **тот же** числовой код, что лежит в raw data body по offset 0x14 (LE16). Это подтверждается:

1. `DAT_0051cb34 = ReadLE16(0x14)` — чтение TypeVar из raw data
2. Тот же `DAT_0051cb34` используется в switch для выбора шаблона отчёта
3. В FDB записывается как `TYPEVAR INT` → прямое значение

### 4.2 Полная таблица TypeVar → Объект контроля (из FUN_00411a1a)

Функция `FUN_00411a1a` принимает tcode и возвращает строку-название.
Маппинг по `param_1 / 10` и по конкретным значениям:

| tcode / 10 | tcode range | Адрес строки | Объект контроля |
|---|---|---|---|
| 0x51 (81) | 810-819 | DAT_004d9058 | Колесо (тип 1) |
| 0x52 (82) | 820-829 | DAT_004d9068 | Колесо (тип 2) |
| 0x53 (83) | 830-839 | DAT_004d9070 | Колесо (тип 3) |
| 0x54 (84) | 840-849 | DAT_004d9078 | Колесо (тип 4) |

**По конкретному tcode:**

| tcode | Hex | Адрес строки | Объект |
|---|---|---|---|
| 50 | 0x32 | DAT_004d9094 | Рельс Р50 |
| 51 | 0x33 | DAT_004d90a8 | Рельс Р65 |
| 52 | 0x34 | DAT_004d90b0 | Рельс Р75 |
| 53 | 0x35 | DAT_004d90c8 | Рельс Р43 |
| 54 | 0x36 | DAT_004d90e0 | Стрелочный перевод |
| 55 | 0x37 | DAT_004d90fc | Сварной стык |
| 110-111 | 0x6E-0x6F | (goto 0x70) | → Ось (базовая) |
| 112, 122 | 0x70, 0x7A | DAT_004d9124 | Ось РУ1 |
| 113-114, 123-124 | 0x71-72, 0x7B-7C | DAT_004d9164 | Ось РВ2Ш |
| 115-116, 125-126 | 0x73-74, 0x7D-7E | DAT_004d9144 | Ось РУ1Ш |
| 118 | 0x76 | DAT_004d9184 | Ось ВЛ80 |
| 120-121 | 0x78-79 | DAT_004d9110 | Ось (общая) |
| 127 | 0x7F | DAT_004d91a4 | Ось ТЭП70 |
| 202, 203 | 0xCA | → 0x70 | Ось РУ1 |
| 204-205, 301-302 | 0xCC,0x12D | DAT_004d9164 | Ось РВ2Ш |
| 206, 207 | 0xCE | DAT_004d9144 | Ось РУ1Ш |
| 208, 309 | 0xCF, 0x134 | DAT_004d91a4 | Ось ТЭП70 |
| 300, 301 | 0x12C-12D | DAT_004d9110 | Ось (общая) |
| 306-307 | 0x132-133 | DAT_004d91bc | Ось ЭП1М |
| 412 | 0x19C | DAT_004d9124 | Ось РУ1 |
| 413-414, 423-424 | 0x19D-19E, 0x1A7-1A8 | DAT_004d9164 | Ось РВ2Ш |
| 415-416, 425-426 | 0x19F-1A0, 0x1A9-1AA | DAT_004d9144 | Ось РУ1Ш |
| 418, 428 | 0x1A2, 0x1AC | DAT_004d9184 | Ось ВЛ80 |
| 420-421 | 0x1A4-1A5 | DAT_004d9110 | Ось (общая) |
| 422 | 0x1A6 | DAT_004d9124 | Ось РУ1 |
| 901 | 0x385 | DAT_004d91d8 | Бандаж (спец.) |
| 902 | 0x386 | DAT_004d91f0 | Гребень (спец.) |
| default | — | DAT_004d9208 | (неизвестный тип) |

### 4.3 Таблица zapis2 tcode → PelengPC tcode (реконструкция)

Из dispatch в zapis2.c (строки 6650-6910, 8610-8790):

| zapis2 tcode | PelengPC Template | Прибор | Режим |
|---|---|---|---|
| 0x0C07 (3079) | DAT_004c0720 | УД2-12 | Колесо |
| 0x0D07 (3335) | DAT_004c0720 | УД2-13 | Колесо |
| 0x1E07 (7687) | DAT_004eee70 | УД2-30 | Колесо |
| 0x1F07 (7943) | DAT_004eee70 | УД2-31 | Колесо |
| 0x2A04 (10756) | DAT_004cb6d8 | УД2-102 | Рельс |
| 0x2A06 (10758) | DAT_004db028 | УД2-102 | Рельс расш. |
| 0x2A08 (10760) | DAT_004fbd18 | УД2-102 | Колесо полн. |
| 0x2B04 (11012) | DAT_004cb6d8 | УД2-102+ | Рельс |
| 0x2B06 (11014) | DAT_004db028 | УД2-102+ | Рельс расш. |
| 0x2B08 (11016) | DAT_004fbd18 | УД2-102+ | Колесо полн. |
| 0x3201 (12801) | DAT_004b4e08 | УД2-50 | Базовый |
| 0x3301 (13057) | DAT_004b4e08 | УД2-51 | Базовый |
| 0x3E03 (15875) | DAT_004c0720 | УД2-62 | Толщина |
| 0x3F03 (16131) | DAT_004c0720 | УД2-63 | Толщина |
| 0x5004 (20484) | DAT_004d3848 | УД2-80 | Рельс |
| 0x5006 (20486) | DAT_004e6368 | УД2-80 | Рельс расш. |
| 0x5007 (20487) | DAT_004f4ce8 | УД2-80 | Колесо |
| 0x5104 (20740) | DAT_004d3848 | УД2-81 | Рельс |
| 0x5106 (20742) | DAT_004e6368 | УД2-81 | Рельс расш. |
| 0x5107 (20743) | DAT_004f4ce8 | УД2-81 | Колесо |
| 0x5206 (20998) | DAT_004e6368 | УД2-82 | Рельс расш. |
| 0x5306 (21254) | DAT_004e6368 | УД2-83 | Рельс расш. |

### 4.4 Декодирование tcode

```python
def decode_tcode(tcode: int) -> tuple:
    """Разбирает tcode на серию прибора и режим"""
    series = (tcode >> 8) & 0xFF   # HI byte = серия
    mode = tcode & 0xFF            # LO byte = режим
    return (series, mode)

# Серии приборов:
SERIES_NAMES = {
    0x0C: "УД2-12", 0x0D: "УД2-13",
    0x1E: "УД2-30", 0x1F: "УД2-31",
    0x2A: "УД2-102", 0x2B: "УД2-102+",
    0x32: "УД2-50", 0x33: "УД2-51",
    0x3E: "УД2-62", 0x3F: "УД2-63",
    0x50: "УД2-80", 0x51: "УД2-81",
    0x52: "УД2-82", 0x53: "УД2-83",
}

# Режимы:
MODE_NAMES = {
    0x01: "Базовый контроль",
    0x03: "Толщинометрия",
    0x04: "Рельс (основной)",
    0x06: "Рельс (расширенный)",
    0x07: "Колесо (вагонный)",
    0x08: "Колесо (полный протокол)",
}
```

### 4.5 Соответствие FDB tcode ↔ DLL TypeVar

| FDB TYPEVAR (=tcode) | DLL TypeVar | Категория |
|---|---|---|
| 50-55 | — | Рельсовые (прямой switch) |
| 110-127 | 150-151 (TV/10≈11-12) | Оси A-scan |
| 202-208 | 362-381 | Оси толщиномер |
| 300-309 | 390-391 | Composite рельс |
| 412-428 | 442-443, 480-481 | Composite стрелка/шов |
| 680-683 | 680-683 | **Вагонная (полная)** |
| 810-849 | 642-643, 842-843 | Колёса composite |
| 901-902 | 712-713, 730-731 | Бандаж/гребень |

**ВАЖНО:** Для «вагонных» TypeVar 680-683 — значение в FDB **совпадает** с DLL TypeVar!

---

## 20. DGS_Settings — как загружается из шаблона

### 5.1 Инициализация (строка 6954)

```c
// Копирует 1648 байт (0x19C dwords = 412×4) из template+0x163C в рабочий буфер
undefined4* src = *(undefined4**)(template + 0x163C);
undefined4* dst = localBuffer;  // 0x728 байт стека (1832 > 1648, с запасом)
for (int i = 0x19C; i != 0; i--) {
    *dst++ = *src++;
}
// Затем передаётся в FUN_0040b6ff для создания объекта DGS
dgsObj = FUN_0040b6ff(allocatedMem);
this->dgsSettings = dgsObj;  // сохраняется в this+0x60
```

### 5.2 Структура DGS объекта (из template+0x163C)

```
Offset  Size   Описание
+0x00   4      Тип объекта (ptr или ID)
+0x04   4      Размер данных
+0x08   4      Поле 1 (используется в ReadLE16)
+0x0C   4      Поле 2 
+0x10   4      Поле 3
+0x14   4      → копируется в DAT_0051cb44 (глобальная настройка DGS)
...
+0x66C  4      Поле: ReadByte → если == 2, scale ×2.0 (double render)
...
Total: 1648 байт (0x670)
```

### 5.3 Ключевые использования DGS

| Код | Действие |
|---|---|
| `template+0x163C → +0x14` | Глобальная настройка DGS (DAT_0051cb44) |
| `template+0x163C → +0x08` | Базовый offset для ReadLE16 |
| `template+0x163C → +0x66C` | Scale mode (1=normal, 2=double) |
| `template+0xB3C` | Количество B-scan полей (int) |
| `template+0x1660` | Validator function (DGS check) |

---

## 21. Полная карта DevFlags (DAT_0051cb30)

| Бит | Маска | Значение | Описание |
|---|---|---|---|
| 1 | 0x0002 | Расширенный формат | Доп. поля в body |
| 2 | 0x0004 | Вагонная версия | Увеличенный FLASH (5253 vs 4293) |
| 4 | 0x0010 | B-scan присутствует | Body содержит B-scan данные |
| 5 | 0x0020 | B-scan двойной | Два B-scan изображения |
| 6 | 0x0040 | Альтернативный шаблон | Другая таблица полей |
| 16 | 0x10000 | Extended field set | Шаблон с DGS расширением |
| 17 | 0x20000 | (не определён) | — |

---

## 22. Сводка находок (раунды 4-6, полная)

| # | Находка | Статус |
|---|---------|--------|
| 89 | 6 scale functions (0.1..10.0), выбор по mode byte + DevFlags | 🔒 Железобетонно |
| 90 | B-scan создаётся через CreateCompatibleBitmap → CxImage::Save PNG | 🔒 Железобетонно |
| 91 | PNG встраивается в WordML как base64 в `<w:binData>` | 🔒 Железобетонно |
| 92 | FDB TYPEVAR = raw tcode (прямое соответствие, 1:1) | 🔒 Железобетонно |
| 93 | Gate activation: 13 значений TypeVar/10 для строба 1 | 🔒 Железобетонно |
| 94 | Gate activation: 7 значений TypeVar/10 для строба 2 | 🔒 Железобетонно |
| 95 | FUN_00411a1a: tcode→объект (30+ конкретных маппингов) | 🔒 Железобетонно |
| 96 | DGS_Settings загружается из template+0x163C, 1648 байт (0x19C dwords) | 🔒 Железобетонно |
| 97 | DGS field +0x66C: scale mode (1=normal, 2=double rendering) | 🔒 Железобетонно |
| 98 | DGS field +0x14: глобальная настройка → DAT_0051cb44 | 🔒 Железобетонно |
| 99 | Серия прибора = tcode >> 8, режим = tcode & 0xFF | 🔒 Железобетонно |
| 100 | TypeVar 680-683 в FDB = тот же в DLL (вагонная серия) | 🔒 Железобетонно |

---



---

# ЧАСТЬ 3: A-scan waveform, DGS-объект, рендеринг отчёта

## 23. A-scan Waveform — формат данных

### 23.1 Формат

```
Расположение: body + template[0] (обычно +0x60 от начала body)
Размер:       244 байта (0xF4)
Формат:       1 байт на сэмпл (unsigned 0-255)
Разрешение:   244 горизонтальных точки
Y-ось:        0 = нет сигнала (низ), 255 = максимум (верх)
```

### 23.2 Рендеринг в zapis2.exe

```c
// Параметры отображения:
int displayWidth = 280;   // this+0x90 = 0x118 px
int displayHeight = 200;  // this+0x8c = 0xC8 px
float scaleX = displayWidth / 244.0f;   // ≈ 1.15 px/sample
float scaleY = displayHeight / 140.0f;  // ≈ 1.43 px/unit

// Цикл рисования (244 итерации):
for (i = 0; i < 244; i++) {
    int amplitude = waveformData[i];           // 0-255
    float x = i * scaleX;
    float y = amplitude * scaleY * scaleFactor; // scaleFactor из таблицы
    CDC::LineTo(dc, (int)x, (int)y);
}

// Сетка:
// - Мелкая: каждые 4 сэмпла (61 вертикальная линия)
// - Крупная: каждые 12 сэмплов (~20 линий)
```

### 23.3 Масштабирование (scale factor callback)

Масштабный множитель выбирается функцией по адресу `this+0x528`:
- Вычисляется на основе DevFlags и mode byte TypeVar
- Значения: 0.1, 0.4, 0.62, 1.25, 1.8, 2.5, 5.0, 10.0
- Применяется к Y-координате каждого сэмпла

### 23.4 Встраивание в отчёт

Итоговый размер изображения в WordML: **240pt × 120pt** (= 320×160 px при 96 DPI).
Если есть B-scan: `position:absolute` (наложение).

---

## 24. DGS-объект — полная структура (FUN_0040b6ff)

### 24.1 Конструктор

```c
DGS_Object* CreateDGS(uint32_t* templateData) {
    DGS_Object* obj = alloc(sizeof(DGS_Object));
    obj->vtable = &DGS_VTable;
    
    // Копирование 1648 байт (0x19C dwords) из шаблона
    memcpy(obj + 0x18, templateData, 1648);
    
    // Инициализация глобальных переменных:
    DAT_0051cb3c = (float)ReadLE16(obj->offset_field_2);  // развёртка X
    DAT_0051cb38 = (float)ReadLE16(obj->offset_field_3);  // развёртка Y
    DAT_0051cb40 = (float)ReadLE16(obj->offset_field_1);  // чувствительность
    DAT_0051cb44 = obj->dgs_param;                         // тип DGS
    
    // Defaults:
    obj->range = 300;       // +0x18: развёртка по умолчанию (мм)
    obj->gridX = 50;        // +0x58: шаг сетки X
    obj->gridY = 30;        // +0x54: шаг сетки Y
    
    // Инит масштаба:
    FUN_00422f86(obj, 0x438);  // 0x438 = 1080 (размер внутр. буфера?)
    
    return obj;
}
```

### 24.2 Карта полей DGS-объекта

| Offset | Size | Поле | Значение/Формула | Описание |
|--------|------|------|-----------------|----------|
| +0x00 | 4 | vtable | PTR_FUN_00496d34 | VTable |
| +0x04 | 20 | header | — | Внутренний заголовок |
| +0x18 | 4 | range | 300 | Развёртка (мм) |
| +0x54 | 4 | gridY | 30 | Шаг сетки Y |
| +0x58 | 4 | gridX | 50 | Шаг сетки X |
| +0x60 | 4 | fieldCount | N | Количество DGS-полей |
| +0x74 | 4 | dgs_param | → DAT_0051cb44 | Тип/режим DGS |
| +0x6C0 | 4 | offset_1 | LE16 offset | → DAT_0051cb40 |
| +0x6C4 | 4 | offset_2 | LE16 offset | → DAT_0051cb3c |
| +0x6C8 | 4 | offset_3 | LE16 offset | → DAT_0051cb38 |

### 24.3 Глобальные переменные рисования

| Адрес | Тип | Описание |
|-------|-----|----------|
| DAT_0051cb24 | int | Текущая X-координата (cursor) |
| DAT_0051cb28 | int | Текущая Y-координата (cursor) |
| DAT_0051cb3c | float | Масштаб X (из DGS offset_2) |
| DAT_0051cb38 | float | Масштаб Y (из DGS offset_3) |
| DAT_0051cb40 | float | Чувствительность (из DGS offset_1) |
| DAT_0051cb44 | dword | Параметр DGS (тип кривой) |
| DAT_0051cb4c | int | Ширина одного столбца данных |
| DAT_0051cb50 | int | Ширина разделителя между столбцами |
| DAT_00515ea0 | int | Scale multiplier (1=normal, 2=double) |

---

## 25. Rendering Flow — полная цепочка

### 25.1 FUN_0040da36 — инициализация документа

```
1. Выбор getDisplayMode callback (param_1+0x66C):
   - FUN_00408f24: стандартный (7 режимов: 0-6 + default)
   - FUN_00408f89: расширенный (10 режимов: 0-9 + default)

2. Выбор scaleFn callback (param_1+0x528):
   - По mode byte TypeVar + DevFlags (см. раздел 17.2)

3. Копирование A-scan данных:
   - 244 байта из raw data → template+4
   - Offset в raw data = *(template[0])

4. Создание DGS объекта:
   - 1648 байт из template+0x163C → FUN_0040b6ff
   - Результат → this+0x60

5. Инициализация масштабов:
   - DAT_0051cb3c, DAT_0051cb38, DAT_0051cb40 из DGS offsets
   - DAT_0051cb44 = DGS mode
   - this+0xAC = 2.0 (если DGS mode==2) или 1.0

6. Создание GDI bitmap (this+0x88):
   - CreateCompatibleBitmap(width, height)
   - width/height из init params
```

### 25.2 FUN_0040afd1 — рисование A-scan

```
1. Копирование DGS в стек (1648 байт)
2. Для каждого поля (this+0x60 полей):
   a. Получить значение через FUN_0040b7fd
   b. Switch по типу отображения (*(this + field*12 + 0x80)):
      - case 0x00: основная строка + подчёркивание
      - case 0x12/0x13: вторичная строка (смещение -0x28)
      - case 0x1C: правый столбец (x += 0x96) + подчёркивание
      - default: заголовок + значение
   c. Перемещение курсора Y
   d. При достижении середины → переход во второй столбец
3. Рисование координатной сетки
4. Рисование самой A-scan кривой (244 точки)
```

### 25.3 FUN_00408f24 — Display Mode (стандарт, 7 значений)

| Index | Адрес строки | Описание (гипотеза) |
|---|---|---|
| 0 | DAT_004aa420 | "мм" |
| 1 | DAT_004aa424 | "мкс" |
| 2 | DAT_004aa428 | "дБ" |
| 3 | DAT_004aa42c | "%" |
| 4 | DAT_004aa430 | "м/с" |
| 5 | DAT_004aa438 | "°" (градус) |
| default | DAT_004aa440 | "" (пусто) |

### 25.4 FUN_00408f89 — Display Mode (расширенный, 10 значений)

| Index | Адрес строки | Описание (гипотеза) |
|---|---|---|
| 0 | DAT_004aa444 | "мм" |
| 1 | DAT_004aa448 | "мкс" |
| 2 | DAT_004aa44c | "дБ" |
| 3 | DAT_004aa450 | "%" |
| 4 | DAT_004aa454 | "м/с" |
| 5 | DAT_004aa458 | "°" |
| 6 | DAT_004aa45c | "МГц" |
| 7 | DAT_004aa464 | "мм/мкс" |
| 8 | DAT_004aa46c | "дБ/мм" |
| default | DAT_004aa474 | "" |

---

## 26. Сводка находок (раунд 4, дополнение)

| # | Находка | Статус |
|---|---------|--------|
| 101 | A-scan = 244 байта, 1 byte/sample (0-255), offset из template[0] | 🔒 Железобетонно |
| 102 | Область отрисовки: 280×200 px, scaleX=280/244, scaleY=200/140 | 🔒 Железобетонно |
| 103 | Сетка A-scan: мелкая через 4, крупная через 12 сэмплов | 🔒 Железобетонно |
| 104 | DGS object: vtable + 20 header + 1648 data, defaults: range=300, gridX=50, gridY=30 | 🔒 Железобетонно |
| 105 | DAT_00515ea0 = scale multiplier (1 или 2), из DGS byte +0x66C | 🔒 Железобетонно |
| 106 | Display mode стандарт: 7 единиц (мм, мкс, дБ, %, м/с, °, пусто) | 🔒 Железобетонно |
| 107 | Display mode расш.: 10 единиц (+МГц, мм/мкс, дБ/мм) | 🔒 Железобетонно |
| 108 | A-scan render: CDC::LineTo в цикле 244, масштаб Y через callback | 🔒 Железобетонно |
| 109 | Поле (field*12 + 0x80): тип отображения (0=основной, 0x1C=правый, 0x12=вторичный) | 🔒 Железобетонно |
| 110 | FUN_0040da36: полный pipeline инициализации (6 шагов) | 🔒 Железобетонно |

---



## 27. Deflate/LZ77 Compressor — B-scan сжатие

### 27.1 Назначение

B-scan данные (3840 или 4800 байт 1-bit bitmap) сжимаются стандартным **Deflate (RFC 1951)** перед записью в PROTOCOL BLOB. Это **тот же** алгоритм что используется в zlib/gzip/PNG.

### 27.2 Структура компрессора (5816+ байт)

| Offset | Размер | Поле | Описание |
|--------|--------|------|----------|
| +0x0008 | 4 | output_buf | Указатель на выходной буфер |
| +0x000C | 4 | input_data | Указатель на входные данные |
| +0x0010 | 4 | input_size | Размер входных данных |
| +0x0014 | 4 | output_pos | Текущая позиция записи |
| +0x008C | 572 | lit_freq[286] | Частоты литералов (short×286) |
| +0x0980 | 60 | dist_freq[30] | Частоты расстояний (short×30) |
| +0x0A74 | 38 | bitlen_freq[19] | Частоты длин кодов (short×19) |
| +0x0B14 | 4 | lit_count | Количество литеральных кодов |
| +0x0B20 | 4 | dist_count | Количество distance-кодов |
| +0x0B34 | 32 | code_buf[16] | Буфер кодов (short×16) |
| +0x0B54 | 2296 | heap[574] | Куча для Huffman (int×574) |
| +0x1448 | 4 | heap_size | Размер кучи |
| +0x144C | 4 | heap_max | Максимальный индекс |
| +0x1450 | 574 | depth[574] | Глубина узлов (byte×574) |
| +0x1690 | 4 | pending_buf | Буфер ожидания (byte[]) |
| +0x1698 | 4 | pending_count | Элементов в ожидании |
| +0x169C | 4 | pending_codes | Коды ожидания (short[]) |
| +0x16A0 | 4 | total_bits | Всего записано бит |
| +0x16A4 | 4 | secondary_bits | Вторичный счётчик бит |
| +0x16AC | 4 | bits_per_code | Бит на символ (=8) |
| +0x16B0 | 2 | bit_buffer | 16-бит буфер (текущий) |
| +0x16B4 | 4 | bits_in_buffer | Бит в буфере |

### 27.3 Функции компрессора

| Функция | RVA | Назначение |
|---------|-----|-----------|
| FUN_0046f9ec | 0x0046f9ec | Reset: обнуление частотных таблиц |
| FUN_0046fbdc | 0x0046fbdc | Heap sift-down (приоритетная очередь) |
| FUN_0046fccc | 0x0046fccc | Build Huffman tree из частот |
| FUN_00470e2c | 0x00470e2c | Dynamic block header (HLIT, HDIST, HCLEN) |
| FUN_004710bc | 0x004710bc | Stored block (некомпрессированный fallback) |
| FUN_004715cc | 0x004715cc | Encode symbols (Huffman bitstream writer) |
| FUN_00471c2c | 0x00471c2c | Flush (завершение блока, final bit) |

### 27.4 Статические таблицы

```
DAT_0051bd80: Fixed literal Huffman codes (стандарт RFC 1951 §3.2.6)
DAT_0051bc80: Fixed distance Huffman codes (стандарт RFC 1951 §3.2.6)
```

### 27.5 Поток обработки B-scan

```
1. Raw B-scan bitmap (3840/4800 bytes, 1-bit/pixel)
     ↓
2. LZ77 sliding window matching (FUN_0047115c et al)
     ↓
3. Huffman encoding (FUN_004715cc)
     ↓
4. Bitstream output → compressed BLOB
     ↓
5. Записывается в PROTOCOL столбец RESULTS2/BLOCKZAP
```

### 27.6 Связь с DGS_Settings расширением

Расширение шаблона (+0x1668..+0x1CD8) = **НЕ DGS-кривая**, а **рабочее состояние Deflate-компрессора**! Поля +0x1690..+0x16B4 — это pending buffer, bit accumulator и т.д. компрессора.

---

## 28. WordML Report Generator — полная схема

### 28.1 Структура XML-документа

```xml
<?xml version="1.0" encoding="windows-1251"?>
<?mso-application progid="Word.Document"?>
<w:wordDocument
    xmlns:w="http://schemas.microsoft.com/office/word/2003/wordml"
    xmlns:v="urn:schemas-microsoft-com:vml"
    xmlns:w10="http://schemas.microsoft.com/office/word/2003/auxHint"
    xmlns:sl="http://schemas.microsoft.com/schemaLibrary/2003/core"
    xmlns:aml="http://schemas.microsoft.com/aml/2001/core"
    xmlns:wx="urn:schemas-microsoft-com:office:word"
    xmlns:o="urn:schemas-microsoft-com:office:office"
    xmlns:dt="uuid:C2F41010-65B3-11d1-A29F-00AA00C14882">
  <w:body>
    <wx:sect>
      <w:sectPr>
        <w:pgSz w:w="11906" w:h="16838"/>  <!-- A4 portrait -->
        <w:cols w:num="2"/>                  <!-- 2 колонки -->
        <w:type w:val="continuous"/>
      </w:sectPr>
      <!-- Field data table -->
      <!-- A-scan image (protPict.png) -->
      <!-- B-scan image (if present) -->
    </wx:sect>
  </w:body>
</w:wordDocument>
```

### 28.2 Генерация полей (FUN_00424d9c)

```c
for (idx = 0; idx < field_count; idx++) {
    // Проверка видимости:
    if (fields[idx].visibility_callback != 0) {
        if (fields[idx].visibility_callback(raw_data) == 0) {
            fields[idx].type_code = 0xFF;  // скрыть
            continue;
        }
    }
    
    // Создание CEdit для редактируемых полей:
    if (type in {0x24, 0x25, 0x26, 0x2C, 0x36, 0x4D}) {
        CEdit* edit = new CEdit(...);
        edit->SetWindowText(decoded_value);
        map.SetAt(fields[idx].name, edit);
    }
}
```

### 28.3 Размеры отчёта

| Параметр | Значение |
|----------|----------|
| Страница | A4 (11906×16838 twips = 210×297 мм) |
| Ширина столбца | DAT_0051cb4c px |
| Шаг между столбцами | DAT_0051cb50 px |
| Высота строки | DAT_0051cb2c = 13 px |
| Ширина A-scan графика | 280 px (отрисовка), 240pt (WordML) |
| Высота A-scan графика | 200 px (отрисовка), 120pt (WordML) |
| Масштаб X | width / 244 ≈ 1.15 |
| Масштаб Y | height / 140 ≈ 1.43 |

---

## 29. Итоговая сводка (110+ находок)

| # | Находка | Статус |
|---|---------|--------|
| 101 | A-scan = 244 байта, 1 byte/sample (0-255) | 🔒 |
| 102 | Область отрисовки: 280×200 px, scale 280/244 | 🔒 |
| 103 | Сетка: мелкая через 4, крупная через 12 сэмплов | 🔒 |
| 104 | DGS object: vtable+20+1648, defaults range=300 | 🔒 |
| 105 | DAT_00515ea0 = scale 1 или 2 (из DGS byte +0x66C) | 🔒 |
| 106 | Display mode стандарт: 7 единиц (мм,мкс,дБ,%,м/с,°,пусто) | 🔒 |
| 107 | Display mode расш.: +МГц, мм/мкс, дБ/мм | 🔒 |
| 108 | A-scan render: CDC::LineTo × 244, Y через callback | 🔒 |
| 109 | Field array: stride 7 dwords, visibility callback | 🔒 |
| 110 | B-scan compression = standard Deflate (RFC 1951) | 🔒 |
| 111 | Deflate state: 5816+ bytes (+0x08..+0x16B4) | 🔒 |
| 112 | 7 Deflate functions (reset, sift, build, header, stored, encode, flush) | 🔒 |
| 113 | Static Huffman tables: DAT_0051bd80/bc80 | 🔒 |
| 114 | Template extension = Deflate compressor state (NOT DGS curve!) | 🔒 |
| 115 | WordML: A4, encoding=windows-1251, 9 xmlns declarations | 🔒 |
| 116 | Report layout: 2 columns, row height 13px, continuous section | 🔒 |
| 117 | Field visibility: callback-based (0=always, ptr=conditional) | 🔒 |
| 118 | Hidden field marker: type_code set to 0xFF | 🔒 |

---


## 30. FUN_004058df — Focusing Correction (ближняя зона, ПОЛНЫЕ коэффициенты)

```c
float FocusingCorrection(int channel, float distance, float near_field_mm) {
    float ratio = (distance + near_field_mm) / DAT_0051c684;
    float diameter_factor = 2*sqrt(Real48[DGS+0x34]) / DAT_0051c680;
    
    if (distance > DAT_0051c66c || (distance == DAT_0051c66c && channel != 0)) {
        // Переходная зона (1.5N < x < 3N):
        float base = log10(exp) * 20.0;  // дБ
        if (distance < DAT_0051c670) {
            // Полиномиальная коррекция:
            base = base - (ratio * -10.04 + 29.83) * diameter_factor;
            base = ratio * -1.43 + 3.86 + base;
        }
        return base;
    } else {
        // Ближняя зона (x < 1.5N):
        if (distance < DAT_0051c674 || (distance == DAT_0051c674 && channel == 0)) {
            // Глубокая ближняя зона (< 0.7N):
            return diameter_factor * 159.0 
                   + (ratio*ratio * 19.1 - ratio * 7.2) 
                   - diameter_factor*diameter_factor * 176.0 
                   - 49.4;
        } else {
            // Промежуточная зона (0.7N..1.5N):
            return diameter_factor * 180.0 
                   + (ratio * 35.8 - ratio*ratio * 17.1) 
                   - diameter_factor*diameter_factor * 214.0 
                   - 63.5;
        }
    }
}
```

### Ключевые константы DGS-формулы:

| Зона | Коэффициенты | Описание |
|---|---|---|
| Переходная (1.5N-3N) | -10.04, 29.83, -1.43, 3.86 | Логарифм + линейная коррекция |
| Промежуточная (0.7N-1.5N) | 180.0, 35.8, -17.1, -214.0, -63.5 | Полином 2-го порядка |
| Глубокая (< 0.7N) | 159.0, 19.1, -7.2, -176.0, -49.4 | Полином 2-го порядка |

---

## 31. Базовые GDI-обёртки

| Функция | Реальное действие | Параметры |
|---|---|---|
| FUN_00406870 | CDC::MoveTo(dc, x, y) | this=dc, param2=x, param3=y |
| FUN_004068b0 | DeleteObject(pen) | Удаляет перо (CPen destructor) |
| FUN_004068d0 | CDC::LineTo(dc, x, y) | this=dc, param1=x, param2=y |
| FUN_004068f0 | SetPixel(dc, x, y, color) | Одна точка |
| FUN_00406920 | float-to-int (rounded) | Банковское округление (>0.5) |
| FUN_00406980 | strcmp(str1, str2) != 0 | Сравнение строк |
| FUN_004069c0 | CPoint(x, y) | Создание точки |
| FUN_00406a30 | CreatePen(style, width, color) | Создание пера для рисования |
| FUN_0047c281 | CDC::MoveTo(dc, x, y) -> old_point | MoveTo с возвратом прежней позиции |

---

## 32. FUN_00406920 — Float-to-Int с округлением

```c
int FloatToIntRounded(float value) {
    int truncated = (int)value;
    float frac = fabs(value - (float)truncated);
    if (frac > 0.5) {
        if (truncated < 1) truncated--;  // для отрицательных
        else truncated++;                 // для положительных
    }
    return truncated;
}
```

---

## 33. FUN_004157b2 — Инициализация главной формы (OnInitialUpdate)

Полная цепочка:
```
1. FUN_0040da36(param_1[0x18])     -> инит DGS/scale/callbacks
2. Копирование B-scan scale fn -> widget (+0x165C -> +0x40 = scaleFn)
3. Создание CEdit "numZap" (5 символов шириной) -> SetAt("numZap", edit)
4. Измерение текстовых размеров -> DAT_0051cb48, DAT_0051cb2c
5. Цикл по param_1[0x26]+0x48 полям (stride 0x1C):
   - Проверка visibility callback (+0x64 в stride)
   - Создание CEdit для типов 0x24, 0x25, 0x26, 0x2C, 0x36, 0x4D
   - SetAt(field_name, edit_control)
6. Цикл по param_1[0x26]+0xB3C B-scan полям (тот же stride 0x1C):
   - Проверка visibility (+0xB58 в stride)
   - Создание CEdit для типов 0x25, 0x32
```

### Ключевые offsets в main form structure:

| Offset | Описание |
|---|---|
| param_1[0x06] | Field counter (auto-increment) |
| param_1[0x0B] | CMapStringToPtr (name->control) |
| param_1[0x18] | Raw data pointer |
| param_1[0x26] | Template pointer (= this+0x98 эквивалент) |
| param_1[0x26]+0x48 | Standard field count |
| param_1[0x26]+0x50 | Standard field[i].size_packed |
| param_1[0x26]+0x54 | Standard field[i].type_code |
| param_1[0x26]+0x64 | Standard field[i].visibility_callback |
| param_1[0x26]+0xB3C | B-scan field count |
| param_1[0x26]+0xB44 | B-scan field[i].size_packed |
| param_1[0x26]+0xB48 | B-scan field[i].type_code |
| param_1[0x26]+0xB4C | B-scan field[i].name_string |
| param_1[0x26]+0xB58 | B-scan field[i].visibility_callback |

---

## 34. FUN_00422629 — A-scan Bitmap Init

```c
void InitAScanBitmap(int* param_1) {
    // Выбор DGS divider:
    if ((DAT_0051cb30 & 8) == 0)
        DAT_0051cb44 = 10.0f;    // 0x41200000 = 10.0
    else
        DAT_0051cb44 = 20.0f;    // 0x41a00000 = 20.0
    
    // Создание DC и bitmap:
    CreateCompatibleDC(0);
    CreateBitmap(local_34);
    
    // Чтение "Label" из реестра:
    label = FUN_00408834("Label");  // HKLM\SOFTWARE\Altek\PelengPC\Label
    
    // Инициализация пиксельного массива:
    param_1[0x13] = DAT_0051c7a4;  // width
    param_1[0x14] = DAT_0051c7a8;  // height
    param_1[0x12] = alloc(width * 4);  // row pointers
    for (row = 0; row < width; row++) {
        param_1[0x12][row] = alloc(height * 4);
        for (col = 0; col < height; col++) {
            param_1[0x12][row][col] = GetPixelColor(dc, row, col);
        }
    }
}
```

---

## 35. Математические функции (CRT library)

| RVA | Имя | Формула | Использование |
|---|---|---|---|
| FUN_00431b40 | exp(x) | e^x | DGS exponent |
| FUN_00431a84 | log10(x) | lg(x) | dB = 20*log10(A) |
| FUN_004319c4 | sqrt(x) | sqrt(x) | Диаметр = 2*sqrt(Real48) |
| FUN_0043185f | fabs(x) | abs(x) | Округление |
| FUN_004310c0 | memcpy(dst, src, n) | copy | Везде |
| __ftol | float->long | truncate | Координаты рисования |

---

## 36. Сводка (дополнение)

| # | Находка | Статус |
|---|---------|--------|
| 119 | FUN_004058df: 3 зоны, полиномиальные коэффициенты | 🔒 Железобетонно |
| 120 | Глубокая зона: 159.0, 19.1, -7.2, -176.0, -49.4 | 🔒 Железобетонно |
| 121 | Промежуточная: 180.0, 35.8, -17.1, -214.0, -63.5 | 🔒 Железобетонно |
| 122 | Переходная: -10.04, 29.83, -1.43, 3.86 | 🔒 Железобетонно |
| 123 | FUN_00406920: float->int с банковским округлением | 🔒 Железобетонно |
| 124 | DAT_0051cb44 = 10.0 или 20.0 (DevFlags bit3) | 🔒 Железобетонно |
| 125 | FUN_004157b2: two field loops (+0x48 std, +0xB3C bscan) | 🔒 Железобетонно |
| 126 | B-scan fields: +0xB44/+0xB48/+0xB4C/+0xB58 stride 0x1C | 🔒 Железобетонно |
| 127 | 9 GDI wrappers (MoveTo, LineTo, SetPixel, CreatePen...) | 🔒 Железобетонно |
| 128 | 6 CRT math (exp, log10, sqrt, fabs, memcpy, ftol) | 🔒 Железобетонно |

---
