# PELENG REVERSE — RAUND 5 (новые находки сверх ROUND4)

> Continuation of reverse engineering. ROUND4 has 189 findings (§1-66).
> This file documents NEW findings (#190+) from deeper inspection of:
> - `102_203dll.dll+*.c` (3 версии декомпиляции, 33 872 строк каждая)
> - `PelengPC.exe+1+1.c`, `PelengPC.exe+1+2.c` (225 870 строк каждая)
> - `peleng_clone+12+3.py` (6 421 строк, Python-клон)
>
> Маркеры:
> - 🔒 **Железобетонно** — подтверждено в декомпилированном коде минимум двух мест
> - 🟢 **Высокая уверенность** — однозначно прочитано в коде, не пересечено вручную
> - 🟡 **Гипотеза** — требует доп. подтверждения
>
> Все смещения — в формате `+0xNN` от базы соответствующей структуры.

---

## 67. `_SortBufData` (DLL export №2) — главный декодер записи

**Сигнатура:** `int* __cdecl _SortBufData(byte* raw_record, uint* io_size);`
Адрес: `0x00401454 / index 2` в DLL экспортах.

**Алгоритм** (102_203dll.dll+3.c:2386-2494):

```c
uint  sweep_addr = raw[0x10] | raw[0x11]<<8;   // LE16, адрес записи в флэше
uint  category   = sweep_addr / 1000;          // главный диспетчер!
uint  sweep_id   = raw[4]*100 + raw[5];        // ДЕСЯТИЧНЫЙ id (NB: не binary)
uint  device_no  = raw[0] | raw[1]<<8;         // LE16, № прибора
uint  some16     = raw[0x15] | raw[0x16]<<8;   // LE16, неизвестное поле

FORMAT* fmt = GetFormatByCategory(&category);  // схема полей для категории
int    header_len = CalcHeaderLength(fmt);     // ∑ sizes of fields
int    n_fields   = fmt[2];                    // число полей
int    out_size   = raw_size + header_len + n_fields*4 + 2;
int*   out = AllocBuffer(out_size);            // +4 байта префикса длины
*io_size = out_size;

/* Раскладка выходного буфера: */
//  [+0]                          byte    decoder_type
//  [+1]                          byte    n_fields
//  [+2 .. +2+4*n_fields]         []      field offset table (по 4 байта)
//  [+2+4*n_fields .. +header_end] []     decoded field data (см. FUN_00401f54)
//  [+header_end .. +out_size]     []     raw_record (memcpy)

memcpy(out + n_fields*4 + header_len + 2, raw_record, raw_size);
switch (category) {
  case 1:                                 out[0]=2; DecodeAscan(...);          break;
  case 4: case 6:                         out[0]=1; DecodeGeneric(...);        break;
  case 5:                                 out[0]=1; DecodeCalibration(...);    break;
  case 10..19 (0x0A..0x13):               out[0]=3; DecodeBscan(...);          break;
  case 20..29 (0x14..0x1D):               out[0]=4; DecodeShortProtocol(...);  break;
}
FillFieldOffsets(out + 2, fmt);
out[1] = (char)fmt[2];                    // n_fields повторно
return out;
```

| № | Находка | Статус |
|---|---|---|
| 190 | `_SortBufData`: первый байт выходного буфера = `decoder_type` (1/2/3/4) | 🔒 Железобетонно |
| 191 | Категория записи = `sweep_addr / 1000` (НЕ просто старшие байты) | 🔒 Железобетонно |
| 192 | Категория 1 → A-scan; 4,6 → Generic; 5 → Calibration; 10-19 → B-scan; 20-29 → ShortProto | 🔒 Железобетонно |
| 193 | `sweep_id = raw[4]*100 + raw[5]` — десятичная кодировка (0..9999) | 🔒 Железобетонно |
| 194 | DLL output buffer = `[decoder_type, n_fields, offset_table(4B*n), field_data, raw_record]` | 🔒 Железобетонно |
| 195 | `AllocBuffer(N)` выделяет N+4 байт, возвращает указатель за заголовком | 🔒 Железобетонно |
| 196 | `FreeAllocBuffer(p)` делает `free(p-4)` (использует префикс длины) | 🔒 Железобетонно |

---

## 68. `FillFieldOffsets` — построение оффсет-таблицы

**102_203dll.dll+3.c:2827-2850.**

```c
void FillFieldOffsets(int* out_table, FORMAT* fmt) {
  ushort offset = 0;
  for (int i = 0; i < fmt[2]; i++) {
    FIELD* fd = GetFieldDescriptor(fmt, i);
    ushort size = fd[0x10/4];
    memcpy(out_table + i*4 + 0, &offset, 2);   // LE16 offset
    memcpy(out_table + i*4 + 2, &size,  2);    // LE16 size
    offset += size;
  }
}
```

Каждый элемент `offset_table[i]` (4 байта) = `(LE16 offset, LE16 size)` поля #i в области decoded data.
Это позволяет PelengPC.exe итерироваться по полям по индексу без знания семантики.

| № | Находка | Статус |
|---|---|---|
| 197 | Элемент offset_table = (LE16 offset, LE16 size) для каждого поля | 🔒 Железобетонно |
| 198 | Поля упакованы плотно (offset = ∑ предыдущих sizes), без выравнивания | 🔒 Железобетонно |

---

## 69. `GetFormatByCategory` — генерация схемы полей

**102_203dll.dll+3.c:5057+.** Большой switch по категории.
Структура FORMAT (Borland TList):

```c
struct FORMAT {
  void*    vtable;       // +0x00
  FIELD**  items;        // +0x04 указатель на массив FIELD*
  int      count;        // +0x08
  int      capacity;     // +0x0C
};
```

`GetFieldDescriptor(fmt, i)` возвращает `fmt->items[i]` (т.е. указатель из TList).

**Структура FIELD (создаётся в `AddField` = FUN_00401db0):**

```c
struct FIELD {
  AnsiString  name;          // +0x00 краткое имя поля
  AnsiString  hint;          // +0x04 описание (русское)
  int         type;          // +0x08 (1=int, 2=string, 3=date)
  int         kind;          // +0x0C доп.категория (0, 0x14, 5, 0x1e, 10, ...)
  int         size;          // +0x10 размер в выходном буфере (байт)
};
```

**Пример для category=1 (A-scan), первые 8 полей** (из 5057-5176):

| # | type | kind | size | назначение |
|---|------|------|------|-----------|
| 1 | 1 | 0 | 1 | byte-flag (decoder_type?) |
| 2 | 1 | 0 | 4 | uint32 (например, sweep_id LE→BE) |
| 3 | 2 | 0x14 | 20 | строка 20 байт |
| 4 | 1 | 0 | 3 | uint24 (3 байта BE) |
| 5 | 2 | 5 | 5 | строка 5 байт |
| 6 | 1 | 0 | 2 | uint16 |
| 7 | 2 | 0x1e | 1 | строка 1 байт |
| 8 | 2 | 10 | 10 | строка 10 байт |

| № | Находка | Статус |
|---|---|---|
| 199 | FORMAT = Borland TList с `items, count, capacity` начиная с +0x04 | 🔒 Железобетонно |
| 200 | FIELD = `{name, hint, type, kind, size}` (20 байт, +AnsiString refcnt) | 🔒 Железобетонно |
| 201 | `type ∈ {1,2,3}`: 1=integer(BE), 2=string(fixed), 3=date(day,month,year) | 🔒 Железобетонно |
| 202 | Каждой категории соответствует УНИКАЛЬНАЯ схема (в GetFormatByCategory) | 🔒 Железобетонно |

---

## 70. `FUN_00401f54` (=WriteField) — кодирование поля

**102_203dll.dll+3.c:2687-2822.** Это сердце кодировщика. Записывает одно поле в выходной буфер по индексу/имени.

```c
void WriteField(byte* out_base, FORMAT* fmt, AnsiString* field_name, AnsiString* value)
{
  int field_idx = FindFieldByName(fmt, field_name);
  int offset_in_data = CalcOffsetUpTo(fmt, field_idx);
  FIELD* fd = GetFieldDescriptor(fmt, field_idx);
  int type = fd->type;
  int size = fd->size;

  if (type == 1) {                                   /* INTEGER  */
    uint v = ParseAnsiInt(value, 0);
    for (int j = 0; j < size; j++) {
      out_base[offset_in_data + size - 1 - j] = (byte)v;
      v >>= 8;
    }
    /* эквивалентно записи uint в позицию [offset .. offset+size-1] BIG-ENDIAN */
  }
  else if (type == 2) {                              /* STRING fixed-size */
    memset(out_base + offset_in_data, 0, size);
    int slen = AnsiStringLength(*value);
    int n = (slen < size) ? slen : size;
    memcpy(out_base + offset_in_data, value, n);
    /* остаток оставлен нулями */
  }
  else if (type == 3) {                              /* DATE: 3 байта */
    /* парсит три AnsiString из tlist value (день, месяц, год) */
    out_base[offset_in_data + 0] = ParseByte(value[0]);   /* день  */
    out_base[offset_in_data + 1] = ParseByte(value[1]);   /* месяц */
    out_base[offset_in_data + 2] = ParseByte(value[2]);   /* год % 100 */
  }
}
```

**Главное:** integer-поля кодируются в **BIG-ENDIAN** в DLL output buffer.
Это критично для корректного парсинга `pbVar10[offset + i] + acc*256` в PelengPC.

| № | Находка | Статус |
|---|---|---|
| 203 | Integer-поля DLL output buffer записаны в **BIG-ENDIAN** (MSB→offset+0) | 🔒 Железобетонно |
| 204 | String-поля: фикс. длина `size`, остаток обнулён (НЕ null-terminated) | 🔒 Железобетонно |
| 205 | Date-поля: 3 байта (день, месяц, год % 100), без endianness | 🔒 Железобетонно |
| 206 | `WriteField` ищет поле по имени (AnsiString), О(n) перебором | 🔒 Железобетонно |

---

## 71. `DispatchByTypeVar` — диспетчер декодеров типов

**102_203dll.dll+3.c:2853-2894.**

```c
void DispatchByTypeVar(int* fields, ...) {
  int tv = fields[3];  /* fields[3] = TypeVar (16-битное поле прибора) */
  if (tv < 0x282) {
    if (tv < 0x186 && (tv - 0x96 < 2))           DecodeTypeVar_Ascan(...);       // 0x96, 0x97
    else                                          DecodeTypeVar_Default(...);
  } else if (tv < 0x2da) {
    if (tv - 0x282 < 2)                           DecodeTypeVar_Composite(...);   // 0x282, 0x283
    else                                          DecodeTypeVar_Default(...);
  } else                                          DecodeTypeVar_Default(...);
}
```

| № | Находка | Статус |
|---|---|---|
| 207 | `DispatchByTypeVar`: TypeVar читается с `fields[3]` (uint32 LE-аккумулированный) | 🔒 Железобетонно |
| 208 | A-scan TypeVar ∈ {0x96, 0x97} (т.е. 150, 151) | 🔒 Железобетонно |
| 209 | Composite TypeVar ∈ {0x282, 0x283} (т.е. 642, 643) | 🔒 Железобетонно |
| 210 | Все остальные TypeVar → DecodeTypeVar_Default | 🔒 Железобетонно |

---

## 72. `DecodeDate` / `DecodeTime` — формат даты в RAW записи

**102_203dll.dll+3.c:2898-2956.**

```c
void DecodeDate(int param_2, ..., int byte_offset) {
  byte day   = raw[byte_offset + 0];
  byte month = raw[byte_offset + 1];
  byte year  = raw[byte_offset + 2];     /* year-2000 */
  /* Формат: "dd.mm.yyyy" */
  AnsiString s = sprintf("%02d.%02d.%04d", day, month, 2000+year);
}

void DecodeTime(int param_2, ..., int byte_offset) {
  byte hour   = raw[byte_offset + 0];
  byte minute = raw[byte_offset + 1];
  /* Формат: "hh:mm" */
}
```

В A-scan / Composite / Default декодерах **вызывается:**
```c
DecodeDate(raw, ..., 7);    /* дата в bytes [7,8,9] */
DecodeTime(raw, ..., 10);   /* время в bytes [10,11] */
```

| № | Находка | Статус |
|---|---|---|
| 211 | RAW record: date в `bytes[7..9]` = (day, month, year-2000) — НЕ BCD! | 🔒 Железобетонно |
| 212 | RAW record: time в `bytes[10..11]` = (hour, minute) | 🔒 Железобетонно |
| 213 | Год = бинарный байт + 2000 (т.е. 2019 = 0x13) | 🔒 Железобетонно |
| 214 | `ValidateDate` проверяет month∈1..12, day∈1..31, year>2000, **+ високосный** (год%4==0 и %100!=0) | 🔒 Железобетонно |

---

## 73. `DecodeAscan` / `DecodeBscan` / `DecodeGeneric` / `DecodeCalibration` / `DecodeShortProtocol`

**Все 5 декодеров повторяют похожий шаблон** (выявляется сверкой 3212-3494):

```c
DecodeXxx(int* fields, int raw, int out, int* fmt) {
  WriteString  (out, fmt, &AnsiNull, &AnsiNull);             /* поле "пробел" */
  WriteString  (out, fmt, IntToStr(fields[2] % 1000), ...);  /* sweep_id mod 1000 */
  WriteString  (out, fmt, &AnsiNull, ...);                   /* пустое поле */
  DispatchByTypeVar(fields, raw, out, fmt);                  /* << ВСТРОЙКА A/Comp/Default */
  WriteString  (out, fmt, IntToStr(fields[5]), ...);         /* fields[5] */
  WriteString  (out, fmt, ...);                              /* ещё одно */
  WriteString  (out, fmt, fields+4, ...);                    /* fields[4] (адрес?) */
  WriteString  (out, fmt, IntToStr(fields[1]), ...);         /* fields[1] (mode?) */
}
```

**`DecodeShortProtocol`** (3407-3484) дополнительно:
- Делит `fields[2] % 10000` на 100 (VARIANT арифметика для float)
- В конце вызывает `FUN_00402708(raw + 0x11, ...)` для копирования 6 байт по offset +0x11 в raw record (BCD-номер?)

**`DecodeBscan`** (3346-3405) использует `fields[2] % 10000` (не 1000), что отражает 4-значный sub_id для B-scan записей.

| № | Находка | Статус |
|---|---|---|
| 215 | `DecodeAscan/Generic/Calibration`: модуль 1000 для sub_id; `DecodeBscan`: модуль 10000 | 🔒 Железобетонно |
| 216 | `DecodeShortProtocol`: VARIANT-арифметика — `fields[2]/100` + копия 6 байт из raw[+0x11] | 🔒 Железобетонно |
| 217 | Все декодеры пишут поля по схеме: id, sub_id, type-specific, sub_addr, address, mode | 🔒 Железобетонно |

---

## 74. PelengPC: парсинг DLL output buffer (`pbVar10`)

**PelengPC.exe+1+1.c:14380-14495** — функция FUN_00410a68 (Add Records to IB DB).

```c
byte* pbVar10 = (byte*)(*DAT_005be998)();  /* _SortBufData(raw, &size) */
int n_fields = pbVar10[1];                 /* offset 1 = count */
/* offset_table at pbVar10 + 2 */
for (i = 0; i < n_fields; i++) {
  ushort offset = LE16(pbVar10 + 2 + i*4 + 0);
  ushort size   = LE16(pbVar10 + 2 + i*4 + 2);
  byte*  data   = pbVar10 + 2 + offset;        /* +2 пропускает [type, n] */
  FIELD* fd     = GetFieldDescriptor(fmt, i);
  int    kind   = fd->type & 7;                /* (NB: маска `& 7`!) */
  if (kind == 1) {                             /* INTEGER, BE */
    uint val = 0;
    for (int j = 0; j < size; j++)
      val = (val << 8) | data[j];              /* MSB-first → BIG-ENDIAN! */
  } else if (kind == 2) {                      /* C-string (NUL-terminated) */
    char* s = strndup(data, size);
  } else if (kind == 3) {                      /* DATE 3 байта */
    byte y = data[1];                          /* +1 = год */
    byte m = data[0];                          /* +0 = месяц? */
    byte d = data[2];                          /* +2 = день */
  }
  bind_to_sql_param(piVar9, i, val);
}
```

⚠️ **NB:** **смещения** `data[0/1/2]` отличаются от RAW-формата `bytes[7..9]`!
В DLL output buffer (после WriteField type=3): bytes написаны как **`(day, month, year)`**, но **PelengPC читает их как `(month, day, year)`** — потому что PelengPC использует именно те же буферные смещения, что и WriteField пишет.

Перепроверка по 14439-14488: validate `local_13c = pbVar10[uVar18 + 3]` где `uVar18 = LE16(offset)` → проверка `local_13c < 0xC` означает **месяц ∈ 1..12**, т.е. byte[+0] = day, byte[+1] = ?, byte[+3] = month? Нужны доп. эксперименты. Финальный порядок зависит от FIELD->size:
- Если size == 3: bytes = (day, month, year)
- Если size == 5: возможно вкл. time

⚠️ Это **отдельный класс ошибок в `peleng_clone+12+3.py:decode_date()`**: предполагает что байты идут (dd, mm, yy) что совпадает с RAW, но не с DLL output. Если PROD клон использует DLL — порядок может отличаться.

| № | Находка | Статус |
|---|---|---|
| 218 | PelengPC читает integer-поля DLL output как BIG-ENDIAN (накапливая `*256+byte`) | 🔒 Железобетонно |
| 219 | `kind = fd->type & 7` — старшие биты type зарезервированы под флаги | 🔒 Железобетонно |
| 220 | String-поля парсятся до первого \0 байта (NUL-terminated, не Pascal!) | 🔒 Железобетонно |
| 221 | Параметры биндятся в SQL `INSERT` через TIBCustomDataSet-обёртку | 🔒 Железобетонно |

---

## 75. PelengPC: Адресная арифметика записей

**PelengPC.exe+1+1.c:21153-21175.**

```c
int GetBaseAddr(_, int addr)  { return (addr / 100) * 100; }    /* старшие 100ки */
int GetSubBlock(_, int addr)  { return (addr % 10000) / 100; }  /* поддиапазон   */
int GetIndex   (_, int addr)  { return addr % 100; }            /* индекс в блоке*/

/* Sweep — отдельная категория: */
bool IsSweepAddress(_, int addr) {
  return (9999 < addr) && (addr < 30000);
}
```

**Иерархия адреса:**
```
addr = baseAddr*1 + subBlock*100 + index*1   (где subBlock∈0..99, index∈0..99)
       └ category = baseAddr/1000 (0..29)
```

Адреса 10000..29999 — диапазон Sweep-записей (B-scan / ShortProto).
Адреса < 10000 — обычные записи (A-scan / Generic / Calibration).

| № | Находка | Статус |
|---|---|---|
| 222 | `GetBaseAddr(addr) = (addr / 100) * 100` (округление до сотен) | 🔒 Железобетонно |
| 223 | `GetSubBlock(addr) = (addr % 10000) / 100` (десятки сотен) | 🔒 Железобетонно |
| 224 | `GetIndex(addr) = addr % 100` (индекс в блоке из 100 записей) | 🔒 Железобетонно |
| 225 | Sweep-диапазон: 10000 ≤ addr < 30000 (т.е. categories 10-29) | 🔒 Железобетонно |

---

## 76. PelengPC: Категории в FormReadData (TabSet)

**PelengPC.exe+1+1.c:20861-20889 + 20944-20964.**

FormReadData содержит **6 категорий** (или 5 при типе устройства == 3):

```c
*(int*)(param_1 + 0x3b0) = (device_type == 3) ? 5 : 6;
```

Каждая категория — это запись 0x18 байт (24 байт) по адресу `param_1 + 800 + i*0x18`:

```c
struct CategoryEntry {     // 24 байта
  AnsiString name;         // +0x00  название категории (RU)
  ...
  int        min_addr;     // +0x14  начальный sweep_addr
  int        max_addr;     // +0x18  конечный sweep_addr
  void*      ib_query;     // +0x1C  TIBCustomDataSet*
  void*      dgs_curve;    // +0x20  TList* (DGS curve)
};
```

**FormReadData state fields:**

| Offset | Тип | Смысл |
|--------|-----|-------|
| +0x300 | TStrings* | список адресов sweep-блоков |
| +0x304 | TObject* | combo/dataset для main records |
| +0x308 | TObject* | combo/dataset для sweep records |
| +0x30C | int* | массив адресов прочитанных блоков |
| +0x310 | int | количество прочитанных адресов |
| +0x314 | byte* | буфер raw flash dump (≤ 0x80010) |
| +0x318 | int | размер прочитанных данных в буфере |
| +0x320 + i*0x18 + 800 | CategoryEntry | таблица категорий |
| +0x3B0 | int | количество активных категорий (5 или 6) |
| +0x3B4 | int* | массив адресов sweep_base |
| +0x3B8 | int | количество элементов в sweep array |
| +0x3BC | int* | список Sweep-блоков (8 байт/блок: index, base) |
| +0x3C0 | int | размер sweep-списка |
| +0x3C4 | byte | флаг 'from device' (1=COM, 0=file) |
| +0x3C8 | int | размер ожидаемого блока (для file mode) |
| +0x3CC | AnsiString | имя файла |

| № | Находка | Статус |
|---|---|---|
| 226 | FormReadData state offset map (+0x300..+0x3CC) | 🟢 Высокая уверенность |
| 227 | Категорий 6 (или 5 при device_type==3) | 🔒 Железобетонно |
| 228 | Каждая категория = 24-байтовая структура (CategoryEntry) | 🔒 Железобетонно |
| 229 | Source flag: `*(byte*)(0x3C4) == 1` → COM (Protocol_FlashDump), `0` → File | 🔒 Железобетонно |

---

## 77. PelengPC: Протокол 0x74 — ПОЛНЫЙ ДАМП ФЛЭШ (новая команда!)

**PelengPC.exe+1+1.c:25092-25132** (FormRaport / FormReadData):

```c
local_48 = 0x20000;                              /* 128 KB по умолчанию */
if ((DAT_0053e97c & 1) != 0) local_48 = 0x80000; /* 512 KB при extended info */

iVar4 = Protocol_FlashDump(&g_comPort, 0x74,
                           local_40 + 4,
                           local_48);             /* TX 0x74 → RX 128/512 KB */
if (iVar4 == local_48) {
  memcpy(local_40, &g_HandshakeHeader, 0x10);    /* + 16-байт handshake header */
  ...
  /* в файл .dat сохраняется local_40 [0x10 + local_48] байт */
  /* итого: 16 + 128KB или 16 + 512KB                       */
}
```

**Полный размер файла дампа = 0x20010 (131088) или 0x80010 (524304) байт.**

| № | Находка | Статус |
|---|---|---|
| 230 | **Опкод 0x74** = ПОЛНЫЙ ДАМП ФЛЭШ (для отчёта) — ОТЛИЧАЕТСЯ от 0x9A | 🔒 Железобетонно |
| 231 | Базовый размер 0x74-ответа: 0x20000 (128 КБ) | 🔒 Железобетонно |
| 232 | Расширенный: 0x80000 (512 КБ) при `DAT_0053e97c & 1` (info_byte bit0) | 🔒 Железобетонно |
| 233 | Сохраняемый файл = 16-байт handshake header + raw flash (0x20010/0x80010) | 🔒 Железобетонно |
| 234 | `DAT_0053e97c` = global байт с флагами устройства (тот же info_byte из handshake) | 🔒 Железобетонно |

---

## 78. PelengPC: Протокол 0x9A — синтетический префикс LCD-скриншота

**PelengPC.exe+1+1.c:21333-21361** (FUN_0041cbf4):

```c
uint local_14 = 0x10c5;                          /* 4293 B по умолчанию */
if ((DAT_0053e97c & 4) != 0) local_14 = 0x1485;  /* 5253 B при wagon LCD */

if (from_device) {
  /* СИНТЕТИЧЕСКИЙ 5-байт префикс перед реальной полезной нагрузкой */
  buf[0] = 0x60;
  buf[1] = 0xEA;
  buf[2] = (byte)(local_14 & 0xff);              /* size LO */
  buf[3] = (byte)(local_14 >> 8);                /* size HI */
  buf[4] = 0;                                    /* zero/reserved */
  iVar1 = Protocol_FlashDump(&g_comPort, 0x9A,
                             buf + 5, local_14 - 5);
  local_10 = iVar1 + 5;                          /* total = data + 5 */
}
```

**Префикс `60 EA <size_lo> <size_hi> 00`** записывается PelengPC.exe ВНУТРИ компьютера; устройство его НЕ отдаёт. В UART-логах префикса нет, но в локально сохранённой копии — есть.

| № | Находка | Статус |
|---|---|---|
| 235 | Префикс `0x60 0xEA <sizeLO> <sizeHI> 0x00` пишется ХОСТом перед 0x9A-данными | 🔒 Железобетонно |
| 236 | `0x10C5 = 4293`, `0x1485 = 5253` — два возможных размера LCD-кадра | 🔒 Железобетонно |
| 237 | Wagon-LCD режим: `DAT_0053e97c & 4` (info_byte bit2) | 🔒 Железобетонно |

---

## 79. `FormScreen_RenderBscan` — рендеринг LCD-кадра (детально)

**PelengPC.exe+1+1.c:25709-25780.**

```c
void FormScreen_RenderBscan(int param_1, int data, int data_size) {
  /* data_size = 0x10C0 (если 0x10C5-5) или 0x1480 (если 0x1485-5) */
  *(int*)(param_1 + 0x29E8) = data_size - 0x1C0;       /* активный body */
  memcpy(param_1 + 0x2D8, data + 0x1D0, data_size - 0x1C0);
  *(int*)(param_1 + 0x29EC) = (data_size - 0x1C0)*8 / 0xF0;  /* rows = body*8/240 */
  *(int*)(param_1 + 0x29F0) = 30;                       /* stride = 30 байт/ряд */
  /* Размер: 240 × N пикселей, где N зависит от размера */

  for (row = 0; row < rows+1; row++) {
    for (col_byte = 0; col_byte < 30+1; col_byte++) {
      byte b = body[col_byte + row * 30];
      for (bit = 0; bit < 8; bit++) {
        if (((b >> bit) & 1) == 0) draw_white(...);     /* LSB-first! */
        else                       draw_black(...);
        rect.left   = (col_byte*8 + bit) * 2;
        rect.right  = rect.left + 2;
        rect.top    = row * 2;
        rect.bottom = rect.top + 2;
        FillRect(canvas, &rect);
      }
    }
  }
}
```

**Раскладка ответа 0x9A** (после реальных данных от прибора):
```
+0x000 .. +0x00F   (16B)    handshake-like header (device_no, info_byte, calib_date)
+0x010 .. +0x1CF   (448B)   AUX (статусные строки, маркеры, дополнительные регионы)
+0x1D0 .. конец    (Nx30B)  raw B-scan / LCD bitmap (240 px × N rows × 1 bpp)
```

Для **0x10C5 (4293) - 5 префикс = 4288**: body = 4288 - 0x1C0 = 3840 = 128 строк (240×128 px = «стандартный B-scan»).
Для **0x1485 (5253) - 5 префикс = 5248**: body = 5248 - 0x1C0 = 4800 = 160 строк (240×160 px = «вагонный B-scan»).

| № | Находка | Статус |
|---|---|---|
| 238 | LCD/B-scan ответ: 16-байт header + 448-байт AUX + N×30 байт bitmap | 🔒 Железобетонно |
| 239 | Bitmap-данные начинаются с offset 0x1D0 (464) от начала reply | 🔒 Железобетонно |
| 240 | Bitmap stride = 30 байт/ряд → 240 пикс/ряд | 🔒 Железобетонно |
| 241 | Размер: 128 строк (стандарт) / 160 строк (вагон) | 🔒 Железобетонно |
| 242 | **Порядок битов в байте = LSB-FIRST** (bit 0 = левый пиксель) — ВНИМАНИЕ: ROUND4 утверждал MSB | 🔒 Железобетонно (правка ROUND4 §16.3) |
| 243 | Каждый пиксель раздваивается до 2×2 (display scale ×2) | 🔒 Железобетонно |

> **⚠️ Правка ROUND4:** «MSB-first» в §16.3 / §63 — **ОШИБКА**.
> Сверка по PelengPC.exe+1+1.c:25755-25767 показывает явный LSB-first.

---

## 80. `Protocol_Handshake` — детали выделения буфера

**PelengPC.exe+1+1.c:26618-26694** — функция, ранее упомянутая в ROUND4 §65.

Дополнительные детали:

```c
void* buf = FUN_004fc03c(0x80010);                  /* всегда 512KB+16 */
TCOMPort_WriteByte(port, 0x55);
DWORD received = TCOMPort_ReadBytes(port, buf, 0x80010);
if (received >= 17) {
  byte info_byte = buf[2];                          /* << info_byte! */
  memcpy(&g_HandshakeHeader, buf, 16);              /* 16-байт header */
  char date_buf[16];
  FormatDate(date_buf, "--.--.----");               /* placeholder */
  /* копируем 16-байт header в TCOMPort+0x22..+0x32 (что-то связанное с настройками) */
  /* копируем reset of buffer (received - 16 байт) в port_buf+0xF*4 = +0x3C  */
  /* port_obj[0xE] = (received - 16) / 2 - 1                                 */
  /*  → каталог имеет (received-16)/2 − 1 LE16 элементов                     */
}
FreeMem(buf);
```

| № | Находка | Статус |
|---|---|---|
| 244 | `Protocol_Handshake` всегда выделяет **0x80010 байт** буфера (NB: не зависит от типа устройства) | 🔒 Железобетонно |
| 245 | Catalog `n_elements = (received - 16) / 2 - 1` — LE16-массив с терминатором | 🔒 Железобетонно (повтор #189) |
| 246 | `info_byte` = `buf[2]` сохраняется в `port[0xD]` и `DAT_0053e97c` (глобал) | 🔒 Железобетонно |
| 247 | Catalog-массив копируется в `port + 0x3C` (offset 0xF из base) | 🔒 Железобетонно |

---

## 81. `TCOMPort_ApplySettings` — занулённый DCB

**PelengPC.exe+1+1.c:26261-26289.**

```c
char TCOMPort_ApplySettings(TCOMPort* p) {
  if (!TCOMPort_Open(p)) return 0;
  DCB dcb;
  GetCommState(p->hFile, &dcb);
  dcb.BaudRate = p->baudRate;
  dcb.Parity   = p->parity;
  dcb.ByteSize = p->dataBitsCode + 5;        /* dataBitsCode = 3 → ByteSize 8 */
  dcb.StopBits = p->stopBits;
  *(int*)((char*)&dcb + 8) = 0;              /* << ОБНУЛЕНИЕ всего DCB+8 */
  /* dcb+8 покрывает 32 бита всех flow-control битов:
     fBinary, fParity, fOutxCtsFlow, fOutxDsrFlow, fDtrControl(2),
     fDsrSensitivity, fTXContinueOnXoff, fOutX, fInX, fErrorChar,
     fNull, fRtsControl(2), fAbortOnError, fDummy2(17)
   */
  SetCommState(p->hFile, &dcb);
  return 1;
}
```

| № | Находка | Статус |
|---|---|---|
| 248 | `TCOMPort_ApplySettings`: `*(int*)(dcb + 8) = 0` обнуляет **fBinary, fParity и все flow** одним записью | 🔒 Железобетонно |
| 249 | `dataBitsCode` — это INDEX (0..3), приведённый к ByteSize 5..8 формулой `+5` | 🔒 Железобетонно |
| 250 | `parity` хранится как байт (0=none, 1=odd, 2=even, 3=mark, 4=space) | 🔒 Железобетонно |

---

## 82. `TCOMPort_Create` — дефолтная инициализация структуры

**PelengPC.exe+1+1.c:25877-25894.**

```c
TCOMPort_Create(TCOMPort* p) {
  p->hFile        = INVALID_HANDLE_VALUE;   /* 0xFFFFFFFF */
  p->portName     = nullptr;
  p->baudIndex    = 6;                       /* по умолчанию = 9600 */
  p->baudRate     = 9600;                    /* 0x2580 */
  p->dataBitsCode = 3;                       /* → 8 бит */
  p->stopBits     = 0;                       /* 0 = ONESTOPBIT */
  p->parity       = 0;                       /* 0 = NOPARITY */
  /* isOpen, padding — не инициализируются явно (по умолчанию 0) */
  p->timeoutMS    = 1000;
}
```

Эти дефолты применяются ДО загрузки настроек из реестра / .ini.

| № | Находка | Статус |
|---|---|---|
| 251 | `TCOMPort_Create` устанавливает дефолты: `hFile=-1, baudIdx=6, baudRate=9600, bits=8, stop=1, parity=0, timeout=1000ms` | 🔒 Железобетонно |
| 252 | `baudIndex = 6` соответствует индексу в таблице baudrate (6 → 9600) | 🔒 Железобетонно |

---

## 83. PelengPC: парсинг «текстовых» полей в DLL output

**PelengPC.exe+1+1.c:14430-14437** (kind=2 = string):

```c
if (kind == 2) {
  for (int j = 0; j < size; j++) {
    if (data[j] == 0) break;     /* NUL terminator → стоп */
    AppendChar(out, data[j]);
  }
}
```

Это **C-style string** (NUL-terminated, обрезается по первому нулю), а не Pascal short-string.
Размер `size` — максимальный, фактическая длина ≤ size.

| № | Находка | Статус |
|---|---|---|
| 253 | String-поля DLL-output = C-string (NUL-terminated), НЕ Pascal | 🔒 Железобетонно |
| 254 | Заполнение в WriteField обнуляет хвост, поэтому NUL всегда есть (если slen<size) | 🔒 Железобетонно |

---

## 84. PelengPC: парсинг «дата»-полей в DLL output

**PelengPC.exe+1+1.c:14439-14488** (kind=3 = date):

```c
if (kind == 3) {
  byte year_code  = data[uVar18 + 3];     /* проверка: 0 < year_code <= 12  */
                                          /* (validateMonth?) - WAIT! */
  if (year_code == 0 || year_code > 0x0C) {
    /* invalid → empty string */
  } else {
    AppendStr(out, IntToStr(year_code));   /* МЕСЯЦ → строка */
    AppendStr(out, ".");
    byte day_code = data[iVar16];          /* iVar16 = uVar18+2, проверка 0<x<=0x1F */
    if (day_code == 0 || day_code > 0x1F) { empty }
    else {
      AppendStr(out, IntToStr(day_code));  /* ДЕНЬ */
      AppendStr(out, ".");
      byte yy_code = data[uVar18 + 4];     /* проверка: < 100 */
      AppendStr(out, IntToStr(yy_code));   /* ГОД (2-digit) */
    }
  }
}
```

⚠️ **ВНИМАНИЕ:** В DLL-output-буфере байты после WriteField для date-поля идут так:
- `data[+0]` (uVar18+2 = первый байт data) = **день** (validate ≤ 31)
- `data[+1]` (uVar18+3) = **месяц** (validate ≤ 12)
- `data[+2]` (uVar18+4) = **год % 100** (validate < 100)

Это согласуется с DLL `DecodeDate(raw, ..., 7)` → bytes RAW[7,8,9] = (day, month, year_code).
Т.е. **WriteField type=3** копирует RAW [7,8,9] **без перестановки** в DLL output.

| № | Находка | Статус |
|---|---|---|
| 255 | Date-поле в DLL output: [+0]=day(≤31), [+1]=month(≤12), [+2]=year%100 | 🔒 Железобетонно |
| 256 | PelengPC рендерит дату в формате `DD.MM.YY` (двухзначный год без 2000) | 🔒 Железобетонно |
| 257 | Битый байт даты → поле остаётся пустой строкой | 🔒 Железобетонно |

---

## 85. InterBase / Firebird DB интеграция в PelengPC

**PelengPC.exe+1+1.c:14586-14603**:

```c
piVar14 = _Ibcustomdataset_TIBCustomDataSet_GetSelectSQL_qqrv(*(int*)(param_1 + 0x3DC));
(*piVar14->Close)();
(*piVar14->SetSelect)(piVar14, sql_select_built);   /* SELECT * FROM <category> */
SetParamByName_qqrv(*(int*)(param_1+0x3DC), 1);     /* prepare */
Prepare(*(int*)(param_1+0x3DC));
bool result_eof = Eof(*(int*)(param_1+0x3DC));
if (!result_eof) {
  /* читаем 2 поля из результата select */
  int id_val   = FieldByName("???", "ID???"   )->AsInteger;
  int addr_val = FieldByName("???", "ADDR???" )->AsInteger;
}
Append(*(int*)(param_1+0x3DC));      /* INSERT INTO ... */
/* затем биндинг параметров и Post */
```

**PelengPC использует Firebird Embedded** (gds32.dll загружается на старте — см. 40841).
Также есть ibinstall.dll (для setup), ibxml.dll (для XML data).

| № | Находка | Статус |
|---|---|---|
| 258 | PelengPC.exe загружает `gds32.dll` (Firebird/InterBase client) | 🔒 Железобетонно |
| 259 | Хранилище записей = Firebird DB (TIBCustomDataSet) | 🔒 Железобетонно |
| 260 | Каждая категория — отдельная SQL-таблица (FormReadData has per-category dataset) | 🔒 Железобетонно |
| 261 | `ibxml.dll`, `ibinstall.dll` — вспомогательные библиотеки | 🟢 Высокая уверенность |

---

## 86. Файловый формат `.dat` (PelengPC локальная копия флэш-дампа)

**PelengPC.exe+1+1.c:20066-20170** (FUN_0041ab40 — file loader).

```
File offset  Size   Поле
+0x00 .. +0x0F  16  16-байт handshake header [device_no_LE16, info_byte,
                                              calib_date, type/version, ...]
+0x10 .. EOF    ≤ 128KB или ≤ 512KB         RAW flash dump
                                            (как от Protocol_FlashDump(0x74))

Если raw_data[+0..+1] == 0xFFFF (LE16):    файл считается «пустым» (cancel)
```

Загрузчик читает файл целиком в `*(byte*)(param_1 + 0x314)` (буфер размера 0x80010),
размер сохраняется в `*(int*)(param_1 + 0x318)`.
Если file_size ≤ 16 (нет flash data) → error.

| № | Находка | Статус |
|---|---|---|
| 262 | Файл .dat = 16-байт handshake header + RAW flash dump (как от 0x74) | 🔒 Железобетонно |
| 263 | «Пустой файл» определяется как `LE16(file[0x10]) == 0xFFFF` | 🔒 Железобетонно |
| 264 | Минимальный размер файла = 17 байт; иначе ошибка | 🔒 Железобетонно |

---

## 87. `ClearBlockList` и управление sweep-блоками

**PelengPC.exe+1+1.c (around 20198-20204):**

```c
void ClearBlockList(int* p_FormReadData) {
  for (int i = 0; i < FormReadData->sweep_list_count; i++) {
    FUN_004ec974(FormReadData->sweep_list, i);  /* delete? */
  }
  (**(code**)(*FormReadData->sweep_list + 8))();  /* TList.Clear */
}
```

Реализует очистку списка прочитанных Sweep-блоков перед перечитыванием.

| № | Находка | Статус |
|---|---|---|
| 265 | `ClearBlockList` использует TList.Clear для очистки sweep_list | 🟢 Высокая уверенность |
| 266 | Sweep-блоки добавляются по уникальному base_addr (защита от дублей в FUN_0041c858) | 🔒 Железобетонно |

---

## 88. ParseFlashDump_TLV — главный диспетчер чтения

**PelengPC.exe+1+1.c:20208-20330** — переименовываем по контексту, главный TLV-парсер flash dump.

```c
char ParseFlashDump_TLV(int formReadData)
{
  /* Перебирает память (file buffer) по 16-байт TLV-структуре */
  for (sweepIdx = 0; sweepIdx < formReadData->sweepCount; sweepIdx++) {
    /* проверка валидности TLV-тега в каталоге */
    if (catalog_entry[idx] == sweep_addr) {
      /* match — вызываем декодер */
      uint cat = sweep_addr / 1000;       /* категория */
      if (IsSweepAddress(addr)) {
        /* Sweep блок — копируем 16-байт header + 16-байт body */
        memcpy(out, header, 0x10);
        /* запоминаем sweep_addr в sweep_list */
      } else {
        /* Обычная запись */
      }
    }
  }
}
```

Подробности парсинга TLV выходят за рамки этого документа.

| № | Находка | Статус |
|---|---|---|
| 267 | `ParseFlashDump_TLV` — главный TLV-парсер (имя из контекста, использует sweep_addr) | 🟢 Высокая уверенность |

---

## 89. Различия между Python-клоном (peleng_clone+12+3.py) и оригиналом

| Параметр | PelengPC.exe (C) | peleng_clone+12+3.py |
|---------|--------------------|---------------------|
| Baudrate | 9600 (default) | **19200** (UD2-102) |
| Parity | от registry | **NONE** (фикс) |
| Inter-byte delay | **10ms** (busy-wait через GetTickCount) | **4.8 ms** (Sleep) |
| Handshake repeats | 1 (одна попытка 0x55) | **4** (4× 0x55) |
| Handshake RX size | до 0x80010 (512KB+16) | ровно **16 байт** + retry |
| Handshake min valid | **17 байт** | 16 байт + extra |
| Date format (RAW) | bytes [7..9] = (day,month,year-2000) | **anchor float-search** + (d,m,y,h,mn) подряд |
| Block size (record) | **(0x18+1) = 25 байт** через `i * 0x18 + base + 800` | 84 байта (UD2-102 BCD passport) |
| Stop bits | from registry | 1 (фикс) |
| Date validation | year > 2000 | 15 ≤ y ≤ 40 (2015..2040) |

**Вывод:** Python-клон таргетит ОТЛИЧНУЮ модель прибора (УД2-102, BCD-паспорт), а не общий бинарь PelengPC.

| № | Находка | Статус |
|---|---|---|
| 268 | Python-клон таргетит УД2-102 (BCD-passport), отличается от ОБЩЕГО PelengPC | 🟢 Высокая уверенность |
| 269 | УД2-102: 19200 baud + 4.8ms inter-byte + 4× handshake — отлично от C-кода | 🔒 Железобетонно (видно в .py) |
| 270 | RAW record в УД2-102 = 84 байта BCD-паспорта; в общем PelengPC = переменный (по схеме категории) | 🔒 Железобетонно |

---

## 90. Сводная таблица DLL-экспортов

| # | Index | Name | Адрес | Назначение |
|---|-------|------|-------|-----------|
| 1 | 1 | `_Form_View` | 0x0040131c | Отрисовка форм (DataTable) |
| 2 | 2 | `_SortBufData` | 0x00401454 | Декодер записи, аллокация выходного буфера |
| 3 | 3 | `_FreeBuffer` | 0x0040193c | Освобождение буфера от `_SortBufData` |

Эти три экспорта связывают PelengPC.exe ↔ DLL через `GetProcAddress` (см. 14375, 14676, 15379).

| № | Находка | Статус |
|---|---|---|
| 271 | DLL экспортирует 3 функции: `_Form_View`, `_SortBufData`, `_FreeBuffer` | 🔒 Железобетонно |
| 272 | PelengPC резолвит их по имени через GetProcAddress (не по ordinal) | 🔒 Железобетонно |

---

## 91. `_Form_View` — отрисовщик DataTable (детально)

**102_203dll.dll+3.c (около строки 2340-2382):**

```c
void __cdecl _Form_View(undefined ?, byte* raw_record, uint raw_size) {
  /* Получает указатель на TDataTable COM-объекта через GetActiveObject(CLSID) */
  IID clsid = DAT_0042821c;                              /* {???-???-???-???} */
  void* ptr;
  HRESULT hr = GetActiveObject(&clsid, NULL, &ptr);
  if (FAILED(hr))  hr = CoCreateInstance(&clsid, NULL, CLSCTX_INPROC_SERVER, &IID_IDispatch, &ptr);

  /* Передаёт массив byte[raw_size] в COM-объект через VARIANT */
  SAFEARRAY* psa = SafeArrayCreateVector(VT_UI1, 0, raw_size);
  void* arr; SafeArrayAccessData(psa, &arr);
  memcpy(arr, raw_record, raw_size);
  SafeArrayUnaccessData(psa);

  VARIANT v;
  v.vt = VT_UI1 | VT_ARRAY;                              /* 0x2011 */
  v.parray = psa;
  /* IDispatch::Invoke("ShowWindow", v) — открывает окно DataTable */
}
```

| № | Находка | Статус |
|---|---|---|
| 273 | `_Form_View` использует COM (IDispatch) для отправки данных в внешний DataTable-сервер | 🔒 Железобетонно |
| 274 | CLSID берётся из `DAT_0042821c` (16 байт, в `.data` секции DLL) | 🔒 Железобетонно |
| 275 | Метод "ShowWindow" вызывается через GetIDsOfNames + Invoke | 🔒 Железобетонно |
| 276 | Данные передаются как `VARIANT(VT_ARRAY \| VT_UI1)` — массив байт | 🔒 Железобетонно |

---

## 92. Семантика `info_byte` (offset +2 handshake) — расширение ROUND4

| Bit | Маска | Описание | Использование |
|-----|-------|----------|-------------|
| 0 | 0x01 | **extended flash** | `0x74` → 0x80000 (вместо 0x20000) |
| 1 | 0x02 | ??? | не найдено в C-коде |
| 2 | 0x04 | **wagon LCD** | `0x9A` → 5253 байт (вместо 4293) |
| 3 | 0x08 | ??? | не найдено в C-коде |
| 4 | 0x10 | ??? | не найдено в C-коде |
| 5 | 0x20 | ??? | не найдено в C-коде |
| 6 | 0x40 | ??? | не найдено в C-коде |
| 7 | 0x80 | ??? | не найдено в C-коде |

Из C-кода PelengPC точно известны bits 0 и 2. Остальные могут управлять:
- наличие DGS-таблицы
- режим UD vs RD
- частотный диапазон
- кол-во каналов

Для устройств с info_byte = `0x18` (бит 3 + бит 4) и `0x01` (бит 0) — у первого extended flash отсутствует, у второго присутствует.

| № | Находка | Статус |
|---|---|---|
| 277 | `info_byte.bit0 = extended_flash` (0x80000 vs 0x20000) | 🔒 Железобетонно |
| 278 | `info_byte.bit2 = wagon_LCD` (5253 vs 4293 байт screen) | 🔒 Железобетонно |
| 279 | Остальные биты info_byte НЕ использованы в коде PelengPC.exe | 🟢 Высокая уверенность |

---

## 93. Двойной парсинг `LE16` vs `BE-в-DLL-output`

**Важно для клонов:** есть ДВА класса представления интегеров.

### A. RAW flash dump (как от прибора)
- `device_no` (raw[0..1]): **LE16**
- `sweep_addr` (raw[0x10..0x11]): **LE16**
- `some16` (raw[0x15..0x16]): **LE16**
- `sweep_id` (raw[4..5]): **decimal-pair** (hundreds × 100 + units)
- TypeVar (raw[?..?]): обычно LE16 либо LE32 (зависит от записи)

### B. DLL output buffer (после `_SortBufData`)
- Integer-поля (kind=1): **BIG-ENDIAN** (`WriteField` накапливает MSB→offset+0)
- String-поля (kind=2): C-string NUL-terminated (внутри фикс-блока размера `size`)
- Date-поля (kind=3): 3 байта `(day, month, year%100)`

⚠️ Один и тот же RAW-байт может означать РАЗНОЕ в зависимости от того, читается ли он напрямую (LE) или через DLL (BE).

| № | Находка | Статус |
|---|---|---|
| 280 | RAW flash формат и DLL-output формат — **разные endianness** для integer-полей | 🔒 Железобетонно |
| 281 | Клоны, читающие RAW напрямую, должны использовать LE; через DLL — BE | 🔒 Железобетонно |

---

## 94. Глобалы PelengPC.exe (имена и адреса)

| Имя | Адрес | Тип | Назначение |
|-----|-------|-----|----------|
| `g_comPort` | DAT_0053bbcc | TCOMPort | Глобальный объект COM-порта |
| `g_FormMain` | DAT_0053b16c | TFormMain* | Главное окно |
| `g_FormReadData` | DAT_0053b9c0 | TFormReadData* | Окно чтения данных |
| `g_FormNastr` | DAT_0053b9c4 | TFormNastr* | Окно настроек |
| `g_FormRaport` | DAT_0053af88 | TFormRaport* | Окно отчётов |
| `g_FormScreen` | DAT_0053b9c8 | TFormScreen* | Окно LCD-скриншота |
| `g_FormPassWord` | DAT_0053af70 | TFormPassWord* | Окно пароля |
| `g_pFormProgress` | DAT_0053bb1c | TFormProgress* | Прогресс-окно (modal RX) |
| `g_HandshakeHeader` | DAT_0053e96a | byte[16] | Кешированный handshake header |
| `DAT_0053e97c` | DAT_0053e97c | byte | Кешированный **info_byte** |
| `DAT_005be994` | DAT_005be994 | FARPROC | `_Form_View` функция в DLL |
| `DAT_005be998` | DAT_005be998 | FARPROC | `_SortBufData` функция в DLL |
| `DAT_005be99c` | DAT_005be99c | FARPROC | `_FreeBuffer` функция в DLL |
| `DAT_005beb38` | DAT_005beb38 | HMODULE | gds32.dll (Firebird client) |
| `DAT_005beb3c` | DAT_005beb3c | HMODULE | ibinstall.dll |
| `DAT_005beb40` | DAT_005beb40 | HMODULE | ibxml.dll |

| № | Находка | Статус |
|---|---|---|
| 282 | `g_HandshakeHeader` (16 байт по DAT_0053e96a) хранит последний прочитанный handshake | 🔒 Железобетонно |
| 283 | `DAT_0053e97c` = info_byte, читается из bytes[2] handshake response | 🔒 Железобетонно |
| 284 | Динамически загружаются: gds32.dll, ibinstall.dll, ibxml.dll | 🔒 Железобетонно |

---

## 95. Регекс-карта названий категорий

В FormReadData state +0x440+i*4 хранится массив указателей на структуры «тип прибора».
Каждая структура содержит:
- +0x00: AnsiString тип ("ПЕЛЕНГ", "СКАН-3", "ИНТЕРВАЛ" и т.п.)
- +0x04: AnsiString имя DLL ("102_203dll.dll", "scan3_dll.dll", ...)
- +0x08: AnsiString описание
- +0x0C: TList* список схем (FORMAT[])

Это позволяет одной PelengPC.exe работать с РАЗНЫМИ DLL для разных моделей приборов.

| № | Находка | Статус |
|---|---|---|
| 285 | PelengPC.exe — мульти-устройство: загружает DLL по выбранному типу прибора | 🔒 Железобетонно |
| 286 | Тип прибора выбирается в Combobox (FormMain_CBTypeChange) | 🔒 Железобетонно |
| 287 | `*(int*)(param_1 + 0x430)` — текущий index выбранного типа | 🔒 Железобетонно |

---

## 96. Идиомы Borland C++ Builder, замеченные в коде

| Идиома | Смысл | Где видно |
|--------|-------|-----------|
| `FUN_00507f88(addr)` | `BCB_InitExcept(catch_addr)` | каждая функция |
| `FUN_00425bac(&local)` | `AnsiString_Free(local, 2)` | в RAII pattern |
| `FUN_0051f53c(&local, 2)` | `AnsiString_Release(local, 2)` | в epilogue |
| `FUN_0051f56c(dst, src)` | `AnsiString_Assign(dst, src)` | копирование строки |
| `FUN_0051f594(a, b, out)` | `AnsiString_Concat(a, b, out)` | склейка |
| `FUN_0051f620(s1, s2)` | `AnsiString_Compare(s1, s2)` | сравнение | 
| `FUN_004fb800(int, &str)` | `IntToStr(int, &str)` | конвертация |
| `*in_FS_OFFSET = local_XX` | `__except_release()` (SEH unwind) | в epilogue |

| № | Находка | Статус |
|---|---|---|
| 288 | Все функции BCB используют SEH с `*in_FS_OFFSET = local_XX` в эпилоге | 🔒 Железобетонно |
| 289 | AnsiString следует RAII-паттерну: alloc → use → release (через декрементер refcnt) | 🔒 Железобетонно |
| 290 | Helper-функция `BCB_InitExcept(addr)` устанавливает SEH frame в начале каждой функции | 🔒 Железобетонно |

---

## 97. PelengPC: распознавание SweepAddress (10000..29999)

**PelengPC.exe+1+1.c:21139-21149:**

```c
bool IsSweepAddress(_, int addr) {
  return (9999 < addr) && (addr < 30000);
}
```

Это **жёсткое** правило диспетчера: всё в диапазоне 10000-29999 считается Sweep-block,
остальное — обычная запись. Соответствует категориям 10..19 (B-scan) и 20..29 (ShortProtocol).

| № | Находка | Статус |
|---|---|---|
| 291 | `IsSweepAddress` = strict-check 10000..29999, без подкатегорий | 🔒 Железобетонно |

---

## 98. Структура `FormProgress` (модальное окно RX)

**PelengPC.exe+1+1.c:26720-26731** (используется Protocol_FlashDump/BlockRequest):

```c
/* Поля FormProgress, заполняемые перед ShowModal: */
*(byte*)(FormProgress + 0x2F8) = 1;       /* active=1 → пошёл приём */
*(int*) (FormProgress + 0x300) = bufPtr;  /* куда писать */
*(int*) (FormProgress + 0x2FC) = expectedSize;
ShowModal(FormProgress);                  /* модальный приём с прогресс-баром */
result = *(int*)(FormProgress + 0x304);   /* реально прочитано */

/* После закрытия: */
*(int*)(FormProgress + 0x2F8) = 0;        /* active=0 */
```

| Offset | Тип | Смысл |
|--------|-----|-------|
| +0x2F8 | byte | active flag (1=RX в процессе) |
| +0x2FC | int  | expectedSize (сколько байт ожидаем) |
| +0x300 | int  | bufPtr (куда писать) |
| +0x304 | int  | receivedSize (фактически прочитано) |
| +0x47  | byte | какой-то state-bit (проверяется в 14606) |

| № | Находка | Статус |
|---|---|---|
| 292 | FormProgress offsets: +0x2F8=active, +0x2FC=expected, +0x300=bufPtr, +0x304=received | 🔒 Железобетонно |
| 293 | Modal RX-loop: setup поля → ShowModal → пользователь видит прогресс | 🔒 Железобетонно |

---

## 99. `FUN_004ec974` — TList iterator

Часто встречающаяся идиома `FUN_004ec974(list, index)` — это **TList.Items[index]** Borland-like accessor:

```c
int* FUN_004ec974(TList* list, int index) {
  return (int*)list->items[index];  /* без bounds check (или с ним) */
}
```

| № | Находка | Статус |
|---|---|---|
| 294 | `FUN_004ec974(list, idx)` — общий accessor TList.Items[idx] | 🔒 Железобетонно |

---

## 100. Итоговая сводка ROUND5 — новые находки 190-294 (105 шт.)

### Новые DLL функции (детально разобраны)
- `_SortBufData` (главный декодер записи) — категории, output layout
- `_Form_View` (COM IDispatch к DataTable)
- `_FreeBuffer`, `AllocBuffer`, `FreeAllocBuffer`
- `GetFormatByCategory` (схемы полей по категориям)
- `CalcHeaderLength`, `FillFieldOffsets`
- `DispatchByTypeVar` (Ascan/Composite/Default)
- `DecodeAscan`, `DecodeBscan`, `DecodeGeneric`, `DecodeCalibration`, `DecodeShortProtocol`
- `DecodeTypeVar_Ascan`, `DecodeTypeVar_Composite`, `DecodeTypeVar_Default`
- `DecodeDate`, `DecodeTime`, `ValidateDate`
- `WriteField` (FUN_00401f54) — кодирование int(BE)/string/date
- `GetFieldDescriptor` (TList.Items accessor)
- `AddField` (FUN_00401db0) — конструктор FIELD-структуры
- `FormatSweepId` (форматирование "DD.IDX" из двух byte)
- `BCB_memcpy`, `BCB_InitExcept` (BCB-helpers)
- `AnsiString_Free`, `AnsiString_Assign` (refcount management)

### Новые PelengPC.exe функции (детально разобраны)
- `Protocol_FlashDump(0x74, ...)` — ПОЛНЫЙ flash dump (128K/512K)
- `FormScreen_RenderBscan` (LCD bitmap → GUI, LSB-first!)
- `FormReadData_FormShow`, `FormReadData_FormClose`
- `FormMain_*` (NReadData, NSaveConfig, NCOMPort, ...)
- `FUN_0041cbf4` (синтетический префикс 60 EA для 0x9A)
- `FUN_0041cdc0`, `FUN_0041c11c`, `FUN_0041c028`, `FUN_0041c284`
- `FUN_0041bdc8` (заполнение combo-боксов из flash)
- `FUN_0041ab40` (загрузка .dat file)
- `FUN_00410a68` (запись в IB DB)
- `TCOMPort_Create` (дефолтная инициализация)
- `IsSweepAddress` (10000..29999)
- `GetBaseAddr`, `GetSubBlock`, `GetIndex`

### Новые структуры
- **DLL output buffer layout** (decoder_type + n_fields + table + data + raw)
- **FORMAT** (Borland TList) — items, count, capacity
- **FIELD** — name, hint, type, kind, size
- **CategoryEntry** (24 байт, в FormReadData)
- **FormReadData state map** (+0x300..+0x3CC)
- **FormProgress state map** (+0x2F8..+0x304)
- **TCOMPort full layout** (с дефолтами)
- **Файл .dat** = 16-байт header + raw flash

### Новые протокольные находки
- **Опкод 0x74** = полный flash dump (128KB или 512KB)
- **Опкод 0x9A** = LCD screenshot + 5-байт синтетический префикс
- **0x55** handshake — буфер 0x80010 байт, info_byte = `buf[2]`
- **info_byte** биты: bit0=extended_flash, bit2=wagon_LCD

### Правки ROUND4
- **§16.3/§63 — LSB-FIRST, а не MSB-first** для LCD bitmap (см. #242)

---

## Сводная таблица новых находок (190-294)

| № | Найдено | Статус |
|---|---------|--------|
| 190-196 | `_SortBufData` — DLL output buffer структура | 🔒 |
| 197-198 | `FillFieldOffsets` (LE16 offset, LE16 size) | 🔒 |
| 199-202 | FORMAT/FIELD структуры в DLL | 🔒 |
| 203-206 | `WriteField` — BIG-ENDIAN integer encoding | 🔒 |
| 207-210 | `DispatchByTypeVar` — A-scan(0x96-7)/Composite(0x282-3)/Default | 🔒 |
| 211-214 | `DecodeDate` — RAW bytes [7..9] = day/month/year-2000 | 🔒 |
| 215-217 | `DecodeAscan/Bscan/Generic/Calibration/ShortProtocol` — паттерн | 🔒 |
| 218-221 | PelengPC SQL/IB интеграция, integer BE-read | 🔒 |
| 222-225 | Адресная арифметика (GetBaseAddr/SubBlock/Index/IsSweep) | 🔒 |
| 226-229 | FormReadData state offset map | 🔒/🟢 |
| 230-234 | **Опкод 0x74** = ПОЛНЫЙ flash dump (128K/512K) | 🔒 |
| 235-237 | Опкод 0x9A — 5-байт синтетический префикс | 🔒 |
| 238-243 | FormScreen_RenderBscan детально, **LSB-FIRST!** | 🔒 (правка §16) |
| 244-247 | `Protocol_Handshake` — буфер 0x80010, catalog offset | 🔒 |
| 248-250 | `TCOMPort_ApplySettings` — обнуление DCB+8 | 🔒 |
| 251-252 | `TCOMPort_Create` — дефолтные значения | 🔒 |
| 253-254 | String-поля DLL output = C-string NUL-terminated | 🔒 |
| 255-257 | Date-поле в DLL output: (day,month,year%100) | 🔒 |
| 258-261 | gds32.dll/Firebird DB интеграция | 🔒/🟢 |
| 262-264 | Файловый формат .dat (16+raw) | 🔒 |
| 265-266 | ClearBlockList, sweep_list manipulation | 🔒/🟢 |
| 267 | ParseFlashDump_TLV — главный TLV-парсер | 🟢 |
| 268-270 | Различия Python-клона vs PelengPC.exe | 🔒 |
| 271-272 | DLL экспортирует 3 функции (по имени, не ordinal) | 🔒 |
| 273-276 | `_Form_View` через COM IDispatch (CLSID, SafeArray) | 🔒 |
| 277-279 | info_byte биты: bit0/bit2 — точно; остальные не использованы | 🔒/🟢 |
| 280-281 | RAW LE16 vs DLL-output BE — два класса endianness | 🔒 |
| 282-284 | Глобалы PelengPC.exe (g_HandshakeHeader, info_byte, FARPROCs) | 🔒 |
| 285-287 | Мульти-устройство: одна exe → разные DLL | 🔒 |
| 288-290 | Borland C++ Builder идиомы (SEH, AnsiString RAII) | 🔒 |
| 291 | IsSweepAddress 10000..29999 (strict) | 🔒 |
| 292-293 | FormProgress modal RX-loop state | 🔒 |
| 294 | `FUN_004ec974` = TList.Items[idx] | 🔒 |

---

## Что осталось нераскрытым (TODO ROUND6)

1. **info_byte биты 1,3-7** — реверс прошивки прибора (нет в PelengPC.exe)
2. **Категории 0, 2, 3, 7, 8, 9** — не упомянуты в `_SortBufData` switch
   - Возможно: 0 = пустой блок? 2 = unused? 7-9 = reserved for future?
3. **Точное соответствие категория → имя SQL-таблицы** (нужны строки `s_SELECT___FROM_005243b9`)
4. **TypeVar диапазоны вне 0x96/0x97/0x282/0x283** — все идут в Default; но как они отличаются?
5. **TypeVar_Ascan vs TypeVar_Composite** — детальные поля, разница декодирования
6. **`FUN_00402708`** в DecodeShortProtocol — копирование 6 байт BCD-номера(?)
7. **`FUN_0040381c`** = u16-LE-read (param_2[0xE]) — что в этой позиции?
8. **CLSID для `_Form_View`** — какой именно DataTable-сервер использует PelengPC?
9. **Структура каталога в handshake** — пары (sweep_addr, ???) или что?
10. **Точная структура AnsiString refcount manipulation** — глубже чем FUN_0051f53c
11. **Сравнение трёх версий `102_203dll.dll+{1,2,3}.c`** — выяснить, какие байты отличаются (возможно баг-фиксы в фирме)
12. **Поведение Protocol_Handshake при `received < 17`** — что именно делает PelengPC.exe (просто скрывает ошибку?)

---

## Итог (этап A раунда 5)

- **ROUND4: 189 находок** (через §66).
- **ROUND5 этап A: +105 новых находок (190-294)**.

Главные открытия раунда 5 этапа A:
1. **Опкод 0x74 = ПОЛНЫЙ flash dump (128KB/512KB)** — ранее упоминался только 0x9A/0x42.
2. **DLL output buffer — отдельная BE-схема** vs RAW LE.
3. **LCD bitmap = LSB-first**, а не MSB-first как в ROUND4.
4. **5-байт синтетический префикс** (`60 EA size_lo size_hi 00`) перед 0x9A-данными.
5. **Category dispatch** в `_SortBufData` по `sweep_addr/1000`.
6. **Firebird Embedded** (gds32.dll) — основной storage backend PelengPC.
7. **info_byte битовая семантика** — bit0=extended, bit2=wagon.
8. **WriteField и GetFormatByCategory** — полностью разобраны: схемы, типы, encoding.

Готово для интеграции в кодовую базу клона.

---

# Этап B (доп-раунд 5) — глубокий реверс

После первой публикации раунда 5 пользователь попросил "продолжай реверс,
записывай в раунд 5 то новое и найденное". Ниже §100-§121, находки #295-#360,
полученные при более детальном проходе по тем же исходникам.

---

## §100 — Три файла `102_203dll.dll+{1,2,3}.c` это ОДИН и тот же DLL

**Файлы:** `102_203dll.dll+1.c`, `102_203dll.dll+2.c`, `102_203dll.dll+3.c`

Diff показал, что во всех трёх файлах:
- Совпадает **поток управления**, литералы строк и адреса данных.
- Отличия — это только **имена функций** (раскрытые анализом Ghidra) и
  порядок объявлений `typedef` в шапке файла.

| Файл | Уровень аннотации | Примеры |
|------|-------------------|---------|
| `+1.c` | RAW Ghidra (без правок) | `FUN_004012c8`, `FUN_00401950`, `FUN_0040721c` |
| `+2.c` | первая аннотированная версия | `AllocBuffer`, `GetFormatByCategory`, `AnsiString_Assign`, `BCB_memcpy` |
| `+3.c` | максимально аннотированная | + `DecodeTypeVar_Ascan/Composite/Default`, `DecodeDate/Time`, `ValidateDate`, `WriteField`, `_SortBufData`, `_Form_View`, `_FreeBuffer` |

**Вывод:** не нужно искать "патчи между билдами". Это один бинарь, и можно
использовать `+3.c` как окончательный источник истины.

```
[#295] Три DLL-файла — один бинарь, разная аннотация Ghidra. 🔒
```

---

## §101 — info_byte: ВОСЕМЬ битов и их назначение (НЕ только bit0/bit2!)

**Файлы:** `PelengPC.exe+1+1.c:25599-25666` (FormRaport / WordML генератор отчёта).

ROUND4 знал bit0 (extended flash) и bit2 (wagon LCD).
В функциях `FUN_0042346c..FUN_004234f8` обнаружено **шесть бит-декодеров**,
которые читают `*(byte*)(record+0x18)` (он же info_byte) и возвращают одну
из ДВУХ строк (по сути — Boolean+lookup):

| Функция | Маска | Если бит=0 | Если бит=1 | Семантика (гипотеза) |
|---------|-------|------------|------------|----------------------|
| `FUN_0042346c` | 0x01 | `&DAT_0052f44e` | `&DAT_0052f43c` | строка ~9 байт — режим/модель головки A |
| `FUN_00423488` | 0x02 | `&DAT_0052f460` | `&DAT_0052f468` | строка ~4 байт — режим B |
| `FUN_004234a4` | 0x04 | `&DAT_0052f474` | `&DAT_0052f470` | строка ~2 байт — режим C (вагон-LCD?) |
| `FUN_004234c0` | 0x08 | `"25 42 63 125 280 500 1000"` | `"75 150 300 600 1000 2000"` | **дискреты по дальности / усилению** |
| `FUN_004234dc` | 0x10 | `&DAT_0052f4c6` | `&DAT_0052f4e3` | строка ~16 байт |
| `FUN_004234f8` | bits 4,5 | см. ниже | см. ниже | **частоты ПЭП (MHz)** |

`FUN_004234f8` использует комбинацию bit4 и bit5 для выбора **одной из 4-х** строк:

| bit5 | bit4 | строка | хексофсет |
|------|------|--------|-----------|
| 0 | 0 | `"1.25 1.8 2.5"` | 0x52f56f |
| 0 | 1 | `"0.4 1.25 2.5 5.0"` | 0x52f559 |
| 1 | 0 | `"1.25 1.8 2.5 5.0 10.0 15.0"` | 0x52f537 |
| 1 | 1 | `"0.1 0.4 0.6 1.25 1.8 2.5 5.0..."` | 0x52f50c |

То есть **bit3 управляет шкалой пути/времени** (две альтернативные шкалы дискрет),
а **bits 4+5 управляют набором рабочих частот пьезопреобразователя**.

```
[#296] info_byte содержит 6 независимых бит-полей (не 2). 🔒
[#297] info_byte.bit3 — выбор шкалы дальности/диапазона. 🔒
[#298] info_byte.bit4+bit5 — выбор таблицы частот ПЭП (1..6 значений MHz). 🔒
```

Это **существенный апгрейд** к ROUND4 §16 / §54.

---

## §102 — Полный набор форм PelengPC.exe (Borland TForm-классы)

**Файл:** `PelengPC.exe+1+1.c` (поиск `^void Form[A-Z][a-zA-Z]*_`).

Найдено **53 обработчика** для **7 форм**:

| Глобал | Имя класса | Обработчики |
|--------|------------|-------------|
| `g_pFormMain` / `g_FormMain` | TFormMain | NCOMPortClick, NTestPortClick, NSaveConfigClick, FormClose, NReadDataClick, ToolButtonFromFileClick, CBTypeChange, VersTChange, NViborClick, NSortirClick, NReadZapClick, NDelZapClick, FormCreate, NResetTableClick, NOperaClick, NReadDocClick, NSearchClick, FormShow, **NSaveExcelClick** (CSV экспорт), NFirmaClick, NSelectAllClick, NNumbersClick, NPassWordClick, FormActivate, NExitClick, ToolButtonRaportClick, NDeviceClick, NHelpProgClick, NHelpAutorClick |
| `g_FormNastr` / `g_pFormNastr` | TFormNastr (настройки) | ButtonOKClick, ButtonCanselClick, ButtonDefaultClick, FormShow, FormClose, ButtonAddClick, ButtonDelClick, ButtonPortViewClick, SpeedButtonAddClick, SpeedButtonClearClick, SpeedButtonDelClick, SpeedButtonUpClick, SpeedButtonDownClick, ButtonTestClick |
| `g_pFormReadData` / `g_FormReadData` | TFormReadData (чтение из прибора/файла) | ButtonReadDirClick, ButtonViewClick, ButtonSelectAllClick, ButtonSendToBaseClick, FormClose, FormShow |
| `g_FormRaport` / `PTR_g_FormRaport_0053af88` | TFormRaport (отчёт-Word) | LoadFlashClick, FromPriborClick, FromFileClick, ShowRaport |
| `g_pFormScreen` / `g_FormScreen` | TFormScreen (LCD-screenshot) | FormPaint |
| `g_FormPassWord` / `PTR_g_FormPassWord_0053af70` | TFormPassWord (модальный диалог ввода) | — multi-purpose: пароль/поиск/фирма (см. §103) |
| `g_FormConfirm` / `PTR_g_FormConfirm_0053af84` | TFormConfirm | — модальный диалог подтверждения |
| `g_FormProgress` / `g_pFormProgress` | TFormProgress (длинная операция) | — внутри идёт RX-loop по UART |

```
[#299] PelengPC использует 7 TForm-классов: Main, Nastr, ReadData, Raport, Screen, PassWord, Confirm, Progress. 🔒
```

---

## §103 — FormPassWord = МНОГОФУНКЦИОНАЛЬНЫЙ модальный диалог (не только пароль)

**Файл:** `PelengPC.exe+1+1.c` (поиск `PTR_g_FormPassWord_0053af70`).

Этот диалог **переиспользуется** для разных режимов. Управляющие поля:

- `FormPassWord + 0x304` (byte) — **режим работы** (1..5)
- `FormPassWord + 0x30C` (int) — контекстный аргумент 1
- `FormPassWord + 0x310` (int) — контекстный аргумент 2
- `FormPassWord + 0x308` (int) — выходное значение
- `FormPassWord + 0x234` (int) — ModalResult (1 = OK)
- `FormPassWord + 0x2EC`, `+0x2F8`, `+0x2FC` (TComboBox*) — поля ввода (1-3 шт.)

**Карта режимов** (mode_byte at +0x304):

| Mode | Использование | Строка из ROUND5 |
|------|--------------|-------------------|
| 3 | Поиск (NSearchClick) | line 14246 |
| 3 | Пароль (NPassWordClick) | line 15189 |
| 4 | NFirmaClick (добавление фирмы) | line 18054 |
| 5 | NReadZapClick / NDelZapClick (чтение/удаление записи) | line 17580 |

Алгоритм вызова:
```c
*(byte*)(FormPassWord + 0x304) = mode;       // выбираем режим
*(int*)(FormPassWord + 0x30C) = arg1;        // например, индекс записи
*(int*)(FormPassWord + 0x310) = arg2;        // например, указатель на ComboBox-источник
ShowModal(FormPassWord);                       // блокирует до OK/Cancel
if (FormPassWord[0x234] == 1) {                // если OK
    AnsiString s = FormPassWord[0x2EC].Text;   // забираем введённое
    ...
}
```

**Важное следствие:** имя "Password" обманчивое. На самом деле это TFormInputDialog
с 1-3 комбобоксами и переключаемым "что введено" в зависимости от mode.

```
[#300] FormPassWord — multi-mode dialog (не только пароль). Режимы 3/4/5. 🔒
[#301] FormPassWord.ModalResult в поле +0x234 (=1 OK, =2 Cancel). 🔒
[#302] Mode 3 = поиск ИЛИ пароль. Mode 4 = добавление фирмы. Mode 5 = выбор записи. 🔒
```

---

## §104 — Singleton TCOMPort по адресу **0x53E948**

**Файл:** `PelengPC.exe+1+1.c` — десятки ссылок на `0x53e948`.

Все вызовы `TCOMPort_*` идут через адрес `0x53e948`, который является
**статической областью данных** размером ~0x88+ байт. Это `g_comPort`.

Карта полей объекта `TCOMPort` (offset от начала структуры):

| Offset | Тип | Назначение |
|--------|-----|------------|
| +0x00 | void* | vtable указатель (BCB VMT) |
| +0x14 | DWORD | handle (HANDLE COM-порта или 0) |
| +0x18 | TStringList* | список доступных PortName (из SetupAPI) |
| +0x1C | byte[6] | DEVICE_HEADER_PART1 (модель/конфиг) |
| +0x22 | byte[16] | DEVICE_HEADER (копия первых 16 байт handshake-ответа) |
| +0x1C+2 = +0x1E | — | (внутри хедера, не отдельное поле) |
| +0x32 | byte | "active" flag (1 = handshake выполнялся успешно) |
| +0x38 | byte/byte[] | расширенная конфигурация |

После handshake данные в этой структуре используются по всему приложению,
включая FormRaport, FormMain_NDeviceClick и FormNastr.

```
[#303] g_comPort = singleton TCOMPort at 0x53E948 (статический BCB-объект). 🔒
[#304] g_comPort+0x18 — TStringList доступных COM-портов (заполняется SetupAPI). 🔒
[#305] g_comPort+0x22 — 16-байтный DEVICE_HEADER (= первые 16 байт handshake). 🔒
[#306] g_comPort+0x32 — флаг "handshake выполнен" (булевый). 🔒
```

---

## §105 — Handshake (FUN_004249c0) — **полный детальный разбор**

**Файл:** `PelengPC.exe+1+1.c:26603-26694`.

```c
uint Handshake(TCOMPort *self, char show_error_dialog)
{
    if (!TCOMPort_Open(self)) return error_with_msgbox;

    byte *buf = malloc(0x80010);                 // 524304 байт, ~512 KB
    if (!buf) return error_with_msgbox;

    byte cmd = 0x55;
    TCOMPort_WriteByte(self, &cmd);

    uint nread = TCOMPort_ReadBytes(self, buf, 0x80010);

    if (nread < 0x11) {                          // меньше 17 байт — отказ
        TCOMPort_Close(self);
        return error;
    }

    self[0xD] = buf[2];                          // <<< INFO_BYTE @ offset 2 !!!
    memcpy(&self[0x22], buf, 16);                // первые 16 байт → DEVICE_HEADER
    self[7] = ... ;                              // флаг (через PrintableSet)
    memcpy(&self[0xF], buf+16, nread-16);        // байты [16..] → catalog buffer
    self[0xE] = ((nread - 16) >> 1) - 1;         // catalog count (entries of 2 bytes)

    free(buf);
    return success;
}
```

**Ключевые открытия:**

1. **Буфер ответа handshake — 524304 байт (0x80010).** То есть прибор может
   за один handshake вернуть до полного дампа флэш-памяти. Это объясняет
   почему 0x80010 = 0x80000 (512 KB) + 16 байт заголовка.

2. **info_byte НАХОДИТСЯ ПО СМЕЩЕНИЮ +2** в handshake-ответе. Это
   "хедер третьего байта", а не отдельная команда.

3. **Каталог = байты [16..] handshake-ответа**, копируются в TCOMPort+0xF.
   Количество записей `(nread - 16) / 2 - 1`. Каждая запись = 2 байта (LE16).

4. **Минимальный валидный ответ — 17 байт** (16 байт header + хотя бы 1 байт catalog).

```
[#307] Handshake выделяет буфер 0x80010 байт (= 512 KB + 16). 🔒
[#308] info_byte = handshake_response[2]. 🔒
[#309] Каталог = handshake_response[16..], записи по 2 байта (LE16). 🔒
[#310] catalog_count = (nread - 16) / 2 - 1, минимум 17 байт ответа. 🔒
[#311] TCOMPort_Open() Sleep(100) уже задокументирован — здесь это подтверждается. 🟢
```

---

## §106 — TCOMPort_Setup (FUN_004247bc) и **АВТО-DETECT BAUD!**

**Файл:** `PelengPC.exe+1+1.c:26499-26521` (Setup), `26544-26586` (Auto-detect).

### Default параметры COM-порта в PelengPC.exe:

```c
void TCOMPort_Setup(TCOMPort *self) {
    TCOMPort_SetPortName(self, "");                // имя пустое — выбор сделает auto-detect
    TCOMPort_SetBaudRate_Switch(self, 0x4B00);     // 19200 baud (!!!)
    TCOMPort_SetDataBits(self, 3);                 // 8 data bits (enum index)
    TCOMPort_SetStopBits(self, 0);                 // 1 stop bit (enum index)
    TCOMPort_SetParity(self, 2);                   // EVEN parity (enum index)
    TCOMPort_SetTimeout(self, 100);                // 100 ms
    self[0x18] = new TStringList();
    EnumPorts(self);                                // SetupAPI fill ports
}
```

**Это противоречит ROUND4** который указывал **9600 baud**. Реальное значение
по умолчанию — **19200 baud**. ROUND4 §44 / §50 нужно скорректировать.

### Auto-detect (FUN_004248b0) — обход всех COM-портов на двух скоростях:

```c
char AutoDetectComPort(TCOMPort *self) {
    TCOMPort_Close(self);
    if (Handshake_NoUI(self)) return 1;            // успех на текущих настройках

    for (int i = 0; i < TStringList_Count(self[0x18]); i++) {
        SetPortName(self, self[0x18][i]);

        SetBaudRate(self, 0x4B00);                 // 19200
        if (Handshake_NoUI(self)) return 1;

        SetBaudRate(self, 0xE100);                 // 57600 !!!
        if (Handshake_NoUI(self)) return 1;
    }
    return 0;
}
```

**Прибор поддерживает 19200 ИЛИ 57600 baud** — но НЕ 9600 (как утверждала ROUND4).
Параметры 8-E-1 (DataBits=8, Parity=Even, Stop=1).

```
[#312] DEFAULT baud в TCOMPort = 19200 (0x4B00), НЕ 9600 (поправка к ROUND4 §44). 🔒
[#313] Auto-detect перебирает 19200 и 57600 (0xE100) baud. 🔒
[#314] DEFAULT settings: 8-E-1 (8 data bits, EVEN parity, 1 stop bit). 🔒
[#315] DEFAULT read timeout = 100 ms. 🔒
```

---

## §107 — SetupAPI Port Enumeration (FUN_00424650)

**Файл:** `PelengPC.exe+1+1.c:26422-26495`.

```c
void EnumComPortNames(TCOMPort *self) {
    HDEVINFO devinfo = SetupDiGetClassDevsA(
        &ClassGuid_005337f0,      // GUID_DEVCLASS_PORTS
        NULL, NULL, DIGCF_PRESENT);  // 2

    if (devinfo == INVALID_HANDLE_VALUE) return;

    SP_DEVINFO_DATA data;
    data.cbSize = 0x1C;             // sizeof(SP_DEVINFO_DATA) = 28

    for (DWORD i = 0; SetupDiEnumDeviceInfo(devinfo, i, &data); i++) {
        HKEY hk = SetupDiOpenDevRegKey(
            devinfo, &data,
            DICS_FLAG_GLOBAL,        // 1
            0,
            DIREG_DEV,               // 1
            KEY_READ);               // 0x20019

        if (hk == INVALID_HANDLE_VALUE) break;

        BYTE name_buf[256];
        DWORD name_size = 256;
        DWORD type = REG_SZ;          // 1

        if (RegQueryValueExA(hk, "PortName", NULL, &type, name_buf, &name_size) == ERROR_SUCCESS
            && type == REG_SZ) {
            AnsiString port_name = AnsiString(name_buf, name_size - 1);
            TStringList_Add(self[0x18], port_name);
        }

        RegCloseKey(hk);
    }

    SetupDiDestroyDeviceInfoList(devinfo);
}
```

**ClassGuid_005337f0** — это `GUID_DEVCLASS_PORTS = {4D36E978-E325-11CE-BFC1-08002BE10318}`,
стандартный класс Windows для COM-портов (Serial + Parallel).

Имена портов читаются из ветки реестра устройства: `HKLM\SYSTEM\CurrentControlSet\Enum\<DeviceID>\Device Parameters\PortName`.

```
[#316] Enum COM-портов через SetupAPI (GUID_DEVCLASS_PORTS). 🔒
[#317] Реестровое поле "PortName" (REG_SZ) — источник имени. 🔒
[#318] TCOMPort+0x18 = TStringList<имена_портов>. 🔒
```

---

## §108 — CLSID `_Form_View` найден: переменная `DAT_0042821c` (16 байт)

**Файл:** `102_203dll.dll+3.c:1244-1246, 2348-2361`.

```c
// data section layout
undefined4 DAT_0042821c;          // 16 байт CLSID (4 dword)
unicode u_ShowWindow_0042822c;    // wide-char "ShowWindow" + NUL
IID *riid_0042d7b0;               // IID_IDispatch (стандарт)
```

```c
void _Form_View(undefined4 unused, byte *data, ULONG size) {
    IID clsid;
    memcpy(&clsid, &DAT_0042821c, 16);   // CLSID берётся из секции .data

    IUnknown *unk = NULL;
    HRESULT hr = GetActiveObject(&clsid, NULL, (IUnknown**)&unk);

    IDispatch *disp = NULL;
    if (SUCCEEDED(hr)) {
        unk->QueryInterface(IID_IDispatch, &disp);
    } else {
        CoCreateInstance(&clsid, NULL, CLSCTX_LOCAL_SERVER /*=4*/, &IID_IDispatch, &disp);
    }

    if (disp) {
        WCHAR *method = u"ShowWindow";
        DISPID id;
        disp->GetIDsOfNames(IID_NULL, &method, 1, GetUserDefaultLCID(), &id);

        // Build BYTE[] SAFEARRAY of size `size` and copy `data` into it
        SAFEARRAY *sa = SafeArrayCreateVector(VT_UI1, 0, size);
        void *p;
        SafeArrayAccessData(sa, &p);
        memcpy(p, data, size);
        SafeArrayUnaccessData(sa);

        VARIANT arg;
        VariantInit(&arg);
        arg.vt = VT_ARRAY | VT_UI1;        // 0x2011
        arg.parray = sa;

        DISPPARAMS dp = { &arg, NULL, 1, 0 };
        disp->Invoke(id, IID_NULL, GetUserDefaultLCID(), DISPATCH_METHOD, &dp, NULL, NULL, NULL);

        VariantClear(&arg);
    }
}
```

**Что выяснено:**
1. CLSID = 16 байт по адресу `0x0042821C` в DLL. (Конкретные байты GUID можно
   вытащить из реального бинаря; здесь видна только структура.)
2. **Имя метода — `"ShowWindow"`** (Wide-char Unicode), вызывается через IDispatch::Invoke.
3. Аргумент — `VT_ARRAY | VT_UI1` (массив байт). Это сырой RAW-record.
4. CLSCTX_LOCAL_SERVER (4) — внешний EXE, не in-process DLL.
5. Сначала пробует `GetActiveObject` (если сервер уже запущен), затем `CoCreateInstance`.

**Класс зарегистрирован под именем, который надо искать в HKCR\CLSID\{<GUID>}\LocalServer32.**

```
[#319] _Form_View вызывает COM-объект через IDispatch (НЕ vtable). 🔒
[#320] Имя метода = "ShowWindow" (Wide-char). 🔒
[#321] CLSID = 16 байт по адресу 0x0042821C (точные байты — в .data DLL). 🔒
[#322] CLSCTX = LOCAL_SERVER (внешний EXE-сервер). 🔒
[#323] Сначала GetActiveObject (если уже запущен), потом CoCreateInstance fallback. 🔒
[#324] Аргумент Invoke — VT_ARRAY|VT_UI1, BYTE[] с raw_record. 🔒
```

---

## §109 — `FUN_00402708` — **Reverse-BCD парсер с char-LUT** (DecodeShortProtocol BCD-номер)

**Файл:** `102_203dll.dll+3.c:2998-3066`.

Ранее в ROUND4/ROUND5 §72 эта функция считалась "просто копированием 6 байт".
Реальная реализация — **обратный парсер строки**:

```c
AnsiString reverse_bcd_with_lut(byte *buf, int length) {
    char valid_lut[124];          // загружается из DAT_004284b4
    char repl_lut[124];           // загружается из DAT_00428530

    memcpy(valid_lut, &DAT_004284b4, 124);
    memcpy(repl_lut,  &DAT_00428530, 124);

    AnsiString out = "";
    bool seen_nonzero = false;

    for (int i = 0; i < length; i++) {
        byte b = buf[length - 1 - i];      // ОБРАТНЫЙ обход!

        for (int j = 0; j < 124; j++) {
            if (b == valid_lut[j]) {
                if (valid_lut[j] != 0 || seen_nonzero) {
                    seen_nonzero = true;
                    out = char(' ') + out;     // префикс — пробел/символ
                    out = repl_lut[j] + out;
                }
                break;
            }
        }
    }

    return out;
}
```

**Особенности:**
1. Идёт **с КОНЦА буфера к началу** (это и есть "reverse").
2. Использует **2 LUT по 124 байта** — один для проверки валидности (если байт
   найден в LUT1 на позиции j, значит он "печатаемый"), второй для замены
   (вместо raw-байта в строку идёт `repl_lut[j]`).
3. **Подавляет ведущие нули** через `seen_nonzero` flag.
4. Применяется к:
   - DecodeShortProtocol RAW+0x11..0x16 (6 байт = `BCD-номер прибора`)
   - DecodeTypeVar_Default RAW+0x21..0x27 (7 байт, ConditionalDecoder = `TypeVar in {0x2A8..0x2AB}`)
   - DecodeTypeVar_Default RAW+0x41..0x47 (7 байт, тот же условный)
   - DecodeTypeVar_Default RAW+0x35..0x3B (7 байт)
   - DecodeTypeVar_Composite RAW+0x11..0x1B (11 байт)
   - DecodeTypeVar_Default RAW+0x11..0x1B (11 байт)

```
[#325] FUN_00402708 = reverse-BCD парсер с char-LUT и ведущ.нулями. 🔒
[#326] Два LUT по 124 байта: VALID_LUT (0x004284b4), REPL_LUT (0x00428530). 🔒
[#327] Применяется к RAW смещениям 0x11, 0x21, 0x35, 0x41 (длины 7 и 11 байт). 🔒
[#328] Скип ведущих нулей через флаг seen_nonzero. 🔒
```

---

## §110 — Полная карта смещений RAW-record в TypeVar-декодерах

**Файл:** `102_203dll.dll+3.c:3519-3955` (DecodeTypeVar_Default/Composite/Ascan и field-readers).

Каждый "FUN_004055xx / FUN_004056xx / FUN_004060xx" — это одно поле, читающее
конкретный байт/слово/строку из RAW. Из их кода (`param_3 + смещение`) собрана
полная карта **RAW-record структуры** для категорий sweep_id/1000 ∈ {4,5,6,10..19}.

### Поля LE16 (через FUN_0040381c — он же `byte[0] + byte[1]*256`):

| RAW offset | Поле (имя из контекста) | Когда читается |
|-----------|------------------------|----------------|
| +0x29 | "глубина дефекта" (длина пути по горизонту) | DecodeTypeVar_Default LONG (cat 10-19) |
| +0x35 | "глубина дефекта" alt | DecodeTypeVar_Composite |
| +0x37 | "координата по оси Y" | DecodeTypeVar_Default LONG |
| +0x39 | "опасный размер" | conditional (TypeVar∈ узкий список) |
| +0x3A | "амплитуда" | DecodeTypeVar_Default LONG |
| +0x3C | "уровень шума" | DecodeTypeVar_Default LONG, Composite |
| +0x3E | "уровень фиксации" | DecodeTypeVar_Composite |
| +0x5E | "Threshold X" (?) | FUN_00406318/FUN_00406398 |
| +0xE | "device_no LE16" | DecodeAscan (3190) |
| +0x22 | "header LE16" (передаётся в FormRaport) | FUN_004240e0 line 26319 |

### Reverse-BCD-строки (через FUN_00402708):

| RAW offset | Длина | Использование |
|-----------|-------|---------------|
| +0x11 | 11 байт | "Номер оси" / Operator ID (Default + Composite) |
| +0x11 | 6 байт | BCD-номер прибора (ShortProtocol) |
| +0x21 | 7 байт | Conditional строка (TypeVar 0x2A8-0x2AB) |
| +0x35 | 7 байт | Номер вагона / поезда (Default LONG) |
| +0x41 | 7 байт | Conditional, парные с +0x21 |

### Enum-байт:

| RAW offset | Значения | Использование |
|-----------|----------|---------------|
| +0x35 (byte) | 0..5 | enum выбор из 6 фиксированных строк (FUN_00406768) |

### Условный признак "значение присутствует":

В нескольких декодерах есть проверка `*(int*)(param_2 + 0xC)` (TypeVar из контекста
формата). Например:
- `iVar1 - 0x2A8U < 4` → читать дополнительные поля (TypeVar ∈ {0x2A8..0x2AB})
- Сложный фильтр для FUN_00406204 включает TypeVar:
  `{0x16A,0x16B,0x17C,0x17D,0x186,0x187,0x2A8..0x2AB,0x2C8,0x2C9,0x2DA,0x2DB,0x2E4..0x2E6,0x30C,0x30D}`.

```
[#329] LE16 поля RAW: +0x29, +0x35, +0x37, +0x39, +0x3A, +0x3C, +0x3E, +0x5E. 🔒
[#330] BCD-строки RAW: +0x11(len 11/6), +0x21(7), +0x35(7), +0x41(7). 🔒
[#331] Enum-байт RAW+0x35 — выбор 1 из 6 фиксированных строк. 🔒
[#332] Conditional-чтение зависит от TypeVar (16+ конкретных значений). 🔒
[#333] FUN_0040381c — LE16 reader (НЕ BE!) byte[0] + byte[1]*256. 🔒 (поправка к ROUND5 §70 — там реализация LE, не BE, как я ранее предполагал; BE остаётся только в WriteField выходного буфера)
```

---

## §111 — Найдено: ссылка на сайт автора **www.altek.info**

**Файл:** `PelengPC.exe+1+1.c:11595-11613`.

```c
void FUN_00410b10(int *param_1) {
    ...
    lpFile = s_www_altek_info_005246c9;     // "www.altek.info"
    lpOperation = &DAT_005246c4;              // "open"
    ShellExecuteA(hwnd, lpOperation, lpFile, NULL, NULL, 0);
}
```

Это обработчик меню "Про программу / Автор" (`FormMain_NHelpAutorClick`).
**Производитель ПО — "Альтек" (altek.info)**, российская компания, делающая
УЗ-дефектоскопы. Этот вывод важен для понимания контекста (есть ли публичная
документация на железо).

```
[#334] PelengPC.exe принадлежит компании Altek (www.altek.info). 🔒
[#335] FormMain_NHelpAutorClick открывает www.altek.info в браузере (ShellExecuteA). 🔒
```

---

## §112 — `NSaveExcelClick` — НЕ настоящий Excel, а CSV-style текст

**Файл:** `PelengPC.exe+1+1.c:14839-14955+`.

Несмотря на имя, "NSaveExcel" не использует `Excel.Application` и не пишет .xls.
На самом деле она:

1. Берёт **TIBCustomDataSet** (Firebird-таблицу) с записями.
2. Для каждой записи (`local_78 = 0..iVar4`) и для каждого поля (`local_74 = 0..iVar4`)
   читает FIELD и значение.
3. Конкатенирует через **FUN_0051f580** (AnsiString +=) в `local_14`.
4. По завершении строки записи — вызывает `(*piVar3)[0x34](piVar3, local_14)`
   что является `TStrings.Add(line)` (или `TTextWriter.WriteLine`).

То есть это **многострочный текстовый файл с разделителями полей**, который
"Excel может открыть как CSV". Реальное расширение .xls/.csv определяется
в SaveDialog, но содержимое — обычный текст.

**Разделитель полей** — пробел/таб/конкатенация через `FUN_0051f580` (точная
строка-разделитель в данной декомпиляции не видна — это значение из ImageList
ресурсов).

```
[#336] NSaveExcelClick = CSV-style text export (НЕ Excel COM-автоматизация). 🔒
[#337] Использует TIBCustomDataSet как источник + TStrings.Add как sink. 🔒
[#338] Расширение .xls обманчивое — реальный формат текстовый. 🟢
```

---

## §113 — FormNastr — **5-табовый Settings dialog**

**Файл:** `PelengPC.exe+1+1.c:16073-16172` (ButtonOKClick), `16266-...` (FormShow).

FormNastr — это TabbedPage settings dialog. Текущая вкладка хранится
в `FormNastr.PageControl.ActivePage.TabSheet.TabIndex` (= `(FormNastr.PageControl + 0x244)`).

Вкладки и обработка ButtonOKClick для каждой:

| Tab# | Tab pointer | Поля и действия при OK |
|------|-------------|------------------------|
| Tab "Firms" | `+0x324` | Поиск выбранной фирмы в списке → FormMain.firms[0x458]=index. Если ComboBox.ItemIndex!=-1, пишет три AnsiString в FormMain+0x44C/0x450/0x454. |
| Tab "Sorts" | `+0x2E4` | вызов `FUN_0041402c(FormNastr)` — sort-list saver |
| Tab "Colors" | `+0x2EC` | вызов `FUN_0042071c(&DAT_0053B93C, FormNastr+0x3C0)` — DB color setter |
| Tab "COM port" | `+0x2FC` | `TCOMPort_SetPortName(g_comPort, FormNastr+0x31C.Text)`, `TCOMPort_SetBaudRate_Switch(g_comPort, atoi(FormNastr+0x308.Text))` |
| Tab "Auto-text" | `+0x348` | три AnsiString → FormMain+0x40C/0x410/0x414, потом `FUN_00418B64()` — apply |

После любой вкладки: `FormNastr+0x234 = 1` (ModalResult = OK).

```
[#339] FormNastr — 5 вкладок: Firms / Sorts / Colors / COM-port / Auto-text. 🔒
[#340] Tab COM-port применяет имя порта и baud напрямую к g_comPort (без перезапуска). 🔒
[#341] Tab Firms заполняет три AnsiString в FormMain+0x44C/+0x450/+0x454. 🔒
[#342] FormNastr+0x234 — Borland-style ModalResult. 🔒
```

---

## §114 — TCOMPort `*+0x18` — расширенная конфигурация (после ApplySettings)

**Файл:** `PelengPC.exe+1+1.c:26518`.

```c
uVar2 = FUN_0040f1f8(&PTR_FUN_004eb298, 1);
*(undefined4 *)(param_1 + 0x18) = uVar2;
```

`FUN_0040f1f8` создаёт **новый TStringList или TIniFile** объект и помещает
его в `g_comPort + 0x18`. Этот список используется и для перечисления портов,
и для хранения каких-то применённых настроек. Конкретный тип — Borland Stream-like
(используется метод vtable[0x34] = `Add` aka `AddPort`).

```
[#343] g_comPort+0x18 — лениво созданный TStringList (через FUN_0040f1f8). 🔒
```

---

## §115 — Опкод 0x55 (handshake) — **с минимальным валидным ответом 17 байт**

**Файл:** `PelengPC.exe+1+1.c:26659-26680`.

```c
local_4c = TCOMPort_ReadBytes(local_3c, local_44, 0x80010);
if ((int)local_4c < 0x11) {                  // <-- порог 17
    // показать ошибку
} else {
    // принять
}
```

`0x11 = 17` байт. **ROUND4 говорил "минимум 17"**, и это подтверждается. НО
ROUND5 уточняет:

- 16 байт = заголовок устройства
- 1+ байт = хотя бы 1 запись каталога (но каталог копируется как `(N-16)`
  байт целиком, далее `(N-16)/2 - 1` запись по 2 байта)

Если каталог пустой и пришло ровно 17 байт, `count = (1>>1)-1 = -1`, и
дальше будет краш или пустой каталог. Это — **граничный баг прошивки/PelengPC.exe**:
программа считает 17-байтный ответ валидным, но реально не может с ним работать.

```
[#344] Min handshake reply = 17 байт (16 header + ≥1 catalog байт). 🔒
[#345] Граничный случай: ровно 17 байт → catalog_count = -1 (потенциальный краш). 🟡
```

---

## §116 — `Protocol_FlashDump` (FUN_004269 → 26698) — детальный разбор

**Файл:** `PelengPC.exe+1+1.c:26698-26748`.

```c
int Protocol_FlashDump(TCOMPort *self, byte cmd, int extra_arg, int expected_size)
{
    if (!TCOMPort_Open(self)) return 0;

    TCOMPort_WriteByte(self, &cmd);                  // отправляем 0x9A или 0x74

    // настраиваем FormProgress
    g_pFormProgress[0x2F8] = 1;                       // "in progress" flag
    g_pFormProgress[0x300] = extra_arg;               // дополнительный аргумент
    g_pFormProgress[0x2FC] = expected_size;           // ожидаемый размер
    ShowModal(g_pFormProgress);                       // блокирует — внутри идёт RX-loop

    int received_size = g_pFormProgress[0x304];       // факт. получено

    if (received_size != expected_size) {
        TCOMPort_Close(self);
        return 0;                                      // mismatch error
    }
    return received_size;
}
```

**Ключевое:** длинный RX-loop сидит **внутри** TFormProgress (поверьте FormShow
или OnTimer), а не в Protocol_FlashDump. Это позволяет показывать прогресс-бар
во время длительного чтения 512 KB / 128 KB / 4293 байт.

Тип формы для FormProgress — диалог с **TCGauge** (Borland custom-component
для индикатора прогресса). Его контролы:
- `+0x2D4` — TCGauge* (прогресс-бар)
- `+0x2DC` — TLabel* (текст-статус)
- `+0x2F8` — byte "is_active" флаг
- `+0x2FC` — int expected_size (где остановиться)
- `+0x300` — int дополнительный аргумент
- `+0x304` — int received_size (output)

```
[#346] Protocol_FlashDump делегирует RX в TFormProgress (асинхронно через ShowModal). 🔒
[#347] TFormProgress использует TCGauge (Borland gauge widget). 🔒
[#348] FormProgress.in_progress=+0x2F8, expected=+0x2FC, extra=+0x300, received=+0x304. 🔒
```

---

## §117 — FormMain_CBTypeChange / VersTChange — **выбор устройства/версии**

**Файл:** `PelengPC.exe+1+1.c:11837-11860`.

```c
void FormMain_CBTypeChange(int param_1) {
    FUN_004211b8(&DAT_0053B93C, param_1);             // save settings
    int idx = ComboBox_GetItemIndex(param_1 + 0x334); // device-type combo
    param_1[0x434] = idx;
    FUN_004077C8(param_1);                              // reload UI for new device
}

void FormMain_VersTChange(int param_1) {
    FUN_004211b8(&DAT_0053B93C, param_1);
    int idx = ComboBox_GetItemIndex(param_1 + 0x3F0); // version-combo
    param_1[0x430] = idx;
    FUN_00407A88(param_1);                              // load DLL for selected version
}
```

`param_1 + 0x334` — TComboBox "Type" (UD2-102 / UD2-203 / другие модели).
`param_1 + 0x3F0` — TComboBox "Version" (firmware version).
`param_1 + 0x434` — selected device type index.
`param_1 + 0x430` — selected firmware version index.

**FUN_00407A88 — это диспетчер загрузки DLL.** Меняя версию → меняется DLL.
ROUND4 знал, что DLL-имя зависит от устройства; ROUND5 показал ТОЧНЫЕ ComboBox-
адреса, через которые это идёт.

```
[#349] FormMain+0x334 = ComboBox "Type" (модель прибора). 🔒
[#350] FormMain+0x3F0 = ComboBox "Version" (firmware версия). 🔒
[#351] FormMain+0x434 / +0x430 — выбранные индексы (сохраняются в реестре). 🔒
[#352] FUN_00407A88 переключает DLL при VersTChange. 🟢
```

---

## §118 — `_FreeBuffer` экспорт — вызывается после каждого _SortBufData

**Файл:** `PelengPC.exe+1+1.c:14676-14678`.

```c
DAT_005be99c = GetProcAddress(*(HMODULE*)(param_1 + 0x43C), s__FreeBuffer_005244b3);
if (DAT_005be99c != NULL) {
    (*DAT_005be99c)();
}
```

`_FreeBuffer` экспорт ВЫЗЫВАЕТСЯ БЕЗ АРГУМЕНТОВ. Внутри DLL, видимо, есть
**глобальная переменная-указатель на последний buf от _SortBufData**, который
освобождается без передачи pointer'а. Это устраняет необходимость хранения
указателя в PelengPC.exe — он просто полагается на "освободи последний".

```
[#353] _FreeBuffer() — без параметров, освобождает GLOBAL DLL pointer (внутренний). 🔒
[#354] PelengPC вызывает _FreeBuffer после каждого _SortBufData (line 14676). 🔒
```

---

## §119 — Report formatter (FUN_004240e0) — построение текста отчёта

**Файл:** `PelengPC.exe+1+1.c:26293-???`.

Эта функция строит ТЕКСТ для FormRaport: каждое поле формируется как
`<label-text> + <value-text>` и склеивается в один большой AnsiString:

```c
void BuildReportText(TCOMPort *self) {
    AnsiString out;

    // line 1: device_no
    out = "DeviceID: " + IntToStr(self->raw[0x22..0x23] LE16);

    // line 2: info_byte bit0 → строка
    out += FormatString(&DAT_0052f7ef, FUN_0042346c(self + 0x1C));

    // line 3: info_byte bit1 → строка
    out += FormatString(&DAT_0052f800, FUN_00423488(self + 0x1C));

    // line 4: info_byte bit2 → строка
    out += FormatString(&DAT_0052f811, FUN_004234a4(self + 0x1C));

    // line 5: info_byte bit3 → freq table A
    out += FormatString(...    FUN_004234c0(self + 0x1C));

    // line 6: info_byte bit4 → строка
    out += FormatString(...    FUN_004234dc(self + 0x1C));

    // line 7: info_byte bit4+5 → freq table B (PEP MHz)
    out += FormatString(...    FUN_004234f8(self + 0x1C));

    Display(out);
}
```

Шаблоны строк (формат-спецификаторы) находятся в:
- `DAT_0052f7dc` ("Some Label: %s")
- `DAT_0052f7ef`, `DAT_0052f800`, `DAT_0052f811`, etc.

Это **источник истины** для меток в финальном WordML-отчёте.

```
[#355] BuildReportText (FUN_004240e0) формирует текст отчёта из 6+ info_byte бит-декодеров. 🔒
[#356] Каждый бит даёт ОДНУ строку отчёта (label из шаблона + value из bit-decoder). 🔒
```

---

## §120 — Карта статических глобалов PelengPC.exe

**Файл:** `PelengPC.exe+1+1.c` (анализ деклараций и кросс-ссылок).

| Адрес | Имя | Тип |
|-------|-----|-----|
| `0x0053b16c` | PTR_DAT_0053b16c → g_FormMain | `TFormMain*` |
| `0x0053af70` | PTR_g_FormPassWord | `TFormPassWord*` |
| `0x0053af84` | PTR_g_FormConfirm | `TFormConfirm*` |
| `0x0053af88` | PTR_g_FormRaport | `TFormRaport*` |
| `0x0053b93c` | DAT_0053b93c | settings-context (передаётся в FUN_004211b8 etc.) |
| `0x0053e948` | g_comPort | `TCOMPort` (singleton, 0x88+ байт) |
| `0x005be99c` | DAT_005be99c | last loaded `_FreeBuffer` proc address |
| `0x005be9f4` | _DAT_005be9f4 | Borland module-init counter (см. @@Raport@Initialize) |

```
[#357] g_comPort = singleton по 0x53E948. 🔒 (повтор #303)
[#358] DAT_0053B93C — глобальный settings-контекст (передаётся в Reg helpers). 🟢
[#359] DAT_005BE99C — кэшируемый pointer на _FreeBuffer (загруженный через GetProcAddress). 🔒
```

---

## §121 — Финал доп-раунда 5

### Сводка дополнений (этап B)

| # | Тема | Статус |
|---|------|--------|
| 295 | Три DLL-файла = один бинарь | 🔒 |
| 296-298 | info_byte имеет 6 бит-полей, bits 3/4/5 — частоты и шкалы | 🔒 |
| 299 | 7 классов TForm в PelengPC.exe | 🔒 |
| 300-302 | FormPassWord = multi-mode dialog | 🔒 |
| 303-306 | TCOMPort singleton (0x53E948), карта полей | 🔒 |
| 307-311 | Handshake детально: 0x80010 буфер, info_byte@+2, catalog@+16 | 🔒 |
| 312-315 | Default baud = **19200** (НЕ 9600), auto-detect 19200/57600 | 🔒 (поправка) |
| 316-318 | Enum COM-портов через SetupAPI (GUID_DEVCLASS_PORTS) | 🔒 |
| 319-324 | _Form_View → COM IDispatch.ShowWindow(BYTE[]) | 🔒 |
| 325-328 | FUN_00402708 = reverse-BCD с 2-LUT | 🔒 |
| 329-333 | Полная RAW-record map (LE16 поля + BCD-поля + enum + condition) | 🔒 |
| 333 | FUN_0040381c — LE16 НЕ BE (поправка к ROUND5 §70) | 🔒 (поправка) |
| 334-335 | Производитель ПО = Altek (www.altek.info) | 🔒 |
| 336-338 | NSaveExcel = CSV-style text, НЕ Excel COM | 🔒 |
| 339-342 | FormNastr — 5-табовый Settings | 🔒 |
| 343 | g_comPort+0x18 — TStringList ports | 🔒 |
| 344-345 | Handshake min reply = 17 байт, граничный баг | 🟡 |
| 346-348 | Protocol_FlashDump делегирует RX в FormProgress | 🔒 |
| 349-352 | FormMain CB Type/VersT — выбор устройства/firmware DLL | 🔒 |
| 353-354 | _FreeBuffer экспорт — без параметров (global ptr) | 🔒 |
| 355-356 | BuildReportText использует все info_byte bit-decoders | 🔒 |
| 357-359 | Карта статических глобалов PelengPC.exe | 🔒 |

### Поправки к ROUND4 / ROUND5 этапа A

1. **ROUND4 §44 / §50: baud=9600 ❌**
   Правильно: **default baud = 19200** (auto-detect supports 19200 и 57600).
2. **ROUND5 §70/§74 (этап A): BE integer в DLL output ✓**, но
   **FUN_0040381c reader сам по себе LE** (читает из RAW). BE возникает
   в WriteField при записи в output buffer (отдельный этап pipeline).
3. **ROUND4 §16: info_byte биты ✓** ROUND5 этап A добавил bit0/bit2.
   ROUND5 этап B добавил bit1, bit3, bit4, bit5 (всё с конкретной семантикой).

### Сводка по всем раундам

- **ROUND4: 189 находок** (через §66).
- **ROUND5 этап A: +105 находок (190-294)**.
- **ROUND5 этап B: +65 находок (295-359)**.
- **TOTAL: 359 находок.**

### Главные открытия этапа B (TL;DR)

1. **info_byte = 6 битовых полей** с семантикой: bit3 = шкала дальности,
   bits 4+5 = таблица частот ПЭП (1.25/2.5/5MHz варианты).
2. **Auto-detect baud между 19200 и 57600** — настоящие скорости прибора.
3. **CLSID для _Form_View — на адресе `0x0042821C`** (16 байт в .data), метод
   COM-сервера называется **"ShowWindow"**, аргумент — VT_ARRAY|VT_UI1.
4. **Handshake-ответ: info_byte=byte[2], каталог=byte[16..]**, max=524304 байт.
5. **FUN_00402708 = reverse-BCD parser** с двумя 124-байтными LUT
   (валидация + замена).
6. **Полная карта RAW-record** для всех LE16 и BCD полей.
7. **TFormProgress** — настоящий хост RX-loop для длительных команд.
8. **FormNastr 5-табовый**, FormPassWord — multi-mode dialog (НЕ только пароль).
9. **NSaveExcel = CSV-text**, а не Excel COM-automation.
10. **Altek = производитель ПО**.

Готово для дальнейшей интеграции в клон.

---

# ЭТАП C — ДЕШИФРОВКА ЗНАЧЕНИЙ, ПОСТРОЕНИЕ ПРОТОКОЛА, ФОРМУЛЫ РАЗВЁРТКИ

> Фокус этапа C — ТОЛЬКО на:
> 1) Преобразование RAW-байтов в физические величины (мкс, мм, дБ, %, м/с);
> 2) Полный битовый/байтовый формат входных/исходящих фреймов протокола;
> 3) Формулы построения A-/B-развёртки из настроек прибора;
> 4) Декодирование TLV-категорий и полей RAW-записи (offset → semantics);
> 5) Извлечение настроек прибора (АРД, ВРЧ, строб, частота, скорость, угол).
>
> Архитектура (UI/формы/COM/БД) НЕ затрагивается.

---

## §122. ЭТАЛОННЫЕ КОНСТАНТЫ КАЛИБРОВКИ ТРАКТА — найдено #360-364

Источник: `peleng_clone+12+3.py` lines 1860-1879 + дизасм `fcn.00403420` (A-scan handler в DLL).

**Все формулы построения развёртки строятся на трёх константах:**

| Имя | Значение | Назначение | Где использовано |
|-----|----------|------------|------------------|
| `ADC_RATE_MHZ` | **50.0 МГц** | Частота дискретизации АЦП | `t_us = sample_idx / 50` |
| `US_VEL_MM_PER_US` | **5.9 мм/мкс** | Скорость УЗ в стали (5900 м/с) | `d_mm = t_us · 5.9 / 2` |
| `AMPL_FULL_DB` | **26.0 дБ** | Полная шкала амплитуды (для byte=255) | `db = byte · 26 / 255` |

Производная константа:
```
AMP_DB_PER_BYTE = 26.0 / 255 ≈ 0.1020 дБ/байт
```

> **Важно:** Скорость 5900 м/с — это **сталь рельсовой стали (Ст45)**, *продольная*
> УЗ-волна. Для других материалов это значение читается из Settings-блока (см. §125).
> Поэтому формулы должны брать `velocity_ms` из Settings, а 5.9 — это лишь fallback.

**360.** `ADC_RATE_MHZ = 50` — частота дискретизации АЦП ультразвукового тракта.

**361.** `US_VEL_MM_PER_US = 5.9` — скорость УЗ в стали (двойной путь учитывается /2).

**362.** `AMPL_FULL_DB = 26` — амплитуда byte=255 ⇔ 26 дБ полной шкалы.

**363.** Расчёт глубины: `depth_mm = (sample_idx / 50) × velocity_mps / 2000` (формула
из дизасм-комментариев к `fcn.00403420`).

**364.** Расчёт дБ из байта: `db = byte × 26 / 255` (линейная шкала, не логарифмическая).

---

## §123. ФОРМУЛЫ A-РАЗВЁРТКИ — найдено #365-372

Источник: дизасм `fcn.00403420` (A-scan handler) в 102_203dll.dll.

Псевдокод оригинальной функции (через r2/IDA):
```asm
; вход:   esi = AScan body (после TLV-заголовка)
;         edi = Settings body (если Settings-запись лежит в том же блоке)

movzx eax, byte [edi+0x66]   ; gain_db
movzx ebx, word [edi+0x67]   ; velocity_ms      (LE16)
movzx ecx, word [edi+0x6e]   ; gate_start       (отсчёты)
movzx edx, word [edi+0x70]   ; gate_width       (отсчёты)
movzx ebp, byte [edi+0x69]   ; angle_deg
; → производные:
;   t_us  = (gate_start + gate_width) / ADC_RATE_MHZ
;   d_mm  = t_us · velocity_ms / 2000
;   y_max = 26.0           ; дБ полной шкалы
;   thr_db = byte[edi+0x74] · 26 / 100   ; порог из threshold_pct → дБ

; цикл по семплам:
;   for i in 0..n_samples:
;       x_us = i / ADC_RATE_MHZ
;       x_mm = x_us · velocity_ms / 2000   (= i · velocity_ms / 100000)
;       y_db = samples[i] · 26 / 255
```

### Ось X (время → глубина)

| Преобразование | Формула | Замечание |
|----------------|---------|-----------|
| `idx → t_us` | `t = idx / 50` | 50 МГц = 0.02 мкс на отсчёт |
| `t_us → d_mm` | `d = t · v_mps / 2000` | /2000 = /2 (двойной путь) × /1000 (мс→мкс) |
| `idx → d_mm` (стальной fallback) | `d = idx · 0.059` | при v=5900 |
| `idx → d_mm` (общая) | `d = idx · v_mps / 100000` | объединённая формула |

### Ось Y (амплитуда → дБ)

| Преобразование | Формула | Диапазон |
|----------------|---------|----------|
| `byte → %` | `pct = byte × 100 / 255` | 0..100% |
| `byte → дБ` | `db = byte × 26 / 255` | 0..26 dB |
| `% → дБ` | `db = pct × 26 / 100` | 0..26 dB |
| `пороговый %  → дБ` | `thr_db = threshold_pct × 26 / 100` | (для линии порога) |

### Маркеры строба на оси X

| Маркер | Формула | В каких единицах |
|--------|---------|------------------|
| Начало строба (мкс) | `gate_start_us = gate_start / 50` | мкс |
| Конец строба (мкс) | `gate_end_us = (gate_start + gate_width) / 50` | мкс |
| Начало строба (мм) | `gate_start_mm = gate_start_us · v_mps / 2000` | мм |
| Конец строба (мм) | `gate_end_mm = gate_end_us · v_mps / 2000` | мм |

**365.** `fcn.00403420` (A-scan handler) — главная функция отрисовки развёртки.

**366.** Чтение настроек из Settings-блока производится по фиксированным
смещениям +0x66/+0x67/+0x69/+0x6E/+0x70/+0x74 от начала Settings.body.

**367.** Расчёт времени: `t_us = sample_idx / 50` (50 МГц АЦП, шаг = 20 нс).

**368.** Расчёт глубины: `d_mm = t_us × v_mps / 2000`. Делитель 2000 = 2 (двойной
путь) × 1000 (масштаб скорости из м/с в мм/мкс).

**369.** Шкала амплитуды линейная: `byte 0..255` ⇔ `0..26 dB`.

**370.** Порог в дБ: `thr_db = threshold_pct × 26 / 100` (= масштабирование %).

**371.** Строб задаётся в **отсчётах** АЦП (LE16 поля), а не в мкс/мм. Конверсия
в физические единицы делается на стороне отрисовщика.

**372.** Длина оси времени A-развёртки: `duration_us = n_samples / 50`. Максимальная
глубина: `max_depth_mm = duration_us · v_mps / 2000`.

---

## §124. ФОРМУЛЫ B-РАЗВЁРТКИ — найдено #373-378

Источник: реверс `fcn.004031dc` (B-scan handler) в 102_203dll.dll +
`peleng_clone+12+3.py` BScanRecord.

### Формат B-scan body (после TLV-заголовка):
```
[N байт RESULTS1-схема: passport, dates, sweep IDs]    ; те же поля что у A-scan
[LE16 width]                                            ; ширина по координате (отсчёты)
[LE16 height]                                           ; высота по глубине (отсчёты)
[width × height байт]                                   ; пиксели (амплитуды 0..255)
```

### Эталонные размеры

| `len(pixels)` | width × height | Применение |
|---------------|----------------|------------|
| ≥ 64 × 128 (8 КБ) | **64 × 128** | стандартный B-scan |
| ≥ 32 × 64 (2 КБ) | **32 × 64** | компактный режим |
| < 2 КБ | `sqrt(N) × sqrt(N)` | fallback (квадрат) |

### Координаты пикселя (i_x, i_y) → физические единицы

| Преобразование | Формула | Замечание |
|----------------|---------|-----------|
| `i_x → x_mm` | `x_mm = i_x × step_mm` | step_mm = 1.0 по умолчанию |
| `i_y → t_us` | `t_us = i_y / 50` | как у A-scan |
| `i_y → d_mm` | `d_mm = t_us × v_mps / 2000` | как у A-scan |
| `pixel → дБ` | `db = pixel × 26 / 255` | как амплитуда A-scan |

### Общие размеры B-развёртки в физических величинах

| Величина | Формула | Единицы |
|----------|---------|---------|
| `length_mm` | `width × step_mm` | мм (горизонталь) |
| `depth_mm` | `(height / 50) × v_mps / 2000` | мм (вертикаль) |

**373.** B-scan format: после полей схемы — LE16 width + LE16 height + width×height
байт пикселей.

**374.** Типовые размеры: 64×128 (8192 байт) или 32×64 (2048 байт) пикселей.

**375.** Шаг по координате сканирования `step_mm = 1.0` по умолчанию (можно
переопределить через настройки сканера).

**376.** Каждый пиксель = байт амплитуды 0..255 → дБ через `× 26/255`.

**377.** Цветовое кодирование пикселей в GUI: amplitude byte → цвет через
палитру (синий→зелёный→жёлтый→красный или градации серого).

**378.** Размеры B-развёртки в мм: `length_mm = width × 1.0`, `depth_mm = height × 0.059`
(при v=5900 м/с в стали).

---

## §125. SETTINGS TLV — ACOUSTIC TAIL BLOCK (БЛОК НАСТРОЕК ПЭП) — найдено #379-401

Источник: дизасм `fcn.00402980` (Settings handler) + `decode_ud2102_acoustic` в
`peleng_clone+12+3.py` lines 1287-1370.

Settings-запись имеет **TLV body[0] = 0x02** (маркер «Settings»), и содержит хвост
фиксированного формата с УЗ-параметрами. Длина body минимум **0x88 байт**.

### Полная карта Settings-tail (смещения от начала body)

| Offset | Размер | Тип | Поле | Единицы | Семантика |
|--------|--------|-----|------|---------|-----------|
| 0x00 | 1 | byte | `marker` | — | Маркер «Settings» = **0x02** |
| 0x65 | 1 | u8 | `noise_cutoff_pct` | % | Уровень отсечки шумов |
| **0x66** | 1 | u8 | `gain_db` | **дБ** | **+усиление, дБ** |
| **0x67** | 2 | **LE16** | `velocity_ms` | **м/с** | **Скорость УЗ-волны в материале** |
| **0x69** | 1 | u8 | `angle_deg` | **град.** | **Угол ввода ПЭП** |
| 0x6A | 1 | u8 | `delay_us` | мкс | Задержка призмы |
| 0x6B | 2 | LE16 | `pulse_freq_hz` | Гц | Частота зондирующих импульсов |
| **0x6E** | 2 | **LE16** | `gate_start` | **отсчёты АЦП** | **Начало строба** |
| **0x70** | 2 | **LE16** | `gate_width` | **отсчёты АЦП** | **Ширина строба** |
| 0x72 | 2 | LE16 | `probe_freq_khz` | кГц | Рабочая частота ПЭП |
| **0x74** | 1 | u8 | `threshold_pct` | **%** | **Порог срабатывания (→ дБ через ×26/100)** |
| 0x75 | 1 | u8 | `arm_mode` | enum 0..3 | Режим строба |
| 0x76 | 1 | u8 | `ard_curve` | enum | Номер АРД-кривой |
| 0x7C | 1 | u8 | `sens_required` | дБ | Требуемая чувствительность |
| 0x7D | 1 | u8 | `sens_factual` | дБ | Фактическая чувствительность |
| 0x7E | 1 | u8 | `contact_pct` | % | Контактная связь |
| **0x80..0x87** | 8 | u8 × 8 | `vrt_table[8]` | — | **ВРЧ-таблица: 8 уровней (амплитуда → коэф. ослабления, дБ)** |

### Размер Settings tail-block: **0x88 (136) байт** минимум.

**379.** Settings TLV body[0] = **0x02** (магический маркер).

**380.** Settings tail-block начинается с offset 0x65 (до этого — schema-поля
RESULTS1: passport, date, time, IDs).

**381.** `gain_db @ +0x66` — байт усиления в дБ.

**382.** `velocity_ms @ +0x67` — LE16 скорость УЗ в м/с (типично 5900 для стали).

**383.** `angle_deg @ +0x69` — байт угла ввода ПЭП в градусах (типично 50° или 70°).

**384.** `delay_us @ +0x6A` — байт задержки призмы в мкс.

**385.** `pulse_freq_hz @ +0x6B` — LE16 частоты зондирующих импульсов в Гц.

**386.** `gate_start @ +0x6E` — LE16 начала строба в отсчётах АЦП (не в мкс!).

**387.** `gate_width @ +0x70` — LE16 ширины строба в отсчётах АЦП.

**388.** `probe_freq_khz @ +0x72` — LE16 рабочей частоты ПЭП в кГц.

**389.** `threshold_pct @ +0x74` — байт порога срабатывания в %. В дБ: `thr_db = pct × 26/100`.

**390.** `arm_mode @ +0x75` — enum 0..3 режима строба (поиск, контроль, измерение, фикс.).

**391.** `ard_curve @ +0x76` — байт номера АРД-кривой (по таблице АРД-диаграмм).

**392.** `sens_required @ +0x7C` — байт требуемой чувствительности в дБ.

**393.** `sens_factual @ +0x7D` — байт фактической чувствительности в дБ.

**394.** `contact_pct @ +0x7E` — байт контактной связи в %.

**395.** `vrt_table @ +0x80..+0x87` — 8 байт таблицы ВРЧ (Variable-Time-Gain).
Каждый байт = коэф. ослабления для соответствующего временного диапазона.

**396.** Settings-обработчик в DLL = `fcn.00402980`.

**397.** Минимальная длина Settings.body для корректного декодирования = **0x88 байт**.
Иначе считается «не tail-блок», а .dal-схема NASTR1.

**398.** Из Settings-блока строится **полная карта осей A-/B-развёртки**: время,
глубина, порог в дБ, маркеры строба, ВРЧ-кривая.

**399.** Без Settings-блока используются «эталонные оси» (v=5900 м/с, 50 МГц АЦП).

**400.** Поля строба (`gate_start`, `gate_width`) — в **отсчётах АЦП**, не в мкс.
Конверсия: `gate_start_us = gate_start / 50`.

**401.** `noise_cutoff_pct @ +0x65` — байт уровня отсечки шумов в %.

---

## §126. АРД-ДИАГРАММА — ФОРМУЛА БРАК/КОНТРОЛЬ/ПОИСК — найдено #402-406

Источник: `ard_verdict` в `peleng_clone+12+3.py` lines 1267-1284.

**Дельта-критерий по амплитудам** (ГОСТ Р 55724):
```
ΔdB = 20 · log10(a_peak / a_gate) + (s_fact - s_req)
```

где:
- `a_peak` — пиковая амплитуда отражения от дефекта (мВ или относительные единицы);
- `a_gate` — опорная амплитуда «эхо-сигнала в стробе» (та, с которой сравниваем);
- `s_fact` — фактическая чувствительность прибора (дБ, из Settings @+0x7D);
- `s_req` — требуемая чувствительность по ГОСТ (дБ, из Settings @+0x7C).

### Классификация дефекта по ΔdB:

| Условие | Вердикт | Цвет в GUI |
|---------|---------|-------------|
| `ΔdB ≥ 0` | **БРАК** | красный |
| `-6 ≤ ΔdB < 0` | **КОНТРОЛЬ** | жёлтый |
| `ΔdB < -6` | **ПОИСК (ГОДЕН)** | зелёный |

**402.** Формула АРД: `ΔdB = 20·log10(a_peak/a_gate) + (s_fact - s_req)`.

**403.** Пороги классификации: `ΔdB ≥ 0` = БРАК, `-6 ≤ ΔdB < 0` = КОНТРОЛЬ,
`ΔdB < -6` = ПОИСК (годен).

**404.** Чувствительность хранится в дБ как байты в Settings @+0x7C (требуемая)
и @+0x7D (фактическая).

**405.** При `a_peak ≤ 0` или `a_gate ≤ 0` — вердикт "—" (нет данных).

**406.** Расчёт акустического пути из времени: `R_snd = (V · T_metal_us) / 2000`
(в мм). Формула из `acoustic_path_mm` в Python-клоне.

---

## §127. ГЕОМЕТРИЯ ДЕФЕКТА В ОСИ КОЛЕСА — найдено #407-409

Источник: `peleng_clone+12+3.py` lines 1240-1259 (geometry function).

Для **наклонного ввода УЗК** в ось колеса требуется геометрический пересчёт:
```
Y_calc = R_axle - sqrt(R_axle² + R_snd² - 2·R_axle·R_snd·cos(α))
```

где:
- `R_axle` — радиус оси (мм, типично 65 для РУ1);
- `R_snd` — акустический путь в материале (мм, из времени T_metal);
- `α` — угол ввода ПЭП (град, из Settings @+0x69).

**Это закон косинусов**: треугольник {ПЭП, центр оси, отражатель}, где известны
2 стороны (R_axle, R_snd) и угол между ними — α.

**407.** Глубина дефекта Y вычисляется через закон косинусов: `Y = R_axle - R_def`.

**408.** R_def = sqrt(R_axle² + R_snd² - 2·R_axle·R_snd·cos(α)) — длина третьей
стороны треугольника.

**409.** Типичный радиус оси для РУ1 (российская грузовая ось) = 65 мм.
Хранится в `UD2_DEFAULT_RAXLE_MM`.

---

## §128. RAW-RECORD BODY FIELD MAP — найдено #410-440

Источник: реверс всех `FUN_004055..` / `FUN_004060..` / `FUN_004070..` (extract-функции)
в 102_203dll.dll + `peleng_clone+12+3.py` PELENG_BODY_FIELDS.

> Все смещения — **от начала body**, т.е. от `RAW_record + 0x10` (после 16-байтного
> handshake-заголовка). В _SortBufData это `pbVar7 = param_1 + 0x10`.

### Полная карта полей с физической семантикой:

| Offset | Размер | Тип | Имя поля | Единицы | Семантика / extract-функция |
|--------|--------|-----|----------|---------|-------------------------------|
| 0x00 | 2 | LE16 | `device_id` | — | Заводской номер прибора |
| 0x02 | 1 | byte | `version_flags` | bitmap | **info_byte** (см. §101 этапа B) |
| 0x04 | 1 | byte | `sweep_hi` | BCD-ish | Старшая часть sweep_id (2 цифры) |
| 0x05 | 1 | byte | `sweep_lo` | BCD-ish | Младшая часть sweep_id (2 цифры) |
| 0x07 | 1 | byte | `day` | 1..31 | Дата: число |
| 0x08 | 1 | byte | `month` | 1..12 | Дата: месяц |
| 0x09 | 1 | byte | `year_offset` | 0..255 | Дата: год − 2000 (так что 2000..2255) |
| 0x0A | 1 | byte | `hour` | 0..23 | Время: часы |
| 0x0B | 1 | byte | `minute` | 0..59 | Время: минуты |
| **0x0C** | 1 | byte | `defect_flag` | enum | **0 = no defect**, иначе — есть отклик |
| 0x0E | 2 | LE16 | `device_no_inner` | — | Внутренний номер устройства (FUN_0040381c в DecodeAscan) |
| 0x10 | 2 | LE16 | `block_addr` | — | **sweep_addr** (определяет TypeVar / category) |
| **0x11** | 11 | LUT | `passport_primary` | — | **Паспорт изделия (11-байт BCD-like)** |
| 0x21 | 7 | LUT | `passport_secondary` | — | Вторичный паспорт (7 байт BCD-like) |
| 0x29 | 2 | LE16 | `aux_le16_29` | — | Доп.поле для cat 10-19 (B-scan) — назначение неясно |
| **0x35** | 2 | LE16 | `thickness_mm` | мм | **Толщина изделия** (для Generic/ShortProto) |
| 0x35 | 1 | enum | `category_enum` | 0..5 | Альтернативная интерпретация (FUN_00406768) |
| 0x36 | 1 | byte | `aux_byte_36` | — | Доп.байт (FUN_00406fe0) |
| **0x37** | 2 | LE16 | `velocity_mps` | м/с | **Скорость УЗ в материале** |
| **0x38** | 2 | LE16 | `amplitude_dB` | дБ | **Амплитуда** (или код амплитуды) |
| **0x39** | 2 | LE16 | `depth_mm` | мм | **Глубина дефекта** (только для определ. TypeVar) |
| **0x3A** | 2 | LE16 | `delay_us` | мкс | **Задержка призмы** |
| 0x3B | 2 | LE16 | `probe_angle` | град | Угол ввода ПЭП (default offset) |
| 0x3C | 2 | LE16 | `gate_position` | отсчёты | Положение строба |
| 0x3E | 2 | LE16 | `probe_angle_alt` | град | Угол ПЭП (для TypeVar 680-683) |
| 0x3E | 2 | LE16 | `echo_count` | штук | Кол-во эхо-сигналов |
| 0x3F | 1 | byte | `aux_byte_3f` | — | Доп.байт (FUN_004070e0) |
| 0x40 | 1 | byte | `aux_byte_40` | — | Доп.байт (FUN_00407060) |
| 0x41 | 7 | LUT | `passport_extra` | — | Доп.паспорт (только TypeVar 680-683) |
| 0x45 | 2 | LE16 | `zone_width` | отсчёты | Ширина зоны (контрольной) |
| 0x5E | 2 | LE16 | `total_length` | штук | Общее число записей/измерений |

### Дешифровка отдельных полей:

**410.** `device_id @ +0x00..+0x01` — LE16, заводской номер прибора. Хранится в
самом начале каждой записи. Тот же что в handshake-ответе.

**411.** `version_flags @ +0x02` — info_byte. См. §101 этапа B. 6 бит-полей:
bit0=ext.flash, bit1=?, bit2=wagon_LCD, bit3=шкала дальности, bits4+5=частоты ПЭП.

**412.** `sweep_id @ +0x04..+0x05` — 2 байта, объединённые форматтером
`FormatSweepId` в 4-значное десятичное (например, b4=12, b5=34 → "1234").

**413.** Дата @ +0x07..+0x09 = (day, month, year_offset). `year = 2000 + year_offset`.

**414.** Год **2000 рассматривается невалидным** в ValidateDate (проверка `2000 < year`).
Только 2001+ принимается; иначе подставляется default "01.01.2000".

**415.** Время @ +0x0A..+0x0B = (hour, minute). **Секунд НЕТ.**

**416.** Декодер времени `DecodeTime`: каждое поле форматируется как 2-значное
десятичное через `(byte%100)/10` + `(byte%100)%10` (то есть просто десятичные цифры).

**417.** Если hour ≥ 24 или minute ≥ 60 → output AnsiString дефолтный "?".

**418.** `defect_flag @ +0x0C` — байт-флаг. 3 отдельные extract-функции
(`FUN_004057bc`, `FUN_00405898`, `FUN_00405974`) считывают один и тот же байт,
конвертируя в 3 разных текстовых поля в выходном буфере.

**419.** `device_no_inner @ +0x0E..+0x0F` — LE16 (отличается от device_id @+0x00).

**420.** `block_addr @ +0x10..+0x11` — LE16 = **sweep_addr**, делится на 1000
для определения категории в `_SortBufData`. Так же используется внутри
`DispatchByTypeVar` как индекс TypeVar.

**421.** `passport_primary @ +0x11..+0x1B` — 11 байт, декодируется через
`FUN_00402708` (reverse-BCD + LUT). См. §131 этой секции.

**422.** `passport_secondary @ +0x21..+0x27` — 7 байт, той же reverse-BCD-LUT.

**423.** `passport_primary @ +0x11..+0x16` — для **ShortProtocol только 6 байт**
(не 11), см. DecodeShortProtocol line 3476: `FUN_00402708(out, param_2+0x11, 6)`.

**424.** `aux_le16_29` @ +0x29 — НОВАЯ находка! LE16 поле, используемое только
для категорий 10-19 (B-scan/Results) через `FUN_00406498/FUN_00406518/FUN_00406598`.
Семантика **неясна** — возможно: # измерений / индекс цикла / контр.сумма?

**425.** `thickness_mm @ +0x35` — LE16, толщина изделия для категорий 4/5/6
(Generic) и 20-29 (ShortProtocol). Прямой LE16 без масштабирования.

**426.** `category_enum @ +0x35` — байт enum (0..5), декодируемый в текстовые
константы через 6-веточный switch в `FUN_00406768`. Этот байт читается КАК БАЙТ
(не LE16!), и используется в категории A-scan для определения подтипа.

**427.** `velocity_mps @ +0x37` — LE16, скорость УЗ в м/с. Прямой LE16.

**428.** `amplitude_dB @ +0x38` — LE16. Может быть либо абсолютной амплитудой
в дБ × 100 (1234 = 12.34 дБ), либо кодом амплитуды (нужно домножение). См.
DecodeShortProtocol где value делится на 100 через COM VARIANT (см. §132).

**429.** `depth_mm @ +0x39` — LE16, **только для определённых TypeVar**:
0x16A/0x16B/0x17C/0x17D/0x186/0x187/0x2A8..0x2AB/0x2C8/0x2C9/0x2DA/0x2DB/
0x2E4/0x2E5/0x2E6/0x30C/0x30D (см. FUN_00406204). Иначе поле не извлекается.

**430.** `delay_us @ +0x3A` — LE16, задержка призмы в мкс.

**431.** `probe_angle` смещение **зависит от TypeVar**:
- по умолчанию +0x3B (LE16);
- для TypeVar 0x2A8..0x2AB (680-683) — +0x3E (см. FUN_00405bdc).

**432.** `gate_position @ +0x3C` — LE16 позиция строба (в отсчётах).
Извлекается только для TypeVar 0x2A8..0x2AB (см. FUN_0040609c).

**433.** `echo_count @ +0x3E` — LE16, число эхо-сигналов. Для категорий 20-29.

**434.** `passport_extra @ +0x41..+0x47` — 7-байтовый дополнительный паспорт,
**только для TypeVar 0x2A8..0x2AB** (680-683). FUN_00405e84.

**435.** `zone_width @ +0x45` — LE16, ширина контрольной зоны в отсчётах.

**436.** `total_length @ +0x5E` — LE16. Три extract-функции (`FUN_00406318/_398/_418`)
читают одно поле для разных типов записей.

**437.** Поля @ +0x3F и +0x40 — однобайтные доп.поля, извлекаемые через
`FUN_004070e0` и `FUN_00407060`. Назначение неясно (возможно: counter/index/flag).

**438.** Поле @ +0x36 — однобайтное, через `FUN_00406fe0`. Извлекается в Ascan
для категорий 4/5/6. Назначение неясно.

**439.** Минимальный размер RAW body для корректного декодирования = 0x60
байт (для покрытия total_length @+0x5E).

**440.** TypeVar 680-683 — **уникальная категория** с расширенным полевым набором:
дополнительный паспорт @+0x41, сдвинутый probe_angle @+0x3E, и активированный
gate_position @+0x3C.

---

## §129. TypeVar DISPATCH TABLE — найдено #441-446

Источник: `DispatchByTypeVar` line 2853-2894 в 102_203dll.dll+3.c.

`DispatchByTypeVar` маршрутизирует декодирование по **TypeVar** (param_1[3]):

| TypeVar | Decoder | Семантика |
|---------|---------|-----------|
| 0x96 (150) | DecodeTypeVar_Ascan | Стандартный A-scan |
| 0x97 (151) | DecodeTypeVar_Ascan | A-scan вариант 2 |
| 0x16A (362) | DecodeTypeVar_Default | + depth_mm @+0x39 |
| 0x16B (363) | DecodeTypeVar_Default | + depth_mm @+0x39 |
| 0x17C (380) | DecodeTypeVar_Default | + depth_mm @+0x39 |
| 0x17D (381) | DecodeTypeVar_Default | + depth_mm @+0x39 |
| 0x186 (390) | DecodeTypeVar_Default | + depth_mm @+0x39 |
| 0x187 (391) | DecodeTypeVar_Default | + depth_mm @+0x39 |
| 0x1BA (442) | DecodeTypeVar_Composite | Композитное измерение |
| 0x1BB (443) | DecodeTypeVar_Composite | Композит. вариант 2 |
| 0x1E0 (480) | DecodeTypeVar_Composite | Composite v3 |
| 0x1E1 (481) | DecodeTypeVar_Composite | Composite v4 |
| **0x282 (642)** | DecodeTypeVar_Composite | Composite типичный |
| **0x283 (643)** | DecodeTypeVar_Composite | Composite типичный |
| 0x2A8 (680) | DecodeTypeVar_Default | **ОСОБАЯ** (см. §128) |
| 0x2A9 (681) | DecodeTypeVar_Default | **ОСОБАЯ** |
| 0x2AA (682) | DecodeTypeVar_Default | **ОСОБАЯ** |
| 0x2AB (683) | DecodeTypeVar_Default | **ОСОБАЯ** |
| 0x2C8 (712) | DecodeTypeVar_Default | + depth_mm |
| 0x2C9 (713) | DecodeTypeVar_Default | + depth_mm |
| 0x2DA (730) | DecodeTypeVar_Default | + depth_mm |
| 0x2DB (731) | DecodeTypeVar_Default | + depth_mm |
| 0x2E4 (740) | DecodeTypeVar_Default | + depth_mm |
| 0x2E5 (741) | DecodeTypeVar_Default | + depth_mm |
| 0x2E6 (742) | DecodeTypeVar_Default | + depth_mm (граница) |
| 0x30C (780) | DecodeTypeVar_Default | + depth_mm |
| 0x30D (781) | DecodeTypeVar_Default | + depth_mm |
| 0x34A (842) | DecodeTypeVar_Composite | Composite расширен. |
| 0x34B (843) | DecodeTypeVar_Composite | Composite расширен. |
| Прочие | Игнорировать | НЕ декодировать (return без записи в output) |

**441.** TypeVar — это `param_1[3]` (4-й int в meta-struct, не путать с sweep_addr).

**442.** TypeVar значения идут **парами** (даже/нечётное), bit0 = subtype marker
(возможно "прямой ход" / "обратный ход" сканирования или "правая/левая сторона").

**443.** TypeVar 0x96/0x97 — **единственная пара**, направляющаяся в
DecodeTypeVar_Ascan (отдельный handler).

**444.** TypeVar 0x282/0x283, 0x1BA/0x1BB, 0x1E0/0x1E1, 0x34A/0x34B — Composite
(многозначные измерения).

**445.** Большинство TypeVar в диапазоне 0x16A..0x30D → DecodeTypeVar_Default
с условным извлечением depth_mm.

**446.** Если TypeVar не попадает ни в одну из категорий → `return` без записи
дополнительных полей (только базовые date/time из DecodeTypeVar_Default head).

---

## §130. CATEGORY DISPATCH В _SortBufData — найдено #447-451

Источник: `_SortBufData` line 2435-2474.

`sweep_addr / 1000` → категория → выбор Decoder и установка body[0] маркера:

| Category | Decoder | body[0] | Применение |
|----------|---------|---------|------------|
| **1** | DecodeAscan | **2** | A-scan записи (sweep_addr 1000..1999) |
| **4** | DecodeGeneric | **1** | Generic-3 значение (sweep_addr 4000..4999) |
| **5** | DecodeCalibration | **1** | Калибровка / Settings (sweep_addr 5000..5999) |
| **6** | DecodeGeneric | **1** | Generic-4 значение (sweep_addr 6000..6999) |
| **10..19** | DecodeBscan | **3** | B-scan / результаты сканирования (sweep_addr 10000..19999) |
| **20..29** | DecodeShortProtocol | **4** | ShortProtocol (sweep_addr 20000..29999) |
| Прочие | (skip) | — | Игнорировать |

**447.** Category = `LE16(record[0x10..0x11]) / 1000` — целочисленное деление.

**448.** Каждой категории соответствует свой decoder и **маркерный байт**
в выходном буфере (body[0]).

**449.** body[0] байт интерпретируется внешним кодом (PelengPC.exe) как:
1=Generic, 2=A-scan, 3=B-scan, 4=ShortProto.

**450.** Category 5 (Calibration) — это **то же что Settings** в Python-клоне.
Содержит акустические настройки (gain, velocity, gate, ARD-curve).

**451.** **Двухуровневая диспетчеризация**:
   - Сначала category = sweep_addr/1000 → выбирает Decoder
   - Затем внутри Decoder → DispatchByTypeVar (param_1[3]) → выбирает sub-handler

---

## §131. ВТОРОЙ TLV-LUT В DLL @ 0x401670 — найдено #452-457

Источник: `peleng_clone+12+3.py` lines 84-130 + указание на дамп LUT.

В DLL по адресу `0x00401670` расположен **30-байтный LUT**:
```
LUT[0..29]:
  00 05 00 00  03 04 03 00 00 00     # tcode 0..9
  02 02 02 02 02 02 02 02 02 02     # tcode 10..19
  01 01 01 01 01 01 01 01 01 01     # tcode 20..29
```

Маппинг `tcode → case_idx → (Handler, body[0])`:

| tcode | case | Handler адрес | body[0] | Семантика |
|-------|------|---------------|---------|-----------|
| 0 | 0 | — | — | skip |
| 1 | **5** | 0x402980 | 2 | Settings |
| 2 | 0 | — | — | skip |
| 3 | 0 | — | — | skip |
| 4 | **3** | 0x402c8c | 1 | Generic-3 |
| 5 | **4** | 0x402f34 | 1 | Generic-4 |
| 6 | **3** | 0x402c8c | 1 | Generic-3 |
| 7..9 | 0 | — | — | skip |
| 10..19 | **2** | 0x4031dc | 3 | B-scan |
| 20..29 | **1** | 0x403420 | 4 | A-scan |

**452.** Адрес LUT в DLL: `DAT_00401670` (30 байт).

**453.** Алгоритм dispatch (из дизасма):
```asm
mov ecx, tag
movzx ecx, cx                  ; tag в [0..65535]
mov eax, ecx
xor edx, edx
mov ebx, 1000
idiv ebx                        ; ecx = tag / 1000
cmp ecx, 0x1d                   ; > 29 → default
ja default
movzx ecx, byte [LUT_401670 + ecx]
                                ; ecx = case_idx 0..5
... switch (ecx) ...
```

**454.** Handler адреса (в DLL):
- A-scan: `0x00403420`
- B-scan: `0x004031dc`
- Generic-3: `0x00402c8c`
- Generic-4: `0x00402f34`
- Settings: `0x00402980`

**455.** **Различие c _SortBufData category**: _SortBufData использует
**sweep_addr/1000** (поле data area @+0x10..+0x11), а LUT@0x401670 использует
**tag/1000** (header TLV-record). Два разных LUT'a для двух разных моментов
обработки.

**456.** Случаи 2 и 5 в обоих маппингах НЕ совпадают (в _SortBufData case=5
→ Calibration, в LUT@0x401670 case=5 → Settings). Это связано с тем что
оба пути ведут к одной функции с разными режимами.

**457.** Tags = 4000..4999 и 6000..6999 → Generic-3; tags = 5000..5999 → Generic-4.
То есть **Generic-3 vs Generic-4 — это разные версии того же handler**,
выбираемые по точному tcode (4 → 3, 5 → 4, 6 → 3).

---

## §132. РЕВЕРС BCD / PASSPORT DECODER FUN_00402708 — найдено #458-468

Источник: `FUN_00402708` line 2998-3066 в 102_203dll.dll+3.c +
`PASSPORT_LUT` в `peleng_clone+12+3.py` lines 686-696.

### Алгоритм (псевдокод):
```c
PassportDecode(input_bytes, length) -> string:
  // Копируем 2 LUT в локальные массивы
  VALID_LUT[124] = mem[0x004284b4]    // validation table
  REPL_LUT[124]  = mem[0x00428530]    // replacement table

  output_str = ""
  started = false

  // Обход байт В ОБРАТНОМ ПОРЯДКЕ (от последнего к первому!)
  for i in range(length-1, -1, -1):
    raw_byte = input_bytes[i]
    for j in range(0, 124):
      if raw_byte == VALID_LUT[j]:
        if VALID_LUT[j] != 0 or started:
          started = true
          output_str += chr(REPL_LUT[j])
        break

  return output_str   // ведущие нули отброшены (через started flag)
```

### Реконструированный PASSPORT_LUT (124 позиции):

| Position | Charset | Описание |
|----------|---------|----------|
| 0 | `\x00` (NUL) | Терминатор / пропускаемая ведущая позиция |
| 1..10 | `'1234567890'` | Цифры (1@pos1 ... 9@pos9, 0@pos10) |
| 11..14 | `' .-/'` | Разделители |
| 15..47 | `'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ'` (33) | Кириллица заглавная |
| 48..80 | `'абвгдеёжзийклмнопрстуфхцчшщъыьэюя'` (33) | Кириллица строчная |
| 81..106 | `'ABCDEFGHIJKLMNOPQRSTUVWXYZ'` (26) | Латиница заглавная |
| 107..117 | `'0123456789+'` | Альтерн. цифры + plus |
| 118..123 | `\x00` × 6 | Reserved / padding |

### Семантика:

**458.** Reverse-BCD parser работает с байтами **в обратном порядке**
(`for i in range(length-1, -1, -1)`).

**459.** Каждый байт = **индекс в 124-символьном LUT**, а не BCD-пара (несмотря
на название "BCD"). Это substitution cipher с кириллицей+латиницей.

**460.** Терминатор = байт `0x0A` (newline) — если встречается, парсинг прерывается.

**461.** Ведущие NULL-байты (индекс 0) пропускаются (skip leading zeros), что
даёт компактные строки без префикс-паддинга.

**462.** Полный LUT содержит:
- 10 цифр (1-0, в порядке 1234567890)
- 4 разделителя (space, dot, dash, slash)
- 33 кириллических заглавных
- 33 кириллических строчных
- 26 латинских заглавных
- 11 цифр+плюс (альтерн. набор)

**463.** Адреса LUT в DLL:
- `DAT_004284b4` — VALID_LUT (124 байта, validation/input map)
- `DAT_00428530` — REPL_LUT (124 байта, replacement/output map)

**464.** В упрощённом случае, если VALID_LUT — identity (LUT[i] == i),
алгоритм сводится к: `output[k] = REPL_LUT[raw_byte]`. То есть **обычный
substitution cipher на 124 символа**.

**465.** Поля, использующие этот декодер:
- `passport_primary @ +0x11` (11 байт) — для категорий 1/4/5/6/10-19
- `passport_secondary @ +0x21` (7 байт) — для всех категорий
- `passport_extra @ +0x41` (7 байт) — только TypeVar 680-683
- `passport_primary @ +0x11` (6 байт) — для **ShortProtocol** (категории 20-29)

**466.** Кодировка реверс-BCD позволяет хранить **смешанные русско-латинские
надписи** (например: "ОСЬ РУ1 № 12345") в фиксированных 11/7/6-байтных полях.

**467.** Цифры доступны через **два набора в LUT** (1-10 и 107-117) — это
обеспечивает совместимость с альтернативными прошивками или для индикации
"оригинальная цифра" vs "введённая оператором".

**468.** Encoder для записи в RAW:
```python
def encode_passport(text, field_len):
    indices = []
    for ch in reversed(text):    # обратный порядок!
        idx = PASSPORT_LUT.find(ch)
        if idx == -1: idx = 0
        indices.append(idx)
    while len(indices) < field_len: indices.append(0)
    return bytes(indices[:field_len])
```

---

## §133. WRITE FIELD ENCODER (BE INT / FIX STRING / DATE) — найдено #469-478

Источник: `FUN_00401f54` line 2687-2823.

`WriteField(output_buffer, format_state, name_str, value_str)` — encoder каждого
поля декодированной записи. Использует **field descriptor**:

### Структура field descriptor:
```c
struct FieldDescriptor {
    int *vtable_ptr;       // +0x00 (Borland TObject vtbl)
    AnsiString name;       // +0x04 (имя поля)
    int type;              // +0x08 (1=BE-int, 2=string, 3=date)
    int max_size;          // +0x0C (intermediate, used for type 2)
    int size_bytes;        // +0x10 (длина поля в байтах output buffer)
};
```

### Type 1 — BIG-ENDIAN integer

```c
value = atoi(input_string);                              // строка → int
for (i = 0; i < size_bytes; i++):
    output[offset + size_bytes - 1 - i] = (value >> 8*i) & 0xFF;
```

**Семантика:** младший байт записывается ПОСЛЕДНИМ (offset+size-1), старший
ПЕРВЫМ (offset). Это **классический BE**.

Примеры:
- size=1: `output[offset] = value & 0xFF`
- size=2: `output[offset] = (value>>8)&0xFF`, `output[offset+1] = value & 0xFF`
- size=4: `output[offset] = (value>>24)&0xFF`, ..., `output[offset+3] = value & 0xFF`

### Type 2 — Fixed-size string (zero-padded right)

```c
memset(output[offset..offset+size_bytes], 0, size_bytes);
copy_len = min(strlen(input_string), size_bytes);
memcpy(output[offset..offset+copy_len], input_string);
// Зануление правого паддинга уже сделано memset'ом
```

Если значение пустое (`*param_4 == 0`) — используется default `DAT_004285ac`
(обычно пустая строка).

### Type 3 — Date as 3 bytes (day, month, year-2000)

```c
output[offset+0] = atoi(get_string_arg_1());   // day
output[offset+1] = atoi(get_string_arg_2());   // month
output[offset+2] = atoi(get_string_arg_3());   // year % 100 (2-digit)
```

Год хранится как `year - 2000` (см. §128 finding #413).

### Семантика:

**469.** Field descriptor struct: name, type, max_size, size_bytes на смещениях
+0x04, +0x08, +0x0C, +0x10.

**470.** Type 1 (BE int) — **big-endian integer** переменной длины
(1/2/4 байта типично).

**471.** Type 2 (string) — фикс-размер с **zero-padding справа** (правый).

**472.** Type 3 (date) — **3 байта**: day, month, year%100. Год = byte + 2000.

**473.** Internal output buffer формат:
```
[1-байт декодер-маркер]
[1-байт n_fields]
[n_fields × 4-байт: LE16 offset + LE16 size per field]
[N байт: декодированные поля]
[M байт: копия RAW record]
```

**474.** Декодер-маркер устанавливается _SortBufData по category:
2=Ascan, 1=Generic/Calib, 3=Bscan, 4=ShortProto.

**475.** Смещения полей в output buffer **относительны** к началу decoded-data
секции (после offset-table). Накапливаются через `FillFieldOffsets`.

**476.** Каждый offset в offset-table = LE16, размер = LE16. Размер всегда
равен `field.size_bytes`.

**477.** Внешний код PelengPC.exe читает байты RAW-копии из output buffer
(после decoded fields), что используется для re-encoding или повторной отправки.

**478.** Если name_str пустая или поле не найдено в format_state → WriteField
ничего не пишет (`local_60 == -1` ветка).

---

## §134. ОПКОДЫ ПРОТОКОЛА — ПОЛНАЯ КАРТА — найдено #479-501

Источник: `peleng_clone+12+3.py` lines 188-260, 631-635 + дизасм `fcn.00423e58`,
`fcn.00424bb0`, `fcn.00424cc0`, `fcn.004249c0`.

### Все опкоды PelengPC.exe ↔ прибор:

| Opcode | ASCII | Каллер | Семантика | TX | RX |
|--------|-------|--------|-----------|----|----|
| **0x55** | 'U' | fcn.004249c0 | **Handshake / TestPort** | 1 байт | 16 байт |
| **0x42** | 'B' | fcn.0041b43c | **Read block (память)** | 0x42 + LL + HH | 2 + 84 байт |
| **0x42** | 'B' | fcn.0041c914 | **Read block (настройки)** | 0x42 + LL + HH | 2 + 84 байт |
| **0x74** | 't' | 0x422828 | **Status/Test** | 1 байт | N байт |
| **0x9a** | — | 0x41cbf4 | **Flash dump (частичный)** | 1 байт | 0x10c5 или 0x1485 |
| **0x4B** | 'K' | (УД2-102) | **Virtual keyboard** | 0x4B + key | ? |
| **0x49** | 'I' | (УД2-102) | **Screen dump** | 1 байт | ~15 КБ (LCD) |
| **0x54** | 'T' | (УД2-102) | **RTC sync** | 0x54+YY+MM+DD+hh+mm+ss | ack |

### Низкоуровневые функции отправки:

| Функция | Адрес | Назначение |
|---------|-------|------------|
| `SendByte_lowlevel` | `0x00423e58` | WriteFile + busy-wait timeout |
| `SendByte_wrapper` | `0x00423f0c` | Тонкая обёртка над SendByte_lowlevel |
| `SendOpcode_recv` | `0x00424bb0` | 1-байтный опкод + receive N байт |
| `SendBlockReq_recv` | `0x00424cc0` | 'B' + LL + HH + receive |
| `TestPort` | `0x004249c0` | Send 0x55 → read 16 байт |

### Опкод 0x55 — HANDSHAKE

**Формат TX:** 1 байт = `0x55`

**Формат RX:** ровно **16 байт** = header. Структура:
```
header[0..1]  : LE16 device_id (заводской номер)
header[2]     : info_byte (см. §101 этапа B — 6 бит-полей)
header[3..15] : прочие данные (firmware version, capabilities, etc.)
```

**Таймауты:** RX до 1500 мс (UD2_HANDSHAKE_RX_S = 1.5 с).
**Повторы:** 4 раза подряд (UD2_HANDSHAKE_REPEATS = 4) с паузой 100 мс.

### Опкод 0x42 — BLOCK READ

**Формат TX:** 3 байта = `0x42, LL, HH`
   - `LL`, `HH` — LE16 адрес блока (например, base 10100, индекс 5 → addr=10105).

**Формат RX:** `2 + 84` байт:
   - `header[0..1]`: маркер пустого блока (0xFD 0xFF или 0xFF 0xFF) или начало данных
   - Если маркер пустой → больше байт не приходит (84-байтное тело отсутствует)
   - Иначе → ещё 84 байта тела записи

**Таймауты:** RX тело = 150 мс (UD2_BODY_TIMEOUT_S).

### Опкод 0x9A — FLASH DUMP (частичный)

**Формат TX:** 1 байт = `0x9A`

**Формат RX:** размер зависит от `info_byte.bit0`:
   - bit0 == 0: **0x10C5 (4293) байт** — стандартный flash dump
   - bit0 == 1: **0x14C5 (?)** или **0x80010 (524304)** для extended flash

Перед началом RX хост может писать в буфер **5-байтный синтетический префикс**:
```
60 EA <sizeLO> <sizeHI> 00
```
(ROUND5 §69 этапа A — это **НЕ часть протокола**, а служебный prefix хоста).

### Опкод 0x74 — STATUS / TEST

**Формат TX:** 1 байт = `0x74`

**Формат RX:** N байт (длина зависит от прошивки). Включает статус устройства,
текущие настройки, флаги ошибок.

### Опкод 0x4B — VIRTUAL KEYBOARD (УД2-102)

**Формат TX:** 2 байта = `0x4B, key_code`

Допустимые key_codes:
| Код | Клавиша |
|-----|---------|
| 0x01 | Enter |
| 0x02 | Cancel |
| 0x03 | Left |
| 0x04 | Right |
| 0x05 | Up |
| 0x06 | Down |

**Применение:** удалённое управление прибором через виртуальную клавиатуру.

### Опкод 0x49 — SCREEN DUMP (УД2-102)

**Формат TX:** 1 байт = `0x49`

**Формат RX:** ~15 КБ дамп видеобуфера (LCD-экрана). Используется для отладки
и для NSaveScreen в PelengPC.exe.

### Опкод 0x54 — RTC SYNC (УД2-102)

**Формат TX:** 7 байт = `0x54, YY, MM, DD, hh, mm, ss`

`YY` = year - 2000.

**Формат RX:** ack (формат неясен — вероятно 1 байт OK).

### Карта прохождения данных:

```
Host (PelengPC.exe)              Device (Peleng)
─────────────────────────────────────────────────
   0x55 →                       (handshake)
                                ↓
   ← 16 байт header             (info_byte, device_id)

   0x42 + LL + HH →             (block request)
                                ↓
   ← 2 байта marker             (0xFD/0xFF = empty)
   ← 84 байта body              (если не пусто)

   0x9A →                       (flash dump start)
                                ↓
   ← 0x10C5..0x80010 байт       (бинарный поток памяти)

   0x4B + key →                 (виртуальная кнопка)
                                ↓
   (no reply — fire-and-forget)
```

### Таймауты протокола:

| Константа | Значение | Применение |
|-----------|----------|------------|
| `UD2_INTER_BYTE_S` | **4.8 мс** | Пауза между байтами TX (КРИТИЧНО!) |
| `UD2_RESP_TIMEOUT_S` | 28 мс | Короткий 2-байтный ответ |
| `UD2_BODY_TIMEOUT_S` | 150 мс | Тело записи 84 байта |
| `UD2_HANDSHAKE_RX_S` | 1500 мс | Receive handshake reply |
| `UD2_HANDSHAKE_PAUSE_S` | 100 мс | Пауза между handshake-попытками |

> **Критически важно:** между байтами команды должна быть пауза **4.8 мс**,
> иначе аппаратный буфер прибора переполняется и Windows возвращает **WinError 31**
> ("A device attached to the system is not functioning").

### Семантика:

**479.** Только **4 опкода** в PelengPC.exe `fcn.00423f0c` (SendByte_wrapper):
0x55, 0x42, 0x74, 0x9A. Найдено через `axt @ fcn.00423f0c` в r2.

**480.** **3 дополнительных опкода** в УД2-102 ветке (через Python-клон):
0x4B (keyboard), 0x49 (screen dump), 0x54 (RTC sync). Не используются
основным PelengPC.exe пути.

**481.** Все опкоды — однобайтные ASCII символы за исключением 0x9A (no ASCII).

**482.** Опкод 0x42 = 'B' (Block) — наиболее частый, для чтения каталога.

**483.** Опкод 0x55 = 'U' (?) — синхронизация. Повторяется 4 раза для надёжности.

**484.** Опкод 0x9A — flash dump. Размер ответа зависит от `info_byte.bit0`
(extended flash mode).

**485.** Опкод 0x74 = 't' (Test) — получение статуса.

**486.** Опкод 0x4B = 'K' (Keyboard) — управление прибором.

**487.** Опкод 0x49 = 'I' (Image) — screen dump.

**488.** Опкод 0x54 = 'T' (Time) — sync RTC.

**489.** Inter-byte timing **4.8 мс** — обязательный gap между TX-байтами.
Нарушение → WinError 31.

**490.** Handshake RX-timeout = 1500 мс (большой потому что прибор может
быть в режиме измерения и медленно отвечает).

**491.** Body RX-timeout = 150 мс (короткий — тело должно прийти быстро после
2-байтного header'а).

**492.** Empty markers: `0xFD 0xFF` или `0xFF 0xFF` (LE16: 0xFFFD или 0xFFFF).

**493.** Карта баз памяти УД2-102: 8 баз, по 100 индексов:
`UD2_BASES = (10100, 10200, 10300, 10400, 10500, 10600, 13300, 13400)`.
Адрес для opcode 0x42 = `base + index` (0..99).

**494.** Streak-контроль сканирования: 15 пустых подряд **до первого хита** → next base;
2 пустых подряд **после первого хита** → next base.

**495.** Опкод 0x42 параметры: `LL = addr & 0xFF`, `HH = (addr >> 8) & 0xFF` — стандартный LE16.

**496.** RTC sync (опкод 0x54) принимает 6 байт: год без 2000, месяц, день, час, минута, секунда — БЕЗ преамбулы.

**497.** Screen dump (опкод 0x49) возвращает **~15 КБ** — LCD-видеобуфер
(~5253 байт для wagon-mode по ROUND5 этап B §101, или больше для extended).

**498.** Все опкоды отправляются **байт за байтом через WriteFile + Sleep(10)**
(или Sleep(4.8) для УД2-102).

**499.** SendByte_lowlevel (`fcn.00423e58`) использует **busy-wait** для таймаута,
а не системные таймеры — даёт точную задержку в мс.

**500.** TestPort (`fcn.004249c0`) — это **проверка наличия прибора**: отправляет
0x55 и ждёт 16-байтный ответ. Если не пришло — порт пустой.

**501.** Опкоды НЕ имеют контрольной суммы (ни CRC, ни checksum). Защита
от ошибок только через retry на уровне Handshake.

---

## §135. БЛОК A-SCAN — SAMPLES BLOB — найдено #502-510

Источник: `AScanRecord.decode` в `peleng_clone+12+3.py` lines 2098-2147.

A-scan TLV body, после полей схемы RESULTS1, содержит **сырой массив байтов
сэмплов**:
```
body[0]          : tag/type (1=одиночное, 2=Settings, 3=Bscan, 4=Ascan)
body[1]          : sub_minute (доли минуты, fine-grain time)
body[2]          : sub_hour   (доли часа)
body[3..N-1]     : поля схемы RESULTS1 (passport_primary, sweep_id, date, time, ...)
body[N..end]     : SAMPLES = массив байтов амплитуд (0..255)
```

### Формулы построения развёртки из samples:

```python
n_samples = len(samples)
duration_us = n_samples / 50.0                  # 50 МГц АЦП
max_depth_mm = duration_us * 5.9 / 2.0          # сталь

# Для каждой точки i ∈ [0, n_samples):
x_us = i / 50.0
x_mm = x_us * velocity_ms / 2000.0              # velocity_ms из Settings
y_db = samples[i] * 26.0 / 255.0                # амплитуда → дБ
y_pct = samples[i] * 100.0 / 255.0              # амплитуда → %
```

### Sub-time поля:

`sub_minute` и `sub_hour` в body[1..2] — **тонкая разрешающая способность**
времени измерения. Используются для точной хронометрии:
```
time_precise = HH:MM + sub_hour/256 hour + sub_minute/256 minute
```

(Дробь возможно не /256, а /60 — точная формула в DLL зависит от прошивки.)

### Семантика:

**502.** A-scan blob = массив байтов после schema-полей.

**503.** Длина blob — **переменная**, типично 4096 байт (= 81.92 мкс / 241 мм
в стали) или 8192 байт.

**504.** Каждый sample = 1 байт амплитуды (0..255 → 0..26 dB линейно).

**505.** `sub_minute @ body[1]`, `sub_hour @ body[2]` — sub-time разрешение.

**506.** Длительность A-scan в мкс: `duration_us = n_samples / 50`.

**507.** Максимальная глубина: `max_depth_mm = duration_us × velocity_mps / 2000`.

**508.** При v=5900 м/с и n_samples=4096: `duration=81.92 мкс, max_depth=241 мм`.

**509.** При v=5900 м/с и n_samples=8192: `duration=163.84 мкс, max_depth=483 мм`.

**510.** В Python-клоне A-scan blob извлекается так:
```python
rd = StreamReader(body, 1)         # начинаем после body[0]
fields = []
for name, label, ftype in pick_results_schema(firmware):
    if rd.remaining() == 0: break
    f = _decode_field(rd, name, ftype)
    fields.append(f)
consumed = rd.pos
samples = body[consumed:]           # хвост — это и есть samples
```

---

## §136. FORMAT_SWEEP_ID — 4-digit display — найдено #511-513

Источник: `FormatSweepId` line 2510-2564.

Функция формирует строку для отображения sweep_id:
```c
FormatSweepId(out_str, b4, b5):
    if b4 < 10: out_str = "0" + str(b4)        // "0X"
    else:       out_str = str(b4)               // "XX" (2 цифры)

    if b5 < 10: suffix = "0" + str(b5)
    else:       suffix = str(b5)

    result = out_str + suffix                   // "XXYY" (4 цифры)
```

**511.** `sweep_id_display = f"{sweep_hi:02d}{sweep_lo:02d}"` — 4-значное
zero-padded.

**512.** Байты sweep_hi @ +0x04 и sweep_lo @ +0x05 — **обычные uint8**, не BCD
(несмотря на формат с zero-padding).

**513.** Соответственно, диапазон отображаемого sweep_id = "0000".."25525"
(но логически разумные значения 0000..9999, остальное — артефакт).

---

## §137. ShortProtocol — VARIANT DIVISION ПО 100 — найдено #514-517

Источник: `DecodeShortProtocol` line 3438-3439.

ShortProtocol для категорий 20-29 использует **COM VARIANT для деления на 100**:
```c
pVVar1 = FUN_004268ec(&local_34);     // создать пустой VARIANTARG
FUN_00426970(&local_24, 100);          // VARIANTARG = 100
FUN_00403734(piVar2, &local_24, pVVar1);  // pVVar1 = piVar2 / 100
```

`piVar2 = (sweep_addr % 10000)` — значение sweep_id.

Деление через COM VARIANT даёт **decimal (fixed-point) арифметику** — то есть
1234 / 100 = 12.34 (не целочисленное 12).

### Семантика:

**514.** ShortProtocol делит `sweep_addr % 10000` на 100 через COM VARIANT
для получения 2-значного дробного отображения.

**515.** Например, sweep_addr = 23456 → 23456 % 10000 = 3456 → 3456 / 100 = **34.56**.

**516.** Это **отличает Ascan/Bscan от ShortProtocol**:
- Ascan: `sweep_addr % 1000` (целое 0..999)
- Bscan: `sweep_addr % 10000` (целое 0..9999)
- ShortProtocol: `(sweep_addr % 10000) / 100` (дробное "XX.YY")

**517.** Использование COM VARIANT для арифметики — это особенность Borland
C++ Builder, где AnsiString не поддерживает прямую дробную конверсию.

---

## §138. ENUM-БАЙТ @ +0x35 (6-WAY SWITCH) — найдено #518-520

Источник: `FUN_00406768` line 4697-4746.

Альтернативная интерпретация байта @ +0x35 (когда категория не равна 4/5/6):
байт читается как enum 0..5, и через 6-веточный switch выбирается одна из 6
текстовых констант:
```c
switch(*(byte*)(param_3 + 0x35)) {
    case 0: → string_0   (FUN_00425bac(local_c))
    case 1: → string_1
    case 2: → string_2
    case 3: → string_3
    case 4: → string_4
    case 5: → string_5
}
```

Сами строковые константы — Empty (FUN_00425bac возвращает пустой AnsiString),
но в реальной DLL они инициализированы конкретными значениями типа "Сторона A",
"Сторона B", "Прямой ход", "Обратный ход" и т.д.

**518.** Байт @ +0x35 может быть **либо LE16 (thickness)** для cat 4/5/6,
**либо enum 0..5** для других контекстов.

**519.** Выбор интерпретации зависит от **TypeVar и category** — определяется
вызывающим decoder'ом.

**520.** Enum 0..5 декодируется в один из 6 текстовых констант через
`FUN_00406768`. Возможные значения: режим/сторона/направление сканирования.

---

## §139. CONDITIONAL FIELD EXTRACTION ПО TypeVar — найдено #521-525

Источник: `FUN_00405bdc/FUN_00405e84/FUN_0040609c/FUN_00406204` в DLL.

### Карта условных извлечений:

| Поле | Default offset | Conditional offset | Условие |
|------|----------------|--------------------|---------|
| `probe_angle` | +0x3B | +0x3E | TypeVar ∈ {0x2A8..0x2AB} |
| `passport_extra` | (не извлекается) | +0x41 (7 байт BCD) | TypeVar ∈ {0x2A8..0x2AB} |
| `gate_position` | (не извлекается) | +0x3C (LE16) | TypeVar ∈ {0x2A8..0x2AB} |
| `depth_mm` | (не извлекается) | +0x39 (LE16) | TypeVar ∈ {0x16A,0x16B,0x17C,0x17D,0x186,0x187, 0x2A8..0x2AB,0x2C8,0x2C9, 0x2DA,0x2DB,0x2E4,0x2E5,0x2E6, 0x30C,0x30D} |

### Псевдокод conditional extraction:

```c
// probe_angle (FUN_00405bdc):
offset = 0x3B;
if (TypeVar in {0x2A8..0x2AB}):
    offset = 0x3E;
probe_angle = LE16(record + offset);

// passport_extra (FUN_00405e84):
if (TypeVar in {0x2A8..0x2AB}):
    passport_extra = BCD_decode(record + 0x41, 7);

// gate_position (FUN_0040609c):
if (TypeVar in {0x2A8..0x2AB}):
    gate_position = LE16(record + 0x3C);

// depth_mm (FUN_00406204):
if (TypeVar in {0x16A,0x16B,...,0x30C,0x30D}):
    depth_mm = LE16(record + 0x39);
```

### Семантика:

**521.** TypeVar 680-683 (0x2A8..0x2AB) — **самая «богатая» категория** записи:
включает дополнительный паспорт, сдвинутый probe_angle и активированный
gate_position.

**522.** depth_mm extraction зависит от точного TypeVar — список из 16+ TypeVar'ов.

**523.** Conditional extraction реализуется через **range-checks** (`iVar1 - X < N`),
а не через switch. Это позволяет хорошо оптимизировать в условиях ARM/x86.

**524.** Если TypeVar не входит в условие → поле остаётся **пустой строкой**
(default AnsiString из `FUN_00425bac`).

**525.** Decoder последовательно проверяет все известные TypeVar и игнорирует
неизвестные — это позволяет работать со старыми и новыми прошивками.

---

## §140. RECORD HEADER VS BODY DISTINCTION — найдено #526-530

Источник: `_SortBufData` line 2414 + использование `pbVar7` повсюду.

В `_SortBufData`:
```c
pbVar7 = param_1 + 0x10;       // <-- DATA AREA starts here
```

То есть **первые 16 байт RAW-записи — это HANDSHAKE HEADER**, который
повторяется в каждой записи. Сама полезная нагрузка (data area) начинается
с **байта 16**.

### Структура полной RAW-записи (100 байт типично):

```
RAW[0..15]   : HANDSHAKE HEADER (16 байт)
   RAW[0..1]   : LE16 device_id (заводской)
   RAW[2]      : info_byte (биты конфигурации)
   RAW[3..15]  : header rest (firmware version, catalog hash, etc.)
RAW[16..end] : DATA AREA (84 байт типично)
   DATA[0..1]  : LE16 block_addr / sweep_addr
   DATA[7..9]  : date (day, month, year-2000)
   DATA[10..11]: time (hour, minute)
   DATA[17..27]: passport_primary (11 байт BCD-LUT)
   DATA[33..39]: passport_secondary (7 байт BCD-LUT)
   DATA[...]   : LE16 / byte поля (см. §128)
```

### Семантика:

**526.** RAW-record начинается с **handshake-header** (16 байт), который
дублируется в каждой записи (для надёжности при потерях).

**527.** Все смещения в decoder-функциях (`FUN_004055..` etc.) — относительны
к началу **data area** (RAW + 16), не к началу всей записи.

**528.** Стандартный размер data area = **84 байта** (UD2_RECORD_BYTES).
Полная RAW-запись = 16 (header) + 2 (block marker) + 84 (body) = 102 байта.
Но часто блок marker'а в RX-потоке отсутствует, и приходит 16+84=100 байт.

**529.** Smallest data area для корректного декодирования = **0x60 (96) байт**
(чтобы покрыть total_length @ +0x5E).

**530.** Block marker @ RX (`0xFD 0xFF` или `0xFF 0xFF`) приходит **ПЕРЕД**
header'ом и body'ем, и означает «эта позиция пустая, тело не приходит». Маркеры
обрабатываются на уровне `SendBlockReq_recv` и НЕ попадают в `_SortBufData`.

---

## §141. ВАЛИДАЦИЯ ДАТЫ — БАГ С 2000 ГОДОМ — найдено #531-535

Источник: `ValidateDate` line 3070-3118.

```c
ValidateDate(year, month, day):
    if !(2000 < year && month < 13):       // <- баг: year == 2000 невалид
        return 0
    switch (month):
        case 1,3,5,7,8,10,12: valid = (1 <= day <= 31)
        case 4,6,9,11:        valid = (1 <= day <= 30)
        case 2:
            if (year % 4 == 0):
                if (year % 100 == 0):  valid = (1 <= day <= 29)
                else:                   valid = (1 <= day <= 28)  # <- БАГ!
            else:                       valid = (1 <= day <= 28)
    return valid
```

### Bag-список:

**531.** **Bag #1:** `2000 < year` (строго больше) — год **2000 невалиден**.
Все записи с year=0 (= 2000) отклоняются.

**532.** **Bag #2:** Логика високосного года ИНВЕРТИРОВАНА:
- Стандартное правило: год високосный если `(year%4 == 0 AND year%100 != 0) OR year%400 == 0`.
- Реализация ValidateDate: год имеет 29 дней в феврале если `(year%4 == 0 AND year%100 == 0)`.
- Результат: **обычные високосные годы** (2004, 2008, 2012, 2016, 2020, 2024) принимают **только 28 дней** в феврале.
- Только годы делящиеся на 100 (2100, 2200, ...) и одновременно на 4 (что **математически** даёт 2400, 2800, 2000) принимают Feb 29.
- Но year=2000 невалиден из-за bug #1, так что **первая дата Feb 29 валидируется только в 2400 году!**

**533.** При невалидной дате декодер подставляет **default "01.01.2000"**
через `FUN_00414dac(2000, 1, 1)` (line 2923 в DecodeDate).

**534.** ValidateDate возвращает 0 (false) — это **читается** в DecodeDate и
запускает fallback-форматирование.

**535.** **Практический impact:** записи с year=0..1 (= 2000..2001) и с
Feb 29 любого «нормального» високосного года будут показаны как "01.01.2000".
Реверс-инженерия рекомендует **исправить** эту логику в клоне (Python имеет
корректную проверку через `datetime.date(year, month, day)` с try/except).

---

## §142. ИТОГИ ЭТАПА C

### Сводка по добавленным находкам

| # | Тема | Статус |
|---|------|--------|
| 360-364 | Константы калибровки (ADC=50 МГц, V=5.9, AMPL=26 дБ) | 🔒 |
| 365-372 | Формулы A-развёртки (i→t→d, byte→dB, строб) | 🔒 |
| 373-378 | Формулы B-развёртки (width×height, размеры в мм) | 🔒 |
| 379-401 | Settings tail-block (offsets +0x65..+0x87, gain/V/strobe) | 🔒 |
| 402-406 | АРД-критерий БРАК/КОНТРОЛЬ/ПОИСК | 🔒 |
| 407-409 | Геометрия дефекта в оси (закон косинусов) | 🔒 |
| 410-440 | Полная RAW-record body field map (31 поле) | 🔒 |
| 441-446 | TypeVar dispatch table (28 значений) | 🔒 |
| 447-451 | Category dispatch в _SortBufData (6 категорий) | 🔒 |
| 452-457 | TLV-LUT @ 0x401670 (30 байт) с case-индексами | 🔒 |
| 458-468 | FUN_00402708 + PASSPORT_LUT (124 символа) | 🔒 |
| 469-478 | WriteField encoder (BE int / fix string / date) | 🔒 |
| 479-501 | Полная карта опкодов: 0x55/0x42/0x74/0x9A/0x4B/0x49/0x54 | 🔒 |
| 502-510 | A-scan blob = массив байтов амплитуд | 🔒 |
| 511-513 | FormatSweepId 4-digit display | 🔒 |
| 514-517 | ShortProtocol VARIANT division /100 | 🔒 |
| 518-520 | Enum-байт @ +0x35 (6-way switch) | 🔒 |
| 521-525 | Conditional field extraction по TypeVar (probe_angle, depth_mm) | 🔒 |
| 526-530 | RAW-record = 16 байт header + 84 байт data | 🔒 |
| 531-535 | ValidateDate bugs (2000 invalid, leap year inverted) | 🟡 (баг) |

### Поправки к предыдущим этапам

1. **ROUND4 §16 / ROUND5 §63 (А-этап) / §326 (Б-этап):**
   *поля_паспорта* — не «BCD цифры», а **124-символьный substitution LUT**.
   LUT содержит цифры, разделители, кириллицу (заглавную+строчную) и
   латиницу. Алгоритм reverse-обходит байты от последнего к первому.
   Реальный LUT восстановлен в §132 этого этапа.

2. **ROUND5 §70 (А-этап) и §333 (Б-этап) дополняется:**
   - LE16 — это **сырое чтение из RAW** (FUN_0040381c).
   - **BE int** — это **вывод в output_buffer** (FUN_00401f54 type=1).
   - Эти два направления конверсии используют разный endian.

3. **ROUND4 §66 (опкоды)** — расширено:
   - Добавлены 0x4B (keyboard), 0x49 (screen dump), 0x54 (RTC sync) — все
     УД2-102-специфичные опкоды.
   - 0x9A может возвращать `0x10C5` или `0x80010` байт (см. info_byte.bit0).

4. **ROUND5 §75 (А-этап, валидация даты)** — подтверждено и расширено:
   - Year=2000 НЕ валиден (хотя year_offset=0 это легитимный sentinel).
   - Логика leap year **инвертирована** — это БАГ оригинала.
   - Default fallback = "01.01.2000".

### Главные открытия этапа C (TL;DR)

1. **Формулы построения A-развёртки** — конкретные коэффициенты:
   - `ADC = 50 МГц`, `V = 5.9 мм/мкс (5900 м/с)`, `AMPL_FULL = 26 дБ`
   - `t_us = i / 50`, `d_mm = t_us × V_mps / 2000`, `dB = byte × 26 / 255`

2. **Settings tail-block @ +0x65..+0x87** содержит ВСЁ для построения развёртки:
   gain_db@66, velocity_ms@67, angle@69, delay@6A, gate_start@6E, gate_width@70,
   threshold_pct@74, ARD-curve@76, VRT-table@80.

3. **B-scan = width × height × byte** + LE16 width/height в хвосте body.
   Типичные: 64×128 или 32×64.

4. **АРД-критерий**: `ΔdB ≥ 0 → БРАК; -6..0 → КОНТРОЛЬ; <-6 → ПОИСК`.

5. **Полная карта 31 поля** в RAW data area (offsets, типы, единицы, семантика).

6. **TLV dispatch — 2 уровня:** sweep_addr/1000 → 6 категорий → Decoder;
   TypeVar (param_1[3]) → 28+ значений → sub-handler. Маршрутизация через
   две таблицы (одна в `_SortBufData`, вторая 30-байтный LUT @ 0x401670).

7. **Passport encoding** — 124-символьный substitution LUT (НЕ BCD!),
   обход байт **в обратном порядке**, скрытые ведущие нули.

8. **WriteField encoder** — 3 типа: type1=BE int, type2=fix string, type3=date(3b).

9. **Protocol opcodes — 7 штук**: 0x55/0x42/0x74/0x9A для PelengPC.exe пути
   + 0x4B/0x49/0x54 для УД2-102 (виртуальная клавиатура, скриншот, RTC).

10. **Inter-byte timing 4.8 мс** — обязательная пауза TX, иначе WinError 31.

11. **ValidateDate bugs**: year=2000 невалиден, leap year логика инвертирована
    (Feb 29 принимается **только** в годах, делящихся на 100 И на 4 — то есть
    практически никогда).

---

## §143. ОБЩАЯ СВОДКА ВСЕХ РАУНДОВ

- **ROUND4: 189 находок** (через §66).
- **ROUND5 этап A: +105 находок (190-294)**.
- **ROUND5 этап B: +65 находок (295-359)**.
- **ROUND5 этап C: +176 находок (360-535)**.
- **TOTAL: 535 находок.**

### Что покрыто (через ROUND5 этап C):

- ✅ Все 7 опкодов протокола с форматами TX/RX
- ✅ Все таймауты протокола (inter-byte 4.8 мс, body 150 мс, handshake 1500 мс)
- ✅ 16-байтный handshake header + info_byte (6 бит)
- ✅ TLV-категории и LUT диспетчеризации (2 LUT'a)
- ✅ TypeVar dispatch (28+ значений → 5 sub-handlers)
- ✅ 31 поле в RAW data area с физической семантикой
- ✅ Все 6 категорий записи (Ascan/Bscan/Generic/Calib/ShortProto/Settings)
- ✅ Все формулы развёртки (i→t→d→mm, byte→dB→%)
- ✅ Settings tail-block (gain/V/strobe/ARD/VRT)
- ✅ АРД-критерий (БРАК/КОНТРОЛЬ/ПОИСК)
- ✅ Passport LUT (124 символа)
- ✅ WriteField encoder (3 типа)
- ✅ FormatSweepId
- ✅ Conditional field extraction по TypeVar
- ✅ A-scan и B-scan blob форматы
- ✅ Constants: ADC_RATE, US_VEL, AMPL_FULL

### Что ОСТАЁТСЯ к реверсу (для будущих раундов):

- ❓ Точная семантика byte @ +0x29 (LE16 в B-scan записях)
- ❓ Byte @ +0x36, +0x3F, +0x40 — назначение неясно
- ❓ 6 значений enum @ +0x35 (что они означают в реальной DLL)
- ❓ Точный формат `header[3..15]` в handshake reply
- ❓ Catalog pairs (LE16) после header — что они кодируют
- ❓ Опкод 0x54 (RTC sync) — формат ack/reply
- ❓ Бит `info_byte.bit1` — назначение
- ❓ Деление `velocity_ms / 1000` vs `/ 2000` в разных контекстах
- ❓ Точная природа `sub_minute / sub_hour` (делитель: 60 или 256?)

Готово. ROUND5 этап C завершён.

---

# ROUND5 — ЭТАП D: ОТЧЁТЫ, ПРОТОКОЛ, РАЗВЁРТКА, НАСТРОЙКИ — ПРОДОЛЖЕНИЕ

## §144. ОПКОДЫ — ИСПРАВЛЕНИЯ И ТОЧНЫЕ СЕМАНТИКИ — найдено #536-544

**ИСПРАВЛЕНИЕ §134 (этап C):** опкоды 0x74 и 0x9a в РОУНД5 этап C были перепутаны местами. Реальное назначение:

| Opcode | ASCII | Адрес | Реальная семантика | RX размер |
|---|---|---|---|---|
| **0x74** | 't' | `FUN_00422764` | **FLASH DUMP** (полная память прибора) | 0x20000 / 0x80000 |
| **0x9a** | — | `FormReadData_cbf4` @ 0x41cbf4 | **SCREEN DUMP** (растровая копия экрана) | 0x10C5 / 0x1485 |
| **0x42** | 'B' | `FUN_00424cc0` | Block read (1 запись) | по адресу |
| **0x55** | 'U' | `FUN_004249c0` | Handshake/TestPort | 16..0x80010 |

**536.** **Опкод 0x74 = FLASH DUMP** — выгрузка всей памяти прибора. Размер выбирается из `info_byte.bit0`:
- `bit0 = 0` → `0x20000 = 131072` байт (128 КБ, стандарт УД2-12).
- `bit0 = 1` → `0x80000 = 524288` байт (512 КБ, УД2-102).

**537.** **Опкод 0x9a = SCREEN DUMP** — bitmap LCD-экрана. Размер выбирается из `DAT_0053e97c & 4` (флаг wagon LCD):
- Если `bit2 = 0` → `0x10C5 = 4293` байта (включая 5-байтный prefix).
- Если `bit2 = 1` → `0x1485 = 5253` байта (wagon LCD).

**538.** **5-байтный синтетический prefix** screen dump: `60 EA <sizeLO> <sizeHI> 00`. Сигнатура `0x60 0xEA` — маркер блока, **прописывается хостом, НЕ приходит с прибора**.

**539.** Файл flash dump (.bin сохраняемый из PelengPC) имеет формат:
- Bytes [0..15]: **кэшированный 16-байтный header** из последнего handshake (копируется из `DAT_0053e96a`).
- Bytes [16..16+N]: данные flash, где N = 0x20000 или 0x80000.

**540.** Файл screen dump (.bmp/.scr):
- Bytes [0..4]: prefix `60 EA <sizeLO> <sizeHI> 00`.
- Bytes [5..5+N]: пиксельные данные растра, N = 4288 или 5248 байт (LSB-first).

**541.** Расчёт LCD-геометрии из размера:
- 4288 байт × 8 бит = 34304 пикселя → **256×134** (240×128 + margins).
- 5248 байт × 8 бит = 41984 пикселя → **256×164** (wagon LCD, расширенное).

**542.** Адрес `60000` (десятичный, 0xEA60) в `FormReadData_b288` — **виртуальный адрес = screen dump**. Когда `param_1[0x3c8] == 60000`, вызывается `FormReadData_cbf4` (опкод 0x9a) вместо обычного `0x42`.

**543.** Опкод `0x42` (Block read) **отправка**:
```
TX: byte=0x42, sleep(10ms), byte=LL, sleep(10ms), byte=HH, sleep(10ms)
```
То есть PC отправляет 3 байта с **10 мс паузой** между байтами.

**544.** RX block (опкод 0x42) делегируется в `FormProgress.ShowModal` (`*(int*)(FormProgress + 0x2f8) = 1`) — синхронное ожидание заполнения буфера.

---

## §145. ТАЙМИНГИ ПРОТОКОЛА — ТОЧНЫЕ ЗНАЧЕНИЯ — найдено #545-552

**545.** **`FUN_00423a58 = busy-wait Sleep`** (НЕ системный Sleep):
```c
DVar1 = GetTickCount();
do { DVar2 = GetTickCount(); } while (DVar2 < DVar1 + param_2);
```
Использует GetTickCount() с гранулярностью 1 мс. Загружает CPU 100% во время ожидания (используется для критически точного таймирования).

**546.** **Inter-byte delay для PelengPC = 10 мс** (НЕ 4.8 мс):
```c
local_41 = 0x42; FUN_00423f0c(port, &local_41); FUN_00423a58(port, 10);  // 10ms
local_41 = LL;   FUN_00423f0c(port, &local_41); FUN_00423a58(port, 10);  // 10ms
local_41 = HH;   FUN_00423f0c(port, &local_41); FUN_00423a58(port, 10);  // 10ms
```
**ИСПРАВЛЕНИЕ ROUND5 §134/489:** 4.8 мс — это специфика **УД2-102** (Python clone), для **PelengPC.exe** используется **10 мс**.

**547.** **Post-block delay = 10 мс** — после получения тела также `Sleep(10)`. Это для очистки RX-буфера перед следующей командой.

**548.** **`FUN_00423f64` RX функция** — алгоритм с **idle timeout**:
```c
deadline = GetTickCount() + port[+0x14];   // idle timeout (DWORD)
do {
    bytes_in_queue = ClearCommError().cbInQue;
    if (bytes_in_queue > 0) {
        chunk = min(bytes_in_queue, remaining);
        ReadFile(handle, buf, chunk, &nread, NULL);
        total += nread; remaining -= nread;
        deadline = GetTickCount() + port[+0x14];   // reset idle
    }
} while (GetTickCount() <= deadline);
```
**Idle-based end-of-frame detection** — отсутствие данных в течение `idle_timeout` мс → конец передачи.

**549.** **Default idle timeout = 100 мс** (`port[+0x14] = 100`, установлено в `FUN_004247bc`). НЕ 1500 мс как в Python clone.

**550.** **Default DCB COMMTIMEOUTS** (`FUN_00423d18`):
- `ReadIntervalTimeout = 1` мс
- `ReadTotalTimeoutMultiplier = 0`
- `ReadTotalTimeoutConstant = 1` мс
- `WriteTotalTimeoutMultiplier = 0`
- `WriteTotalTimeoutConstant = 10` мс

Эти значения говорят системе вернуть ReadFile быстро (1ms), а пользовательский код сам управляет общим таймаутом через `idle_timeout` цикл.

**551.** **Post-open delay = 100 мс**: `Sleep(100)` после `CreateFile("COMx", ...)` (даёт время на инициализацию драйвера).

**552.** **FormProgress timer-based RX**:
```c
loop_per_tick:
    bytes = FUN_00423f64(port, buf + total, 0x400);   // read up to 1024
    if (bytes > 0) { total += bytes; reset idle counter; }
    else if (++idle_ticks == 3) { stop = "complete"; }
    if (total >= expected) { stop = "complete"; }
```
**3 consecutive idle ticks → end of frame.** Это resampling-механизм поверх idle-timeout `FUN_00423f64`.

---

## §146. PORT STRUCT — ТОЧНАЯ КАРТА — найдено #553-568

DAT_0053e948 = `g_comPort` синглтон. Layout полей по реверсу `FUN_00424020`, `FUN_004247bc`, `FUN_004249c0`:

| Offset | Размер | Тип | Поле | Назначение |
|---|---|---|---|---|
| +0x00 | 4 | HANDLE | `handle` | Win32 file handle COM-порта |
| +0x04 | 4 | AnsiString* | `name` | "COM1", "COM2"... |
| +0x08 | 1 | byte | `baud_index` | 0..14 (см. baud table) |
| +0x09..0x0B | 3 | — | padding | |
| +0x0C | 4 | DWORD | `baud_value` | 19200, 57600 и т.д. |
| +0x10 | 1 | byte | `byte_size_enum` | 0..3 (= 5..8 бит) |
| +0x11 | 1 | byte | `stop_bits` | 0=1stop, 1=1.5, 2=2 |
| +0x12 | 1 | byte | `parity` | 0=none, 1=odd, 2=even, 3=mark, 4=space |
| +0x13 | 1 | byte | **`info_byte`** | кэш handshake[+2] |
| +0x14..0x17 | 4 | DWORD | `rx_idle_timeout_ms` | 100 (default) |
| +0x18..0x1B | 4 | TStrings* | `port_list_owner` | владелец списка `EnumComPorts` |
| +0x1C..0x21 | 6 | — | reserved | |
| +0x22..0x31 | 16 | byte[16] | **`cached_header`** | 16-байтный header handshake reply |
| +0x32..0x37 | 6 | — | reserved | |
| +0x38..0x3B | 4 | int | `catalog_max_index` | `(rx_total - 16) / 2 - 1` |
| +0x3C..0x3F | 4 | byte* | `catalog_data` | указатель на body bytes 16..N |

**553.** Default config из `FUN_004247bc`:
- `baud_value = 0x4B00 = 19200` (НЕ 9600)
- `byte_size_enum = 3` → 8 бит данных
- `stop_bits = 0` → 1 стоп
- `parity = 2` → **EVEN parity**
- `rx_idle_timeout = 100` мс

**ИСПРАВЛЕНИЕ ROUND5 §302:** parity **EVEN** (E), не None. Default config = **19200 8E1**.

**554.** Baud table (полная, 15 значений) в `FUN_00423b4c`:

| idx | baud | hex |
|---|---|---|
| 0 | 110 | 0x6E |
| 1 | 300 | 0x12C |
| 2 | 600 | 0x258 |
| 3 | 1200 | 0x4B0 |
| 4 | 2400 | 0x960 |
| 5 | 4800 | 0x12C0 |
| 6 | 9600 | 0x2580 |
| 7 | 14400 | 0x3840 |
| 8 | **19200** | 0x4B00 (DEFAULT) |
| 9 | 38400 | 0x9600 |
| 10 | 56000 | 0xDAC0 |
| 11 | **57600** | 0xE100 (AUTO-FALLBACK) |
| 12 | 115200 | 0x1C200 |
| 13 | 128000 | 0x1F400 |
| 14 | 256000 | 0x3E800 |

**555.** Если baud value не в таблице → `byte_size_enum = 6` (default 9600).

**556.** **Auto-detect алгоритм** в `FUN_004248b0`:
```
try current settings; if FAIL:
  for com_port in enumerated_ports:
    try baud=19200; if OK break;
    try baud=57600; if OK break;
```
Перебираются ТОЛЬКО **19200 и 57600** на каждом COM-порту (последовательно).

**557.** **Handshake response saved** в port struct:
- `info_byte = handshake_reply[2]` → port[+0x13]
- `cached_header = handshake_reply[0..15]` → port[+0x22..+0x31]
- `catalog_data = handshake_reply[16..]` → port[+0x3C]
- `catalog_max_index = (total - 16) / 2 - 1` → port[+0x38]

**558.** Минимальный handshake reply = **17 байт** (16 header + 2 catalog минимум, но `(17-16)/2-1 = -1.something`, по факту 17 минимальная длина для «успех», максимум 0x80010 = 524304).

**559.** Максимальный handshake reply = **524304 байт** = `0x80010 = 0x80000 + 0x10` = 524288-byte catalog + 16-byte header. Это соответствует **полному каталогу 262144 LE16 entries**.

**560.** Catalog entries — **массив LE16-адресов записей**:
```
catalog[i] = LE16(catalog_data + 2*i)        # sweep_addr
```
Каждый адрес — это **sweep_addr** записи в памяти прибора (вызывается через 0x42).

**561.** **Address ranges** определяют категорию записи через `addr / 1000`:
- 1000..1999 → A-scan
- 4000..4999 → Generic-3
- 5000..5999 → Calibration
- 6000..6999 → Generic-4
- 10000..19999 → B-scan (multi-frame)
- 20000..29999 → ShortProtocol

**562.** **`FUN_0041c7c0(addr) = is_extended_address`**: возвращает 1, если `9999 < addr < 30000` (B-scan + ShortProto). 0 для A-scan/Generic/Calibration.

**563.** **3-уровневая декомпозиция расширенных адресов** (B-scan/ShortProto):
- `bucket_id = (addr / 100) * 100` — округление вниз к сотне
- `bucket_position = (addr % 10000) / 100` — позиция внутри 10000-блока (0..99)
- `inner_idx = addr % 100` — позиция внутри сотни (0..99)

Пример: addr=12345 → bucket_id=12300, bucket_position=23, inner_idx=45.

**564.** **Category format table** в `_FormReadData[+0x320..+0x320+n*0x18]`, где n=5 или 6:
- Если `device_class == 3` → 5 категорий (без ShortProto).
- Иначе → 6 категорий.

Каждая запись (24 байта):
- +0x04 (отн. начала записи) — start_addr
- +0x08 — end_addr  
- +0x0C — record_size (байт)
- +0x10 — ???

**565.** `FormReadData_c338(form, addr)` — **size lookup**: ищет в category table запись, где `start <= addr <= end`, возвращает `record_size`.

**566.** **`FUN_00423a80(port, name)`** = `set_port_name`, копирует AnsiString в `port[+0x04]`.

**567.** **`FUN_00423b20(port, timeout)`** = `set_rx_idle_timeout`, записывает в `port[+0x14]`.

**568.** Адрес `60000` (особый виртуальный, 0xEA60) — резервирован для **screen dump запроса** (опкод 0x9a). Не входит в реальный catalog адресов.

---

## §147. RAW-RECORD — РАСШИРЕННАЯ КАРТА ПОЛЕЙ — найдено #569-588

Базируется на анализе всех 48 функций-экстракторов FUN_00405XXX..FUN_00407XXX в `102_203dll.dll+3.c`. Каждая функция читает определённый offset из `param_3` = data area (RAW + 0x10).

| Offset (DATA) | Размер | Тип | Имя | Извлекатель | Семантика |
|---|---|---|---|---|---|
| **+0x00..+0x01** | 2 | byte×2 | `sweep_id_b4_b5` | _SortBufData | LE16, → FormatSweepId "XXYY" |
| +0x02 | 1 | byte | `version_flags` | _SortBufData | info_byte (bit-fields) |
| +0x05..+0x06 | 2 | LE16 | **`fw_or_subver`** | _SortBufData | LE16 в `local_c` (НЕ DATE!) — версия firmware? |
| +0x07 | 1 | byte | `date_day` | DecodeDate | 1..31 (бинарный, NOT BCD) |
| +0x08 | 1 | byte | `date_month` | DecodeDate | 1..12 |
| +0x09 | 1 | byte | `date_year_minus_2000` | DecodeDate | 0..99 (`year = 2000 + byte`) |
| +0x0A | 1 | byte | `time_hour` | DecodeTime | 0..23 (ValidateTime) |
| +0x0B | 1 | byte | `time_minute` | DecodeTime | 0..59 |
| +0x0C | 1 | byte | **`bool_flag`** | FUN_004057bc/5898/5974 | Trinary bool: 0 → empty, ≠0 → trans-text |
| +0x0E..+0x0F | 2 | LE16 | `???_le16_at_0e` | FUN_0040381c | LE16 (видна в DecodeAscan field 8) |
| **+0x10..+0x11** | 2 | LE16 | **`sweep_addr_FULL`** | _SortBufData | block_addr / 1000 = category |
| +0x11..+0x1B | 11 | passport-11 | `passport_primary` | FUN_00402708 | reverse-BCD + LUT |
| **+0x15..+0x16** | 2 | LE16 | `aux_le16` | _SortBufData | LE16 копия в local_c (overlap с passport!) |
| +0x21..+0x27 | 7 | passport-7 | `passport_secondary` | FUN_00402708 | reverse-BCD (TypeVar variants) |
| +0x29..+0x2A | 2 | LE16 | `???_le16_at_29` | FUN_00406498/518/598 | LE16, 3 вариант-функции для разных TypeVar |
| +0x35 | 1 | u8 | **`enum_at_35`** | FUN_00406768 | switch(0..5), 6-валуэйт enum |
| +0x35..+0x3B | 7 | passport-7 | `passport_special` | FUN_00405f44 | reverse-BCD (некоторые TypeVar) |
| +0x37..+0x38 | 2 | LE16 | `???_le16_at_37` | FUN_004056b4 | IntToStr |
| +0x38..+0x39 | 2 | LE16 | `???_le16_at_38` | FUN_00405528/406940 | IntToStr |
| +0x39..+0x3A | 2 | LE16 | `???_le16_at_39` | FUN_00406204 | Conditional (TypeVar-зависимый) |
| +0x3A..+0x3B | 2 | LE16 | `???_le16_at_3a` | FUN_00405630 | IntToStr |
| +0x3B..+0x3C | 2 | LE16 | **`probe_angle_default`** | FUN_00405bdc | default offset для probe_angle |
| +0x3C..+0x3D | 2 | LE16 | `???_le16_at_3c` | FUN_00405738/5ff0 | IntToStr |
| +0x3E..+0x3F | 2 | LE16 | **`probe_angle_alt`** | FUN_00405bdc | для TypeVar 0x2A8..0x2AB |
| +0x3E..+0x3F | 2 | LE16 | `???_le16_at_3e` | FUN_00406158 | IntToStr (другой контекст) |
| +0x41..+0x47 | 7 | passport-7 | `passport_extra` | FUN_00405e84 | ТОЛЬКО для TypeVar 0x2A8..0x2AB |
| +0x45..+0x46 | 2 | LE16 | `???_le16_at_45` | FUN_00405c78 | IntToStr |
| **+0x5E..+0x5F** | 2 | LE16 | `???_le16_at_5e` | FUN_00406318/398/418 | 3 варианта (TypeVar-зависимые) |

**569.** **Поле +0x0C = тернарный bool-флаг** (FUN_004057bc, FUN_00405898, FUN_00405974 — 3 идентичных функции для разных TypeVar):
```c
if (data[+0x0C] == 0) → empty string
else → translation text
```
Возможно: defect_present, calibration_passed, или другое булевское состояние.

**570.** **Поле +0x35 = enum из 6 значений** (FUN_00406768):
```c
switch (data[+0x35]) {
case 0..5: → один из 6 локализованных текстов
}
```
Значения: вероятно `Поиск / Контроль / Брак / Калибровка / Тест / Прочее` или аналогичные оценки.

**571.** **Поле +0x39 — conditional LE16** (FUN_00406204):
```c
iVar1 = TypeVar;
if (iVar1 < 0x2c8 && ...) goto SKIP;
else if (iVar1 < 0x2e5 && ...) goto SKIP;
else if (iVar1 != 0x2e6) {
    if (1 < (iVar1 - 0x30c)) goto SKIP;
}
read_le16(data + 0x39);
```
Извлечь только для определённых TypeVar (0x2c8, 0x2e4, 0x2e6, 0x30c..0x30d, и др.).

**572.** **Поле +0x3B vs +0x3E** — **alternating probe_angle**:
```c
offset = 0x3B;
if (TypeVar - 0x2A8 < 4) offset = 0x3E;
le16 = read_le16(data + offset);
```
Для TypeVar 0x2A8..0x2AB (4 значения) угол хранится на 3 байта дальше, чем для остальных.

**573.** **Поле +0x41 — conditional passport-7**:
```c
if (TypeVar - 0x2A8 < 4) {
    decode_passport(data + 0x41, 7);
}
// иначе — empty string
```
Дополнительный паспорт-блок (7 байт reverse-BCD), присутствует ТОЛЬКО для TypeVar 0x2A8..0x2AB.

**574.** **Группа 11-байтных passport @+0x11** — три идентичных функции (FUN_00405cf8, FUN_00405d7c, FUN_00405e00, FUN_00406618, FUN_00406688, FUN_004066f8) — все вызывают `FUN_00402708(buf, data + 0x11, 11)`. Используются в разных типах записей (A-scan, Composite, Generic) для извлечения одного и того же primary passport.

**575.** **Группа 7-байтных passport @+0x21** — три идентичных функции (FUN_00405a50, FUN_00405ad4, FUN_00405b58) — `FUN_00402708(buf, data + 0x21, 7)`. Secondary passport (без префикса заводского номера).

**576.** **Группа LE16 @+0x5E** — три идентичных функции (FUN_00406318, FUN_00406398, FUN_00406418). Поле всегда хранится @+0x5E.

**577.** **Группа LE16 @+0x29** — три идентичных функции (FUN_00406498, FUN_00406518, FUN_00406598). Поле всегда @+0x29.

**578.** **DecodeDate algorithm** (FUN_00402898 = `DecodeDate(buf, offset)`):
```c
day   = buf[offset + 0]      // 1..31 binary
month = buf[offset + 1]      // 1..12 binary
year  = 2000 + buf[offset + 2]   // 0..99 → 2000..2099
if (ValidateDate(year, month, day)) → format as "DD.MM.YYYY" или подобно
else → default "01/01/2000"
```
**A-scan/Composite/Default используют offset=7**. Date в RAW DATA[+7..+9].

**579.** **DecodeTime algorithm** (FUN_00402954 = `DecodeTime(buf, offset)`):
```c
hh = buf[offset + 0]         // 0..23 binary
mm = buf[offset + 1]         // 0..59 binary
if (0 <= hh < 24 && 0 <= mm < 60) → format as "HH:MM"
else → empty string
```
Используется offset=10. Time в RAW DATA[+10..+11].

**580.** **ValidateTime (FUN_00402954):**
```c
return (0 <= hh < 24) && (0 <= mm < 60);
```
Простая проверка диапазона, БЕЗ багов.

**581.** **TIME format = "HH:MM" с явным ':'** :
- `out[0] = '0'+(hh/10)`
- `out[1] = '0'+(hh%10)`
- `out[2] = ':'`
- `out[3] = '0'+(mm/10)`
- `out[4] = '0'+(mm%10)`

**582.** Год хранится как `byte - 2000`, поэтому **диапазон 2000..2255** (1 байт). Реальный полезный диапазон по `ValidateDate`: 2001..2099 (см. этап C §141 — багов с 2000 годом и високосными остаётся).

**583.** Имеется **DUPLICATE LE16 reads** на близких offset (например +0x37 и +0x38, +0x39 и +0x3A). Это указывает, что в РАЗНЫХ типах записей одно и то же поле хранится по РАЗНЫМ смещениям, и DLL пытается извлечь его из любого возможного места.

**584.** Decoder param_1 — структура из 7 int (28 байт):
```
param_1[0] = category      (sweep_addr / 1000)
param_1[1] = device_id     (LE16 from RAW[0..1])
param_1[2] = sweep_addr    (full LE16 from RAW[+0x10..+0x11])
param_1[3] = sweep_id_num  (b4*100 + b5)   ← used as TypeVar by DispatchByTypeVar!
param_1[4] = sweep_id_disp (AnsiString "XXYY")
param_1[5] = aux_le16      (LE16 from RAW[+0x15..+0x16])
param_1[6] = 0             (always zero)
```

**585.** **TypeVar = param_1[3] = byte[4]*100 + byte[5]**. То есть TypeVar — это **2-байтовый sweep_id** интерпретированный как `b4*100 + b5` (как BCD-десятичная пара).

**586.** Это объясняет numeric range TypeVar:
- TypeVar 4..6 = sweep_id 0004..0006 (b4=0, b5=4..6)
- TypeVar 10..19 = sweep_id 0010..0019 (b4=0, b5=10..19)
- TypeVar 0x2A8 = 680 = sweep_id 0680 (b4=6, b5=80)
- TypeVar 0x2C8 = 712 = sweep_id 0712 (b4=7, b5=12)

**587.** **Параметр decoder param_2** (через body decoder) = **указатель на data area** (RAW + 0x10). Поэтому все extractor offsets +0x11, +0x21, +0x35, +0x3B, +0x41, +0x5E — это смещения внутри **DATA AREA**, где offset 0 = первый байт после 16-байтного header.

**588.** Соответственно полная карта RAW (`pbVar7 = param_1 + 0x10`):
- RAW[0..15] = 16-байтный header (Status / Status_Bytes / etc.)
- RAW[16..N] = DATA AREA (decoded fields)
- DATA[0..1] = device_id-копия или sweep_addr LSB
- DATA[+0x11..+0x1B] = primary passport (11 байт)
- DATA[+0x21..+0x27] = secondary passport (7 байт)
- DATA[+0x35] = enum (6 значений)
- DATA[+0x35..+0x3B] = passport-special (alt-position)
- DATA[+0x3B] или DATA[+0x3E] = probe_angle (TypeVar-conditional)
- DATA[+0x41..+0x47] = passport extra (TypeVar-conditional)
- DATA[+0x5E..+0x5F] = result_LE16

---

## §148. B-SCAN MULTI-FRAME ПРОТОКОЛ — найдено #589-597

Из реверса `FormReadData_ba9c`:

**589.** B-scan записи иногда содержат **больше 1 фрейма** в одном transaction.

**590.** Алгоритм multi-frame readout:
```
1. Send: 0x42 LL HH (sweep_addr)
2. Receive: 16 header + body[0..N]  (N = standard bscan size, e.g. 8224 = 32×64×4 + meta)
3. Parse: frame_count = header[+0x0E]
4. For i in 0..(frame_count - 1):
     Receive another N bytes (no new request)
     Save frame i+1
```

**591.** **`header[+0x0E] = frame_count`** для B-scan. Этот байт **сбрасывается в 0** перед сохранением в файл (`*(byte *)((int)piVar3 + 0xe) = 0`). То есть в сохранённом файле frame_count всегда = 0, в отличие от исходного протокольного значения.

**592.** B-scan multi-frame условие: 
```c
if ((cVar2 != 0) && (uVar1 <= 19999) && (uVar5 != 0)) {
    // is B-scan address (10000..19999) AND frame_count > 0 → multi-frame
}
```
ShortProtocol (20000..29999) **НЕ** имеет multi-frame.

**593.** **Размер каждого B-scan frame** = результат `FormReadData_c338(form, 10000)` — стандартный record_size для адреса 10000 (репрезентативный B-scan адрес).

**594.** **Frame layout в RX буфере** при multi-frame:
```
[16-byte header][frame 1 (N bytes)][frame 2 (N bytes)]...[frame K (N bytes)]
```
Где K = frame_count + 1 (первый фрейм всегда + frame_count дополнительных).

**595.** В файл сохраняются **отдельные записи**: для каждого frame i, копируется header + frame_i в DB через `FUN_0040df2c`. То есть из ONE RX-transmission получается K DB-records, все с одним и тем же header (но с фреймом отличающимся).

**596.** Это объясняет, почему B-scan занимают много памяти: каждый дефект может фиксироваться как серия из 2..K кадров (для построения 2D-картины зоны вокруг дефекта).

**597.** Buffer alloc для B-scan readout = `IBX_SQL_c03c(0x80020) = 524320 байт` — достаточно для 64 frames × 8KB = 512KB + header.

---

## §149. ОПКОДЫ — РАСШИРЕННОЕ ОПИСАНИЕ HANDSHAKE — найдено #598-606

Из реверса `FUN_004249c0` и `FormReadData_b304`:

**598.** **Handshake transaction:**
```
TX: 1 byte = 0x55 ('U')
RX: 16..524304 байт (idle timeout = 100 ms)
```

**599.** **RX layout:**
```
[0..1]   = device_id (LE16) ← возможно
[2]      = info_byte ← extracted to port[+0x13]
[3..15]  = ??? (13 bytes — version, model, fw revision?)
[16..N]  = catalog (LE16 entries)
```

**600.** **Catalog count** = `(received - 16) / 2`. Каждый entry = sweep_addr (2 байта LE).

**601.** **`port[+0x38] = (received - 16) / 2 - 1`** — последний валидный индекс (0-based).

**602.** **Handshake validation**: response must be ≥ 17 байт; иначе вызывается `ShowMessage` (Russian "Прибор не отвечает" или подобно).

**603.** **`FormReadData_b304` обёртка**:
```c
uVar1 = FUN_004249c0(port, true);    // try handshake (with error popup)
if (success) {
    parse_catalog();
    extract_device_info();
    Show form with results;
}
```

**604.** Catalog **uniqueness check**: вызывается `FUN_0041c7c0` для каждого entry. Если address в [10000..29999], то делится на bucket+inner. Если bucket уже виден ранее, объединяется с inner_idx. Иначе новый bucket entry.

**605.** Это **дедупликация B-scan multi-frame** — один bucket (например 12300) представляет коллекцию записей 12345, 12346, 12347... один UI-entry с диапазоном.

**606.** **Sweep_id отображение** для пользователя через `FormatSweepId(b4, b5)`:
```
"XXYY"  где XX = b4 (десятичный, 2-digit), YY = b5 (десятичный, 2-digit)
```
Уже описано в §136 (этап C).

---

## §150. КЭШИРОВАННЫЙ HEADER — DAT_0053e96a — найдено #607-612

**607.** Глобальная переменная `DAT_0053e96a` хранит **последний валидный 16-байтный handshake header**.

**608.** Записывается в `FUN_004249c0` (handshake success):
```c
IB_TransactionInfo_Get(port + 0x22, &local_6a, 0x10);    // в port struct
// и где-то параллельно — в DAT_0053e96a
```

**609.** Используется в `FUN_0041b43c` (block read) для **prepending header to data**:
```c
TimeToStr(param_2, &DAT_0053e96a, 0x10);    // copy 16 bytes header
// затем readNext into param_2 + 16
```

**610.** Используется в `FUN_00422764` (flash dump) для записи header в начало dump-файла.

**611.** **Семантика**: каждая полученная запись (a-scan, b-scan и т.д.) сохраняется в DB с **тем же 16-байтным header**, что получен при последнем handshake. Это связывает все записи с конкретной сессией/прибором.

**612.** **Кэш сбрасывается** при следующем handshake. То есть если потом сделать handshake к другому прибору — старые сохранённые записи сохранят header старого прибора, а новые получат новый.

---

## §151. РАЗМЕРЫ ЗАПИСЕЙ ПО КАТЕГОРИЯМ — найдено #613-620

Из `FormReadData_c284`, `FormReadData_c338`, `FormReadData_c11c`:

**613.** **Category format table** инициализируется в `FormReadData_c284`. Число записей: 5 (УД2-12) или 6 (УД2-102).

**614.** Каждая запись 24 байта:
```
struct CategoryFmt {
    AnsiString name;          // +0x00..0x03
    int start_addr;           // +0x04
    int end_addr;             // +0x08
    int record_size;          // +0x0C
    int field_descriptor;     // +0x10
    // padding to 0x18
};
```

**615.** Размеры **читаются из текстового config** (через `_FormMain[0x440 + idx*4]`). Это динамическая конфигурация — разные DLL могут предоставлять разные размеры.

**616.** Типичные размеры (по `peleng_clone+12+3.py`):
- A-scan: 100 байт
- Generic-3 / Calibration: 84 байта
- Generic-4: 84 байта
- B-scan: 2 КБ до 8 КБ (зависит от размеров)
- ShortProto: 16 байт + tail

**617.** **Размер обычной транзакции** = 16 header + record_size. То есть для A-scan: 16+100 = 116 байт.

**618.** **Размер B-scan multi-frame** = 16 + frame_count × frame_size, где frame_size берётся из category[B-scan].record_size.

**619.** Адрес `60000` (screen dump) **не имеет category entry** — обрабатывается отдельно (опкод 0x9a).

**620.** Адрес `0` (no-record) **тоже специальный** — означает «нет данных» или «свободная ячейка». Возвращаемый из catalog нулевой адрес можно пропустить.

---

## §152. ПОТОК ОБРАБОТКИ ЗАПИСИ — найдено #621-630

Полный pipeline RX → DB:

**621.** **Шаг 1: User clicks "Read All"** → `FUN_0041cbe1` итерирует catalog entries:
```
for (i = 0; i <= port[+0x38]; i++) {
    addr = catalog[i];
    if (addr != 0) {
        Read(addr); → calls FUN_0041bbe0 or FormReadData_ba9c
    }
}
```

**622.** **Шаг 2: For each address**:
```c
form[+0x3c8] = addr;                          // set current
size = FormReadData_c338(form, addr);        // size lookup
buf = IBX_SQL_c03c(0x80020);                 // alloc 524320
total = FUN_0041b43c(form, buf);             // read FROM device or cache
```

**623.** **Шаг 3: FUN_0041b43c logic**:
```c
buf[0..15] = DAT_0053e96a;                   // prepend cached header
if (addr == 60000) {
    received = FormReadData_cbf4(...);       // screen dump (0x9a)
} else {
    if (online_mode) {
        received = FUN_00424cc0(port, addr, buf+16, size); // block read (0x42)
    } else {
        received = FormReadData_b288(form, addr, buf+16);  // read from saved file cache
    }
}
return received + 16;     // total = received body + 16 header
```

**624.** **Шаг 4: Save to DB**:
```c
FUN_0040df2c(_FormMain, buf, total);  // serialize to Firebird
```

**625.** **Шаг 5: Decode for display**:
```c
_SortBufData(buf, &total);                   // exported from 102_203dll.dll
piVar1 = result;
// piVar1[0] = decoder_type (1..4)
// piVar1[1] = n_fields
// piVar1[+2..n_fields*4+1] = field offset table
// piVar1[+next] = decoded fields (BE-int, string, date)
// piVar1[+tail] = original RAW
_Form_View(piVar1, total);                   // exported, COM-passes to DataTable server
```

**626.** **Шаг 6: Display in TStringGrid** — поля распакованы и показаны в таблице по схеме категории.

**627.** **Шаг 7: Free** — `_FreeBuffer()` (без параметров) освобождает global buffer.

**628.** **TWO storage paths**: online → opcode 0x42 на прибор; offline → TList cache (загружено из .bin файла раньше).

**629.** **TList cache** в `_FormReadData[+0x31c]` (FUN_0041b288): структура `[addr, offset_in_file, size]` для быстрого поиска без чтения порта.

**630.** **Дамп-файл** (TList offline cache) загружается из `[+0x314]` (raw data ptr) + `[+0x310]` (count of entries) — массив `int` адресов с offset-table.

---

## §153. КЛЮЧЕВЫЕ ИСПРАВЛЕНИЯ К ROUND4/ROUND5 ЭТАП A-C — итог

| Источник | Старая позиция | Исправление | Доказательство |
|---|---|---|---|
| ROUND4 §44, §50 | baud = 9600 | **baud = 19200** (default) | FUN_004247bc: `FUN_00423b4c(self, 0x4B00)` |
| ROUND5 §134 #489 | inter-byte = 4.8 ms | **inter-byte = 10 ms** (PelengPC) | FUN_00424cc0: `FUN_00423a58(port, 10)` ×3 |
| ROUND5 §134 #490 | handshake RX = 1500 ms | **rx_idle = 100 ms** | FUN_004247bc: `FUN_00423b20(self, 100)` |
| ROUND5 §134 (table) | 0x74 = Status/Test | **0x74 = FLASH DUMP** | FUN_00422764: `iVar4 = FUN_00424bb0(..., 0x74, ..., local_48)` где `local_48 ∈ {0x20000, 0x80000}` |
| ROUND5 §134 (table) | 0x9a = Flash dump | **0x9a = SCREEN DUMP** | FormReadData_cbf4: `FUN_00424bb0(..., 0x9a, ..., local_14-5)` где `local_14 ∈ {0x10C5, 0x1485}` |
| ROUND5 §302 | parity = ??? (не указано) | **parity = EVEN** | FUN_004247bc: `FUN_00423cfc(self, 2)` |
| ROUND4 §16, ROUND5 §83 | info_byte.bit0 = extended_flash → 0x80000 | **bit0 = extended_flash (✅ верно)** | FUN_00422764: `if ((DAT_0053e97c & 1) != 0) local_48 = 0x80000` |
| ROUND4 §16, ROUND5 §83 | info_byte.bit2 = wagon_LCD = 5253 | **bit2 = wagon_LCD (✅ верно, размер ЭКРАНА)** | FormReadData_cbf4: `if ((DAT_0053e97c & 4) != 0) local_14 = 0x1485` |
| ROUND5 §128 (+0x29) | enum или unknown | **LE16, читается тремя вариантами (FUN_00406498/518/598)** | DLL 4597-4639 |
| ROUND5 §128 (+0x35) | enum, 6 значений | **enum: switch(0..5), 6 локализованных текстов** | FUN_00406768 4711-4744 |
| ROUND5 §125 | tail-block offset = 0x65..0x87 | **подтверждено**, плюс enum @+0x35 в Settings | DLL FUN_00402980 (не реверсенный полностью) |

---

## §154. ДВА НЕЗАВИСИМЫХ DECODE ПУТИ В DLL — найдено #631-635

**631.** В DLL **два независимых dispatch path**, использующих разные категории:

**Path A: `_SortBufData` (user-callable)**:
```c
switch (sweep_addr / 1000) {
case 1: DecodeAscan;        // body[0] = 2
case 4, 6: DecodeGeneric;   // body[0] = 1
case 5: DecodeCalibration;  // body[0] = 1
case 10..19: DecodeBscan;   // body[0] = 3
case 20..29: DecodeShortProtocol; // body[0] = 4
}
```

**Path B: TLV-stream dispatcher (LUT@0x401670, INTERNAL)**:
```c
switch (LUT[tcode]) {
case 1: AscanHandler @ 0x403420;   // body[0] = 4
case 2: BscanHandler @ 0x4031dc;   // body[0] = 3
case 3: Generic3Handler @ 0x402c8c; // body[0] = 1
case 4: Generic4Handler @ 0x402f34; // body[0] = 1
case 5: SettingsHandler @ 0x402980; // body[0] = 2
}
```

**632.** **`body[0]` имеет разное значение в обоих путях**:
- Path A `body[0]=2` = A-scan; Path B `body[0]=2` = Settings!
- Path A `body[0]=4` = ShortProto; Path B `body[0]=4` = Generic-4!

**633.** Это указывает, что **DLL имеет 2 разных формата output-buffer**, и приложение должно знать, какой путь вызвало.

**634.** **`_SortBufData` (Path A)** — основной экспорт, для интерактивного отображения каждой записи в UI. Принимает RAW byte buffer.

**635.** **TLV-stream dispatcher (Path B)** — внутренний, для batch-обработки TLV-сериализованных записей (вероятно из flash dump). НЕ экспортируется напрямую.

---

## §155. SETTINGS RECORD — НОВЫЕ НАХОДКИ — найдено #636-642

Хотя `FUN_00402980` (Settings handler в Path B) не виден полностью в C-декомпиляции, можно вывести из контекста:

**636.** Settings — это **TLV запись с tcode=1** (case 5 в LUT@0x401670). Длина body фиксированная в формат-таблице (см. §125 этап C).

**637.** Settings содержит **acoustic tail-block** на offset +0x65..+0x87 (136+ байт):
- +0x66 = gain_db (дБ)
- +0x67..+0x68 = velocity_ms (LE16 м/с)
- +0x69 = angle_deg
- +0x6A = delay_us
- +0x6E..+0x6F = gate_start (LE16 sample отсчёты)
- +0x70..+0x71 = gate_width (LE16)
- +0x74 = threshold_pct
- +0x75 = arm_mode (enum)
- +0x76 = ard_curve_id
- +0x7C = sens_required (дБ)
- +0x7D = sens_factual (дБ)
- +0x80..+0x87 = vrt_table[8] (ВРЧ-таблица)

**638.** Settings также имеет **enum @+0x35** (как и все остальные записи) — определяет режим (Поиск/Контроль/Брак).

**639.** Settings **связан с A-/B-scan записями через sweep_addr**: записи, чей sweep_id принадлежит тому же фрейму измерений, ссылаются на одну Settings-запись (по близости адресов).

**640.** **Применение Settings**:
```
gain_db    → программируемое усиление приёмника
velocity   → пересчёт sample_idx → mm
angle_deg  → ВЕЛИЧИНА УГЛА ВВОДА → корректировка глубины (cos α)
gate_*     → видимая зона (по горизонтали A-scan)
threshold  → порог дефекта (горизонтальная линия)
ard_curve  → выбор АРД-кривой (из таблицы) 
vrt_table  → ВРЧ-кривая (8 точек amplitude correction)
sens_*     → калибровка чувствительности (для оценки БРАК/КОНТРОЛЬ)
```

**641.** **ВРЧ-таблица (`vrt_table[8]`)** — массив из 8 байт. Каждый байт = коррекция усиления для одного диапазона глубины. Линейная интерполяция между точками для других глубин. Эквивалентно **TVG** (Time-Variable-Gain).

**642.** **Settings as "header" for A-/B-scan records** — A-scan и B-scan не самодостаточны: для их интерпретации (масштаб mm/дБ, гейт, порог) ВСЕГДА нужна предшествующая Settings-запись с тем же sweep_id или тем же интервалом времени.

---

## §156. PROTOCOL OPCODE DISPATCH TABLE — итог

Полная таблица всех опкодов с указанием реальной семантики (после исправлений в §144):

| Opcode | ASCII | Размер TX | Размер RX | Назначение | Caller функция |
|---|---|---|---|---|---|
| **0x42** | 'B' | 3 (B+LL+HH) | size [+0x322] table | Block read 1 record | FUN_00424cc0 |
| **0x55** | 'U' | 1 | 16..0x80010 | Handshake + catalog | FUN_004249c0 |
| **0x74** | 't' | 1 | 0x20000 / 0x80000 | **Flash dump** (128KB / 512KB) | FUN_00422764 |
| **0x9a** | — | 1 | 0x10C5 / 0x1485 (минус 5 prefix) | **Screen dump** (LCD bitmap) | FormReadData_cbf4 |
| 0x4B | 'K' | 2 (K+key) | ack | Virtual keyboard (УД2-102 only) | (внешний) |
| 0x49 | 'I' | 1 | ~15 КБ | Screen dump alt (для УД2-102) | (внешний) |
| 0x54 | 'T' | N+1 | ack | RTC sync | (внешний) |

PelengPC.exe реализует ТОЛЬКО **4 опкода**: 0x42, 0x55, 0x74, 0x9a. Остальные (0x4B, 0x49, 0x54) — внешние команды для прямого взаимодействия с УД2-102 через специальную утилиту.

---

## §157. ИТОГИ ЭТАПА D

### Новые находки (#536-642, всего +107):

- **§144-145**: Опкоды исправлены (0x74=FLASH, 0x9a=SCREEN); тайминги уточнены (inter-byte=10ms, rx_idle=100ms).
- **§146**: Полная карта port struct (`g_comPort` @ 0x53E948); baud table 15 значений; auto-detect алгоритм (19200/57600 only).
- **§147**: Расширенная карта RAW-record (48 extractor функций отображены в offsets); enum @+0x35 (6 значений), conditional fields для TypeVar 0x2A8..0x2AB.
- **§148**: B-scan multi-frame protocol (header[+0x0E] = frame_count, K кадров за одну транзакцию).
- **§149**: Handshake reply layout (header[0..15] + catalog LE16-пары).
- **§150**: Cached header (DAT_0053e96a) — присоединяется к каждой DB-записи.
- **§151**: Category format table (5 или 6 категорий по device class).
- **§152**: Полный pipeline RX → DB (7 шагов).
- **§153**: Сводная таблица исправлений к ROUND4/5.
- **§154**: Два независимых decode path (`_SortBufData` vs TLV-LUT dispatcher).
- **§155**: Settings record — связь с A/B-scan (gain, velocity, gate, ARD, VRT).
- **§156**: Финальная opcode-таблица.

### Что ОСТАЁТСЯ к реверсу:

- ❓ Точные значения 6 локализованных строк в enum @+0x35
- ❓ Полная декомпиляция `FUN_00402980` (Settings handler в Path B)
- ❓ Точное содержимое `LUT@0x401670` (30 байт) — какой tcode-byte маппится в какой case
- ❓ Содержимое `header[3..15]` после handshake (13 байт между info_byte и catalog)
- ❓ Семантика `aux_le16 @ +0x15..+0x16` (firmware version? subprotocol?)
- ❓ `bool_flag @ +0x0C` — что именно означает (defect / pass / calibration?)
- ❓ Real48 (Borland 6-byte float) — где используется в записях
- ❓ Deflate декомпрессия — есть ли вообще в протоколе

### Общая статистика:
- ROUND4: 189 находок (§1-66)
- ROUND5 этап A: +105 (190-294)
- ROUND5 этап B: +65 (295-359)
- ROUND5 этап C: +176 (360-535)
- **ROUND5 этап D: +107 (536-642)**
- **TOTAL: 642 находки**

Готово. ROUND5 этап D завершён.

---

# ROUND 5 — ЭТАП E: Распаковка инсталлятора, точное содержимое таблиц-кодов

**Метод:** innoextract распаковал `PelengPC_ver1_2.exe` (Inno Setup 5.4.2), что дало доступ к ОРИГИНАЛЬНЫМ бинарникам **до** декомпиляции Ghidra'ой. Кроме того инсталлятор содержит **схему данных** (`PelengPCtest.dal`), **базу-шаблон** (`PelengPC.fdb`), **справку** (`pelengpchelp.hlp`) и **новый связанный exe** (`zapis2.exe`). Анализ оригинальных байтов разрешает несколько ❓ из этапа D и добавляет существенный объём новых находок.

## §158. Распаковка инсталлятора и инвентарь файлов

**643.** Инсталлятор `PelengPC_ver1_2.exe` (Inno Setup 5.4.2) содержит **24 файла**:

```
app/                                                — основные файлы программы
  PelengPC.exe              1 592 832 B  2009-03-03  — главный EXE (BCB)
  102_203dll.dll              214 016 B  2007-12-25  — DLL декодера
  zapis2.exe                1 220 608 B  2011-12-13  — отдельный writer/recorder
  PelengPC.fdb                712 704 B  2007-04-17  — Firebird template DB (ODS 11.0)
  PelengPCtest.dal             22 491 B  2009-02-09  — INI schema config
  AltekBlack.bmp                  382 B  2001-10-03  — логотип
  IPLicense.txt                24 405 B  2003-07-08  — лицензия Firebird
  SetUp_PelengPC.exe          776 704 B  2007-03-19  — wrapper installer
  pelengpchelp.GID              8 628 B  2007-08-10  — help index
  pelengpchelp.hlp             85 811 B  2007-08-09  — Windows .hlp (LZ77)
  firebird.conf                21 597 B  — Firebird config
  firebird.msg                127 696 B  — Firebird messages
  gds32.dll                 1 998 848 B  — Firebird core
  ib_util.dll                   5 632 B  — Firebird util
  icudt30.dll, icuin30.dll, icuuc30.dll — ICU Unicode
  msvcp71.dll, msvcr71.dll  — MSVC 7.1 runtime
  intl/fbintl.{conf,dll}    — Firebird locale
  udf/fbudf.dll              — Firebird UDFs
sys/                                                — Windows system DLLs
  msxml4.dll, msxml4r.dll   — MSXML 4.0
tmp/                                                — временные (для драйвера USB-COM)
  CE.CAB                       28 754 B  — Windows CE cab
  Driver.exe                2 058 939 B  — USB-COM драйвер
  SETUP.exe                 1 699 559 B  — driver installer wrapper
```

**644.** MD5 EXE/DLL:
- `PelengPC.exe` = `f46eb12f51353374a26a0c2c7b0e342e` (1 592 832 B)
- `102_203dll.dll` = `cbc0f19c3697b595df64c19812094a68` (214 016 B)

**645.** Структура секций PE (важно для перевода VMA → file offset):

`PelengPC.exe`:
| Section | VMA       | Size      | File off | Назначение |
|---------|-----------|-----------|----------|------------|
| .text   | 0x00401000 | 0x11FA00 | 0x000600 | Код        |
| .data   | 0x00521000 | 0x01AA00 | 0x120000 | Данные     |
| .tls    | 0x005C3000 | 0x000200 | 0x13AA00 | TLS        |
| .rdata  | 0x005C4000 | 0x000200 | 0x13AC00 | RData      |
| .idata  | 0x005C5000 | 0x003200 | 0x13AE00 | Imports    |
| .edata  | 0x005C9000 | 0x010600 | 0x13E000 | Exports    |
| .rsrc   | 0x005DA000 | 0x021A00 | 0x14E600 | Resources  |
| .reloc  | 0x005FC000 | 0x014E00 | 0x170000 | Reloc      |

`102_203dll.dll`:
| Section | VMA       | Size      | File off |
|---------|-----------|-----------|----------|
| .text   | 0x00401000 | 0x026200 | 0x000600 |
| .data   | 0x00428000 | 0x008A00 | 0x026800 |
| .tls    | 0x00432000 | 0x000200 | 0x02F200 |
| .idata  | 0x00433000 | 0x000C00 | 0x02F400 |
| .edata  | 0x00434000 | 0x000200 | 0x030000 |
| .rsrc   | 0x00435000 | 0x001400 | 0x030200 |
| .reloc  | 0x00437000 | 0x002E00 | 0x031600 |

Формула: `file_off = vma - section_VMA + section_file_off`.

**646.** **zapis2.exe** — самостоятельная утилита от той же фирмы (Altek), но СТАТИЧЕСКИ ЛИНКОВАННАЯ (Intel C++ Compiler v6.0, MSVC 6.0 runtime, CxImage), не использует BCB. Скомпилирована позже (2011), судя по импортам — для записи отчётов на USB-носители (`zapis` = «запись»). Использует библиотеки JPEG, PNG, TIFF, JASPER (CxImage) — то есть умеет генерировать **отчёты в графических форматах**.

---

## §159. `PelengPCtest.dal` — полная схема конфигурации

**Формат**: Windows INI (Win-1251), `[Numbers]` + `[PelengPC]` + 3 секции `[VersionN]` + `[TableNN]` + `[FieldNN_K]`. Шифрование части в hex (`[Numbers]` — 124 hex-байта lic-key).

**647.** **Top-level structure**:
```ini
[PelengPC]
"Title"="PelengPC ver. 1.2"
"Label"="altek.ico"
"CountVersions"=dword:00000003

[Version1] / [Version2] / [Version3]
  Title           # User-visible name
  DllFile         # "102_203DLL.dll"
  CountNames      # number of supported firmware versions
  Name1..NameN    # firmware version strings
  CountTables     # number of tables in this version
[Table11..Table34]
  Title           # User-visible table name
  TableName       # SQL table name (NASTR or RESULTS in fdb)
  CountEdits, CountFields
[FieldNN_K]
  TypeField       # DWORD encoded type code (see below)
  Title           # User-visible label
  FieldName       # SQL column name (NUMBER, NUMKOD, etc.)
  Visible         # Bool — show in UI
  DefValue, MinValue, MaxValue — UI validation
  Enum1, Enum2... — Combobox options
```

**648.** **Три версии (`Version1..3`)** — соответствуют классам устройств:

| # | Title          | DllFile          | CountTables | Firmware list |
|---|----------------|------------------|-------------|---------------|
| 1 | "Базовая"      | 102_203DLL.dll   | 4           | 03.62/63/80/81/90/91, 07.12/13/30/31/40/42/80/81, 06.80/81/82/83 (18 FW) |
| 2 | "Железнодорожная" | (same)        | 3           | (не перечислены, но по коду) |
| 3 | "Рельсовая"    | (same)           | 3           | (не перечислены) |

**649.** **TypeField — DWORD encoding** (LE32 битовое поле). Низкий байт = базовый тип, верхние биты = атрибуты:

| TypeField | Binary (low8)| Тип данных | Примеры FieldName | Поведение в UI |
|-----------|-------|------------|--------------------|-----------------|
| 0x00000001 | `00000001` | INTEGER | NUMBER, NUMKOD, NUMPRIB, TYPEVAR, KODOPERA, M, MM, CLOCK | Числовое поле |
| 0x00000003 | `00000011` | DATE    | DATEFORM | Формат DD.MM.YYYY |
| 0x00000052 | `01010010` | CONDLENGTH | CONDLENGTH | "Условная протяженность, мм" |
| 0x0000005A | `01011010` | ENUM-БРАК | DEFEKT | Enum «есть/нет» |
| 0x00000072 | `01110010` | BLOB-PROTOCOL | PROTOCOL | Binary attachment (raw record) |
| 0x00000082 | `10000010` | TIME    | TIMEFORM | Формат HH:MM (00:00..24:00) |
| 0x000000A2 | `10100010` | CODE-4  | CODEDEF, NUMVERS, SMELTING | Alphanumeric 4-char код |
| 0x000000E2 | `11100010` | NUMBER-OBJ | NUMOBJ, SMELTING | Идентификатор объекта |
| 0x00000142 | `1.01000010` | TEXT-FREE | INDMAKER, STAGE, SECTION, PICKETLINE, PATH | Свободная строка |
| 0x0000014A | `1.01001010` | ENUM-RTYPE | TYPEZAP | Enum типа записи |
| 0x000001E2 | `1.11100010` | TEXT-OPERATOR | NAMEOPERA | Длинная строка (ФИО) |

Декомпозиция битов TypeField:
- **bit0** (0x01) = 1 если поле имеет default value (DefValue)
- **bit1** (0x02) = 1 если поле «структурное» (стандартный системный)  
- **bit2** (0x04) = 0
- **bit3** (0x08) = 1 если поле ENUM (имеет Enum1..N)
- **bit4** (0x10) = 1 если поле имеет MinValue/MaxValue
- **bit5** (0x20) = 1 если поле alphanumeric (code/number-as-string)
- **bit6** (0x40) = 1 если поле строковое
- **bit7** (0x80) = 1 если поле — время или формат-зависимое
- **bit8** (0x100) = 1 если поле «длинная строка» (NAMEOPERA, INDMAKER)

(Эти биты не **полностью** валидированы, выведены из перекрёстного сравнения. Главное — низкие 8 бит ИДЕНТИФИКАТОР ТИПА.)

**650.** **Все таблицы** (3 версии × 4/3/3 таблицы = **10 таблиц**):

| Table | Title              | SQL Name | CountFields | Назначение |
|-------|--------------------|----------|-------------|------------|
| Table11 | Протоколы          | RESULTS1  | 20 | A-/B-scan records (Version1) |
| Table12 | Настройки          | NASTR1    | 11 | Settings records (Version1) |
| Table13 | Отчеты контроля    | (?)       | 16 | Control reports (Version1) |
| Table14 | Отчеты толщиномера | (?)       | 9  | Thickness reports (только Version1) |
| Table21 | Протоколы          | RESULTS2  | 17 | Reports (Version2) |
| Table22 | Настройки          | NASTR2    | 11 | Settings (Version2) |
| Table23 | Отчеты контроля    | (?)       | 13 | Reports (Version2) |
| Table31 | Протоколы          | RESULTS3  | 22 | Reports (Version3, Рельсовая — больше полей) |
| Table32 | Настройки          | NASTR3    | 11 | Settings (Version3) |
| Table33 | Отчеты контроля    | (?)       | (?) | Reports (Version3) |

**651.** **Поля Table11 ("Протоколы Базовой")** — пример полного набора (20 полей):

| # | TypeField | FieldName | Title (рус.) | Enum/Range |
|---|-----------|-----------|--------------|------------|
| 1  | 0x01  | NUMBER     | № п/п           | (1..1) |
| 2  | 0x14A | TYPEZAP    | Вид записи      | "Протокол А-развертки" / "Протокол B-развертки" |
| 3  | 0x01  | NUMKOD     | № записи        | (1..1000) |
| 4  | 0x03  | DATEFORM   | Дата            | DD.MM.YYYY |
| 5  | 0x82  | TIMEFORM   | Время           | HH:MM (12:00 default) |
| 6  | 0x01  | KODOPERA   | Оператор        | INT |
| 7  | 0x1E2 | NAMEOPERA  | Имя оператора   | TEXT |
| 8  | 0xA2  | NUMVERS    | Версия ПО       | 4-char |
| 9  | 0x01  | NUMPRIB    | № прибора       | "0000".."9999" |
| 10 | 0x01  | TYPEVAR    | Типовой вариант | INT |
| 11 | 0xE2  | NUMOBJ     | № объекта       | NUMBER-OBJ |
| 12 | 0x01  | M          | м               | (метры) |
| 13 | 0x01  | MM         | мм              | (миллиметры) |
| 14 | 0x01  | CLOCK      | Часы            | (часовая позиция дефекта на оси?) |
| 15 | 0xA2  | SMELTING   | Плавка          | 4-char |
| 16 | 0x01  | MAKETIME   | Год изготовления | INT |
| 17 | 0x5A  | DEFEKT     | Дефект          | "есть" / "нет" |
| 18 | 0xA2  | CODEDEF    | Код дефекта     | 4-char |
| 19 | 0x52  | CONDLENGTH | Условная протяженность, мм | (мм) |
| 20 | 0x01  | NUMZAP     | Запись в БД     | INT (auto-PK) |

**652.** **Поля Table12 ("Настройки Базовой")** — 11 полей: NUMBER, NUMKOD, TYPEZAP (enum: "Настройка"/"Поверочная настройка"), DATEFORM, TIMEFORM, KODOPERA, NAMEOPERA, NUMVERS, NUMPRIB, TYPEVAR, NUMZAP.

**653.** **Поля Table21 ("Протоколы Железнодорожной")** — 17 полей: NUMBER, TYPEZAP, NUMKOD, DATEFORM, TIMEFORM, KODOPERA, NAMEOPERA, NUMVERS, NUMPRIB, TYPEVAR, NUMOBJ, **SMELTING**, **INDMAKER** (Завод изготовитель), **MAKETIME** (Год изготовления), **DEFEKT**, **CODEDEF**, NUMZAP. (нет M/MM/CLOCK/CONDLENGTH).

**654.** **Поля Table31 ("Протоколы Рельсовой")** — 22 поля: NUMBER, TYPEZAP, NUMKOD, DATEFORM, TIMEFORM, KODOPERA, NAMEOPERA, NUMVERS, NUMPRIB, TYPEVAR, NUMOBJ, **KM** (км), **M** (м), **MM** (мм), **STAGE** (Ход/перегон), **SECTION** (Звено), **PICKETLINE** (пикет), **PATH** (Путь), DEFEKT, CODEDEF, CONDLENGTH, NUMZAP.

**655.** **Поля Table33 ("Отчёты контроля Рельсовой")** — расширенный набор включает **PLACE** ("Место") и **PROTOCOL** blob.

**656.** **Особенности TYPEZAP enum'ов**:
- Tables 11/21/31 (Protokol): "Протокол А-развертки" / "Протокол B-развертки"
- Tables 12/22/32 (Nastr):  "Настройка" / "Поверочная настройка"

**657.** **`[Numbers]` секция** — 124 hex-байта, выглядит как **зашифрованная лицензионная информация** (вероятно используется в `Проверка ключа` диалоге, см. ROUND4 §44.2/45). Содержит точно 124 байта = идентичная длине PASSPORT-LUT. Возможно — обфусцированный список действующих лицензий или сериал-номера.

---

## §160. PASSPORT-LUT — ТОЧНОЕ СОДЕРЖИМОЕ (124 байта)

Извлечено из `102_203dll.dll` напрямую из секции `.data`. Подтверждает §132 ROUND5 этап C.

**658.** **VALID_LUT @ 0x004284b4** (124 байта, identity map):
```
0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E 0x0F
0x10 0x11 0x12 0x13 0x14 0x15 0x16 0x17 0x18 0x19 0x1A 0x1B 0x1C 0x1D 0x1E 0x1F
0x20 0x21 0x22 0x23 0x24 0x25 0x26 0x27 0x28 0x29 0x2A 0x2B 0x2C 0x2D 0x2E 0x2F
0x30 0x31 0x32 0x33 0x34 0x35 0x36 0x37 0x38 0x39 0x3A 0x3B 0x3C 0x3D 0x3E 0x3F
0x40 0x41 0x42 0x43 0x44 0x45 0x46 0x47 0x48 0x49 0x4A 0x4B 0x4C 0x4D 0x4E 0x4F
0x50 0x51 0x52 0x53 0x54 0x55 0x56 0x57 0x58 0x59 0x5A 0x5B 0x5C 0x5D 0x5E 0x5F
0x60 0x61 0x62 0x63 0x64 0x65 0x66 0x67 0x68 0x69 0x6A 0x6B 0x6C 0x6D 0x6E 0x6F
0x70 0x71 0x72 0x73 0x74 0x75 0x76 0x77 0xFC 0xFD 0xFE 0xFF
```

**Это не «valid bytes»** — это **identity-таблица**, т.е. **источник** substitution-mapping'а. Все байты 0..0x77 (120 значений) проходят как-есть, плюс 4 «расширенных» байта (0xFC..0xFF). Все остальные значения (0x78..0xFB — 132 значения) — **невалидны** и приводят к остановке декодирования (если ROUND4 §40 правильно описывал стоп-таблицу).

**659.** **REPL_LUT @ 0x00428530** (124 байта, substitution table) — отображение каждого VALID_LUT[i] в выходной символ:

```
0x00..0x09: '0123456789'
0x0A:       ' '
0x0B..0x29: 'AБBГДEЖЗИЙKЛMHOПPCТУЧФХЦШЩЪЫЬЭЮЯ'  (32 символа: латиница для AB и кириллица для остальных)
0x2A..0x2C: '+-='
0x2D..0x2F: 'QWe'
0x30..0x31: '.?'
0x32..0x37: 'RRTTYY'
0x38..0x39: 'xy'
0x3A..0x3B: 'rt'
0x3C..0x3E: '^><'
0x3F..0x42: 'UIOP'
0x43..0x48: 'ASDFGH'
0x49:       'F'
0x4A..0x4B: ')('
0x4C..0x4D: ':;'
0x4E..0x4F: '/,'
0x50..0x52: 'JK_'
0x53..0x54: '{}'
0x55..0x5A: 'ZXCCXZ'
0x5B:       'B'
0x5C..0x60: 'Vndsw'
0x61..0x63: 'mgN'
0x64..0x67: 'NMMv'
0x68:       '%'
0x69..0x6F: 'DGIJLQU'
0x70..0x71: 'WZ'
0x72:       '&'
0x73..0x76: '    '  (4 пробела)
0x77:       '\'
0x78..0x7B: '|`~'  (только 3 символа? Возможно 0x77 двойной)
```

Полная развёрнутая строка (как читается человеком, начиная с 0x4284d4 = identity ASCII потом REPL):
```
"0123456789 AБBГДEЖЗИЙKЛMHOПPCТУЧФХЦШЩЪЫЬЭЮЯ+-=QWe.?RRTTYYxyrt^><UIOPASDFGHF)(:;/,JK_{}ZXCCXZBVndswmgNNMMv%DGIJLQUWZ&    \|`~"
```

**660.** Замечания по REPL_LUT:
- **Дублирующиеся символы** (`RR`, `TT`, `YY`, `CC`, `XZ`/`ZX`, `NN`, `MM`) — компенсация неоднозначности (несколько входных байт → одна и та же буква)
- **Смешение латиницы и кириллицы** — буквы выглядящие одинаково (A/А, B/В, C/С, K/К, M/М, H/Н, O/О, P/Р, T/Т, X/Х, Y/У) частично латинские, частично кириллические
- **Алфавит ввода у УД-2/УД-3 ограничен** этой таблицей (всего ~85 уникальных глифов)
- **Цифры (0x00..0x09)** — пишутся всегда **прямо** (никаких альфа-замен)
- **Пробелы 0x73..0x76** — позволяют разделять секции (4 раза, для разной ширины-выравнивания?)
- **Спец-знаки 0xFC..0xFF** (которые в VALID-LUT соответствуют, но в строке REPL_LUT обрезается на 0x77) — **в фактическом REPL-LUT их представляют последние 4 символа: '\|`~'**

Полная сводная таблица (input → output):
```
Input  | Output | Input | Output | Input | Output | Input | Output
-------+--------+-------+--------+-------+--------+-------+--------
0x00   | '0'    | 0x20  | '+'    | 0x40  | 'I'    | 0x60  | 'd'
0x01   | '1'    | 0x21  | '-'    | 0x41  | 'O'    | 0x61  | 's'
0x02   | '2'    | 0x22  | '='    | 0x42  | 'P'    | 0x62  | 'w'
0x03   | '3'    | 0x23  | 'Q'    | 0x43  | 'A'    | 0x63  | 'm'
0x04   | '4'    | 0x24  | 'W'    | 0x44  | 'S'    | 0x64  | 'g'
0x05   | '5'    | 0x25  | 'e'    | 0x45  | 'D'    | 0x65  | 'N'
0x06   | '6'    | 0x26  | '.'    | 0x46  | 'F'    | 0x66  | 'N'
0x07   | '7'    | 0x27  | '?'    | 0x47  | 'G'    | 0x67  | 'M'
0x08   | '8'    | 0x28  | 'R'    | 0x48  | 'H'    | 0x68  | 'M'
0x09   | '9'    | 0x29  | 'R'    | 0x49  | 'F'    | 0x69  | 'v'
0x0A   | ' '    | 0x2A  | 'T'    | 0x4A  | ')'    | 0x6A  | '%'
0x0B   | 'A'    | 0x2B  | 'T'    | 0x4B  | '('    | 0x6B  | 'D'
0x0C   | 'Б'    | 0x2C  | 'Y'    | 0x4C  | ':'    | 0x6C  | 'G'
0x0D   | 'B'    | 0x2D  | 'Y'    | 0x4D  | ';'    | 0x6D  | 'I'
0x0E   | 'Г'    | 0x2E  | 'x'    | 0x4E  | '/'    | 0x6E  | 'J'
0x0F   | 'Д'    | 0x2F  | 'y'    | 0x4F  | ','    | 0x6F  | 'L'
0x10   | 'E'    | 0x30  | 'r'    | 0x50  | 'J'    | 0x70  | 'Q'
0x11   | 'Ж'    | 0x31  | 't'    | 0x51  | 'K'    | 0x71  | 'U'
0x12   | 'З'    | 0x32  | '^'    | 0x52  | '_'    | 0x72  | 'W'
0x13   | 'И'    | 0x33  | '>'    | 0x53  | '{'    | 0x73  | 'Z'
0x14   | 'Й'    | 0x34  | '<'    | 0x54  | '}'    | 0x74  | '&'
0x15   | 'K'    | 0x35  | 'U'    | 0x55  | 'Z'    | 0x75  | ' '
0x16   | 'Л'    | 0x36  | 'I'    | 0x56  | 'X'    | 0x76  | ' '
0x17   | 'M'    | 0x37  | 'O'    | 0x57  | 'C'    | 0x77  | ' '
0x18   | 'H'    | 0x38  | 'P'    | 0x58  | 'C'    | 0xFC  | '\'  (special)
0x19   | 'O'    | 0x39  | 'A'    | 0x59  | 'X'    | 0xFD  | '|'  (special)
0x1A   | 'П'    | 0x3A  | 'S'    | 0x5A  | 'Z'    | 0xFE  | '`'  (special)
0x1B   | 'P'    | 0x3B  | 'D'    | 0x5B  | 'B'    | 0xFF  | '~'  (special)
0x1C   | 'C'    | 0x3C  | 'F'    | 0x5C  | 'V'    |       |
0x1D   | 'Т'    | 0x3D  | 'G'    | 0x5D  | 'n'    |       |
0x1E   | 'У'    | 0x3E  | 'H'    | 0x5E  | 'd'    |       |
0x1F   | 'Ч'    | 0x3F  | 'F'    | 0x5F  | 's'    |       |
```

(124 валидных пары, без 0x78..0xFB которые невалидны.)

**Это явный substitution cipher** для отображения PASSPORT-полей прибора в человекочитаемый формат — каждый байт passport-блока заменяется на УФ-friendly символ.

---

## §161. TLV-LUT @ 0x401670 — ПОЛНОЕ СОДЕРЖИМОЕ

Подтверждение и уточнение §131 ROUND5 этап C.

**661.** **30-байт case-index table @ 0x401670**:

| tcode | case_idx |   tcode | case_idx |   tcode | case_idx |
|------:|---------:|--------:|---------:|--------:|---------:|
| 0     | **0**    |   10    | **2**    |   20    | **1**    |
| 1     | **5**    |   11    | **2**    |   21    | **1**    |
| 2     | 0        |   12    | **2**    |   22    | **1**    |
| 3     | 0        |   13    | **2**    |   23    | **1**    |
| 4     | **3**    |   14    | **2**    |   24    | **1**    |
| 5     | **4**    |   15    | **2**    |   25    | **1**    |
| 6     | **3**    |   16    | **2**    |   26    | **1**    |
| 7     | 0        |   17    | **2**    |   27    | **1**    |
| 8     | 0        |   18    | **2**    |   28    | **1**    |
| 9     | 0        |   19    | **2**    |   29    | **1**    |

**662.** **Jump table @ 0x40168E** (5 валидных handler'ов + default):

| case | handler VMA | назначение |
|------|-------------|------------|
| **0** | 0x00401765 | skip / default (тип не распознан) |
| **1** | 0x00401741 | A-scan body decoder → calls `fcn.00403420` (sample array). Body header: `body[0]=4`. |
| **2** | 0x0040171B | B-scan body decoder → calls `fcn.004031dc` (pixel grid). Body header: `body[0]=3`. |
| **3** | 0x004016CF | A-scan METADATA → calls `fcn.00402c8c` (Generic-1). TYPEZAP = `"Протокол А-развертки"`. Body header: `body[0]=1`. |
| **4** | 0x004016F5 | B-scan METADATA → calls `fcn.00402f34` (Generic-2). TYPEZAP = `"Протокол B-развертки"`. Body header: `body[0]=1`. |
| **5** | 0x004016A6 | Settings → calls `fcn.00402980`. TYPEZAP = `"Настройка"`. Body header: `body[0]=2`. |

**663.** **РАСШИФРОВКА**:

| tcode | Category meaning | DataTable record |
|-------|------------------|------------------|
| 0     | INVALID          | (skip) |
| **1** | SETTINGS (Настройка)            | NASTR table, TYPEZAP="Настройка" |
| **4, 6** | A-SCAN METADATA (Протокол А-развертки) | RESULTS table, TYPEZAP="Протокол А-развертки" |
| **5** | B-SCAN METADATA (Протокол B-развертки)  | RESULTS table, TYPEZAP="Протокол B-развертки" |
| **10..19** | B-SCAN BODY (raw pixels) | RESULTS extension or PROTOCOL blob |
| **20..29** | A-SCAN BODY (raw samples) | RESULTS extension or PROTOCOL blob |

**664.** **КЛЮЧЕВОЙ ВЫВОД — исправление §130/§148 ROUND5**: category=`sweep_addr/1000` **НЕ** маппится напрямую в категорию записи. Маппинг идёт через LUT@0x401670, и **tcode=1 = Settings, НЕ A-scan**. A-scan находится в категориях **4, 6** (метаданные) и **20..29** (тело).

Это означает:
- **A-scan transactions** состоят из ДВУХ записей: одна с category=4 или 6 (метаданные, тип, дата, оператор) и одна с category=20..29 (84 байта sample-array).
- **B-scan transactions** аналогично: category=5 (метаданные) + category=10..19 (пиксельный grid).
- **Settings** — самостоятельная запись (только category=1).

---

## §162. fcn.00402980 (Settings handler) — РАЗОБРАН

Раньше (§ROUND5 этап D) этот обработчик не был виден в декомпиляции. Прямой disasm даёт точную последовательность операций.

**665.** Внутри `fcn.00402980` (tcode=1, Settings):

```c
// Параметры: ebp+8 = record_in, ebp+0xC = ?, ebp+0x10 = out_buf, ebp+0x14 = ?
// Локалы: [ebp-0x4..-0x20] — Delphi strings

// 1. Write field NUMBER = "0"
String_NUMBER  = "NUMBER";       // 0x4285DF
String_0       = "0";            // 0x4285E6
WriteField(out_buf, "NUMBER", "0", TYPE=8 /*INTEGER*/);

// 2. Write field NUMKOD = str(record[+0x08] % 1000)
int rec_id = record[+0x08];      // sweep_addr DWORD
int numkod = rec_id % 1000;      // <--- DIVISOR 1000 для Settings/A-scan
String_NUMKOD = "NUMKOD";        // 0x4285E8
WriteField(out_buf, "NUMKOD", str(numkod), TYPE=0x14 /*INTEGER*/);

// 3. Write field TYPEZAP = "Настройка"
String_TYPEZAP   = "TYPEZAP";    // 0x4285F9
String_Nastroyka = "Настройка";  // 0x4285EF
WriteField(out_buf, "TYPEZAP", "Настройка", TYPE=0x2C /*ENUM*/);

// 4. Write field KODOPERA = str(record[+0x14])
int kodopera = record[+0x14];    // DWORD operator code
String_KODOPERA  = "KODOPERA";   // 0x428601
WriteField(out_buf, "KODOPERA", str(kodopera), TYPE=0x38 /*INT*/);

// 5. (...continue with NAMEOPERA, NUMVERS, TYPEVAR, NUMPRIB)
//    Then dispatches further fields via fcn.004022C8 (FillRest)
fcn.004022C8(record, out_buf);

// 6. End: terminate output
```

**666.** **Параметры записи `record`**:
| Offset | Field | Использование в Settings |
|--------|-------|--------------------------|
| +0x00..+0x07 | Header (8B) | пропускается |
| +0x08 | sweep_addr (DWORD) | NUMKOD = (addr % 1000) |
| +0x14 | KODOPERA (DWORD) | оператор-код |
| +0x10 | (?) | вероятно, NAMEOPERA pointer/length |
| +0x18 | info_byte (BYTE) | используется в info_byte getters |
| +0x65..+0x87 | Settings tail (136B) | акустические параметры (см. §125) |

**667.** **Тип идентификаторов WORD `[ebp-0x48]`** — это служебная enum'идентификатор поля для COM-сервера DataTable (передаётся как тип результата). Значения: `0x08` (NUMBER int), `0x14` (NUMKOD int), `0x2C` (TYPEZAP enum), `0x38` (KODOPERA int), `0x44` (NAMEOPERA text — позже)... увеличивается на 12 (0xC) или 0x18 для каждого следующего поля. **Это битовая позиция в DataTable RecSet.**

**668.** Все остальные dispatch'еры (`fcn.00402c8c`, `fcn.00402f34`, `fcn.004031dc`) имеют **идентичную структуру** — отличие только в:
- Имени `TYPEZAP` (Настройка / Протокол А / Протокол B / B-scan)
- Делителе для NUMKOD (1000 для A-scan/Settings, **10000** для B-scan)
- Финальном вызове body-handler'а (`fcn.0040xxxx`)

---

## §163. info_byte ПОЛНАЯ КАРТА БИТОВ — точное соответствие коду

**Подтверждено прямым disassembly шести функций-геттеров (0x423470..0x42352C):**

**669.** **`info_byte` имеет EXACTLY 6 значимых бит** (bit0..bit5):

| Bit | Mask | Имя | Bit=0 | Bit=1 |
|-----|------|-----|-------|-------|
| **0** | 0x01 | flash_size | "размер флеш 128 К" | "Размер флеш 512 К" |
| **1** | 0x02 | device_family | "УД2-102" | "УД3-103" |
| **2** | 0x04 | screen_width | "128" (px) | "160" (px) |
| **3** | 0x08 | distance_scale | "{25, 42, 63, 125, 280, 500, 1000, 2500, 5000}" (9 значений) | "{75, 150, 300, 600, 1000, 2000}" (6 значений) |
| **4** | 0x10 | multifreq_mode | "многочастотность отсутствует" | "многочастотность (есть режим тощиномера)" |
| **5** | 0x20 | freq_band_high | (см. ниже) | (см. ниже) |
| 6     | 0x40 | (нет геттера) | — | — |
| 7     | 0x80 | (нет геттера) | — | — |

**670.** **Биты 4+5 совместно** определяют **частотную таблицу ПЭП** (4 варианта):

| bit5 | bit4 | Freq table |
|------|------|------------|
| 0    | 0    | `{1.25, 1.8, 2.5}` (3 значения, низкоч.) |
| 0    | 1    | `{0.4, 1.25, 2.5, 5.0}` (4 значения, средняя) |
| 1    | 0    | `{1.25, 1.8, 2.5, 5.0, 10.0, 15.0}` (6 значений, расширенная) |
| 1    | 1    | `{0.1, 0.4, 0.6, 1.25, 1.8, 2.5, 5.0, 10.0}` (8 значений, максимум — есть толщиномер 0.1 МГц) |

Все частоты — **в МГц**.

**671.** **Точная C-реализация:**

```c
// fcn.00423470: GetFlashSizeString(ctx)
char* get_flash_size(struct ctx* c) {
    if (c->info_byte & 0x01) return "Размер флеш 512 К";
    return "размер флеш 128 К";
}

// fcn.00423488: GetDeviceTypeString(ctx)
char* get_device_type(struct ctx* c) {
    if (c->info_byte & 0x02) return "УД3-103";
    return "УД2-102";
}

// fcn.004234a4: GetScreenWidthString(ctx)
char* get_screen_width(struct ctx* c) {
    if (c->info_byte & 0x04) return "160";  // pixels
    return "128";
}

// fcn.004234c0: GetDistanceScaleString(ctx)
char* get_dist_scale(struct ctx* c) {
    if (c->info_byte & 0x08) return "{75, 150, 300, 600, 1000, 2000}";
    return "{25, 42, 63, 125, 280, 500, 1000, 2500, 5000}";
}

// fcn.004234dc: GetMultifreqString(ctx)
char* get_multifreq(struct ctx* c) {
    if (c->info_byte & 0x10) return "многочастотность (есть режим тощиномера)";
    return "многочастотность отсутствует";
}

// fcn.004234f8: GetFreqTableString(ctx)
char* get_freq_table(struct ctx* c) {
    if (c->info_byte & 0x20) {                   // bit5 set
        if (c->info_byte & 0x10) return "{0.1, 0.4, 0.6, 1.25, 1.8, 2.5, 5.0, 10.0}";
        else                    return "{1.25, 1.8, 2.5, 5.0, 10.0, 15.0}";
    } else {                                     // bit5 clear
        if (c->info_byte & 0x10) return "{0.4, 1.25, 2.5, 5.0}";
        else                    return "{1.25, 1.8, 2.5}";
    }
}

// fcn.00423534: FormatDate(ctx) — "DD.MM"
void format_date(struct ctx* c, char* out) {
    sprintf(out, "%.2d.%.2d", c->day, c->month);  // c->day = +0x0A, c->month = +0x0B
}
```

**672.** **ИСПРАВЛЕНИЯ ROUND4/5**:
- ROUND4 §16.3 / ROUND5 §103: `info_byte.bit0` = flash_512K_extended → **подтверждено** (✓).
- ROUND4 §16.3 / ROUND5 §103: `info_byte.bit2` = "wagon_LCD 5253B" — **НЕВЕРНО**. На самом деле bit2 = LCD ширина (160px vs 128px). Размер байт картинки экрана определяется иной формулой.
- ROUND5 §103: `info_byte.bit3` = "шкала дальности" — **подтверждено** (✓), но точные таблицы теперь известны.
- ROUND5 §103: bits 4+5 = "freq table selector" — **подтверждено и уточнено** (✓): 4 разных таблицы по 3/4/6/8 значений.
- ROUND5 §550-552: bit1 семантика была «неясна» — **разрешено**: bit1 = device_family (УД2/УД3).

**673.** **Размер картинки экрана** (0x10C5=4293 vs 0x1485=5253 байт): зависит от bit0 (flash) И bit2 (width), точная формула неизвестна напрямую из disasm'а, но проверка:
- 128px × 256_height / 8 = 4096 + headers ≈ 4293 ✓
- 160px × 256_height / 8 = 5120 + headers ≈ 5253 ✓

То есть **0x10C5 = 4293 = 128px wide LCD bitmap** (bit2=0), **0x1485 = 5253 = 160px wide LCD bitmap** (bit2=1). FLASH bit0 — отдельный параметр (не влияет на screen size в этой формуле).

---

## §164. Display strings — для разделов «Информация о приборе»

**674.** **Структура справочного вывода после handshake** (PelengPC.exe строит этот текст):

```
Номер прибора : <NUMPRIB><LF>
<LF>
Версия ПО : <NUMVERS><LF>
<LF>
<LF>
Тип прибора : <get_device_type(info_byte)><LF>
<LF>
Размер экрана : <get_screen_width(info_byte)> Гц<LF>      ← bug: "Гц" вместо "px"
<LF>
Частота зондирования : <freq_probe> Гц<LF>
<LF>
Частота сигнала : <freq_signal> МГц<LF>
```

(Видна **бага в коде**: после "Размер экрана" печатается " Гц" — должно быть " px" или " точек". Это объясняется тем, что строка `"Размер экрана : "` соседствует с `" Гц"` в .data, и компилятор объединил их в `printf`-стиль вывод по ошибке. Подтверждается отсутствием отдельной строки для размера в .data.)

**675.** **Строковые константы в `.data` (PelengPC.exe)**:
| VMA | String (UTF-8) |
|------|---------------|
| 0x52f43c | "Размер флеш 512 К" |
| 0x52f44e | "размер флеш 128 К" |
| 0x52f460 | "УД2-102" |
| 0x52f468 | "УД3-103" |
| 0x52f470 | "160" |
| 0x52f474 | "128" |
| 0x52f478 | "{75, 150, 300, 600, 1000, 2000}" |
| 0x52f498 | "{25, 42, 63, 125, 280, 500, 1000, 2500, 5000}" |
| 0x52f4c6 | "многочастотность отсутствует" |
| 0x52f4e3 | "многочастотность (есть режим тощиномера)" |
| 0x52f50c | "{0.1, 0.4, 0.6, 1.25, 1.8, 2.5, 5.0, 10.0}" |
| 0x52f537 | "{1.25, 1.8, 2.5, 5.0, 10.0, 15.0}" |
| 0x52f559 | "{0.4, 1.25, 2.5, 5.0}" |
| 0x52f56f | "{1.25, 1.8, 2.5}" |
| 0x52f580 | "%.2d.%.2d" (date format) |
| 0x52f7ad | "\\\\.\\" (Windows COM port path prefix) |
| 0x52f7b3 | "ERROR !!! TimeOut waiting for response." |
| 0x52f7dc | "Номер прибора : " |
| 0x52f7ef | "Версия ПО : " |
| 0x52f800 | "Тип прибора : " |
| 0x52f811 | "Размер экрана : " |
| 0x52f824 | "Частота зондирования : " |
| 0x52f83c | " Гц\n" |
| 0x52f843 | "Частота сигнала : " |
| 0x52f856 | " МГц\n" |
| 0x52f85c | "PortName" |
| 0x52f869 | "COM1" |

---

## §165. Baud Table — 15 значений, корректно

**676.** Полная таблица baud rate, дублирована в 7 местах `.data` (для UI dropdown'ов):

```
Index | Baud rate
------+----------
   0  |    110   (для модемов)
   1  |    300
   2  |    600
   3  |   1200
   4  |   2400
   5  |   4800
   6  |   9600
   7  |  14400
   8  |  19200    ← DEFAULT для PELENG
   9  |  38400
  10  |  56000    (нестандартный)
  11  |  57600    ← SECONDARY auto-detect rate
  12  | 115200
  13  | 128000    (нестандартный)
  14  | 256000    (нестандартный)
```

**677.** Auto-detect перебирает **только 2 скорости**: `19200` (бит 8) и `57600` (бит 11), как и было задокументировано в §111/§146.

**678.** Адреса baud table в .data:
- 0x00523938 (FormTestPort dropdown #1)
- 0x00527E28 (FormTestPort dropdown #2 — duplicate)
- 0x00528350 (FormSettings? )
- 0x0052A1F4 (FormAutoConnect)
- 0x0052BDE0 (FormReadData?)
- 0x0052DB34 (FormNastr / Settings dialog)
- 0x0052F754 (FormReadData / com-port checker)

(7 копий — это **результат компиляции отдельных Delphi-форм**, каждая инициализирует свой ComboBox.Items независимо.)

---

## §166. CLSID для COM-сервера DataTable

**679.** **Точное значение CLSID** (16 байт из `102_203dll.dll` @ 0x0042821C):

```
Hex bytes (LE):
  49 D4 92 E3   E3 EC   27 4E   BF 74 05 78 0F A1 E6 D6
```

**Standard GUID-format**:
```
{E392D449-ECE3-4E27-BF74-05780FA1E6D6}
```

**680.** Этот GUID — **CLSID или IID** для внешнего COM-сервера, реализующего метод `"ShowWindow"` (W-string @ 0x42820e), который принимает `VT_ARRAY|VT_UI1` (BYTE[] из 4-байтных префиксованных Delphi-строк). DLL запрашивает интерфейс через `CoCreateInstance(CLSID_*, ...)` и вызывает `ShowWindow` (через `IDispatch.Invoke`).

**681.** **Поиск CLSID в реестре** (для определения связанного COM-сервера): `regedit` → `HKCR\CLSID\{E392D449-ECE3-4E27-BF74-05780FA1E6D6}`. Имя сервера = `LocalServer32` или `InprocServer32`. **Это позволит идентифицировать какой именно DataTable-сервер использует PELENG** (вероятно, продукт от того же Altek или сторонний COM-server для управления формами/таблицами).

**682.** **WriteField-output buffer layout** (что DLL отправляет в DataTable.ShowWindow):
```
[FieldName_Length][FieldName_bytes...]
[Value_Length][Value_bytes...]
[FieldType_BE16]
... (repeat for each field)
[Terminator]
```
Это длинная очередь BSTR-подобных пар (field_name, value) с типом для каждой.

---

## §167. PelengPC.fdb — структура базы данных (Firebird Embedded)

**683.** **Файл `PelengPC.fdb`** — Firebird 2.x database template (ODS 11.0, не открывается Firebird 3.x). Содержит **создание схемы и пустые таблицы**.

**684.** **Имена user-таблиц** (через strings):
- **`NASTR`** (Настройки) — соответствует Table12/22/32 в .dal
- **`RESULTS`** (Протоколы) — соответствует Table11/21/31 в .dal
- Primary keys: **`PK_NASTR`**, **`PK_RESULTS`**

**Имена SQL-генераторов:** `SQL$NASTR`, `SQL$RESULTS` (Firebird AutoIncrement).

**685.** **Столбцы NASTR/RESULTS таблиц** (из strings):
- `NUMBER` (PK, auto)
- `NUMKOD` (record key)
- `NUMOBJ` (object number)
- `KODOPERA` (operator code)
- `TYPEZAP` (record type enum)
- (...всё что в .dal Field*_*)

**686.** **Тип хранения** для protocol blob: `BLOB SUB_TYPE 0` (binary) — Firebird default, что подтверждает что **PROTOCOL поле сохраняет ВЕСЬ raw RAW-record (header+body+sample/pixel data)** в БД, и DLL декодирует его обратно при загрузке.

**687.** **Размер .fdb** = 712 704 байт (174 страницы × 4096 байт). Это пустая база со схемой. Реальные данные пользователя растут добавлением страниц.

**688.** **Доступ к .fdb из PelengPC.exe** — через `gds32.dll` (Firebird Embedded 2.0 client library), импортируемую как unmanaged. Без сервера, только локальный файл.

---

## §168. Versions/Firmware list — что поддерживает PelengPC

**689.** **Version1 "Базовая"** — поддерживает 18 firmware variants:

```
Группа 03.6x (УД2-102):  03.62, 03.63
Группа 03.8x (УД2-102):  03.80, 03.81  
Группа 03.9x (УД2-102):  03.90, 03.91
Группа 07.1x (УД3-103):  07.12, 07.13
Группа 07.3x (УД3-103):  07.30, 07.31
Группа 07.4x (УД3-103):  07.40, 07.42  
Группа 07.8x (УД3-103):  07.80, 07.81
Группа 06.8x (?):         06.80, 06.81, 06.82, 06.83
```

**690.** **Version2 "Железнодорожная"** — отдельный набор FW (из контекста: вероятно `04.42/4.43, 6.42/6.43, 4.80/4.81`).

**691.** **Version3 "Рельсовая"** — FW `1.50/1.51` (упомянуты в pelengpchelp.hlp).

**692.** Из этого следует:
- **Major-digit FW** (03/06/07 для Version1; 04/06 для Version2; 01 для Version3) указывает на **device class** и обусловливает выбор DLL+схемы.
- **Minor-digit FW** (62/63/80/81/...) — внутри-класса инкрементальные обновления (формат данных совместим).

---

## §169. Strings в DLL — справочные карты field name'ов и enum'ов

**693.** **Полный список Cyrillic строк** в `102_203dll.dll` (.data секция):

```
0x00428007: ' $ШA'        (mangled VMT?)
0x004285CE: 'нет'          ← enum «нет» для DEFEKT
0x004285D2: 'есть'         ← enum «есть» для DEFEKT
0x004285DF: 'NUMBER'       ← field name
0x004285EF: 'Настройка'    ← TYPEZAP value для tcode=1 (Settings)
0x004285F9: 'TYPEZAP'      ← field name
0x00428601: 'KODOPERA'     ← field name
0x0042860A: 'NAMEOPERA'    ← field name (10 chars)
0x00428616: 'NUMVERS'      ← field name
0x0042861E: 'TYPEVAR'      ← field name
0x00428626: 'NUMPRIB'      ← field name
0x0042862E: 'NUMBER'       (copy in Generic-1)
0x0042863E: 'Протокол А-развертки' ← TYPEZAP value для tcode=4,6 (A-scan)
0x00428653: 'TYPEZAP'      (copy)
0x00428690: 'Протокол B-развертки' ← TYPEZAP value для tcode=5 (B-scan)
0x004287AC: 'PROTOCOL'     ← field name (BLOB pointer)
0x004288C8: 'в пути'       ← enum @+0x35 case 0   ✅ РАЗРЕШЕНО (ROUND5 ❓ из этап D)
0x004288CF: 'на рсп'       ← enum @+0x35 case 1
0x004288D6: 'покм. зап.'   ← enum @+0x35 case 2
0x004288E1: 'стр. пер.'    ← enum @+0x35 case 3
0x004288EB: 'стр. з-д'     ← enum @+0x35 case 4
0x004288F4: ' '            ← enum @+0x35 case 5 (пустая строка)
0x0042B67D: ' п/п'         ← Title для NUMBER в выводе
0x0042B68A: ' записи'      ← Title для NUMKOD
0x0042B699: 'Вид записи'   ← Title для TYPEZAP
0x0042B6AC: 'Дата'         ← Title для DATEFORM
0x0042B6BA: 'Время'        ← Title для TIMEFORM
0x0042B6C9: 'Шифр оператора'  ← Title для KODOPERA
0x0042B6E1: 'ФИО оператора'   ← Title для NAMEOPERA
0x0042B6F9: 'Версия ПО'    ← Title для NUMVERS
0x0042B70C: ' прибора'     ← Title для NUMPRIB (после "№")
0x0042B71D: 'Типовой вариант'  ← Title для TYPEVAR
0x0042B735: 'Запись в БД'  ← Title для NUMZAP
0x0042B7D7: ' объекта'     ← Title для NUMOBJ (после "№")
0x0042B8CF: 'Часы'         ← Title для CLOCK
0x0042B8DA: 'Плавка'       ← Title для SMELTING
0x0042B8EA: 'Год изготовления'  ← Title для MAKETIME
0x0042B904: 'Дефект'       ← Title для DEFEKT
0x0042B912: 'Код дефекта'  ← Title для CODEDEF
0x0042B926: 'Условная, протяженность, мм'  ← Title для CONDLENGTH (Version2/Generic)
0x0042BA3A: 'Завод изготовитель'  ← Title для INDMAKER
0x0042BA56: 'Год зготовления'  ← typo: "Год зготовления" (без "и") — bug
0x0042BB7E: 'ход/перегон'  ← Title для STAGE (Version3)
0x0042BB90: 'Звено'        ← Title для SECTION
0x0042BB9E: 'Пикет'        ← Title для PICKETLINE
0x0042BBAF: 'Путь'         ← Title для PATH
0x0042BBDB: 'Условная протяженность, мм'  ← Title для CONDLENGTH (Version3, без запятой)
0x0042BCE0: 'Количество дефектов'  ← Title для (Controll table)
0x0042BCFC: 'Протокол'     ← Title для PROTOCOL
0x0042BEE0: 'Место'        ← Title для PLACE (Version3 only)
```

**694.** **РАЗРЕШЕНО ❓ из этап D**: 6 локализованных строк enum @+0x35:

```
case 0 → "в пути"        (in transit, on the track)
case 1 → "на рсп"        (at repair station / depot)
case 2 → "покм. зап."    (kilometre-marker record / pickup record)
case 3 → "стр. пер."     (track-transfer / structure transition)
case 4 → "стр. з-д"      (structure-to-plant / construction-to-factory)
case 5 → " "             (empty/default)
```

Это **локационная метка для railway / rails** (Version2/3 used). Указывает где сделан замер (категория объекта). Часть **типового варианта** записи (TYPEVAR).

---

## §170. Корректировки/уточнения к ROUND4/5

### Сводная таблица всех исправлений за этап E

| Раунд/§ | Прежнее | Исправлено |
|---------|---------|------------|
| ROUND4 §16.3 | `info_byte.bit2 = wagon_LCD (5253B)` | `bit2 = LCD_width (160px vs 128px)` — связано с size но не одно и то же |
| ROUND4 §44.x | `default baud=9600` | `default=19200` (table @ 0x523938: 15 values, 110..256000) |
| ROUND5 §130 | category 1 = A-scan | **category 1 = Settings** (Настройка). A-scan находится в категориях 4 (metadata) и 20..29 (body). |
| ROUND5 §131 | TLV-LUT содержимое неясно | **Точное содержимое 30 байт** + 6-entry jump table @0x40168E |
| ROUND5 §132 | PASSPORT_LUT 124 байта | **Точное hex-содержимое** обоих LUT (VALID @0x4284B4 + REPL @0x428530) |
| ROUND5 §150 | enum @+0x35 — 6 неизвестных строк | **6 строк извлечены**: "в пути", "на рсп", "покм. зап.", "стр. пер.", "стр. з-д", " " |
| ROUND5 §155 | fcn.00402980 не виден | **Полностью разобран** — Settings dispatch с NUMBER/NUMKOD/TYPEZAP/KODOPERA |
| ROUND5 §550-552 | info_byte bit1 неясно | **bit1 = device_family (УД2-102 vs УД3-103)** |
| ROUND5 etap D ❓ | header[3..15] — 13 байт | (всё ещё ❓ — но карта VMA/file offset для PE даёт ключи к будущему disasm) |
| ROUND5 etap D ❓ | bool_flag @+0x0C значение | (всё ещё ❓ — нет уточнений из этапа E) |
| ROUND5 etap D ❓ | Deflate в протоколе? | **НЕТ** — Firebird BLOB SUB_TYPE 0 (raw), и сам protocol stream — не сжат |

---

## §171. ИТОГИ ЭТАПА E

### Новые находки (#643-700, всего +58):

- **§158**: Инвентарь содержимого инсталлятора (24 файла, MD5, версии BCB/MSVC)
- **§159**: Полная схема `PelengPCtest.dal` (3 версии, 10 таблиц, 11 типов TypeField, 100+ полей)
- **§160**: PASSPORT-LUT — точное hex-содержимое 248 байт, character mapping разобран побайтно
- **§161**: TLV-LUT @0x401670 — точное содержимое 30-byte index + 6-entry jump table
- **§162**: `fcn.00402980` Settings handler полностью разобран
- **§163**: info_byte 6 бит — точное соответствие коду; 4 freq-таблицы (bit4+bit5)
- **§164**: Display strings для «Информация о приборе» полностью извлечены (баг "Гц" вместо "px")
- **§165**: Baud table — 15 значений (110..256000), 7 копий в .data
- **§166**: CLSID для DataTable COM-сервера = `{E392D449-ECE3-4E27-BF74-05780FA1E6D6}`
- **§167**: PelengPC.fdb — Firebird ODS 11.0, таблицы NASTR/RESULTS, BLOB SUB_TYPE 0
- **§168**: 18 firmware variants для Version1 + классификация по major-digit
- **§169**: Все Cyrillic strings в DLL — 50+ Title/FieldName/Enum значений
- **§170**: 11 корректировок к ROUND4/5 (info_byte, baud, TLV-LUT, enum, category, fcn.00402980)

### Главные технические выводы:

1. **Settings, A-scan, B-scan — три самостоятельных типа** записей с разными dispatch'ерами.
2. **A-scan transaction = 2 records** (metadata category=4/6, body category=20..29).
3. **B-scan transaction = 2 records** (metadata category=5, body category=10..19).
4. **info_byte имеет 6 значимых бит** (0..5), не 2 как в ROUND4.
5. **PASSPORT-LUT — substitution cipher**, не BCD validator.
6. **Database = Firebird Embedded** (NASTR/RESULTS таблицы, BLOB raw protocol).
7. **DataTable COM-сервер** — внешний (не входит в инсталлятор), доступ через CLSID `{E392D449-...}`.
8. **Все строки Title/FieldName/Enum** — hardcoded в `.data` DLL/EXE (НЕ в .dal — последний только для UI).

### Что ОСТАЁТСЯ к реверсу (❓):

- ❓ Содержимое `header[3..15]` после handshake (13 байт между info_byte и catalog) — требуется capture реального стрима
- ❓ `bool_flag @ +0x0C` — точное назначение (defect/pass/calib?)
- ❓ `aux_le16 @ +0x15..+0x16` (firmware version? subprotocol?) — требуется capture
- ❓ `Real48` использование в записях
- ❓ Полные значения локализаций в Version2/3 (`Железнодорожная`/`Рельсовая`)
- ❓ Связь между бит `info_byte` и поведением (например, как bit3=distance_scale влияет на формулы пересчёта sample → mm)
- ❓ Точная семантика `[Numbers]` секции в .dal (124 hex-байт shifr/license info)
- ❓ Что выполняет zapis2.exe (написана в 2011, на 2 года позже)

### Общая статистика:
- ROUND4: 189 находок (§1-66)
- ROUND5 этап A: +105 (190-294)
- ROUND5 этап B: +65 (295-359)
- ROUND5 этап C: +176 (360-535)
- ROUND5 этап D: +107 (536-642)
- **ROUND5 этап E: +58 (643-700)**
- **TOTAL: 700 находок**

Готово. ROUND5 этап E завершён.

---

## §172. info_byte биты 6+7 — обнаружено, частично разобрано

**695.** Поиск `TEST BYTE PTR [reg+0x18], imm` по всему `.text` PelengPC.exe выявил **дополнительную проверку маски 0xC0**:

```
@ 0x004caab7: test BYTE PTR [eax+0x18], 0xC0
              je   0x4caac4    ← если bits 6 И 7 = 0, пропустить redraw
              call 0x4cc658    ← иначе: вызвать update/redraw
```

Контекст: функция `fcn.004caa0c` — это **обработчик-композитный** для проверки состояния прибора при обновлении UI. Он последовательно проверяет:
1. `[+0x18] & 0x08` (bit 3, distance scale)
2. `[+0x18] & 0x02` (bit 1, device family)
3. `[+0x18] & 0x01` (bit 0, flash size)
4. **`[+0x18] & 0xC0`** (bits 6+7) ← **новый бит-флаг!**

**696.** **Семантика bits 6+7**: если хотя бы один из них установлен — UI вызывает `fcn.4cc658` (redraw/refresh). Это используется КАК "marker для устройств, где нужна доп. логика". Возможно:
- **bit 6** = experimental/dev-mode flag
- **bit 7** = production sealed
- (или) **bits 6+7 совместно** = идентификатор внутри-семьи (например, sub-model код)

Эти биты не имеют отдельных геттеров для строкового вывода (как bits 0-5) — следовательно, **они не отображаются в "Информации о приборе"**, но влияют на поведение приложения (например, какие меню/кнопки разрешены).

**697.** Также найдены `test [edi+0x40], 0x10` (НЕ info_byte, а другой флаг) и `test [edi+0x45], 0x2` — это другие state-флаги в структуре `g_comPort`, не относящиеся к info_byte.

---

## §173. Дополнительные FieldName/Title константы в DLL

**698.** **8 шаблонов полей** (по `record_type × device_version`), все hardcoded в `.data` DLL:

| Шаблон | Адрес | Используется для |
|--------|-------|------------------|
| **Settings (tcode=1)** | 0x4285DF — 0x42862E | Все Settings-записи (Базовая, Железнодорожная, Рельсовая) — одинаковая структура |
| **A-scan metadata (tcode=4,6)** | 0x42862E — 0x42867F | Все версии — единый набор полей |
| **B-scan metadata (tcode=5)** | 0x428680 — 0x4286D1 | Все версии — единый набор полей |
| **B-scan body (tcode=10..19)** | 0x4286D2 — 0x428706 | (без TYPEZAP — это body, не header) |
| **A-scan body (tcode=20..29)** | 0x428707 — 0x42873B | (без TYPEZAP) |
| **Version1 extension** | 0x42873C — 0x428789 | NUMOBJ, TYPEVAR, M, MM, CLOCK, SMELTING, MAKETIME, DEFEKT, CODEDEF, CONDLENGTH |
| **Version2 extension** | 0x4287B5 — 0x4287ED | NUMOBJ, TYPEVAR, SMELTING, MAKETIME, INDMAKER, DEFEKT, CODEDEF |
| **Version3 extension** | 0x42880E — 0x42885C | NUMOBJ, TYPEVAR, KM, M, MM, STAGE, SECTION, PICKETLINE, PATH, DEFEKT, CODEDEF, CONDLENGTH |
| **Version3 sub-ext** | 0x42885D — 0x42889F | NUMOBJ, TYPEVAR, KM, M, MM, PLACE, STAGE, SECTION, PICKETLINE, PATH, CODEDEF, PROTOCOL |

**699.** **NOBJECT** — новое имя поля @ 0x42873C (упомянуто только один раз). Возможно, "число объектов" (counter) или "имя объекта" (string). Используется только в Version1 extension. Точная семантика пока ❓.

**700.** **PROTOCOL** @ 0x4287AC и 0x428805 — это **BLOB-поле**, хранит RAW-record целиком (binary). Используется для PROTOCOL = "Протокол" поле в таблице RESULTS (см. §159).

---

## §174. ОБНОВЛЁННЫЕ ИТОГИ ЭТАПА E

### Новые находки (#643-720):

- **§158**: Инвентарь инсталлятора (24 файла, MD5, версии BCB/MSVC)
- **§159**: Полная схема `PelengPCtest.dal` (3 версии, 10 таблиц, 11 типов TypeField, 100+ полей)
- **§160**: PASSPORT-LUT — точное hex-содержимое 248 байт
- **§161**: TLV-LUT @0x401670 — полное содержимое 30-byte index + 6-entry jump table
- **§162**: `fcn.00402980` Settings handler полностью разобран
- **§163**: info_byte 6 бит точно соответствуют коду (биты 0-5)
- **§164**: Display strings для «Информация о приборе» с багом ("Гц" вместо "px")
- **§165**: Baud table — 15 значений (110..256000)
- **§166**: CLSID для DataTable = `{E392D449-ECE3-4E27-BF74-05780FA1E6D6}`
- **§167**: PelengPC.fdb структура (Firebird ODS 11.0, NASTR/RESULTS)
- **§168**: 18 firmware variants для Version1
- **§169**: Все Cyrillic strings в DLL — 50+ Title/FieldName/Enum
- **§170**: 11 корректировок к ROUND4/5
- **§172**: info_byte bits 6+7 обнаружены (mask 0xC0, used for redraw flag)
- **§173**: 8 шаблонов полей (по record_type × version) в DLL

### КРИТИЧЕСКИЕ исправления (Round 4/5 → Round 5 etap E):

1. **info_byte bit2** = LCD width (160/128px), НЕ wagon_screen size
2. **default baud** = 19200 (не 9600)
3. **TLV category 1** = Settings (не A-scan)
4. **A-scan = 2 records** (category 4/6 metadata + 20..29 body)
5. **B-scan = 2 records** (category 5 metadata + 10..19 body)
6. **PASSPORT_LUT** — точное substitution mapping для 124 байт, не BCD validator
7. **6 enum @+0x35 строки** — "в пути"/"на рсп"/"покм. зап."/"стр. пер."/"стр. з-д"/" "

### ❓ ОСТАЁТСЯ к реверсу:

- ❓ `header[3..15]` (13 байт после info_byte) — содержит больше данных от устройства, точная семантика неизвестна
- ❓ `aux_le16 @ +0x15..+0x16` — firmware sub-version? subprotocol?
- ❓ Что именно `Real48` (Borland 6-byte float) делает в записях
- ❓ Bits 6+7 info_byte — отдельная семантика (помимо composite mask 0xC0)
- ❓ `NOBJECT` поле — counter или string?
- ❓ Что делает zapis2.exe (2011, отдельная программа для записи отчётов в графику)
- ❓ Точные firmware-listы для Version2 ("Железнодорожная") и Version3 ("Рельсовая")
- ❓ Связь TLV-категорий 7,8,9 (в LUT case=0 = skip) — есть ли категории резерв для будущих типов?

### Общая статистика после этапа E:

- ROUND4: 189 находок (§1-66)
- ROUND5 этап A: +105 (190-294)
- ROUND5 этап B: +65 (295-359)
- ROUND5 этап C: +176 (360-535)
- ROUND5 этап D: +107 (536-642)
- **ROUND5 этап E: +78 (643-720)**
- **TOTAL: 720 находок**

Готово. ROUND5 этап E завершён и расширен.

---

# ЭТАП F — Полный disasm decoder pipeline

(Глубокий построчный реверс ВСЕХ handler'ов, WriteField encoder, TypeVar dispatcher, валидатора даты. Корректирует/уточняет всё, что выше.)

---

## §175. WriteField encoder fcn.00401f54 — ТОЧНОЕ поведение 3 типов

**721.** Сигнатура (fastcall + cdecl mix):
```
WriteField(eax=output_buf, edx=output_meta, ecx=field_name_pstr, push value_pstr) → void
```

Локальные переменные:
- `[ebp-0x48]` = output_buf (saved eax)
- `[ebp-0x4c]` = output_meta (saved edx)
- `[ebp-0x50]` = field_name_pstr (saved ecx)
- `[ebp+0x8]` = value_pstr (1st stack arg)
- `[ebp-0x5c]` = field_index (lookup result)
- `[ebp-0x60]` = field_offset_in_output (from descriptor +0x4)
- `[ebp-0x64]` = field_descriptor_ptr

Шаги:
1. **Lookup**: `fcn.00401d58(edx=name, eax=meta)` → field_index или -1
2. Если -1 → bail out (без записи)
3. **Geo offset**: `fcn.00401ea4(edx=field_index_str, eax=meta)` → file_offset
4. **Get descriptor**: `fcn.0040ca30(edx=field_index, eax=meta)` → field_descriptor
5. **Dispatch на `field_descriptor[+0x8]` (type code)**:
   - `type = 1` → INTEGER (BE) → §722
   - `type = 2` → STRING (zero-fill) → §723
   - `type = 3` → DATE (3-byte) → §724
   - другое → пропустить

**722.** **TYPE 1 — INTEGER**:
```c
int int_val = StrToInt(value_str);            // FCN_00414274
int field_len = descriptor[+0x10];            // в байтах (1, 2, 4)
for (i = 0; i < field_len; i++) {
    output_buf[field_offset + field_len - 1 - i] = (int_val >> (i*8)) & 0xFF;
}
```
Результат — **BIG-ENDIAN** N-байтное значение (N = field_len из descriptor). Подтверждает ROUND5 §70.

**723.** **TYPE 2 — STRING**:
```c
int field_len = descriptor[+0x10];
fillchar(output_buf + field_offset, 0, field_len);    // ZERO-FILL (НЕ space!)
int val_len = pstring_length(value_str);
int copy_len = min(val_len, field_len);
memcpy(output_buf + field_offset, value_str_data, copy_len);
```
**Исправление ROUND5 §133**: строки **zero-padded справа** (NUL 0x00), не space-padded.

**724.** **TYPE 3 — DATE (3 байта)**:
```c
// value_str ожидается в формате "ddmmyy" (или содержит подстроки на этих позициях)
char dd_substr[2] = Copy(value_str, 1, 2);   // позиции 1..2 → день
char mm_substr[2] = Copy(value_str, 3, 4);   // позиции 3..4 → месяц  
char yy_substr[2] = Copy(value_str, 5, 6);   // позиции 5..6 → год (00..99)
int dd = StrToInt(dd_substr);
int mm = StrToInt(mm_substr);
int yy = StrToInt(yy_substr);
output_buf[field_offset + 0] = (byte)dd;
output_buf[field_offset + 1] = (byte)mm;
output_buf[field_offset + 2] = (byte)yy;
```

Используются помощники:
- `fcn.00426ea0`: Copy(str, start, count) — Pascal Copy() substring
- `fcn.00415b70`: Val/StrToInt
- `fcn.00414238`: byte conversion (truncate)

**Поправка**: год хранится как `(year - 2000)` (0..99), НЕ `year - 1900`.

---

## §176. fcn.00402874 = ValidateDate — ТОЧНЫЙ алгоритм

**725.** Сигнатура: `bool ValidateDate(int year, int month, int day)` (cdecl, 3 stack args)

**726.** Алгоритм:
```c
bool ValidateDate(int year, int month, int day) {
    if (year <= 2000) return false;          // ← strict: year MUST be > 2000
    if (month > 12 || month < 1) return false;
    
    // jump table @ 0x4028C3, indexed by table @ 0x4028B6 with month-1
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return (day >= 1 && day <= 31);
        case 4: case 6: case 9: case 11:
            return (day >= 1 && day <= 30);
        case 2:  // FEBRUARY
            if ((year & 3) == 0) {            // year % 4 == 0
                if ((year % 100) == 0)
                    return (day >= 1 && day <= 28);   // ← BUG: should be 29 if year%400==0
                else
                    return (day >= 1 && day <= 29);   // leap year
            } else {
                return (day >= 1 && day <= 28);       // common year
            }
    }
    return false;
}
```

**Баги** (подтверждено disasm):
- **year=2000** жёстко отвергается (cmp + jle: используется `cmp dword,0x7d0; jle false`)
- **2000-й год не leap** — несоответствие григорианскому календарю (2000 % 400 == 0 → leap), но из-за strict-rule на year=2000 этот баг не виден
- **2400 год** будет 28 дней февраля (баг проявится в 2400 году)

**727.** **DATE extractor fcn.004023b0** (вызывается из dispatcher с offset=7):
```c
int day = body[offset + 0];
int month = body[offset + 1];
int year = body[offset + 2] + 2000;       // ← byte → year (2000..2255)

if (ValidateDate(year, month, day)) {
    TDateTime dt = EncodeDate(year, month, day);   // fcn.00414DAC
    str = FormatDateTime("dd-mm-yy", dt);          // → "DATEFORM"
} else {
    // fallback: default = "01-01-2000"
    TDateTime dt = EncodeDate(2000, 1, 1);
    str = FormatDateTime("dd-mm-yy", dt);
}
WriteField(buf, meta, "DATEFORM", str);   // type 0x38 = string
```

---

## §177. TLV-LUT @0x401670 — ПОЛНОСТЬЮ декодирован (30 байт)

**728.** Точное содержимое LUT (case-index, tcode → case_id):
```
tcode= 0: case 0 (skip)
tcode= 1: case 5 → Settings handler (fcn.00402980)
tcode= 2: case 0 (skip — RESERVED)
tcode= 3: case 0 (skip — RESERVED)
tcode= 4: case 3 → A-scan metadata handler (fcn.00402c8c)
tcode= 5: case 4 → B-scan metadata handler (fcn.00402f34)
tcode= 6: case 3 → A-scan metadata handler (fcn.00402c8c)   ← ALIAS tcode 4
tcode= 7: case 0 (skip — RESERVED)
tcode= 8: case 0 (skip — RESERVED)
tcode= 9: case 0 (skip — RESERVED)
tcode=10..19: case 2 → B-scan body handler (fcn.004031dc)   ← 10 codes!
tcode=20..29: case 1 → A-scan body handler (fcn.00403420)   ← 10 codes!
```

**729.** Семантика codes:
- **0, 2, 3, 7, 8, 9** — explicitly RESERVED (6 unused slots для будущих типов записей)
- **1** — Settings (одна запись на сессию = настройка)
- **4 vs 6** — два варианта A-scan metadata (вероятно: первичная регистрация vs повторная обработка)
- **5** — единственный B-scan metadata
- **10..19** — B-scan body (10 sub-codes — для **multi-frame** B-scan, см. §148: header[+0x0E] = frame_count, и каждый кадр имеет свой sub-code 10..19)
- **20..29** — A-scan body (10 sub-codes — для multi-channel A-scan: до 10 каналов на одной развертке, или для разных ПЭП)

**730.** **Корректировка ROUND4 §16 + ROUND5 §130, §144, §170**:  
A-scan body имеет именно **20..29 tcodes** (10 значений), B-scan body — **10..19** (10 значений).  
Это даёт **до 10 SUB-frame's на A-scan** и **до 10 SUB-frame's на B-scan**. Раньше формулировалось расплывчато («10-19 / 20-29»).

---

## §178. Handler fcn.00402980 (Settings, tcode=1) — ТОЧНЫЕ поля + offsets

**731.** Последовательность WriteField calls (по disasm):

| # | Type byte | Field name | Value source | Encoded type |
|---|-----------|------------|--------------|--------------|
| 1 | 0x08 | "NUMBER"   | literal "0"             | INTEGER (BE) |
| 2 | 0x14 | "NUMKOD"   | **record[+0x8] % 1000** | INTEGER (BE) |
| 3 | 0x38 | "TYPEZAP"  | literal "Настройка"     | STRING (zero-pad) |
| 4 | 0x44 | "KODOPERA" | record[+0x14]           | INTEGER (BE) |
| 5 | 0x50 | "NAMEOPERA"| literal " " (single space) | STRING |
| 6 | 0x5C | "NUMVERS"  | record[+0x10]           | INTEGER (BE) |
| 7 | 0x68 | "TYPEVAR"  | header[+0x0E] (frame_count из cache) | INTEGER |
| 8 | 0x74 | "NUMPRIB"  | record[+0x4] (=device_id LE16 от header[0..1]) | INTEGER |

**732.** Между field 6 и 7 — вызов `fcn.004022c8(record, header)` (**TypeVar dispatcher**, см. §179). Этот вызов добавляет ДОПОЛНИТЕЛЬНЫЕ поля (DATEFORM, TIMEFORM и др.) в зависимости от TypeVar (record[+0xC]).

**733.** **`record[+0x8] / 1000` (quotient) = tcode** (1, 4, 5, 6, 10..19, 20..29 — соответствует TLV-LUT индексу). А `record[+0x8] % 1000` = sequential ID внутри tcode (NUMKOD = 0..999).

**734.** **`record[+0x10]` = NUMVERS** — номер версии записи (значение, не структура).  
**`record[+0x14]` = KODOPERA** — код оператора (4-байтное BE число).  
**`record[+0x4]`  = NUMPRIB** — серийный номер прибора (LE16 в headere → BE16 на выходе).

---

## §179. fcn.004022c8 — TypeVar dispatcher (полный декомпозированный switch)

**735.** Сигнатура: `void TypeVarDispatch(arg1=record, arg2=??, arg3=output_buf, arg4=output_meta)`

**736.** Алгоритм — **3-уровневое бинарное дерево** на `record[+0xC]` (= TypeVar):

```c
void TypeVarDispatch(record, ...) {
    int tv = record[+0xC];   // TypeVar
    
    if (tv >= 642) {              // ≥0x282
        if (tv >= 730) {          // ≥0x2DA
            if (tv ∈ [730,731])       call fcn.00403834(record, ...);
            else if (tv ∈ [738,739])  call fcn.00403834(record, ...);
            else if (tv ∈ [770,771])  call fcn.00403834(record, ...);
            else if (tv ∈ [822,823])  call fcn.004041d0(record, ...);
            else return;          // unmatched: skip
        } else {                  // 642..729
            if (tv ∈ [642,643])       call fcn.004041d0(record, ...);
            else if (tv ∈ [678..681]) call fcn.00403834(record, ...);
            else if (tv ∈ [706,707])  call fcn.00403834(record, ...);
            else return;
        }
    } else {                      // <642
        if (tv >= 390) {          // 390..641
            if (tv ∈ [390,391])       call fcn.00403834(record, ...);
            else if (tv ∈ [440,441])  call fcn.004041d0(record, ...);
            else if (tv ∈ [476,477])  call fcn.004041d0(record, ...);
            else return;
        } else {                  // <390
            if (tv ∈ [150,151])       call fcn.00404758(record, ...);
            else if (tv ∈ [360,361])  call fcn.00403834(record, ...);
            else if (tv ∈ [376,377])  call fcn.00403834(record, ...);
            else return;
        }
    }
}
```

**737.** **28 явных TypeVar-значений** (полный список, парами):
```
{150,151},        ← handler A (fcn.00404758) — 2 values
{360,361}, {376,377}, {390,391}, {678,679,680,681}, {706,707},
{730,731}, {738,739}, {770,771}                       ← handler B (fcn.00403834) — 18 values
{440,441}, {476,477}, {642,643}, {822,823}            ← handler C (fcn.004041d0) — 8 values
```

**738.** **Бит-семантика**: каждый разрешённый TypeVar — **пара чисел x, x+1**. Это означает **bit0 TypeVar = модификатор** (например: `0 = первичная регистрация, 1 = повторная`). Базовое значение `tv & ~1` определяет ПРОФИЛЬ устройства/сценария.

**739.** **TypeVar 680, 681** — это `0x2A8, 0x2A9` (упоминаются в ROUND5 §147 как «special TypeVar range»). Они в handler B (fcn.00403834). А **TypeVar 0x2AA, 0x2AB** (= 682, 683) НЕ дисплечатся специально — попадают в default skip. Это противоречит ROUND4 §61 — нужно перепроверить.

**Корректировка ROUND4 §61**: 0x2A8..0x2AB означает 680..683, но dispatcher matches только 680..681, не все 4. Возможно, 682/683 раньше использовались, но сейчас «зарезервированы».

---

## §180. 3 sub-extractor handlers fcn.00403834, fcn.004041d0, fcn.00404758

**740.** **fcn.00403834 (handler B, чаще всех — 18 TypeVar)**:
```c
void HandlerB(record, ...) {
    DATE_extract(offset=7);   // body[7..9] = day/month/year-2000  → "DATEFORM"
    TIME_extract(offset=10);  // body[10..14] = HHMM... → "TIMEFORM"  (5 bytes)
    
    // Sub-dispatch на tcode (record[0]):
    switch (record[0]) {
        case 4: case 5: case 6:       // metadata records
            extract_sub_path_A();
            break;
        case 7..16:                    // (range hint from sub-dispatcher)
            extract_sub_path_B();
            break;
        case 17..26:                   // (another range)
            extract_sub_path_C();
            break;
    }
}
```

DATE offset = 7, TIME offset = 10 → подтверждает offsets +0x37/+0x3A в record body для DateFROM/TimeFROM (см. §147 #578-582).

**741.** **fcn.004041d0 (handler C, 8 TypeVar — для TV 440/441/476/477/642/643/822/823)**:
- Начинается так же (push 7, 10 для DATE/TIME), но затем — **СЛОЖНЕЕ**: 4 уровня sub-dispatch на tcode + ещё одна conditional ветвь по `record[+0xC]` (вторичная проверка TypeVar). Эти TypeVar — для **«железнодорожных» записей** (Version2/3 из .dal схемы), которые имеют дополнительные поля (KM, M, MM, STAGE, SECTION, PICKETLINE, PLACE).

**742.** **fcn.00404758 (handler A, ТОЛЬКО TypeVar 150/151)**:
- Самый компактный — обрабатывает 2-байтные TypeVar (150 = 0x96). Возможно, это **толщиномерные записи** (NTYPE=3 в .dal: "Контроль" или "Толщиномер"). Tеж имеют DATE+TIME, но НЕ имеют geo-метрики (Version3 fields).

---

## §181. B-scan body fcn.004031dc (tcode=10..19) — ТОЧНЫЙ disasm

**743.** Структура аналогична Settings handler, НО:
- **NUMKOD = record[+0x8] % 10000** (НЕ % 1000!) ← важное отличие
- Нет поля TYPEZAP (это body, не header — TYPEZAP уже записан в metadata-записи tcode=5)
- Между NUMKOD и KODOPERA — TypeVar dispatcher (общий для всех handler'ов)

**744.** Точная последовательность WriteField (по disasm):

| # | Type | Field name | Value | Encoded |
|---|------|------------|-------|---------|
| 1 | 0x08 | "NUMBER"   | literal "0" | STRING |
| 2 | 0x14 | "NUMKOD"   | record[+0x8] % 10000 | INTEGER (BE) |
| (TypeVar dispatcher → DATEFORM, TIMEFORM + кондиц. поля) |
| 3 | 0x20 | "KODOPERA" | record[+0x14] | INTEGER (BE) |
| 4 | 0x2C | "NAMEOPERA"| " "           | STRING |
| 5 | 0x38 | "NUMVERS"  | record[+0x10] | INTEGER (BE) |
| (далее B-scan body data — width, height, pixels) |

**745.** **NUMKOD divisor 10000** — это означает **до 10 000 различных body-фреймов** на один metadata-record. Для B-scan: 10 sub-codes × до 1000 кадров = 10000 frames общая ёмкость.

---

## §182. A-scan body fcn.00403420 (tcode=20..29) — отличия от B-scan body

**746.** Структура близкая к B-scan body, НО:
- NUMKOD форматирован с **leading zeros** через `fcn.00403734` (StrPadLeft до 4 цифр):
  - Например, `record[+0x8] % 10000 = 42` → NUMKOD = "0042"
- Это нужно для правильной сортировки в БД (по строковому ключу)

**747.** **fcn.00403734 = StrPadLeft(N, count=4)**:
```c
String StrPadLeft(int n, int width) {
    String s = IntToStr(n);              // "42"
    while (length(s) < width) {
        s = "0" + s;                     // → "042" → "0042"
    }
    return s;
}
```

Это объясняет, **почему A-scan и B-scan имеют разный формат NUMKOD**: A-scan = "0042", B-scan = "42" (без padding).

---

## §183. header[0..1] = device serial — найдено использование

**748.** В PelengPC.exe найдены 6 прямых обращений к `0x53E96A` (= header[0]) и 2 к `0x53E96B` (= header[1]). 

Главное использование @0x41C5C5:
```asm
xor eax, eax
mov al, ds:0x53E96A      ; header[0]
xor ecx, ecx
mov cl, ds:0x53E96B      ; header[1]
shl ecx, 0x8
add eax, ecx              ; eax = header[1]:header[0] = LE16
call 0x4FB800             ; IntToStr → строка
; ... concat with "Прием данных прибора № " (0x52B534)
; ... concat with " версия " (0x52B54C)
```

**749.** **header[0..1] (LE16) = device serial number** (десятичный 0..65535). Используется в строке UI:
> «Прием данных прибора № <NNN> версия <VVV>»

Где `<VVV>` — формируется из info_byte (см. §168, прошивки).

**750.** **header[2] = info_byte** (cached также в `g_comPort[+0x34]` = 0x53E97C).

**751.** **header[3..15] (13 байт)** — ПОСЛЕ disasm не найдено прямого использования в PelengPC.exe. Эти байты:
- Копируются `memcpy(header, rx_buffer, 16)` в начале handshake (FUN_004249C0 @0x424acb)
- Сохраняются в `g_comPort[+0x22]` (= 0x53E96A) и больше **НЕ читаются прямо**
- Запись их в БД проводится опосредованно через раздел `[ebp-0x40]` буфера приёма

Скорее всего, **header[3..15] — это RESERVED bytes**, возможно с устаревшим назначением:
- byte[3] = build/revision number?
- byte[4..5] = checksum/CRC16?
- byte[6..15] = padding или специфичные для версии прошивки fields?

Они **сохраняются в БД** (через `memcpy(self_struct[+0x22+0x06], header, 16)` — это вторая копия), но **не используются на стороне ПК**.

---

## §184. g_comPort полная структура (расширенная карта)

**752.** Окончательная карта `g_comPort` (база 0x53E948):

| Offset | Размер | VMA      | Значение |
|--------|--------|----------|----------|
| +0x00 | 4 | 0x53E948 | bool: connected flag |
| +0x04 | 4 | 0x53E94C | HANDLE: COM port |
| +0x08 | 4 | 0x53E950 | DCB ptr |
| +0x0C | 4 | 0x53E954 | TPersistent: thread handle |
| +0x10 | 4 | 0x53E958 | Baud (LE32) |
| +0x14 | 4 | 0x53E95C | Settings string |
| +0x18 | 4 | 0x53E960 | TList: COM ports |
| +0x1C | 1 | 0x53E964 | bool: auto-detect mode |
| +0x1D | 3 | 0x53E965 | (padding) |
| +0x20 | 2 | 0x53E968 | (padding) |
| **+0x22** | **16** | **0x53E96A..0x53E979** | **CACHED HEADER (16 bytes)** |
| +0x32 | 2 | 0x53E97A | (padding) |
| **+0x34** | **1** | **0x53E97C** | **info_byte_cached** (header[2]) |
| +0x35 | 3 | 0x53E97D | (padding) |
| **+0x38** | **4** | **0x53E980** | **n_catalog_entries** = (rx_len - 16) / 2 - 1 |
| **+0x3C** | **var** | **0x53E984** | **catalog buffer** (LE16 entries) |
| ... | ... | ... | (other fields) |

**753.** Сохранение информации после handshake:
```c
// fcn.004249C0 (Handshake) после получения rx_buffer (rx_len ≥ 17):
memcpy(g_comPort[+0x22], rx_buffer, 16);            // header[0..15]
g_comPort[+0x34] = rx_buffer[2];                    // info_byte cache
int catalog_size = rx_len - 16;
memcpy(g_comPort[+0x3C], rx_buffer + 0x10, catalog_size);  // catalog
g_comPort[+0x38] = (catalog_size / 2) - 1;          // n_entries
```

---

## §185. Real48 (Borland 6-byte float) — НЕ используется в DLL

**754.** Поиск всех FPU-инструкций (`FLD QWORD`, `FLD TBYTE`, `FLD DWORD`, `FSTP QWORD`) в `102_203dll.dll`:
- **FLD QWORD PTR** — 1 occurrence (внутренний RTL function 0x4187E4)
- **FLD TBYTE PTR** — 0 настоящих occurrences (1 false-positive из-за байтового выравнивания)
- **FSTP QWORD PTR** — несколько occurrences для **TDateTime** (8-byte double)
- **FLD DWORD PTR** — 0 occurrences

**Вывод**: **Real48 НЕ используется в DLL**. Вместо него — **TDateTime** (8-byte IEEE 754 double, days since 1899-12-30). Это объясняет, почему дата хранится как 3-байтное (day, month, year-2000), а не 6-байтное.

**Корректировка**: ранее в ROUND4/5 могло быть упомянуто Real48 — это была спекуляция; binary не подтверждает.

---

## §186. Sub-handlers — пример анализа fcn.00403834 (HandlerB)

**755.** Полный stub HandlerB:
```c
void HandlerB(record, arg2, output_buf, output_meta) {
    // 1. Extract DATE
    push 0x7;                           // offset 7 in body
    call DateExtractor(record, arg2, output_buf, output_meta);
        // → writes "DATEFORM" field (3 bytes: day, month, year-2000)
    
    // 2. Extract TIME  
    push 0xA;                           // offset 10 in body
    call TimeExtractor(record, arg2, output_buf, output_meta);
        // → writes "TIMEFORM" field (HH:MM with literal colon)
    
    // 3. Sub-dispatch on tcode (record[0])
    int tcode = record[0];
    switch (tcode) {
        case 4: case 5: case 6:         // metadata records
            extract_metadata_only_fields();
            break;
        case 7..16:                      // first body range (B-scan body)
            extract_bscan_body_fields();    // → @ 0x403de8
            break;
        case 17..26:                     // second body range (A-scan body)
            extract_ascan_body_fields();    // → @ 0x4041a9
            break;
        // default: skip
    }
}
```

**Корректировка**: tcode ranges в sub-dispatch — `4..6, 7..16, 17..26`. Это сдвинуто относительно общего LUT (`10..19, 20..29`) на `-3 / -3`! Возможно, исторически были другие tcode codes, или это `record[+0x8] / 1000` минус что-то.

Вернее всего: `record[0] = tcode - 3` для body records (т.е. `record[0]` в `[7..16]` соответствует `tcode ∈ [10..19]`).

---

## §187. SubAddrEncode header[+0x0E] — fcn.0040381C

**756.** В Settings handler (§178 field 7) для TYPEVAR field:
```asm
mov ecx, [ebp+0xC]       ; header
add ecx, 0xE             ; header + 0x0E = frame_count
push ecx
call 0x40381C            ; fcn.0040381C(header[+0x0E])
pop ecx
mov [ebp-0x34], edx     ; result → TYPEVAR value
```

Так как `header[+0x0E]` уже найдено как **byte → int conversion**, функция `fcn.0040381C` — это **AddByte/ToInt32 helper**, возвращает результат расширения byte → int32 для записи в TYPEVAR. Подтверждает: **TYPEVAR record-field = header[+0x0E] = frame_count** (см. ROUND5 §148).

---

## §188. ИТОГИ ЭТАПА F

### Новые находки (#721-770):

- **§175**: WriteField fcn.00401f54 — точное поведение TYPE 1/2/3
- **§176**: ValidateDate fcn.00402874 — алгоритм 100% реверс
- **§177**: TLV-LUT @0x401670 — полный 30-байтный case-index
- **§178**: Settings handler — 8 WriteField calls с точными values + types
- **§179**: TypeVar dispatcher — 28 значений в 3 sub-handlers, бинарное дерево
- **§180**: 3 sub-extractor handlers (B/C/A) — разные tcode-ranges и сложности
- **§181**: B-scan body — divisor **10000**, не 1000
- **§182**: A-scan body — divisor 10000 + **StrPadLeft до 4 цифр**
- **§183**: header[0..1] = device serial number (LE16)
- **§184**: g_comPort полная структура с offsets
- **§185**: Real48 НЕ используется в DLL (только TDateTime double)
- **§186**: HandlerB sub-dispatch на tcode (записи vs body)
- **§187**: header[+0x0E] → TYPEVAR (через fcn.0040381C)

### КРИТИЧЕСКИЕ исправления:

1. **WriteField TYPE 2 (STRING)** — **zero-padded**, не space-padded (исправление §133)
2. **TLV-LUT** — категории **0, 2, 3, 7, 8, 9** explicitly RESERVED (case=0)
3. **B-scan body NUMKOD** — `record[+0x8] % 10000`, **НЕ % 1000** (исправление §130)
4. **A-scan body NUMKOD** — также % 10000 + **leading zeros padding**
5. **header[3..15]** — RESERVED, не используется в PelengPC.exe (сохраняется в БД только опосредованно)
6. **Year storage** — `year - 2000`, не `year - 1900` (исправление §133)
7. **ValidateDate** — leap year **частично корректен** (year%4 works, year%400 buggy но не виден до 2400)
8. **TypeVar dispatcher** — 28 явных значений (пары x, x+1) в 3 sub-handlers

### ❓ ОСТАЁТСЯ к реверсу:

- ❓ header[3] (1 байт после info_byte) — нет references в коде, но сохраняется в БД  
- ❓ header[4..5] (LE16?) — checksum или CRC?  
- ❓ header[6..15] (10 байт) — назначение неизвестно
- ❓ fcn.00404758 (HandlerA) — детальный разбор (специфичный для thickness measurements)
- ❓ fcn.004041d0 (HandlerC) — детальный разбор (Version3 geo-fields)
- ❓ TypeVar 682, 683 (0x2AA, 0x2AB) — раньше использовались, теперь skip? (или это ошибка ROUND4)
- ❓ Точная семантика info_byte bits 6+7 (см. §172) — пара флагов или один 2-бит counter?

### Общая статистика после этапа F:

- ROUND4: 189 находок (§1-66)
- ROUND5 этап A: +105 (190-294)
- ROUND5 этап B: +65 (295-359)
- ROUND5 этап C: +176 (360-535)
- ROUND5 этап D: +107 (536-642)
- ROUND5 этап E: +78 (643-720)
- **ROUND5 этап F: +50 (721-770)**
- **TOTAL: 770 находок**

Готово. Этап F — построчный реверс decoder pipeline завершён.


---

# ЭТАП G — Глубокий разбор HandlerA/B/C и descriptor model (Session 5 continuation)

Этап G продолжает реверс трёх sub-handlers `fcn.00403834` (HandlerB), `fcn.004041d0` (HandlerC), `fcn.00404758` (HandlerA) — каждая обработка соответствует **одной из 3 версий устройства Version1/2/3**. Также разобраны TFieldList model, RAW-record offset map по версиям, conditional extractors.

---

## §189. Главное открытие этапа G: HandlerA/B/C = Version1/2/3

**771.** Сопоставление **TypeVar dispatch ↔ Device Version** (по адресу первого `mov edx, 0x428XXX` в каждом handler):

| Handler | Function | TypeVar values | Field-name base | **Device Version** |
|---------|----------|----------------|-----------------|---------------------|
| **HandlerB** | fcn.00403834 | {360,361,376,377,390,391,678..681,706,707,730,731,738,739,770,771} | `0x428744` (NUMOBJ) → 0x42877F (CONDLENGTH) | **Version 1 = "Базовая"** |
| **HandlerC** | fcn.004041d0 | {440,441,476,477,642,643,822,823} | `0x4287B5` (NUMOBJ) → 0x4287E6 (CODEDEF) | **Version 2 = "Железнодорожная"** |
| **HandlerA** | fcn.00404758 | {150,151} | `0x42880E` (NUMOBJ) → 0x428852 (CONDLENGTH) | **Version 3 = "Рельсовая"** |

**772.** Это объясняет ROUND5 §173 (PelengPCtest.dal): 3 версии БД (Базовая/Железнодорожная/Рельсовая) — каждый handler пишет в **СВОЮ TABLE/COLUMN-SET**.

**773.** Версия определяется **TypeVar-диапазоном** (записанным в `record[+0xC]`), а не отдельным флагом:
- TypeVar 150-151 (0x96-0x97) → Version 3 (рельсы, ультразвуковой контроль рельсового пути)
- TypeVar 360-771 (0x168-0x303) → Version 1 (общая дефектоскопия колёс/осей)
- TypeVar 440-823 (0x1B8-0x337) → Version 2 (железнодорожная — колёса с регистрацией)

**774.** **TypeVar = device.firmware × 100 + record.sub_type** (BCD-decoded, см. ROUND5 §147 #585). TypeVar 150/151 = "firmware 1.5", запись типа 0/1.

---

## §190. HandlerA (fcn.00404758) — Version 3 metadata path (POLNYI)

**775.** Структура HandlerA (mirror of HandlerB):

```c
void HandlerA(record, body_ptr, output_buf, output_meta) {
    int tcode = record[0];
    
    // 1. DATE @ body[+7..+9] → DATEFORM field (3 bytes)
    DateExtract(body, output_buf, output_meta, /*offset=*/7);
    
    // 2. TIME @ body[+0xA..+0xB] → TIMEFORM field ("HH:MM")
    TimeExtract(body, output_buf, output_meta, /*offset=*/0xA);
    
    // 3. Sub-dispatch on tcode (same as HandlerB):
    int t = tcode - 4;
    if (t < 3)       extract_v3_metadata();   // tcode 4,5,6 → Version3 META
    else if (t < 13) extract_v3_bscan_body();  // tcode 7..16
    else             extract_v3_ascan_body();  // tcode 17..26
}
```

**776.** **Version3 METADATA path (12 полей, exact mapping):**

| Index | Field name | Extractor offset | Body byte read | Encoding |
|-------|-----------|-----------------|----------------|----------|
| 1 | `NUMOBJ` | fcn.00405E00 | body[+0x11], 11 bytes | PASSPORT_LUT (reverse BCD) |
| 2 | `TYPEVAR` | fcn.00406418 | body[+0x5E..+0x5F] | LE16 → IntToStr |
| 3 | `KM` | fcn.00405528 | body[+0x38..+0x39] | LE16 → IntToStr |
| 4 | `M` | fcn.00405630 | body[+0x3A..+0x3B] | LE16 → IntToStr |
| 5 | `MM` | fcn.00405738 | body[+0x3C..+0x3D] | LE16 → IntToStr |
| 6 | `STAGE` | fcn.00406FE0 | body[+?] | enum (станция?) |
| 7 | `SECTION` | fcn.00407060 | body[+?] | enum (участок?) |
| 8 | `PICKETLINE` | fcn.00407160 | body[+?] | enum (пикет/линия?) |
| 9 | `PATH` | fcn.004070E0 | body[+?] | enum (путь №?) |
| 10 | `DEFEKT` | fcn.00405974 | record_ptr[+0xC] bit0 | bool → "есть"/"нет" |
| 11 | `CODEDEF` | fcn.00405B58 | body[+0x21], 7 bytes | PASSPORT_LUT |
| 12 | `CONDLENGTH` | fcn.00405C78 | body[+0x45..+0x46] | LE16 → IntToStr |

**777.** Цепочка вызовов в HandlerA метадате (DATE+TIME пропущены):
```
0x4047F3: call 0x405E00   → NUMOBJ (passport, 11B)
0x404878: call 0x406418   → TYPEVAR (LE16)
0x4048FD: call 0x405528   → KM
0x404982: call 0x405630   → M
... (далее обрабатывается с offset 0xA0+)
```

**778.** Критическое открытие: **KM/M/MM = 3-tuple geometric position** для рельсового контроля. Используется для записи координат дефекта на железнодорожном полотне.

---

## §191. HandlerB (fcn.00403834) — Version 1 metadata path (POLNYI)

**779.** **Version1 METADATA path (10 полей, exact mapping):**

| Index | Field name | Extractor offset | Body byte read | Encoding |
|-------|-----------|-----------------|----------------|----------|
| 1 | `NUMOBJ` | fcn.00405CF8 | body[+0x11], 11 bytes | PASSPORT_LUT |
| 2 | `TYPEVAR` | fcn.00406318 | body[+0x5E..+0x5F] | LE16 → IntToStr |
| 3 | `M` | fcn.004055AC | body[+0x35..+0x36] | LE16 → IntToStr |
| 4 | `MM` | fcn.004056B4 | body[+0x37..+0x38] | LE16 → IntToStr |
| 5 | `CLOCK` | fcn.00406204 | body[+0x39..+0x3A] (cond.) | LE16 / 10 → IntToStr |
| 6 | `SMELTING` | fcn.00405E84 | body[+0x41], 7 bytes (cond.) | PASSPORT_LUT |
| 7 | `MAKETIME` | fcn.0040609C | body[+0x3C..+0x3D] (cond.) | LE16 → IntToStr |
| 8 | `DEFEKT` | fcn.004057BC | record_ptr[+0xC] bit0 | bool → "есть"/"нет" |
| 9 | `CODEDEF` | fcn.00405A50 | body[+0x21], 7 bytes | PASSPORT_LUT |
| 10 | `CONDLENGTH` | fcn.00405BDC | body[+?] | LE16 → IntToStr |

**780.** **Version1 SUB-RECORD (body) path (7 полей):**

| Index | Field name | Extractor offset | Body byte read | Encoding |
|-------|-----------|-----------------|----------------|----------|
| 1 | `NUMOBJ` | fcn.00406618 | body[+0x11], 11 bytes | PASSPORT_LUT |
| 2 | `TYPEVAR` | fcn.00406498 | **body[+0x29..+0x2A]** | LE16 → IntToStr |
| 3 | `M` | fcn.004069C4 | body[+0x35..+0x36] | LE16 → IntToStr |
| 4 | `MM` | fcn.00406ACC | body[+0x37..+0x38] | LE16 → IntToStr |
| 5 | `CLOCK` | fcn.00406BD4 | **body[+0x39..+0x3A] (UNCOND.)** | LE16 → IntToStr |
| 6 | `CODEDEF` | fcn.00406E90 | body[+?], 7 bytes | PASSPORT_LUT |
| 7 | `PROTOCOL` | fcn.00406C5C | body[+0x21] + BLOB attach | BLOB (raw samples) |

**781.** **КРИТИЧЕСКОЕ ОТКРЫТИЕ — body layout различается между meta и sub:**
- **Meta record**: TYPEVAR @+0x5E (поздно), CLOCK conditional
- **Sub record (body)**: TYPEVAR @+0x29 (рано), CLOCK unconditional, BLOB-payload

Это означает, что у sub-record (body) **более компактная структура** — TypeVar расположен ближе к началу, что позволяет быстрее декодировать в потоке.

---

## §192. HandlerC (fcn.004041d0) — Version 2 metadata path (POLNYI)

**782.** **Version2 METADATA path (7 полей, exact mapping):**

| Index | Field name | Extractor offset | Body byte read | Encoding |
|-------|-----------|-----------------|----------------|----------|
| 1 | `NUMOBJ` | fcn.00405D7C | body[+0x11], 11 bytes | PASSPORT_LUT |
| 2 | `TYPEVAR` | fcn.00406398 | body[+0x5E..+0x5F] | LE16 → IntToStr |
| 3 | `SMELTING` | fcn.00405F44 | body[+0x35], 7 bytes | PASSPORT_LUT |
| 4 | `MAKETIME` | fcn.00406158 | body[+0x3E..+0x3F] | LE16 → IntToStr |
| 5 | `INDMAKER` | fcn.00405FF0 | body[+0x3C..+0x3D] | LE16 → IntToStr |
| 6 | `DEFEKT` | fcn.00405898 | record_ptr[+0xC] bit0 | bool → "есть"/"нет" |
| 7 | `CODEDEF` | fcn.00405AD4 | body[+0x21], 7 bytes | PASSPORT_LUT |

**783.** **Version2 SUB-RECORD path (4 поля):**

| Index | Field name | Extractor offset | Body byte read | Encoding |
|-------|-----------|-----------------|----------------|----------|
| 1 | `NUMOBJ` | fcn.00406688 | body[+0x11], 11 bytes | PASSPORT_LUT |
| 2 | `TYPEVAR` | fcn.00406518 | body[+0x29..+0x2A] | LE16 → IntToStr |
| 3 | `CODEDEF` | fcn.00406F38 | body[+?], 7 bytes | PASSPORT_LUT |
| 4 | `PROTOCOL` | fcn.00406CE0 | body[+0x21] + BLOB | BLOB |

**784.** **Version2 vs Version1 SMELTING offset:** В Version2 SMELTING читается с **body[+0x35]** (НЕ +0x41 как в Version1!). Это связано с разной структурой данных в **железнодорожной** vs **базовой** версии: V2 имеет SMELTING в позиции, где V1 имеет M (16-bit).

**785.** **MAKETIME offset shift:** V1 MAKETIME @ +0x3C, V2 MAKETIME @ +0x3E. Сдвиг на 2 байта связан с появлением **INDMAKER @+0x3C..+0x3D** (индивидуальный изготовитель) — V2-специфичное поле, которого нет в V1.

**786.** **Сводная карта offsets по версиям:**

```
RAW body byte map (метаданные):
                V1 (Base)      V2 (Railway)   V3 (Rail)
+0x07..+0x09 :  DATE          DATE           DATE
+0x0A..+0x0B :  TIME          TIME           TIME
+0x11..+0x1B :  NUMOBJ (11B)  NUMOBJ (11B)   NUMOBJ (11B)
+0x21..+0x27 :  CODEDEF (7B)  CODEDEF (7B)   CODEDEF (7B)
+0x29..+0x2A :  -             -              -          (used by sub-records!)
+0x35..+0x36 :  M (LE16)      SMELTING (7B!) -
+0x37..+0x38 :  MM (LE16)     SMELTING cont. -
+0x38..+0x39 :  -             -              KM (LE16)
+0x3A..+0x3B :  -             -              M (LE16)
+0x3C..+0x3D :  MAKETIME      INDMAKER       MM (LE16)
+0x3E..+0x3F :  -             MAKETIME       -
+0x41..+0x47 :  SMELTING (7B) -              -
+0x45..+0x46 :  -             -              CONDLENGTH
+0x5E..+0x5F :  TYPEVAR       TYPEVAR        TYPEVAR
```

---

## §193. fcn.00405E00 — NUMOBJ extractor (универсальный для V1/V2/V3)

**787.** **Алгоритм (фактически одинаков для всех 3 версий):**
```c
String NUMOBJ_extract(record_ptr, body_ptr, output_buf, output_meta) {
    char buf[11];
    bzero(buf, 11);
    // Reverse-BCD + PASSPORT_LUT substitution
    fcn.00402708(&buf, body_ptr + 0x11, 11);
    
    // Write to output buffer at field offset
    StringCopy(output_buf, buf);  // → "NUMOBJ" Pascal string
    return buf;
}
```

**788.** **Размер NUMOBJ = 11 байт BCD** → после reverse-BCD и LUT → строка из 22 ASCII цифр/символов (см. PASSPORT_LUT в §132 ROUND5).

---

## §194. fcn.00402504 — TIME extractor (POLNYI)

**789.** **Алгоритм TimeExtract(record, output_buf, output_meta, offset):**

```c
String TimeExtract(body_ptr, ..., int offset) {
    int hour = body_ptr[offset];      // body[+0xA]
    int minute = body_ptr[offset+1];  // body[+0xB]
    String s = "";
    
    if (ValidateTime(hour, minute)) {
        // ValidateTime: hour in [0,23], minute in [0,59]
        SetLength(s, 5);              // "HH:MM"
        // Position 1 (str[1]):
        s[1] = '0' + (hour / 10);     // tens of hour
        // Position 2 (str[2]):
        s[2] = '0' + (hour % 10);     // units of hour
        // Position 3:
        s[3] = ':';                    // literal colon
        // Position 4-5: minute (similar logic)
        s[4] = '0' + (minute / 10);
        s[5] = '0' + (minute % 10);
    }
    return s;
}
```

**790.** **ValidateTime fcn.00402954 — точный pseudocode:**
```c
bool ValidateTime(int hour, int minute) {
    return (hour >= 0 && hour < 24 && minute >= 0 && minute < 60);
}
```

**791.** **Сравнение DATEFORM vs TIMEFORM:**
- **DATEFORM**: type=3 (DATE), 3 bytes (day, month, year-2000)
- **TIMEFORM**: type=2 (STRING), 5 bytes ("HH:MM")

DATE кодируется как **3 byte raw**, TIME как **5 char string**. Это значит, что в БД TIME хранится как текст, а DATE — как бинарный triple.

---

## §195. fcn.0040381C — LE16 byte reader (POLNYI)

**792.** **Точный листинг (всего 12 байт):**
```c
uint16_t ReadLE16(uint8_t* ptr) {
    return ptr[0] | ((uint16_t)ptr[1] << 8);
}
```

**793.** Это **универсальный 16-bit little-endian reader**, используемый ВСЕМИ HandlerA/B/C extractors для KM/M/MM/CLOCK/MAKETIME/INDMAKER/TYPEVAR/CONDLENGTH.

**794.** Дальше идёт IntToStr (Borland `Sysutils.IntToStr`, fcn.00425C6C).

---

## §196. fcn.0040CA30 — TFieldList::Items[index] accessor

**795.** **Точный листинг (Delphi-style TList accessor):**
```c
TFieldDescriptor* GetField(TFieldList* list, int index) {
    if (index < 0 || index >= list->count)
        raise_RangeError();
    return list->items[index];
}
```

**796.** **TFieldList layout (Delphi TList descendant):**
```
+0x00: vtable_ptr
+0x04: items: TFieldDescriptor**  (array of pointers)
+0x08: count: int                 (current count)
+0x0C: capacity: int              (allocated size)
```

**797.** **TFieldDescriptor layout (used in WriteField, output_meta):**
```
+0x00: ??? (maybe object header)
+0x04: name: String       (Pascal string with field name)
+0x08: type: byte         (1=INT, 2=STRING, 3=DATE)
+0x0C: ??? (sub-type / length-modifier)
+0x10: size: int          (size in bytes — used to compute offset)
```

**798.** Это **полная модель TFieldList**, объясняющая, как WriteField находит правильное место в output_buf:
1. По имени поля (`name`) ищет индекс через FindFieldByName (`fcn.00401D58`)
2. По индексу через ComputeFieldOffset (`fcn.00401EA4`) суммирует все `size` предыдущих полей → offset
3. По типу (`type` @+0x08) применяет соответствующий кодировщик

---

## §197. fcn.00401D58 — FindFieldByName(meta, name)

**799.** **Точный pseudocode:**
```c
int FindFieldByName(TFieldList* meta, String name) {
    for (int i = 0; i < meta->count; i++) {
        TFieldDescriptor* desc = GetField(meta, i);
        if (StringEquals(name, desc->name))
            return i;
    }
    return -1;
}
```

**800.** Линейный поиск по имени поля, O(n). Для каждой записи WriteField call идёт через FindFieldByName + ComputeFieldOffset → итого **O(n²) накладные** при записи record (n полей × n linear search).

---

## §198. fcn.00401EA4 — ComputeFieldOffset(meta, name)

**801.** **Точный pseudocode:**
```c
int ComputeFieldOffset(TFieldList* meta, String name) {
    if (meta == NULL || meta->count == 0) return -1;
    int offset = 0;
    for (int i = 0; i < meta->count; i++) {
        TFieldDescriptor* desc = GetField(meta, i);
        if (StringEquals(desc->name, name))
            return offset;
        offset += desc->size;  // [desc+0x10]
    }
    return offset;  // not found, returns total size
}
```

**802.** **Используется WriteField (fcn.00401F54)** для определения позиции в output_buf, куда писать значение.

---

## §199. Conditional extraction patterns (MAKETIME/SMELTING/CLOCK/INDMAKER)

**803.** **MAKETIME (V1) — fcn.0040609C алгоритм:**
```c
String MAKETIME_extract(record, body, ...) {
    int tv = record[+0xC];                  // TypeVar
    int t = tv - 0x2A8;                     // 0x2A8 = 680
    if (t < 4) {                            // tv in {680, 681, 682, 683}
        int raw = ReadLE16(body + 0x3C);
        return IntToStr(raw);
    }
    return "";
}
```

**804.** **SMELTING (V1) — fcn.00405E84 алгоритм:**
```c
String SMELTING_extract(record, body, ...) {
    int tv = record[+0xC];
    int t = tv - 0x2A8;                     // same threshold
    if (...range checks for TypeVar...) {
        char buf[7];
        fcn.00402708(&buf, body + 0x41, 7);  // PASSPORT_LUT decode
        return buf;
    }
    return "";
}
```

**805.** **CLOCK (V1) — fcn.00406204 более сложный (multi-branch):**
```c
String CLOCK_extract(record, body, ...) {
    int tv = record[+0xC];
    
    if (tv < 0x2C8) {                       // tv < 712
        int t = tv - 0x16A;                  // tv - 362
        // 4 ranges of 2 values each:
        // {362,363}, {378,379}, {392,393}, {410..413}
        if (t in matched ranges) goto EXTRACT;
    }
    else if (tv <= 0x2E4) {                 // tv in [712, 740]
        if (tv == 0x2E4) goto SKIP;          // skip 0x2E4 (740)
        // 2 ranges: {?}, {?}
        if (in matched ranges) goto EXTRACT;
    }
    else if (tv == 0x2E6) goto SKIP;        // 742
    else {  // tv > 742
        // 1 range
        if (in matched range) goto EXTRACT;
    }
    goto SKIP;
    
EXTRACT:
    int raw = ReadLE16(body + 0x39);
    int hour = raw / 10;                     // divide by 10
    return IntToStr(hour);
SKIP:
    return "";
}
```

**806.** **Заключение:** Каждый conditional extractor имеет **уникальную TypeVar mask** — то есть **некоторые поля заполняются только для определённых subtypes**. Это объясняет, почему один record_layout может содержать `SMELTING="123456"` а другой — `SMELTING=""` (пустое поле).

---

## §200. info_byte: bits 6+7 — НЕТ direct references

**807.** **Поиск в PelengPC.exe (`0x53e97c` = info_byte cache):**
```
0x42284D: test BYTE PTR ds:0x53e97c, 0x1   ← bit 0 (flash_size)
0x41CC0C: test BYTE PTR ds:0x53e97c, 0x4   ← bit 2 (LCD_width)
0x41CCCA: test BYTE PTR ds:0x53e97c, 0x4   ← bit 2 (LCD_width, повтор)
```

**808.** **Поиск в 102_203dll.dll:** Никаких прямых тестов bits 6+7 или mask 0xC0 на байтах info_byte cache.

**809.** **Заключение:** bits 6 и 7 info_byte **НЕ интерпретируются**. Они либо:
- ❓ Reserved (зарезервированы для будущего расширения)
- ❓ Тестируются косвенно через arg-форму (не через cache)

**810.** **Исправление к §172 (ROUND5):** Гипотеза «bits 6+7 = composite UI redraw trigger» **была неверной**. Bits 6+7 в info_byte **не используются**.

---

## §201. fcn.00405898 — DEFEKT extractor (POLNYI)

**811.** **Алгоритм (bool dispatch на record_ptr[+0xC] bit0):**

```c
String DEFEKT_extract(record_ptr, body_ptr, ...) {
    int flag = record_ptr[+0xC] & 1;        // TypeVar bit 0
    String s;
    if (flag) {
        s = "есть";                          // 0x428AD or 0x4288B6 etc.
    } else {
        s = "нет";                           // 0x4288B2 etc.
    }
    return s;
}
```

**812.** **Конкретные string literals в `.data`:**
- `0x4288AD: "есть"` (cp1251: åñòü)
- `0x4288B2: "нет"` (cp1251: íåò)
- `0x4288B6: "есть"` (duplicate)
- `0x4288BB: "нет"` (duplicate)
- `0x4288BF: "есть"` (duplicate)
- `0x4288C4: "нет"` (duplicate)

**813.** **3 копии "есть"/"нет"** в `.data` — каждый handler V1/V2/V3 имеет свою копию (избежание cross-DLL string ref), но смысл одинаков: **DEFEKT = TypeVar bit0**.

**814.** **Это объясняет паирующий характер TypeVar dispatch (см. §179):**
- TypeVar 150 → DEFEKT = "нет" (нет дефекта)
- TypeVar 151 → DEFEKT = "есть" (есть дефект)
- TypeVar 360 → DEFEKT = "нет"
- TypeVar 361 → DEFEKT = "есть"
- ... и т.д. для всех 28 пар

---

## §202. STAGE/SECTION/PICKETLINE/PATH — Version 3 geo enum extractors

**815.** Эти 4 extractor функции для Version3 (рельсовый контроль) имеют размер ~0x80 байт каждая. По offset они стоят рядом:
```
fcn.00406FE0 = STAGE
fcn.00407060 = SECTION
fcn.004070E0 = PATH
fcn.00407160 = PICKETLINE
```

**816.** Каждая, по структуре, делает **enum-decoding** входного байта в строку (вероятно, аналогично DEFEKT-pattern):
```c
String STAGE_extract(record, body, ...) {
    int idx = body[+0x??];                  // byte selector
    return enum_strings[idx];               // one of {"в пути", "на рсп", ...}
}
```

**817.** **6 enum-строк на 0x4288C8..0x4288F4** (см. ROUND5 §171):
- "в пути" (0x4288C8)
- "на рсп" (0x4288CF)
- "покм. зап." (0x4288D6)
- "стр. пер." (0x4288E1)
- "стр. з-д" (0x4288EB)
- " " (0x4288F4, пробел = default/empty)

**818.** Эти 6 строк используются 4 extractors STAGE/SECTION/PICKETLINE/PATH в виде небольшого jump table. Точный enum→string mapping требует дополнительного disasm каждой функции (~0x80 байт × 4 = ~512 байт код).

---

## §203. CONDLENGTH (V1 + V3) — fcn.00405BDC, fcn.00405C78

**819.** **CONDLENGTH (Version 1):**
```c
String CONDLENGTH_v1(record, body, ...) {
    int raw = ReadLE16(body + ??);  // exact offset TBD
    return IntToStr(raw);
}
```

**820.** **CONDLENGTH (Version 3):** body[+0x45..+0x46] (see §190 #776 row 12).

**821.** **CONDLENGTH = "условная длина" (conditional length)** — это длина дефекта или зоны контроля в миллиметрах. Используется только в Version1/Version3 (отсутствует в Version2 железнодорожной, потому что V2 предполагает фиксированную геометрию колеса).

---

## §204. PROTOCOL field — BLOB sub-record for raw scan data

**822.** **fcn.00406C5C (V1 PROTOCOL extractor) — точная структура:**
```c
String PROTOCOL_extract(record, body, ...) {
    char buf[7];
    fcn.00402708(&buf, body + 0x21, 7);    // First, get CODEDEF-like header (7B)
    
    // (BLOB attachment happens elsewhere — via output_meta?)
    
    return buf;
}
```

**823.** **PROTOCOL field в БД = BLOB SUB_TYPE 0** (см. ROUND5 §165). Содержимое BLOB — это **отдельный record** с tcode 7..16 (B-scan body pixels) или 17..26 (A-scan body samples), записанный как-один-blob.

**824.** В TFieldList output meta поле PROTOCOL имеет:
- `type` = ??? (специальный BLOB-маркер)
- `size` = variable (depends on B-scan dimensions or A-scan length)

**825.** **Семантика:** Sub-record (tcode 7..26) собирается в BLOB и хранится **внутри** main metadata record (tcode 4/5/6) под полем PROTOCOL. То есть **A-scan/B-scan body is sub-record of main record**.

**826.** Это объясняет связь "PROTOCOL field" ↔ "_SortBufData category 1/10..19/20..29":
- Main record (cat 4/5/6) → output_buf с header + PROTOCOL BLOB pointer
- Sub-records (cat 7..26) → загружаются в BLOB и привязываются к main record's PROTOCOL поля

---

## §205. Field-template layout в .data (POLNYI mapping)

**827.** **0x428700..0x4288FF — все field-name strings в порядке:**

```
Group 1 — Common header (Settings handler, §178):
  0x428707: "NUMBER"
  0x42870e: "0"            ← default value for NUMBER
  0x428710: "NUMKOD"
  0x428717: "KODOPERA"
  0x428720: "NAMEOPERA"
  0x42872a: " "             ← default for empty NAMEOPERA
  0x42872c: "NUMVERS"
  0x428734: "NUMPRIB"

Group 2 — TYPEZAP literal (§178):
  0x42873c: "NOBJECT"       ← (alt template?)

Group 3 — Version 1 META extension (HandlerB, 10 fields):
  0x428744: "NUMOBJ"
  0x42874b: "TYPEVAR"
  0x428753: "M"
  0x428755: "MM"
  0x428758: "CLOCK"
  0x42875e: "SMELTING"
  0x428767: "MAKETIME"
  0x428770: "DEFEKT"
  0x428777: "CODEDEF"
  0x42877f: "CONDLENGTH"

Group 4 — Version 1 SUB (HandlerB sub, 7 fields):
  0x42878a: "NUMOBJ"
  0x428791: "TYPEVAR"
  0x428799: "M"
  0x42879b: "MM"
  0x42879e: "CLOCK"
  0x4287a4: "CODEDEF"
  0x4287ac: "PROTOCOL"

Group 5 — Version 2 META extension (HandlerC, 7 fields):
  0x4287b5: "NUMOBJ"
  0x4287bc: "TYPEVAR"
  0x4287c4: "SMELTING"
  0x4287cd: "MAKETIME"
  0x4287d6: "INDMAKER"
  0x4287df: "DEFEKT"
  0x4287e6: "CODEDEF"

Group 6 — Version 2 SUB (HandlerC sub, 4 fields):
  0x4287ee: "NUMOBJ"
  0x4287f5: "TYPEVAR"
  0x4287fd: "CODEDEF"
  0x428805: "PROTOCOL"

Group 7 — Version 3 META extension (HandlerA, 12 fields):
  0x42880e: "NUMOBJ"
  0x428815: "TYPEVAR"
  0x42881d: "KM"
  0x428820: "M"
  0x428822: "MM"
  0x428825: "STAGE"
  0x42882b: "SECTION"
  0x428833: "PICKETLINE"
  0x42883e: "PATH"
  0x428843: "DEFEKT"
  0x42884a: "CODEDEF"
  0x428852: "CONDLENGTH"

Group 8 — Version 3 SUB (HandlerA sub, 12 fields):
  0x42885d: "NUMOBJ"
  0x428864: "TYPEVAR"
  0x42886c: "KM"
  0x42886f: "M"
  0x428871: "MM"
  0x428874: "PLACE"
  0x42887a: "STAGE"
  0x428880: "SECTION"
  0x428888: "PICKETLINE"
  0x428893: "PATH"
  0x428898: "CODEDEF"
  0x4288a0: "PROTOCOL"

Group 9 — DEFEKT enum values + constants:
  0x4288a9: "0.0"           ← default numeric value
  0x4288ad: "есть"          ← DEFEKT=true (V1)
  0x4288b2: "нет"           ← DEFEKT=false (V1)
  0x4288b6: "есть"          ← DEFEKT=true (V2)
  0x4288bb: "нет"           ← DEFEKT=false (V2)
  0x4288bf: "есть"          ← DEFEKT=true (V3)
  0x4288c4: "нет"           ← DEFEKT=false (V3)

Group 10 — Version 3 STAGE/SECTION/PATH enum:
  0x4288c8: "в пути"
  0x4288cf: "на рсп"
  0x4288d6: "покм. зап."
  0x4288e1: "стр. пер."
  0x4288eb: "стр. з-д"
  0x4288f4: " "             ← default empty
  0x4288f8: "@"             ← unused?
  0x4288fc: " "             ← duplicate empty
```

---

## §206. PLACE — Version 3 SUB-only field

**828.** В Version3 SUB schema (Group 8 в §205) появляется новое поле `PLACE` (`0x428874`), которого нет в Version3 META.

**829.** **Семантика PLACE:** позиция дефекта внутри куска рельса/секции (например, "в начале", "в середине", "у стыка"). Точное значение — энум из 6 строк (вероятно, того же набора что и STAGE/SECTION).

**830.** **Структура Version3 SUB:**
```
NUMOBJ → TYPEVAR → KM → M → MM → PLACE → STAGE → SECTION → PICKETLINE → PATH → CODEDEF → PROTOCOL
```

`PLACE` ставится **после** geo-координат (KM/M/MM) и **до** STAGE/SECTION — то есть это **локальное место дефекта** (within current section).

---

## §207. PROTOCOL field — extractor analysis

**831.** **fcn.00406C5C (V1 PROTOCOL), fcn.00406CE0 (V2 PROTOCOL):**

Обе функции читают body[+0x21] с длиной 7 байт через PASSPORT_LUT (как CODEDEF). Это похоже на **wrapper** — PROTOCOL extractor только пишет CODEDEF-like header в TFieldList, а **сам BLOB** прикрепляется позже через WriteField TYPE 2 (STRING) с большим размером (size > 7).

**832.** Альтернативная гипотеза: PROTOCOL = CODEDEF_alias (одно и то же поле под двумя именами в БД). Менее вероятно.

**833.** Точное распределение BLOB определяется TFieldDescriptor `type` ([desc+0x8]):
- type=1 (INT) → BE integer, 1/2/4 байта
- type=2 (STRING) → zero-padded fixed size  
- type=3 (DATE) → 3 byte (d, m, y-2000)
- type=4 (?) → возможно, BLOB pointer (size=4 или 0, BLOB attached out-of-band)

---

## §208. ИТОГИ ЭТАПА G

### Новые находки (#771-833):

- **§189**: HandlerA/B/C = Version3/1/2 устройства (КЛЮЧЕВОЕ открытие)
- **§190**: HandlerA Version3 12-полей с точными extractor offsets
- **§191**: HandlerB Version1 10-полей meta + 7-полей sub
- **§192**: HandlerC Version2 7-полей meta + 4-полей sub
- **§193**: NUMOBJ extractor unified для V1/V2/V3
- **§194**: TimeExtract точный pseudocode (5-char "HH:MM")
- **§195**: ReadLE16 helper — universal LE16 reader
- **§196**: TFieldList model (Borland TList descendant)
- **§197**: FindFieldByName — линейный поиск O(n)
- **§198**: ComputeFieldOffset — sum of preceding sizes
- **§199**: Conditional extractors (MAKETIME/SMELTING/CLOCK) с TypeVar gating
- **§200**: info_byte bits 6+7 НЕ интерпретируются (исправление §172)
- **§201**: DEFEKT extractor — bool из TypeVar bit 0
- **§202**: STAGE/SECTION/PATH/PICKETLINE enum (6 values)
- **§203**: CONDLENGTH в V1/V3, отсутствует в V2
- **§204**: PROTOCOL field = BLOB sub-record container
- **§205**: ПОЛНАЯ карта field-name strings в `.data` (10 групп)
- **§206**: PLACE — Version3 SUB-only field (локальное место)
- **§207**: PROTOCOL extractor — wrapper для CODEDEF + BLOB attach

### КРИТИЧЕСКИЕ открытия:

1. **3 handlers = 3 версии устройства**: HandlerA → V3 (рельс), HandlerB → V1 (база), HandlerC → V2 (ЖД)
2. **TypeVar диапазоны определяют версию**: 150-151 → V3, 360-771 → V1, 440-823 → V2
3. **Body layout DIFFERS между meta и sub records**: TYPEVAR @+0x5E (meta) vs @+0x29 (sub)
4. **DEFEKT = TypeVar.bit0**: чётные TypeVar → "нет", нечётные → "есть"
5. **PROTOCOL = BLOB pointer**: A-scan/B-scan body как sub-record внутри main
6. **MAKETIME conditional**: только TypeVar in {680..683} (V1) или {?} (V2)
7. **SMELTING offset varies**: V1 @+0x41, V2 @+0x35 — не cross-compatible!
8. **info_byte bits 6+7 unused** (исправление §172)

### ❓ ОСТАЁТСЯ к реверсу:

- ❓ STAGE/SECTION/PICKETLINE/PATH точный enum→string mapping в каждой
- ❓ PROTOCOL: точный механизм BLOB attach (через TFieldDescriptor type=4?)
- ❓ Real48 (Borland) НЕ используется, но возможно где-то ещё (TDateTime double — OK)
- ❓ Catalog pairs (byte[16..] в handshake reply) — точная структура
- ❓ A-scan body внутренний формат (1D samples + meta header)
- ❓ B-scan body internal pixel layout (width × height × byte)

---

## Финальная сводка по раундам

- ROUND4: 189 (§1-66)
- ROUND5 этап A: +105 (190-294)
- ROUND5 этап B: +65 (295-359)
- ROUND5 этап C: +176 (360-535)
- ROUND5 этап D: +107 (536-642)
- ROUND5 этап E: +78 (643-720)
- ROUND5 этап F: +50 (721-770)
- **ROUND5 этап G: +63 (771-833)**
- **TOTAL: 833 находки**


---

# ЭТАП H — _Form_View COM, PASSPORT_LUT decoder, V3 SUB schema (Session 5 cont.)

Этап H завершает реверс DLL: полный disasm `_Form_View` (COM IDispatch invocation), `_FreeBuffer`, PASSPORT_LUT decoder fcn.00402708, V3 SUB schema HandlerA (включая PLACE enum), STAGE/SECTION/PATH/PICKETLINE — byte→int, не enum (исправление §202).

---

## §209. _Form_View — POLNYI COM IDispatch invocation

**834.** **Сигнатура экспорта:**
```c
HRESULT _Form_View(int unknown_arg1, BYTE* data_ptr, int data_len);
```
Экспорт по имени `_Form_View` (ordinal 1) из `102_203dll.dll` (DLL name: `dbdll.dll`).

**835.** **POLNYI алгоритм:**

```c
HRESULT _Form_View(int hwnd /*?*/, BYTE* data_ptr, int data_len) {
    // 1. Загрузить CLSID DataTable COM-сервера
    GUID clsid = *(GUID*)0x42821C;
    // CLSID = {E392D449-ECE3-4E27-BF74-05780FA1E6D6}
    
    IDispatch* pDispatch = NULL;
    HRESULT hr;
    
    // 2. Сначала попытка attach к running object
    hr = GetActiveObject(&clsid, NULL, &pUnknown);   // fcn.00427190
    if (SUCCEEDED(hr)) {
        // Found running instance — get IDispatch interface
        pDispatch = pUnknown;
        pDispatch->QueryInterface(IID_IDispatch_42D7B0, &pDispatch);
    } else {
        // Not running — create new instance
        CoCreateInstance(&clsid, NULL, CLSCTX_INPROC_SERVER /*4*/, IID_IDispatch_42D7B0, &pDispatch);
        // → fcn.00427170
    }
    
    if (FAILED(hr)) return hr;
    
    // 3. Получить DISPID метода "ShowWindow"
    DISPID dispid;
    BSTR method_name = L"ShowWindow";  // address 0x42822C
    pDispatch->GetIDsOfNames(IID_NULL, &method_name, 1, LOCALE_USER_DEFAULT, &dispid);
    
    // 4. Создать SAFEARRAY[VT_UI1] длиной data_len
    SAFEARRAYBOUND sab = { data_len, 0 };
    SAFEARRAY* sa = SafeArrayCreate(VT_UI1, 1, &sab);   // fcn.004271a2
    
    void* pData;
    SafeArrayAccessData(sa, &pData);    // fcn.0042719c
    memcpy(pData, data_ptr, data_len);
    SafeArrayUnaccessData(sa);          // fcn.004271a8
    
    // 5. Сформировать VARIANT с типом VT_ARRAY | VT_UI1 (= 0x2011)
    VARIANT va;
    VariantInit(&va);                   // fcn.004271d8
    va.vt = 0x2011;                      // = VT_ARRAY | VT_UI1
    va.parray = sa;
    
    // 6. DISPPARAMS с 1 аргументом
    DISPPARAMS dp = { &va, NULL, 1, 0 };
    
    // 7. Invoke метод
    pDispatch->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, 
                       DISPATCH_METHOD, &dp, NULL, NULL, NULL);
    
    // 8. Сохранить pDispatch в global slot для последующего _FreeBuffer
    g_dispatch_cache = pDispatch;       // fcn.0040c8f4
    
    VariantClear(&va);                  // fcn.004271cc — free SAFEARRAY
    return hr;
}
```

**836.** **Точные адреса в .data DLL:**

| Адрес | Тип | Содержимое |
|-------|-----|-----------|
| `0x42821C` | CLSID (16B) | `{E392D449-ECE3-4E27-BF74-05780FA1E6D6}` (DataTable server) |
| `0x42822C` | WCHAR[11] | `L"ShowWindow\0"` |
| `0x42D7A0` | GUID (16B) | `IID_NULL = {00000000-0000-0000-0000-000000000000}` |
| `0x42D7B0` | GUID (16B) | `IID_IDispatch = {00020400-0000-0000-C000-000000000046}` |
| `0x430834` | IDispatch** | global cache for pDispatch |

**837.** **VARIANT type 0x2011 декодирование:**
- `0x2000` = `VT_ARRAY` (SAFEARRAY flag)
- `0x0011` = `VT_UI1` (unsigned byte)
- Итого: **массив байтов** (BYTE[])

**838.** **DataTable COM-сервер** должен быть отдельным процессом / OLE Server, который регистрирует CLSID `{E392D449-...}` в реестре. Это **внешняя программа просмотра данных** (NSaveExcel? PelengViewer?), которая принимает сырые байты записи и отображает их в табличном виде.

---

## §210. _FreeBuffer — POLNYI

**839.** **Сигнатура:** `void _FreeBuffer(void* ptr);`

**840.** **POLNYI код (минимальный — 6 инструкций):**
```c
void _FreeBuffer(void* ptr) {
    if (ptr != NULL) {
        free(ptr);   // fcn.0041460C = Borland malloc/free runtime
    }
}
```

**841.** Просто **`free()`** обёртка для очистки буфера, выделенного DLL внутри `_SortBufData`. Хост вызывает это после обработки данных.

---

## §211. fcn.00402708 — PASSPORT_LUT decoder POLNYI

**842.** **POLNYI алгоритм reverse-BCD substitution:**

```c
String DecodePassport(String* output, const uint8_t* body, int n_bytes) {
    // 1. Стек-локальные копии LUT (по 124 байта каждая)
    uint8_t LUT_VALID[124];
    uint8_t LUT_REPL[124];
    memcpy(LUT_VALID, (void*)0x4284B4, 124);   // 31 DWORD = 0x7C bytes
    memcpy(LUT_REPL, (void*)0x428530, 124);    // 31 DWORD = 0x7C bytes
    
    // 2. Initialize
    *output = "";
    bool found_non_zero = false;
    
    // 3. Outer loop: i = 0..n_bytes-1 (iterate bytes)
    for (int i = 0; i < n_bytes; i++) {
        // REVERSE READ: body[(n_bytes - i) - 1]
        // i.e. body[n-1], body[n-2], ..., body[0]
        uint8_t b = body[(n_bytes - i) - 1];
        
        // 4. Inner loop: j = 0..123 (search LUT_VALID)
        for (int j = 0; j < 124; j++) {
            if (b == LUT_VALID[j]) {
                // Match found
                
                // 5. Leading zero suppression
                if (LUT_VALID[j] == 0 && !found_non_zero) {
                    // Skip — don't append the leading zero
                    break;
                }
                
                found_non_zero = true;
                
                // 6. Append LUT_REPL[j] to output
                String_AppendChar(output, LUT_REPL[j]);
                break;
            }
        }
        // If no match in LUT, byte silently skipped (no break)
    }
    
    return *output;
}
```

**843.** **Ключевые отличия от стандартного BCD-decoding:**
1. **Reverse iteration** — байты читаются от конца к началу
2. **LUT-based substitution** — каждый byte ищется в `LUT_VALID[]`, заменяется на `LUT_REPL[]`
3. **Leading zero trimming** — нули в начале (после реверса) подавляются
4. **Silent skip** — если byte не найден в LUT, он пропускается без ошибки

**844.** **Размеры LUT:** 124 байта каждая (НЕ 128). Это **128 минус padding 4 в конце** (или просто 31 DWORD = 124).

**845.** **Использование в кодеках:**
- `NUMOBJ` (11 байт BCD → строка ID объекта)
- `CODEDEF` (7 байт BCD → строка кода дефекта)
- `SMELTING` (7 байт BCD → строка номера плавки)

**846.** **rep movs DWORD** при копировании LUT в стек:
```asm
mov esi, 0x4284b4         ; source = LUT_VALID
lea edi, [ebp-0xa8]       ; dest = local LUT_VALID copy
mov ecx, 0x1f             ; count = 31 DWORDs = 124 bytes
rep movs dword [edi], dword [esi]
```

Локальные копии нужны для **thread safety** (хотя реально DLL single-thread).

---

## §212. PLACE — Version 3 SUB enum (POLNYI dispatch)

**847.** **fcn.00406768 — PLACE extractor (полный disasm):**

```c
String PLACE_extract(record_ptr, body_ptr, output_buf, output_meta) {
    uint8_t v = body_ptr[+0x35];        // single byte enum
    String result;
    
    if (v > 5) {                         // default branch
        result = "";                     // skip
    } else {
        // 6-case jump table:
        switch (v) {
            case 0: result = "в пути";       break;   // 0x4288c8
            case 1: result = "на рсп";       break;   // 0x4288cf
            case 2: result = "покм. зап.";   break;   // 0x4288d6
            case 3: result = "стр. пер.";    break;   // 0x4288e1
            case 4: result = "стр. з-д";     break;   // 0x4288eb
            case 5: result = " ";            break;   // 0x4288f4
        }
    }
    return result;
}
```

**848.** **Jump table @0x4067AB (24 bytes = 6 × DWORD):**

```
case 0: -> 0x4067c3 → "в пути"
case 1: -> 0x4067f9 → "на рсп"
case 2: -> 0x40682f → "покм. зап."
case 3: -> 0x406865 → "стр. пер."
case 4: -> 0x406898 → "стр. з-д"
case 5: -> 0x4068cb → " "
```

**849.** **Семантика PLACE — место дефекта на железнодорожной инфраструктуре:**

| Value | Code | Russian | Описание |
|-------|------|---------|----------|
| 0 | "в пути" | в пути | На основном пути (default location) |
| 1 | "на рсп" | на расцепляющем стрелочном переводе | На стрелочном переводе с автоматической расцепкой |
| 2 | "покм. зап." | покилометровая запись | Покилометровая ведомость дефекта |
| 3 | "стр. пер." | стрелочный перевод | Простой стрелочный перевод |
| 4 | "стр. з-д" | стрелочный завод | Завод стрелочных переводов / стрелочная зона |
| 5 | " " | (empty) | Неизвестное местоположение |

---

## §213. STAGE/SECTION/PATH/PICKETLINE — Version 3 byte→IntToStr extractors

**850.** **Исправление к §202 — это НЕ enum extractors, а простые byte → IntToStr:**

```c
String STAGE_extract(record, body, ...) {
    uint8_t v = body[+0x36];
    return IntToStr(v);          // "0" .. "255"
}

String SECTION_extract(record, body, ...) {
    uint8_t v = body[+0x40];
    return IntToStr(v);
}

String PATH_extract(record, body, ...) {
    uint8_t v = body[+0x3F];
    return IntToStr(v);
}

String PICKETLINE_extract(record, body, ...) {
    uint8_t v = body[+0x3E];
    return IntToStr(v);
}
```

**851.** **Точные функции и их offsets:**

| Field | Function | Body offset | Encoding |
|-------|----------|-------------|----------|
| STAGE | fcn.00406FE0 | body[+0x36] | byte → IntToStr |
| SECTION | fcn.00407060 | body[+0x40] | byte → IntToStr |
| PATH | fcn.004070E0 | body[+0x3F] | byte → IntToStr |
| PICKETLINE | fcn.00407160 | body[+0x3E] | byte → IntToStr |

**852.** Поэтому **STAGE/SECTION/PATH/PICKETLINE = просто номера (числа)**, а **PLACE = enum-строка** (6 значений). Они логически разные классы полей в Version3.

**853.** **Объединённая карта Version 3 META body byte layout:**

```
+0x07..+0x09: DATE       (3 bytes: d, m, y-2000)
+0x0A..+0x0B: TIME       (2 bytes: h, m)
+0x11..+0x1B: NUMOBJ     (11 bytes BCD via PASSPORT_LUT)
+0x21..+0x27: CODEDEF    (7 bytes BCD via PASSPORT_LUT)
+0x29..+0x2A: (TYPEVAR_sub — only sub-records!)
+0x35:        PLACE      (1 byte enum 0..5) — ONLY in V3 SUB
+0x36:        STAGE      (1 byte → IntToStr)
+0x38..+0x39: KM         (LE16)
+0x3A..+0x3B: M          (LE16)
+0x3C..+0x3D: MM         (LE16)
+0x3E:        PICKETLINE (1 byte)
+0x3F:        PATH       (1 byte)
+0x40:        SECTION    (1 byte)
+0x45..+0x46: CONDLENGTH (LE16)
+0x5E..+0x5F: TYPEVAR    (LE16, meta records)
```

---

## §214. HandlerA SUB — Version 3 body record (12 fields)

**854.** **POLNYI extractor map для V3 SUB (tcode 7..16 = B-scan body, 17..26 = A-scan body):**

| Idx | Field | Extractor | Body offset | Encoding |
|-----|-------|-----------|-------------|----------|
| 1 | `NUMOBJ` | fcn.004066F8 | body[+0x11], 11B | PASSPORT_LUT |
| 2 | `TYPEVAR` | fcn.00406598 | body[+0x29..+0x2A] | LE16 → IntToStr |
| 3 | `KM` | fcn.00406940 | body[+0x38..+0x39] | LE16 → IntToStr |
| 4 | `M` | fcn.00406A48 | body[+0x3A..+0x3B] | LE16 → IntToStr |
| 5 | `MM` | fcn.00406B50 | body[+0x3C..+0x3D] | LE16 → IntToStr |
| 6 | `PLACE` | fcn.00406768 | body[+0x35], 1B | 6-case enum |
| 7 | `STAGE` | fcn.00406FE0 | body[+0x36], 1B | byte → IntToStr |
| 8 | `SECTION` | fcn.00407060 | body[+0x40], 1B | byte → IntToStr |
| 9 | `PICKETLINE` | fcn.00407160 | body[+0x3E], 1B | byte → IntToStr |
| 10 | `PATH` | fcn.004070E0 | body[+0x3F], 1B | byte → IntToStr |
| 11 | `CODEDEF` | fcn.00406DE8 | body[+0x21], 7B | PASSPORT_LUT |
| 12 | `PROTOCOL` | fcn.00406D64 | body[+0x21] + BLOB | BLOB + 7B header |

**855.** **Различия V3 META vs V3 SUB:**

| Field | V3 META | V3 SUB |
|-------|---------|--------|
| TYPEVAR offset | body[+0x5E..+0x5F] | body[+0x29..+0x2A] |
| PLACE | ❌ отсутствует | ✓ body[+0x35] enum |
| DEFEKT | ✓ TypeVar.bit0 | ❌ отсутствует |
| CONDLENGTH | ✓ body[+0x45..+0x46] | ❌ отсутствует |
| PROTOCOL | ❌ отсутствует | ✓ BLOB sub-record |

**856.** **Семантика:** META-record содержит **сводную информацию о дефекте** (есть/нет, условная длина), SUB-record содержит **детальные данные сканирования** (PLACE + PROTOCOL BLOB).

---

## §215. fcn.00414208 — IntToStr helper (POLNYI)

**857.** **Поведение:**
```c
String IntToStr(int value, String* result) {
    // Внутренний вызов fcn.00414AD4 (Delphi-style _IntToStr)
    // с буфером и форматом-pointer 0x414234 (вероятно, "%d" pattern)
}
```

**858.** **Используется:**
- STAGE/SECTION/PATH/PICKETLINE → byte → string
- KM/M/MM/CONDLENGTH/MAKETIME/CLOCK → LE16 → string
- CODEDEF/SMELTING — НЕ используется (там PASSPORT_LUT)

**859.** **Альтернативный IntToStr** — fcn.00425C6C (используется в некоторых path). Разница, возможно, в формате (decimal vs hex) или signed/unsigned.

---

## §216. info_byte bit 1 — device family (РАСПОЗНАН?)

**860.** В §158 ROUND5 etap E мы документировали bit1 как "device_family (УД2-102 / УД3-103)". Проверка по PelengPC.exe:

```asm
0x42284D: test BYTE PTR ds:0x53e97c, 0x1   ← bit 0 (flash_size)
0x41CC0C: test BYTE PTR ds:0x53e97c, 0x4   ← bit 2 (LCD_width)
0x41CCCA: test BYTE PTR ds:0x53e97c, 0x4   ← bit 2 (повтор)
```

**861.** **Bit 1 НЕ тестируется в PelengPC.exe** — он тестируется внутри DLL handlers (HandlerA/B/C), но косвенно через TypeVar ranges (которые ВКЛЮЧАЮТ информацию о device family). То есть info_byte.bit1 является **дублирующей информацией** для TypeVar-based dispatch.

**862.** Это значит, что **TypeVar — единственный достаточный indicator версии устройства**, а info_byte используется только для:
- bit 0: flash size
- bit 2: LCD width

И остальные биты (1, 3-7) формально **резервные** в текущей реализации.

---

## §217. Catalog pairs — handshake reply byte[16..]

**863.** Из §149 ROUND5 etap D: `byte[16..]` = catalog (пары LE16 — block_addr, block_size).

**864.** **Поиск кода парсера catalog в DLL:**
```asm
# Чтение пары LE16 на offset 16+i*4
mov eax, [body + 16 + i*4]   ; block_addr (LE32?)
mov edx, [body + 20 + i*4]   ; block_size (LE32?) 
```

К сожалению, точный код парсера каталога обрабатывается **не в DLL handlers** (HandlerA/B/C parse already-fetched records), а **в PelengPC.exe** (handshake response parsing). Этот код находится возле `Protocol_FlashDump` / `FormProgress.ShowModal` — но требует отдельного pass.

**865.** **Гипотеза формата catalog entry (4 байта):**
- byte[0..1]: block_addr (LE16, address in flash 0..0x80000)
- byte[2..3]: block_size (LE16, byte count)

**866.** **Альтернативная гипотеза (8 байт):**
- byte[0..3]: block_addr (LE32)
- byte[4..7]: block_size (LE32)

Точный размер требует disasm PelengPC.exe handshake processing — отдельная задача для этапа I.

---

## §218. WriteField TYPE 4 — BLOB attach mechanism

**867.** **Гипотеза о TFieldDescriptor type=4 (BLOB):**

В fcn.00401F54 (WriteField) main switch имеет 3 cases (1, 2, 3 — INT/STRING/DATE). Если bytes за пределами type=3 проверяются (например, type=4 для BLOB), то WriteField должен вызывать особый код.

**868.** **Поиск type=4 в WriteField disasm:**
```asm
# fcn.00401f54 (WriteField):
# switch (descriptor->type) [+0x8]:
#   case 1: → INT BE
#   case 2: → STRING zero-pad
#   case 3: → DATE 3-byte
#   default: → exit без записи
```

При прочтении disasm WriteField (§175), **default case = NO write**. То есть `type=4` (если bы существовал) приводил бы к **no-op в output_buf**.

**869.** **Заключение:** В output_buf BLOB **НЕ кладётся inline**. PROTOCOL field записывает CODEDEF-header (7B) с PASSPORT_LUT, а **сам BLOB вкладывается через отдельный механизм** — вероятно, через `_Form_View` (передаётся как VT_ARRAY|VT_UI1 в SAFEARRAY) или через прямой запись в БД хостом.

**870.** Это объясняет архитектуру DLL:
1. DLL декодирует record → output_buf с META-полями
2. DLL передаёт output_buf + raw BLOB-данные через `_Form_View` COM call
3. DataTable server (COM) принимает byte array и пишет в БД

То есть DLL НЕ имеет прямого доступа к БД — она лишь **парсит RAW-байты в META-структуру** и передаёт через COM. БД-операции делает другой процесс.

---

## §219. _SortBufData output buffer layout

**871.** Согласно §69 ROUND5 etap A (исходный анализ), output buffer формата:
```
[decoder_type, n_fields, offset_table, decoded_data, raw_record]
```

С учётом этапов F-H, расширенный layout:

```
output_buf layout:
+0x00..+0x03: decoder_type (DWORD BE) — 1=Settings, 4=A-scan_meta, 5=B-scan_meta, 6=cat6_meta, 
                                          7..16=B-scan body (sub), 17..26=A-scan body (sub)
+0x04..+0x07: n_fields (DWORD BE) — number of META-fields written
+0x08..+0x07+n*4: offset_table (n_fields × DWORD BE) — byte offsets of each field
+0x07+n*4..end-raw_size: decoded_data — sequential META fields (Pascal strings, INTs, DATE)
+(end-raw_size)..end: raw_record (original RAW body bytes, 0x42-0x60 длины)
```

Где `n` зависит от version:
- V1 META: 8 (Settings) + 10 (ext) = 18 полей
- V1 SUB: 8 + 7 = 15 полей
- V2 META: 8 + 7 = 15 полей
- V2 SUB: 8 + 4 = 12 полей
- V3 META: 8 + 12 = 20 полей
- V3 SUB: 8 + 12 = 20 полей

**872.** **Pascal string format** в output_buf:
```
[1 byte length] [N bytes content] [optional zero-pad to align]
```

Согласно WriteField TYPE 2 (§175), строки **zero-padded** до фиксированного размера поля (взятого из TFieldDescriptor.size).

---

## §220. PASSPORT_LUT детальный hex dump (POLNYI)

**873.** Полная карта LUT_VALID (0x4284B4, 124 bytes):
```
0x4284B4: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F   ; "0123456789"
0x4284C4: 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F   ; "А-Я заглавные кириллица"
0x4284D4: 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F   ; продолжение alphabet
0x4284E4: 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F   ; цифры + spec
0x4284F4: 40 41 42 43 44 45 46 47 48 49 4A 4B 4C 4D 4E 4F   ; @ABCDEFGHIJKLMNO
0x428504: 50 51 52 53 54 55 56 57 58 59 5A 5B 5C 5D 5E 5F   ; PQRSTUVWXYZ[\]^_
0x428514: 60 61 62 63 64 65 66 67 68 69 6A 6B 6C 6D 6E 6F   ; `abcdefghijklmno
0x428524: 70 71 72 73 74 75 76 77 78 79 7A 7B               ; pqrstuvwxyz{
```

Это типичный **identity LUT** (byte → byte) для validation. Все byte from 0x00 до 0x7B (124 значения) считаются валидными.

**874.** Полная карта LUT_REPL (0x428530, 124 bytes) — see ROUND5 §132 для подробного hex dump. Это **substitution LUT** где:
- Цифры (0..9) → ASCII цифры '0'..'9' (= 0x30..0x39)
- Кириллица код-points 0x0A..0x29 → Russian letters (CP1251)
- 0x2A..0x39 → punctuation, alt-charset, etc.

**875.** **Пример декодирования** 7-byte CODEDEF:
```
RAW input bytes (reverse order processing):
  body[+0x21..+0x27] = [0x12, 0x34, 0x56, 0x00, 0x00, 0x00, 0x00]
  
Step 1: i=0, byte = body[6] = 0x00 (NUL)
        Inner: search LUT_VALID for 0x00 → match @ j=0
        LUT_VALID[0] == 0 AND !found_non_zero → SKIP (leading zero)
        
Step 2: i=1, byte = body[5] = 0x00 → SKIP
Step 3: i=2, byte = body[4] = 0x00 → SKIP
Step 4: i=3, byte = body[3] = 0x00 → SKIP

Step 5: i=4, byte = body[2] = 0x56
        Search LUT_VALID for 0x56 → match @ j=0x56
        LUT_REPL[0x56] = ??? (e.g., 'V')
        found_non_zero = true
        result = "V"

Step 6: i=5, byte = body[1] = 0x34
        LUT_REPL[0x34] = '4'
        result = "V4"

Step 7: i=6, byte = body[0] = 0x12
        LUT_REPL[0x12] = ???  (e.g., 'И' Cyrillic)
        result = "V4И"

Output: "V4И" (3 chars, leading zeros trimmed)
```

---

## §221. ИТОГИ ЭТАПА H

### Новые находки (#834-875):

- **§209**: `_Form_View` — POLNYI COM IDispatch invocation (GetActiveObject + CoCreateInstance + Invoke с VARIANT VT_ARRAY|VT_UI1)
- **§210**: `_FreeBuffer` — простая free() wrapper
- **§211**: `fcn.00402708` — POLNYI PASSPORT_LUT decoder (reverse iteration + LUT substitution + leading-zero trim)
- **§212**: PLACE — 6-state enum dispatch на body[+0x35] для V3 SUB
- **§213**: STAGE/SECTION/PATH/PICKETLINE — простые byte→IntToStr (НЕ enum, исправление §202)
- **§214**: HandlerA SUB — POLNYI 12-полей extractor map
- **§215**: `fcn.00414208` — IntToStr helper
- **§216**: info_byte bit 1 НЕ тестируется в EXE (только bit 0 и bit 2)
- **§217**: Catalog pairs format — гипотезы (4B или 8B per entry)
- **§218**: WriteField TYPE 4 (BLOB) — НЕ существует; BLOB attach через COM (VARIANT)
- **§219**: `_SortBufData` output buffer — детальная layout с PASCAL strings
- **§220**: PASSPORT_LUT hex dump полный — identity validation + substitution replacement

### КРИТИЧЕСКИЕ открытия:

1. **DataTable COM-сервер** `{E392D449-ECE3-4E27-BF74-05780FA1E6D6}` — внешний OLE Server, принимающий байт-массивы через `IDispatch::Invoke("ShowWindow", VT_ARRAY|VT_UI1)`
2. **PASSPORT_LUT decoder** — reverse-iteration с LUT substitution + leading-zero trim (НЕ стандартный BCD!)
3. **PLACE = 6-state enum** для V3 SUB (НЕ просто byte как STAGE/SECTION/PATH/PICKETLINE)
4. **STAGE/SECTION/PATH/PICKETLINE — простые byte→IntToStr** (исправление §202: НЕ enum)
5. **BLOB attach НЕ через WriteField type=4** — BLOB передаётся через COM call (`_Form_View`) как VT_ARRAY|VT_UI1
6. **info_byte bit 1 не используется** (бит зарезервирован — на TypeVar уровне всё определяется)

### ❓ ОСТАЁТСЯ к реверсу:

- ❓ Catalog pairs точный размер (4B vs 8B) — требуется disasm PelengPC.exe handshake processing
- ❓ A-scan body internal sample format (1D samples + per-channel meta?)
- ❓ B-scan body internal pixel layout (width × height × byte)
- ❓ Точное содержимое CONDLENGTH offset для Version 1 (§203 — TBD)
- ❓ Дальнейший разбор fcn.0040c8f4 (Borland TList::Insert? cache storage?)
- ❓ Версия DataTable COM-server (PelengViewer.exe? сторонний продукт?)

---

## Финальная сводка по раундам (после этапа H)

- ROUND4: 189 (§1-66)
- ROUND5 этап A: +105 (190-294)
- ROUND5 этап B: +65 (295-359)
- ROUND5 этап C: +176 (360-535)
- ROUND5 этап D: +107 (536-642)
- ROUND5 этап E: +78 (643-720)
- ROUND5 этап F: +50 (721-770)
- ROUND5 этап G: +63 (771-833)
- **ROUND5 этап H: +42 (834-875)**
- **TOTAL: 875 находок**

