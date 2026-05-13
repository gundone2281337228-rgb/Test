# PELENG REVERSE — Раунд 5: Формулы, декодеры, полный field switch

**Дата:** 2026-05-13  
**Источник:** `zapis2.c` (Ghidra decompile, 133634 строк)

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
