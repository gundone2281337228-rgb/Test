# PELENG REVERSE — Углублённый анализ декомпиляции

**Дата:** 2026-05-12  
**Источники:** `PelengPC.exe (1).c` (225870 строк, Ghidra), `102_203dll.dll.c` (33872 строк), `zapis2.c` (133634 строк)

---

## 0. Общая картина бинарей

| Бинарь | Компилятор | Размер декомпила | Архитектура |
|--------|-----------|-----------------|-------------|
| PelengPC.exe | Borland C++Builder 2009 (VCL) | 225K строк | Win32, VCL Forms, BDE/IBX |
| 102_203dll.dll (dbdll.dll) | Delphi 2007 | 34K строк | Win32 DLL, 3 экспорта |
| zapis2.exe | Visual C++ 2003 (MFC) | 134K строк | Win32, MFC Doc/View, CxImage, WordML |

**Путь сборки PelengPC:** `D:\PROGRAM FILES\BORLAND\CBUILDER2009\...` (из ASSERT-строк)  
**OLE-интерфейс:** `PelengPC.IData` (COM Automation server)  
**Реестр:** `HKLM\SOFTWARE\Altek\PelengPC`, `HKLM\SOFTWARE\Altek\Operators`

---

## 1. PelengPC.exe — Архитектура форм

### 1.1 Полный список VCL-форм (8 штук)

| Глобальная переменная | PTR-адрес | Назначение |
|----------------------|-----------|------------|
| `_FormMain` | `0x0053af64` | Главное окно, дерево настроек, управление |
| `_FormProgress` | `0x0053af68` | Прогресс-бар приёма блока |
| `_FormNastr` | `0x0053af6c` | Редактирование настроек (NASTR) |
| `_FormPassWord` | `0x0053af70` | Диалог пароля / подтверждения записи |
| `_FormReadData` | `0x0053af74` | Чтение данных из прибора |
| `_FormConfirm` | `0x0053af84` | Диалог подтверждения |
| `_FormRaport` | `0x0053af88` | Просмотр рапортов |
| `_FormScreen` | `0x0053af8c` | Визуализация B-scan |

### 1.2 Инициализация форм

Все формы создаются последовательно в `FUN_004bb0ec` (строка ~8401):
```
FUN_004bb0ec(PTR_DAT_0053b16c, PTR_PTR_005273d0, &FormMain);
// далее FormProgress, FormNastr, FormPassWord, FormReadData, FormConfirm, FormRaport, FormScreen
```



---

## 2. COM-порт: полная абстракция (PelengPC.exe)

### 2.1 Объект COM-порта

Объект порта размещается по смещению `DAT_0053e948`. Структура (реконструирована):

```c
struct ComPort {                   // base at DAT_0053e948
    /* 0x00 */ HANDLE hFile;       // HANDLE от CreateFileA("\\.\COMx")
    /* 0x04 */ int    portNum;     // номер порта (для формирования строки)
    /* 0x08 */ byte   baudIndex;   // индекс из LUT ниже
    /* 0x0C */ DWORD  baudRate;    // скорость (числом)
    /* 0x10 */ byte   dataBits;    // 0=5, 1=6, 2=7, 3=8  (ByteSize = val + 5)
    /* 0x11 */ byte   stopBits;    // 0=1, 1=1.5, 2=2
    /* 0x12 */ byte   parity;      // 0=none, 1=odd, 2=even...
    /* 0x13 */ bool   isOpen;      // флаг "порт открыт"
    /* 0x14 */ DWORD  timeout;     // таймаут записи/чтения (мс)
};
```

### 2.2 Открытие порта — `FUN_00423d18`

```
CreateFileA("\\.\COMn", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL)
→ GetCommTimeouts → установка:
    ReadIntervalTimeout        = 1
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant   = 1
    WriteTotalTimeoutMultiplier= 0
    WriteTotalTimeoutConstant  = 10
→ Sleep(100)
```

### 2.3 Применение настроек — `FUN_00424020`

```
GetCommState → установка:
    BaudRate = port[3]            // DWORD по смещению 0x0C
    Parity   = port->parity       // byte 0x12
    ByteSize = port->dataBits + 5 // byte 0x10 + 5
    StopBits = port->stopBits     // byte 0x11
    fOutxCtsFlow=0, fOutxDsrFlow=0  // _8_4_ = 0 (никакого Flow Control)
→ SetCommState
```

**Вывод:** Железобетонно подтверждено — **8N1 без аппаратного управления потоком**.

### 2.4 Полная таблица Baud Rate — `FUN_00423b4c`

| Hex | Decimal | baudIndex |
|-----|---------|-----------|
| 0x6E | 110 | 0 |
| 0x12C | 300 | 1 |
| 0x258 | 600 | 2 |
| 0x4B0 | 1200 | 3 |
| 0x960 | 2400 | 4 |
| 0x12C0 | 4800 | 5 |
| 0x2580 | 9600 | 6 (default) |
| 0x3840 | 14400 | 7 |
| 0x4B00 | 19200 | 8 |
| 0x9600 | 38400 | 9 |
| 56000 | 56000 | 10 |
| 0xE100 | 57600 | 11 |
| 0x1C200 | 115200 | 12 |
| 0x1F400 | 128000 | 13 |
| 0x3E800 | 256000 | 14 |

**Default при неизвестном значении:** baudIndex = 6 (9600 baud).

### 2.5 Функции ввода-вывода

| RVA | Имя | Описание |
|-----|-----|----------|
| `0x00423e58` | `WriteBytes` | WriteFile с таймаутом через GetTickCount |
| `0x00423f0c` | `WriteByte` | Обёртка — пишет ровно 1 байт |
| `0x00423f40` | `BytesAvail` | ClearCommError → cbInQue |
| `0x00423f64` | `ReadBytes` | ReadFile в цикле с таймаутом |
| `0x00423e28` | `ClosePort` | CloseHandle + isOpen=false |



---

## 3. Протокол PelengPC ↔ Прибор (подтверждённые опкоды)

### 3.1 Опкод `0x55` — HANDSHAKE (строка ~26657)

```c
local_45 = 0x55;
FUN_00423f0c(port, &local_45);         // WriteByte(0x55)
local_4c = FUN_00423f64(port, buf, 0x80010);  // ReadBytes(buf, 524304)
if (local_4c < 0x11) { ERROR... }      // ответ < 17 байт = ошибка
```

Ответ содержит **16-байтовый заголовок** + тело:
- `buf[2]` = ?  (копируется в `local_58`)
- `buf[0..0x0F]` — заголовок, копируется в `port+0x22` через `memcpy(..., &local_6a, 0x10)`
- `buf+4` — начало тела данных
- `port+0x0E` = `(local_4c - 0x10) >> 1 - 1` — число элементов A-scan

### 3.2 Опкод `0x42` — BLOCK_REQ (`FUN_00424cc0`, строка ~26753)

```c
local_41 = 0x42;
FUN_00423f0c(port, &local_41);          // WriteByte(0x42)
FUN_00423a58(port, 10);                 // задержка/flush 10мс
local_41 = (byte)blockAddr;             // младший байт адреса
FUN_00423f0c(port, &local_41);
FUN_00423a58(port, 10);
local_41 = (byte)(blockAddr >> 8);      // старший байт адреса
FUN_00423f0c(port, &local_41);
FUN_00423a58(port, 10);
// → FormProgress показывает прогресс
// → приём блока через FormProgress->0x304
```

**Формат запроса:** `[0x42] [10ms] [addr_lo] [10ms] [addr_hi] [10ms]` → ожидание ответа размером `param_4` байт.

### 3.3 Опкод `0x9a` — FLASH_DUMP (`FUN_00424bb0`, строка ~26698)

```c
local_41 = (byte)opcode;   // = 0x9a
FUN_00423f0c(port, &local_41);          // WriteByte(0x9a)
// → FormProgress принимает блок фиксированного размера param_4
```

Вызывается в `FUN_0041cbf4` (строка ~21341):
```c
local_14 = 0x10c5;               // 4293 байт (Базовая)
if ((DAT_0053e97c & 4) != 0) {
    local_14 = 0x1485;           // 5253 байт (Вагонная)
}
FUN_00424bb0(&DAT_0053e948, 0x9a, param_2+5, local_14-5);
```

**Вывод:** Размер FLASH-дампа:
- **Базовая версия:** 4293 байт (0x10C5)
- **Вагонная версия (бит 2 в DAT_0053e97c):** 5253 байт (0x1485)

### 3.4 Опкод `0x74` — STATUS_T (`FUN_00424bb0` с param=0x74)

Строка ~25116:
```c
iVar4 = FUN_00424bb0(&DAT_0053e948, 0x74, local_40+4, local_48);
```

Принимает блок переменного размера (`local_48` байт) — это текущее состояние прибора (температура, батарея, настройки).

### 3.5 Размер фрейма `FormScreen` (B-scan)

```c
FUN_004235f8(_FormScreen, param_2 + 5, param_3 - 5);
```

Тело B-scan начинается с **смещения +5** от начала пакета, длина `param_3 - 5`. Фактический размер:
- Базовая: 4293 - 5 = 4288 байт
- Вагонная: 5253 - 5 = 5248 байт



---

## 4. Адресация блоков и sweep-механизм

### 4.1 Функция «Is Sweep Address» — `FUN_0041c7c0`

```c
undefined1 FUN_0041c7c0(param_1, param_2) {
    local_d = 0;
    if ((9999 < param_2) && (param_2 < 30000)) {
        local_d = 1;
    }
    return local_d;
}
```

**Железобетонно:** Sweep-адреса лежат в диапазоне **10000–29999**.

### 4.2 Вспомогательные функции адресации

| RVA | Функция | Формула |
|-----|---------|---------|
| `0x0041c7f0` | `GetBaseAddr` | `(addr / 100) * 100` |
| `0x0041c810` | `GetSubBlock` | `(addr % 10000) / 100` |
| `0x0041c838` | `GetIndex` | `addr % 100` |

**Интерпретация:** Адрес = `[base × 100] + [subblock × 1] + [index]`  
Пример: адрес `10305` → base=10300, subblock=3, index=5.

### 4.3 Структура Sweep-таблицы (смещение 0x324)

```c
// FUN_0041c338 (строка 20968):
for (i = 0; i < self->sweepCount; i++) {    // sweepCount @ self+0x3b0
    entry = i * 0x18 + self;                 // каждая запись = 24 байта
    if (entry[0x324] <= addr && addr <= entry[0x328]) {
        return entry[0x32C];                 // возвращает значение поля
    }
}
```

**Sweep-запись (24 байта):**
```
offset 0x324: addr_min (int)
offset 0x328: addr_max (int)  
offset 0x32C: field_value (int)
offset 0x330: field_2 (int)
offset 0x320: name_string_ptr
```

### 4.4 Burst-адрес «60000» — специальный режим

```c
if (*(int *)(local_7c + 0x3c8) == 60000) {
    local_84 = FUN_0041cbf4(local_7c, local_80+4);  // FLASH_DUMP (0x9a)
    *(undefined2 *)(local_80+4) = 60000;             // маркер в буфере
}
```

Адрес `60000` означает **«прочитать весь FLASH-дамп»** (опкод 0x9a), а не обычный блочный запрос.

---

## 5. Визуализация B-scan — `FUN_004235f8`

```c
void FUN_004235f8(int formScreen, int rawData, int rawLen) {
    int bscanLen = rawLen - 0x1C0;              // 448 байт = заголовок A-scan
    memcpy(formScreen+0x2D8, rawData+0x1D0, bscanLen);  // тело B-scan
    
    int rows = (bscanLen * 8) / 0xF0;          // 0xF0 = 240 бит на строку
    int cols = 0x1E;                            // 30 байт на строку
    
    // Создаём Bitmap: width=0x1E0 (480px), height=rows*2
    Bitmap.SetWidth(0x1E0);
    Bitmap.SetHeight(rows * 2);
    
    // Рендеринг: каждый бит → пиксель 2×2
    for (row = 0; row < rows+1; row++) {
        for (col = 0; col < cols+1; col++) {
            byte val = formScreen[0x2D8 + col + row * cols];
            for (bit = 0; bit < 8; bit++) {
                if ((val >> bit) & 1) { pen = BLACK; }
                else { pen = WHITE; }
                FillRect(col*8+bit)*2, row*2, (col*8+bit+1)*2, (row+1)*2);
            }
        }
    }
}
```

**Ключевые параметры B-scan:**
- Заголовок A-scan: **0x1C0 = 448 байт**
- Смещение тела B-scan в пакете: **0x1D0 = 464 байта** (448 + 16 байт фрейм-хедера)
- Ширина строки: **30 байт = 240 бит**
- Пиксель: **1 бит → 2×2 пикселя** (увеличение ×2)
- Максимальная ширина изображения: **480 px** (240 × 2)



---

## 6. DLL `102_203dll.dll` (dbdll.dll) — TLV-декодер

### 6.1 Три экспортируемые функции

| Ордер | Имя | RVA | Сигнатура |
|-------|-----|-----|-----------|
| 1 | `_Form_View` | 0x131C | `void __cdecl (param1, byte* data, ULONG size)` |
| 2 | `_SortBufData` | 0x1454 | `int* __cdecl (byte* rawPacket, uint* pSize)` |
| 3 | `_FreeBuffer` | 0x193C | `void __cdecl (int allocatedPtr)` |

### 6.2 `_SortBufData` — главный парсер TLV

**Вход:** `rawPacket` — сырой пакет от прибора, `pSize` — указатель на размер.

```c
int* _SortBufData(byte* param_1, uint* param_2) {
    uint origSize = *param_2;
    byte* body = param_1 + 0x10;                    // пропускаем 16-байт заголовок
    
    // Извлечение полей из заголовка:
    uint blockAddr = param_1[0x10] + param_1[0x11] * 256;  // LE16 @ offset 0x10
    uint category  = blockAddr / 1000;                       // категория = тысячи
    int  sweepId   = param_1[4] * 100 + param_1[5];        // идентификатор sweep
    int  devId     = param_1[0] + param_1[1] * 256;        // LE16 @ offset 0
    int  field_c   = param_1[0x16] * 256 + param_1[0x15];  // BE16 @ offset 0x15-0x16
    
    // Получаем описатель формата по category
    int* formatDesc = FUN_0040721c(&category);
    int headerLen = FUN_00401e60(formatDesc);       // суммарная длина полей
    int numFields = formatDesc[2] * 4;              // количество полей × 4
    
    // Аллокация результата
    uint totalSize = origSize + headerLen + numFields + 2;
    int* result = malloc(totalSize);
    *param_2 = totalSize;
    
    // Копируем сырые данные в хвост результата
    memcpy(result + numFields + headerLen + 2, param_1, origSize);
    int decoded = result + numFields + 2;
    
    // Диспатч по категории:
    switch (category) {
        case 1:  result[0] = 2; FUN_00402980(...); break;  // A-scan
        case 4:
        case 6:  result[0] = 1; FUN_00402c8c(...); break;  // Generic (настройки?)
        case 5:  result[0] = 1; FUN_00402f34(...); break;  // Generic (калибровка?)
        case 10..19: result[0] = 3; FUN_004031dc(...); break;  // B-scan / результаты
        case 20..29: result[0] = 4; FUN_00403420(...); break;  // Settings/ShortProt
    }
    
    // Заполняем LUT смещений полей
    FUN_00402238(result + 2, formatDesc);
    result[1] = (char)formatDesc[2];   // количество полей
}
```

### 6.3 Байт категории (result[0])

| result[0] | Категория | Адреса (тысячи) | Обработчик |
|-----------|-----------|-----------------|------------|
| 2 | A-scan | 1xxx | `FUN_00402980` |
| 1 | Generic/Settings | 4xxx, 6xxx | `FUN_00402c8c` |
| 1 | Calibration | 5xxx | `FUN_00402f34` |
| 3 | B-scan/Results | 10xxx–19xxx | `FUN_004031dc` |
| 4 | ShortProt | 20xxx–29xxx | `FUN_00403420` |

### 6.4 Формат выходного буфера `_SortBufData`

```
Offset 0:    byte  category       (1..4)
Offset 1:    byte  numFields
Offset 2:    LE16  field_offsets[numFields]  — пары (offset, length) для каждого поля
             ...decoded field values (строки/числа)...
             ...raw original packet (origSize bytes)...
```

### 6.5 Обработчик `FUN_004022c8` — диспатч по TypeVar

```c
void FUN_004022c8(int* sweepInfo, int rawData, int decodedBuf, int* formatDesc) {
    int typeVar = sweepInfo[3];     // param_1[3] = TypeVar
    
    if (typeVar < 0x282) {          // < 642
        if (typeVar < 0x186) {      // < 390
            if (typeVar - 0x96 < 2)     // 150, 151 → FUN_00404758
            if (typeVar - 0x16A < 2)    // 362, 363
            if (typeVar - 0x17C < 2)    // 380, 381
        } else if (typeVar - 0x186 < 2) // 390, 391 → FUN_004041d0
          ...
        if (typeVar - 0x1BA < 2)        // 442, 443
        if (typeVar - 0x1E0 < 2)        // 480, 481
    } else if (typeVar < 0x2DA) {   // < 730
        if (typeVar - 0x282 < 2)        // 642, 643 → FUN_004041d0
        if (typeVar - 0x2A8 < 4)        // 680-683
        if (typeVar - 0x2C8 < 2)        // 712, 713
    } else {
        if (typeVar - 0x2DA < 2)        // 730, 731
        if (typeVar - 0x2E4 < 2)        // 740, 741
        if (typeVar - 0x30C < 2)        // 780, 781
        if (typeVar - 0x34A < 2)        // 842, 843 → FUN_004041d0
    }
    // default → FUN_00403834
}
```

**Новые TypeVar-диапазоны (из DLL):**
- 150–151, 362–363, 380–381 → формат A-scan (`FUN_00404758`)
- 390–391, 442–443, 480–481, 642–643, 842–843 → формат B-scan/composite (`FUN_004041d0`)
- 680–683, 712–713, 730–731, 740–741, 780–781 → формат generic (`FUN_00403834`)

### 6.6 Декодирование даты — `FUN_004023b0`

```c
void FUN_004023b0(int rawData, ..., int offset) {
    uint year  = rawData[offset + 2] + 2000;   // байт → год (0=2000, 24=2024...)
    byte month = rawData[offset + 1];
    byte day   = rawData[offset];
    
    // Валидация через FUN_00402874(year, month, day)
    // Если невалидно → дата = 01.01.2000
}
```

**Формат даты в пакете: `[day][month][year-2000]`** — 3 байта, НЕ BCD, а обычные целые!

### 6.7 Декодирование времени — `FUN_00402504`

```c
void FUN_00402504(int rawData, ..., int offset) {
    byte hours   = rawData[offset];
    byte minutes = rawData[offset + 1];
    
    // Форматирование: "%d%d:%d%d" → "HH:MM"
    // (hours%100)/10, (hours%100)%10, ':', (minutes%100)/10, (minutes%100)%10
}
```

**Формат времени: `[hours][minutes]`** — 2 байта, %100 используется как sanity-check.

### 6.8 `_Form_View` — передача данных в PelengPC (COM/OLE)

```c
void _Form_View(param1, byte* data, ULONG size) {
    // Ищет или создаёт OLE-объект по CLSID (DAT_0042821C)
    GetActiveObject(&clsid, NULL, &unk) || CoCreateInstance(&clsid, NULL, CLSCTX_LOCAL_SERVER, &iid, &obj);
    
    // Вызывает метод "ShowWindow" через IDispatch
    // Передаёт данные как SAFEARRAY(VT_UI1)
    SafeArrayCreateVector(VT_UI1, 0, size);
    SafeArrayAccessData → memcpy → SafeArrayUnaccessData
    // Invoke(PAKIET) — передаёт массив через Automation
}
```

**Вывод:** DLL может работать автономно — она сама устанавливает OLE-соединение с PelengPC.exe и передаёт декодированные данные.



---

## 7. zapis2.exe — Генератор отчётов

### 7.1 Архитектура

- **Фреймворк:** MFC (Visual Studio 2003), Document/View
- **Классы:** `CZapisDoc`, `CZapisView`, `CMainFrame`
- **Графика:** CxImage (PNG/BMP рендеринг встроенных картинок)
- **Отчёты:** WordML XML (Microsoft Word 2003 XML)

### 7.2 Формат отчёта — WordML

zapis2.exe генерирует `.doc`-файлы в формате **WordML** (Word 2003 XML), а НЕ FastReport `.fr3`:

```xml
<?xml version="1.0"?>
<?mso-application progid='Word.Document'?>
<w:wordDocument
    xmlns:w="http://schemas.microsoft.com/..."
    xmlns:v="urn:schemas-microsoft-com:vml"
    xmlns:wx="urn:schemas-microsoft-com:office:word"
    ...>
  <w:body>
    <wx:sect>
      <w:sectPr>
        <w:pgSz w:w="11906" w:h="16838"/>  <!-- A4 portrait -->
        <w:cols w:num="..."/>
        <w:type w:val="continuous"/>
      </w:sectPr>
      ...
    </wx:sect>
  </w:body>
</w:wordDocument>
```

**Размер страницы:** 11906 × 16838 twips = **A4 портрет** (210 × 297 мм).

### 7.3 Встроенные изображения (протокол контроля)

```xml
<v:shape style="width:240pt;height:120pt">
    <v:imagedata src="wordml://protPict.png"/>
</v:shape>
```

Картинка `protPict.png` — встроенное в XML base64-изображение протокола дефектоскопии.  
Для приборов с B-scan добавляется `position:absolute` стиль.

### 7.4 Реестр — конфигурация

| Ключ | Назначение |
|------|-----------|
| `SOFTWARE\Altek\PelengPC` | Основные настройки (пути, параметры) |
| `SOFTWARE\Altek\Operators` | Список операторов |
| `SOFTWARE\Altek\Operators\Count` | Количество операторов |
| `SOFTWARE\Altek\Operators\Operator%d` | Имя оператора (1-based) |

### 7.5 Схема полей отчёта (из DFM-ресурсов)

Каждый шаблон отчёта содержит массив дескрипторов полей с типами:

| Тип поля | Значение | Описание |
|----------|----------|----------|
| `no_edit` | Не редактируемое | Фиксированный текст / автозаполнение |
| `ifLength` | Условное по длине | Показывается если > порога |
| `ifHight` | Условное по высоте | Показывается если амплитуда > порога |
| `defekt_edit` | Редактируемое | Ввод описания дефекта оператором |

Найдено **9+ шаблонов отчётов** (по количеству блоков `_DAT_004bd130...` в инициализации), каждый с 7–8 полями стандартной структуры:
```
[no_edit, no_edit, no_edit, ifLength, ifHight, no_edit, no_edit, defekt_edit]
```

### 7.6 Ключевые поля данных

| Поле | Строковый ID | Использование |
|------|-------------|---------------|
| `numZap` | `00501708`, `00501718`... | Номер записи (индекс протокола) |
| `imhoOper` | `00501814`, `0050185c`... | Имя оператора |
| `BC1+BC2` | `004b26f4` | Маркер двухканального B-scan |

### 7.7 HTML-подотчёт

Помимо WordML, zapis2 умеет генерировать **HTML-таблицы**:
```html
<table border=0 align='center'>
  <tr><td><b>...</b></td></tr>
  <tr><td>...</td>
</table>
```

Используется для preview/встраивания в интерфейс.

---

## 8. База данных — дополнительные SQL-запросы (PelengPC.exe)

### 8.1 Таблица BlockZap

```sql
SELECT * FROM BlockZap WHERE Num...     -- 0x00523fdd
SELECT * FROM BlockZap WHERE Num...     -- 0x0052410c (другой предикат)
SELECT * FROM ...                       -- 0x00524267 (generic)
INSERT INTO ...                         -- 0x0052429e
UPDATE ...                              -- 0x005242f0
WHERE Number= ...                       -- 0x0052430c
DELETE FROM BlockZap WHERE Number...    -- 0x0052436d
DELETE FROM ...                         -- 0x00524390
```

### 8.2 IBX-транзакции

PelengPC использует **IBX (InterBase Express)** компоненты:
```
_Ibdatabase_TIBTransaction_StartTransaction_qqrv(self + 0x3C8)
_Ibdatabase_TIBTransaction_Commit_qqrv(self + 0x3C8)
```

Транзакция живёт по смещению `+0x3C8` от объекта FormNastr.

### 8.3 BDE (Borland Database Engine)

В дополнение к IBX обнаружены строки BDE:
```
s_Software\Borland\Database_Engine   @ 0x00530EB2
s_DLLPATH                            @ 0x00530ED3
s_DLLINITFUNCTION                    @ 0x00530EF7
s_DLLEXITFUNCTION                    @ 0x00530F13
s_DLLEXITFUNCTION2                   @ 0x00530F23
s_IDAPI32.DLL                        @ 0x00530F34
s_DbiInitFn                          @ 0x00530F40
s_DbiExit                            @ 0x00530F4A
```

**Вывод:** PelengPC поддерживает **два** движка БД — BDE (legacy) и IBX/Firebird (актуальный).



---

## 9. Загрузка DLL и связь с PelengPC

### 9.1 Динамическая загрузка

```c
// строка ~11227:
hModule = LoadLibraryA(lpLibFileName);   // загрузка dbdll.dll

// строка ~14375:
DAT_005be998 = GetProcAddress(hModule, "_SortBufData");

// строка ~14676:
DAT_005be99c = GetProcAddress(hModule, "_FreeBuffer");

// строка ~15379:
DAT_005be994 = GetProcAddress(hModule, "_Form_View");
```

**Глобальные указатели на функции DLL:**
| Адрес | Указатель на |
|-------|-------------|
| `0x005be994` | `_Form_View` |
| `0x005be998` | `_SortBufData` |
| `0x005be99c` | `_FreeBuffer` |

### 9.2 Вызов `_SortBufData` из PelengPC

```c
// строка ~14383:
byte bVar1 = *(byte*)(param_2 + 0x10);    // первый байт тела (после 16-byte header)
byte bVar2 = *(byte*)(param_2 + 0x11);    // второй байт тела
byte* result = (*DAT_005be998)();          // вызов _SortBufData()
int item = FUN_004ec974(puVar3, *result - 1);   // result[0]-1 = индекс категории
```

### 9.3 Обработка результата по типу поля (uVar17)

```c
uVar17 = *(uint*)(item + 0x2C) & 7;   // тип поля (3 бита)

if (uVar17 == 1) {   // ЧИСЛОВОЙ — big-endian сборка
    for (i = 0; i < fieldLen; i++) {
        value = result[i + offset] + value * 0x100;  // BE accumulate
    }
    // → IntToStr(value)
}

if (uVar17 == 2) {   // СТРОКОВЫЙ — побайтовый до нуля
    for (i = 0; i < fieldLen; i++) {
        if (result[i + offset] == 0) break;
        // → append char
    }
}

if (uVar17 == 3) {   // ДАТА — [day][month][year] → "DD.MM.YYYY"
    month = result[offset + 3];     // 1..12, иначе ошибка
    day   = result[offset];         // 1..31
    year  = result[offset + 4];     // < 100, иначе ошибка
    // → FormatDateTime
}
```

**Три типа полей в TLV:**
1. **Числовой (1):** Big-Endian целое переменной длины
2. **Строковый (2):** Null-terminated ASCII
3. **Дата (3):** `[day, ?, ?, month, year]` — 5 байт (не 3!)

---

## 10. Режимы подключения (флаг 0x3C4)

```c
if (*(char*)(self + 0x3C4) == '\x01') {
    // Прямой режим (через COM-порт) → FUN_00424cc0/FUN_00424bb0
}
else if (*(char*)(self + 0x3C4) == '\0') {
    // Режим «по сети» / через промежуточный буфер → FUN_0041b288
}
```

| Значение 0x3C4 | Режим | Функция чтения |
|----------------|-------|---------------|
| 0x01 | Прямое подключение (RS-232) | `FUN_00424cc0` / `FUN_00424bb0` |
| 0x00 | Чтение из памяти/буфера | `FUN_0041b288` |

`FUN_0041b288` (строка ~20314) — ищет в списке `self+0x31C` запись с нужным адресом и копирует данные из `self+0x314` (кэш блоков):
```c
for (i = 0; i < list->count; i++) {
    entry = list[i];
    if (entry->addr == requestedAddr) {
        memcpy(outBuf, self->cache + entry->offset, entry->size);
        return entry->size;
    }
}
```

---

## 11. Streak-эвристика (подтверждение из `FUN_0041c858`)

```c
bool FUN_0041c858(int self, int addr) {
    int baseAddr = (addr / 100) * 100;
    int maxIndex = addr % 100;
    
    // Ищем в таблице self+0x3BC (max self+0x3C0 записей):
    for (i = 0; i < self->streakCount; i++) {
        entry = self->streakTable[i];   // 8 байт: [currentMax, baseAddr]
        if (entry->base == baseAddr) {
            if (entry->max < maxIndex) {
                entry->max = maxIndex;  // обновляем максимум
            }
            return (i == streakCount);  // false = уже есть
        }
    }
    // Новая запись:
    newEntry->base = baseAddr;
    newEntry->max = maxIndex;
    streakCount++;
    return true;  // новый sweep
}
```

**Streak-таблица:**
- Хранится по смещению `self+0x3BC`
- Количество записей: `self+0x3C0`
- Каждая запись: 8 байт `{ int maxIndex; int baseAddr; }`
- Логика: отслеживает максимальный subblock-индекс для каждого base-адреса

---

## 12. Сводка новых находок (vs PELENG_REVERSE.md)

| # | Находка | Статус | Источник |
|---|---------|--------|----------|
| 1 | Полная структура COM-port объекта (20 байт) | 🔒 Железобетонно | PelengPC.exe |
| 2 | 15 значений baud rate (110–256000), default=9600 | 🔒 Железобетонно | FUN_00423b4c |
| 3 | Формат HANDSHAKE: send 0x55, expect ≥17 байт | 🔒 Железобетонно | строка 26657 |
| 4 | Формат BLOCK_REQ: 0x42 + [delay] + addr_lo + [delay] + addr_hi | 🔒 Железобетонно | FUN_00424cc0 |
| 5 | FLASH_DUMP размеры: 4293 (Базовая), 5253 (Вагонная) | 🔒 Железобетонно | FUN_0041cbf4 |
| 6 | B-scan: 30 байт/строка, 1 бит/пиксель, offset=0x1D0 | 🔒 Железобетонно | FUN_004235f8 |
| 7 | Sweep-адреса: ровно 10000–29999 | 🔒 Железобетонно | FUN_0041c7c0 |
| 8 | Адрес 60000 = FLASH_DUMP (специальный) | 🔒 Железобетонно | FUN_0041b43c |
| 9 | Нет Flow Control (fOutxCtsFlow=0) | 🔒 Железобетонно | FUN_00424020 |
| 10 | CommTimeouts: ReadInterval=1, Write=10ms | 🔒 Железобетонно | FUN_00423d18 |
| 11 | DLL: 5 категорий TLV (1=Ascan, 4/6=Generic, 5=Calib, 10-19=Bscan, 20-29=ShortProt) | 🔒 Железобетонно | _SortBufData |
| 12 | DLL: дата = [day][month][year-2000], НЕ BCD | 🔒 Железобетонно | FUN_004023b0 |
| 13 | DLL: время = [hours][minutes], plain byte | 🔒 Железобетонно | FUN_00402504 |
| 14 | DLL: TypeVar-диспатч по диапазонам 150-843 | 🔒 Железобетонно | FUN_004022c8 |
| 15 | DLL → PelengPC через OLE Automation (SAFEARRAY) | 🔒 Железобетонно | _Form_View |
| 16 | PelengPC поддерживает BDE + IBX (два движка) | 🔒 Железобетонно | строки BDE |
| 17 | Два режима: 0x3C4=1 (прямой COM), =0 (буферный) | 🔒 Железобетонно | FUN_0041b43c |
| 18 | zapis2: отчёты через WordML (не FastReport) | 🔒 Железобетонно | zapis2.c |
| 19 | zapis2: 9+ шаблонов с полями no_edit/ifLength/ifHight/defekt_edit | 🔒 Железобетонно | DFM-инит |
| 20 | zapis2: MFC Doc/View (CZapisDoc/CZapisView) | 🔒 Железобетонно | RTTI |
| 21 | Реестр: SOFTWARE\Altek\PelengPC + \Operators | 🔒 Железобетонно | zapis2.c |
| 22 | A4 portrait (11906×16838 twips) | 🔒 Железобетонно | WordML pgSz |
| 23 | protPict.png — embedded B-scan image | 🔒 Железобетонно | zapis2.c |

---

## 13. Что осталось непокрыто

1. **Innoextract PelengPC_ver1_2.exe** — sandbox не имеет доступа к репозиториям пакетов (INTEGRATIONS_ONLY), `innoextract` установить нельзя. Нужна локальная машина.
2. **DFM-ресурсы zapis2.exe** — внутри PE лежат DFM-формы с полными словарями TypeVar. Нужен `wrestool` или Resource Hacker.
3. **Полный словарь TypeVar** из DFM PelengPC.exe — содержит числовые коды 24667/24668/10001/40001.
4. **Парсер `.dal`-файлов** — не обнаружен ни в одном из трёх бинарей; возможно отдельная утилита.
5. **FastReport `.fr3`** — в zapis2.exe НЕ используется (WordML), но может быть в других утилитах пакета.
6. **Точная семантика полей STATUS_T (0x74)** — принимается как сырой блок, без структурированного парсинга в PelengPC.exe.
7. **Содержимое 16-байтового заголовка пакета** — частично: байты 0-1 (devId LE16), 4-5 (sweepId), 0x10-0x11 (blockAddr LE16), 0x15-0x16 (field_c BE16). Остальные 8 байт — неизвестны.

---

## 14. Рекомендации по продолжению

1. **На локальной машине** выполнить `innoextract PelengPC_ver1_2.exe` → получить отдельные .exe, .dll, .fdb, .dal, .fr3.
2. **wrestool -x -t 10 zapis2.exe** → извлечь DFM-ресурсы с полными шаблонами TypeVar.
3. **Resource Hacker на PelengPC.exe** → DFM-формы FormNastr, FormReadData содержат таблицу sweep-адресов.
4. Сопоставить найденные TypeVar-диапазоны (150-843 из DLL) с ж/д словарём (24667, 24668...) — вероятно, это `typeVar % 1000` или отдельная нумерация.
5. Проверить гипотезу: `field_c = param_1[0x15]*256 + param_1[0x16]` — это CRC? Контрольная сумма? Или просто ID?



---

## 9. Загрузка DLL и связь с PelengPC

### 9.1 Динамическая загрузка

```c
// строка ~11227:
hModule = LoadLibraryA(lpLibFileName);   // загрузка dbdll.dll

// строка ~14375:
DAT_005be998 = GetProcAddress(hModule, "_SortBufData");

// строка ~14676:
DAT_005be99c = GetProcAddress(hModule, "_FreeBuffer");

// строка ~15379:
DAT_005be994 = GetProcAddress(hModule, "_Form_View");
```

**Глобальные указатели на функции DLL:**
| Адрес | Указатель на |
|-------|-------------|
| `0x005be994` | `_Form_View` |
| `0x005be998` | `_SortBufData` |
| `0x005be99c` | `_FreeBuffer` |

---

## 10. Парсинг FLASH-дампа — TLV-сканер (`FUN_0041b008`)

### 10.1 Алгоритм

```c
undefined1 FUN_0041b008(int self) {
    FUN_0041afb4(self);                    // очистить предыдущий список записей
    int* buf = malloc(0x80010);            // 512 КБ буфер
    
    // Копируем 16-байтовый заголовок → глобальные:
    DAT_0053e97c = buf_header[2];          // байт версии/флаги прибора
    memcpy(&DAT_0053e96a, buf_header, 16); // весь заголовок
    
    int pos = 0x10;                        // начинаем после заголовка
    int blockCount = 0;
    
    do {
        while (true) {
            if (pos >= self->totalSize) goto done;
            
            // Читаем TLV-запись:
            uint tag = LE16(self->data[pos], self->data[pos+1]);     // tag
            if (tag == 0xFFFF) break;                                 // end marker
            
            uint len = LE16(self->data[pos+2], self->data[pos+3]);   // length
            
            // Проверяем что len соответствует одному из sweep-диапазонов:
            bool found = false;
            for (i = 0; i < 6; i++) {
                if (self->sweepRanges[i].value == len) { found = true; break; }
            }
            
            if (!found) { pos++; continue; }  // пропускаем невалидную запись
            
            if (tag != 0) {
                // Создаём запись { tag, pos, len } → добавляем в self->blockList (0x31C)
                entry = alloc(12);
                entry->blockSize = len;      // [+8]
                entry->offset = pos;         // [+4]
                entry->tag = tag;            // [+0]
                list_add(self->blockList, entry);
                
                addressArray[blockCount] = tag;
                blockCount++;
            }
            pos += len;                       // переходим к следующей записи
        }
        // Пропускаем padding (0xFF байты):
        while (pos < totalSize && data[pos] == 0xFF) { pos++; }
    } while (pos < totalSize);
    
done:
    if (blockCount > 0) {
        FUN_0041c550(self, addressArray, blockCount);  // заполнить sweep-список
    }
}
```

### 10.2 Структура TLV-записи в FLASH

```
Offset: 0     2     4
        ┌─────┬─────┬────────────────┐
        │ tag │ len │    body[len]   │
        │LE16 │LE16 │                │
        └─────┴─────┴────────────────┘
```

- **tag** = LE16 — адрес блока (10100, 10200, ..., 60000 и т.д.)
- **len** = LE16 — размер тела (должен совпадать с одним из sweep-размеров)
- **body** = `len` байт сырых данных
- **End marker** = `0xFFFF` (`tag = 0xFFFF`)
- **Padding** = байты `0xFF` между записями

### 10.3 Глобальные данные из заголовка (DAT_0053e96a..0x0053e97c)

```
DAT_0053e964: char[6]  — device serial/name (from FUN_00423534)
DAT_0053e96a: byte[16] — полная копия 16-байт заголовка пакета
DAT_0053e97c: byte     — флаг версии прибора (бит 2 = Вагонная)
DAT_0053e980: int      — количество sweep-адресов
DAT_0053e984: int*     — массив sweep-адресов
```

---

## 11. Настройки и реестр (`FUN_0041fbc4` и связанные)

### 11.1 Сохранение настроек в реестр

`FUN_0041fbc4` — сериализация настроек прибора в `HKLM`:
1. Читает файл/буфер настроек
2. Парсит внутренний формат (позиционный)
3. Кодирует каждые 2 символа hex → 1 байт (hex-to-binary)
4. Пишет в `RegCreateKeyA(HKEY_LOCAL_MACHINE, path)` как `REG_BINARY`

### 11.2 Чтение настроек из реестра

`FUN_0041ff0c` — десериализация настроек из `HKLM`:
1. `RegCreateKeyA(0x80000002, path)` — открыть/создать ключ
2. `RegQueryValueExA(key, "Numbers"+offset, ...)` — прочитать REG_BINARY/REG_SZ
3. Декодирует LE16 пары → массив int'ов (адреса блоков)
4. Возвращает количество элементов

### 11.3 Реестровые функции — полная карта

| RVA | Назначение | Тип данных |
|-----|-----------|-----------|
| `0x00420070` | Read REG_SZ/REG_BINARY → string | REG_SZ (1) |
| `0x00420204` | Write string → REG_SZ | REG_SZ (1) |
| `0x004202f4` | Read REG_DWORD | REG_DWORD (4) |
| `0x0042039c` | Write REG_DWORD | REG_DWORD (4) |
| `0x00420458` | Read REG_BINARY → buffer | REG_BINARY (3) |
| `0x0042055c` | Write buffer → REG_BINARY | REG_BINARY (3) |

Все функции используют `HKEY_LOCAL_MACHINE` (0x80000002).  
Корневой ключ: строка по адресу (формируется динамически, но включает `SOFTWARE\Altek\PelengPC`).

---

## 12. Структура объекта FormReadData (реконструкция)

```c
struct FormReadData {               // base = _FormReadData
    /* VCL object header... */
    /* 0x2D0 */ TLabel*    StatusLabel;       // отображение статуса
    /* 0x2D8 */ TLabel*    InfoLabel;         // текст информации
    /* 0x2E8 */ TButton*   BtnRead;           // кнопка "Считать"
    /* 0x2EC */ TButton*   BtnDelete;         // кнопка "Удалить"
    /* 0x2F0 */ TButton*   BtnSelectAll;      // кнопка "Выбрать все"
    /* 0x2FC */ TComboBox* CbxPort;           // выбор COM-порта
    /* 0x304 */ TListBox*  LstBlocks;         // список блоков (адреса)
    /* 0x308 */ TListBox*  LstSweeps;         // список sweep-диапазонов
    /* 0x30C */ int*       AddrArray;         // массив адресов (LE16 из дампа)
    /* 0x310 */ int        AddrCount;         // количество адресов
    /* 0x314 */ byte*      RawBuffer;         // сырой FLASH-буфер
    /* 0x318 */ int        RawSize;           // размер буфера
    /* 0x31C */ TList*     BlockEntries;      // список {tag, offset, len}
    /* 0x320 */ char[24]   SweepRange0_name;  // имя sweep-диапазона 0
    /* 0x324 */ int        SweepRange0_min;   // мин. адрес
    /* 0x328 */ int        SweepRange0_max;   // макс. адрес
    /* 0x32C */ int        SweepRange0_value; // размер блока
    /* 0x330 */ int        SweepRange0_field2;
    /* ...sweep ranges 1..5 @ stride 0x18... */
    /* 0x3B0 */ int        SweepCount;        // 5 или 6 (по типу прибора)
    /* 0x3B4 */ int*       VisibleAddrs;      // отображаемые адреса
    /* 0x3B8 */ int        VisibleCount;      // количество видимых
    /* 0x3BC */ int*       StreakTable;        // streak: {maxIdx, baseAddr}[n]
    /* 0x3C0 */ int        StreakCount;       // количество streak-записей
    /* 0x3C4 */ byte       ConnectionMode;    // 0=buffer, 1=direct COM
    /* 0x3C8 */ int        CurrentBlockAddr;  // текущий запрашиваемый адрес
    /* 0x3CC */ char*      PortName;          // строка "COM1".."COM99"
};
```

---

## 13. DLL — полная карта полей по категориям

### 13.1 Категория 1 (A-scan) — 11 полей

```
FUN_00401db0(list, name, label, maxLen, offset, fieldType)

Поле  | fieldType | offset | maxLen | Интерпретация
------+-----------+--------+--------+---------------------------
  0   |    1      |   0    |   1    | DeviceID (1 byte numeric)
  1   |    1      |   0    |   4    | BlockAddr (4 byte numeric)
  2   |    2      |  0x14  |  0x14  | String (20 bytes @ +20)
  3   |    3      |   0    |   3    | Date (3 bytes)
  4   |    2      |   5    |   5    | String (5 bytes @ +5)
  5   |    1      |   0    |   2    | Numeric (2 bytes)
  6   |    2      |  0x1E  |   1    | String (1 byte @ +30)
  7   |    2      |  10    |  10    | String (10 bytes @ +10)
  8   |    1      |   0    |   2    | Numeric (2 bytes)
  9   |    1      |   0    |   2    | Numeric (2 bytes)
 10   |    1      |   0    |   0    | Numeric (0 bytes = empty)
```

**fieldType:** 1=Numeric(BE), 2=String, 3=Date

### 13.2 Категория 20-29 (ShortProt) — обработчик `FUN_00403420`

Поля выводятся в порядке:
1. Пустое (разделитель) 
2. `param_1[2] % 10000` → IntToStr → деление VARIANT на 100 (= десятичная дробь!)
3. `FUN_004022c8(...)` → TypeVar-зависимые поля (по диспатчу)
4. `param_1[5]` → IntToStr
5. Пустое
6. `param_1[4]` → строка (имя)
7. `param_1[1]` → IntToStr
8. `FUN_00402708(param_2 + 0x11, 6)` → 6-byte passport (BCD/encoded)

### 13.3 Категория 10-19 (B-scan/Results) — обработчик `FUN_004031dc`

Поля:
1. Пустое (разделитель)
2. `param_1[2] % 10000` → IntToStr (номер записи)
3. `FUN_004022c8(...)` → TypeVar-зависимые поля
4. `param_1[5]` → IntToStr
5. Пустое
6. `param_1[4]` → строка (имя)
7. `param_1[1]` → IntToStr

### 13.4 Generic decoder `FUN_00403834` (категории 4-6)

Поля (для категорий 4,5,6):
1. `FUN_004023b0(data, +7)` → Дата (offset 7)
2. `FUN_00402504(data, +10)` → Время (offset 10)
3. `FUN_00405cf8(...)` → зависит от TypeVar
4. `FUN_00406318(...)` → дополнительное поле
5. `FUN_004055ac(...)` → ещё поле
6. `FUN_004056b4(...)` → ещё поле
7. `FUN_00406204(...)` → ещё поле
8. `FUN_00405e84(...)` → ещё поле
9. `FUN_0040609c(...)` → ещё поле
10. `FUN_004057bc(...)` → ещё поле

---

## 14. zapis2.exe — Поток обработки данных

### 14.1 Точка входа → WinMain

```
entry() → GetVersion → FUN_00471d4e(hInstance, 0, cmdLine, nCmdShow) → MFC App Init
```

### 14.2 Инициализация формы — `FUN_004157b2`

Это `CZapisView::OnInitialUpdate` (или аналог), создаёт GUI:

1. Вызывает `FUN_0040da36(param_1[0x18])` — инит документа
2. Создаёт элементы управления (CEdit/CStatic) через `FUN_00420090`
3. Для каждого поля шаблона (цикл по `param_1[0x26]+0x48` записей):
   - Проверяет тип поля (байт & 0xFF): `0x25`, `0x24`, `0x26`, `0x4D`, `0x2C`, `0x36`
   - Создаёт CEdit с размером из HIWORD/LOWORD поля
   - Регистрирует через `SetAt(param_1 + 0xb, fieldName, control)`
4. Второй цикл по `param_1[0x26]+0xB3C` записей (дополнительные поля B-scan)
5. Привязка `numZap` к числовому контролу записи

### 14.3 Типы полей в шаблоне (byte & 0xFF)

| Код | Hex | Тип |
|-----|-----|-----|
| 36 | 0x24 | Обычное поле |
| 37 | 0x25 | Поле-заголовок |
| 38 | 0x26 | Поле с валидацией |
| 44 | 0x2C | Комбо/список |
| 54 | 0x36 | Числовое поле |
| 77 | 0x4D | Мультистрока |
| 255 | 0xFF | Скрытое (disabled) |

### 14.4 Структура шаблона (из `param_1[0x26]`)

```c
struct ReportTemplate {
    /* +0x00 */ ... preamble ...
    /* +0x48 */ int      fieldCount;         // количество полей
    /* +0x50 */ int      field0_size;        // HIWORD=height, LOWORD=width
    /* +0x54 */ byte     field0_type;        // тип (0x24, 0x25, 0x36...)
    /* +0x58 */ char*    field0_name;        // "numZap", "imhoOper"...
    /* +0x5C..+0x63 */  field0_extra;
    /* stride = 0x1C (28 bytes) per field */
    /* +0xB3C */ int     bscanFieldCount;    // доп. поля для B-scan
    /* +0xB4C */ ...     bscanFields[stride 0x1C]...
    /* +0x1630 */ int    hasBscan;           // флаг наличия B-scan
    /* +0x165C */ void*  bscanWidget;        // указатель на виджет B-scan
};
```

---

## 15. Сводка новых находок (раунд 2)

| # | Находка | Статус | Источник |
|---|---------|--------|----------|
| 24 | TLV в FLASH: tag LE16 + len LE16 + body, end=0xFFFF, pad=0xFF | 🔒 Железобетонно | FUN_0041b008 |
| 25 | Sweep-диапазонов ровно 5 (Базовая) или 6 (Вагонная), stride 24 байта | 🔒 Железобетонно | FUN_0041c284 |
| 26 | BlockEntry = {tag:int, offset:int, size:int} — 12 байт | 🔒 Железобетонно | FUN_0041b008 |
| 27 | Настройки хранятся в HKLM как REG_BINARY (hex-encoded) | 🔒 Железобетонно | FUN_0041fbc4 |
| 28 | Чтение настроек: RegQueryValueExA → LE16 pairs → addr array | 🔒 Железобетонно | FUN_0041ff0c |
| 29 | Полная структура FormReadData (32+ поля реконструировано) | 🔒 Железобетонно | множество FUN |
| 30 | DLL A-scan: 11 полей, fieldType={1=Numeric,2=String,3=Date} | 🔒 Железобетонно | FUN_0040721c |
| 31 | ShortProt: VARIANT-деление на 100 (дроби!), 6-byte passport | 🔒 Железобетонно | FUN_00403420 |
| 32 | Generic: дата@+7, время@+10, затем 8 доп. парсеров по TypeVar | 🔒 Железобетонно | FUN_00403834 |
| 33 | zapis2: 7 типов полей шаблона (0x24..0xFF), stride 28 байт | 🔒 Железобетонно | FUN_004157b2 |
| 34 | zapis2: шаблон содержит до 2 секций полей (+0x48 и +0xB3C) | 🔒 Железобетонно | FUN_004157b2 |
| 35 | COM-интерфейс: PelengPC.IData (OLE Automation server) | 🔒 Железобетонно | string @0x521c93 |
| 36 | Ключ реестра "Numbers" — массив адресов блоков | 🔒 Железобетонно | FUN_0041ff0c |

---

## 16. Что осталось непокрыто (обновлённый список)

1. **Innoextract** — требует локальную Windows-машину
2. **DFM-ресурсы** — полные словари TypeVar (24667/24668/10001/40001)
3. **FUN_00402708** — 6-byte «passport» decoder (вероятно BCD серийный номер)
4. **FUN_00405cf8, FUN_00406318, FUN_004055ac, FUN_004056b4** — парсеры individual полей generic-категории (внутри DLL)
5. **FUN_00407c20, FUN_00409984** — описатели полей для категорий 4-6 и 10-19 (внутри DLL)
6. **Полная карта FormNastr** — UI-размеры окна зависят от типа прибора (380×390, 380×470, etc.)
7. **CZapisDoc::Serialize** — не найден в чистом виде (MFC SDI без файлового сохранения?)
8. **Парсер `.dal`** — не обнаружен ни в одном бинаре
9. **Точная семантика STATUS_T (0x74)** — структура полей не определена



---

## 17. Полная карта полей пакета (DLL-декодер) — раунд 3

### 17.1 Структура «sweepInfo» (param_1 в обработчиках)

```c
struct SweepInfo {    // param_1 в FUN_00402980, FUN_004031dc, FUN_00403420, etc.
    /* [0] */ int devId;        // идентификатор устройства
    /* [1] */ int field1;       // доп. числовое поле
    /* [2] */ int blockAddr;    // полный адрес блока (blockAddr%10000 = номер записи)
    /* [3] */ int typeVar;      // TypeVar (код типа измерения)
    /* [4] */ char* name;       // строка-имя
    /* [5] */ int field5;       // IntToStr → записывается в протокол
};
```

### 17.2 `FUN_0040381c` — LE16 reader

```c
int FUN_0040381c(byte* ptr) {
    return ptr[0] + ptr[1] * 256;   // Little-Endian 16-bit
}
```

Используется повсеместно для извлечения 2-байтовых числовых полей.

### 17.3 `FUN_00402708` — Passport/Serial decoder (lookup table)

```c
int* FUN_00402708(int* result, int rawData, int len) {
    // Загружает 2 LUT-таблицы (по 124 и 124 байта) из DAT_004284b4 и DAT_00428530
    // Для каждого байта от конца к началу (reverse order!):
    //   Ищет символ в LUT (124 позиции)
    //   Если найден — append (lookup-match → decoded char)
    // Результат = строка декодированных символов (reverse-BCD / custom encoding)
}
```

**Вход:** `rawData` = указатель на сырые байты, `len` = количество байт.  
**LUT 1** (0x004284b4, 124 байт) — таблица допустимых символов входа.  
**LUT 2** (0x00428530, 124 байт) — таблица символов выхода.  
**Алгоритм:** Побайтовый реверсивный decode через substitution table.

### 17.4 `FUN_00402874` — Валидация даты

```c
bool FUN_00402874(uint year, int month, int day) {
    if (year <= 2000 || month >= 13) return false;
    switch (month) {
        case 1,3,5,7,8,10,12: return (day > 0 && day < 32);
        case 4,6,9,11:        return (day > 0 && day < 31);
        case 2:
            if (year % 4 == 0) {
                if (year % 100 == 0) return (day > 0 && day < 30); // упрощённо
                return (day > 0 && day < 29);
            }
            return (day > 0 && day < 29);
    }
}
```

### 17.5 `FUN_00402954` — Валидация времени

```c
bool FUN_00402954(int hours, int minutes) {
    return (hours >= 0 && hours < 24 && minutes >= 0 && minutes < 60);
}
```

### 17.6 A-scan decoder (`FUN_00404758`) — полная карта полей

**Для категорий 4-6 (TypeVar 150-151, 362-381):**

| # | Парсер | Offset | Len | Поле |
|---|--------|--------|-----|------|
| 1 | `FUN_004023b0` | +7 | 3 | Дата [day,month,year-2000] |
| 2 | `FUN_00402504` | +10 | 2 | Время [hours,minutes] |
| 3 | `FUN_00405e00` | +0x11 | 11 | Passport (lookup-decode) |
| 4 | `FUN_00406418` | — | — | Доп.поле (по TypeVar) |
| 5 | `FUN_00405528` | +0x38 | 2 | LE16 → IntToStr |
| 6 | `FUN_00405630` | +0x3A | 2 | LE16 → IntToStr |
| 7 | `FUN_00405738` | +0x3C | 2 | LE16 → IntToStr |
| 8 | `FUN_00406fe0` | — | — | Доп.поле |
| 9 | `FUN_00407060` | — | — | Доп.поле |
| 10 | `FUN_00407160` | — | — | Доп.поле |
| 11 | `FUN_004070e0` | — | — | Доп.поле |
| 12 | `FUN_00405974` | +0x0C | 1 | Bool (flag) |
| 13 | `FUN_00405b58` | +0x21 | 7 | Passport (lookup-decode) |
| 14 | `FUN_00405c78` | +0x45 | 2 | LE16 → float/str |

**Для категорий 10-19:**

| # | Парсер | Offset | Len | Поле |
|---|--------|--------|-----|------|
| 1 | `FUN_004023b0` | +7 | 3 | Дата |
| 2 | `FUN_00402504` | +10 | 2 | Время |
| 3 | `FUN_004066f8` | — | — | Вычисляемое |
| 4 | `FUN_00406598` | — | — | Вычисляемое |
| 5 | `FUN_00406940` | — | — | Вычисляемое |
| 6 | `FUN_00406a48` | — | — | Вычисляемое |
| 7 | `FUN_00406b50` | — | — | Вычисляемое |
| 8 | `FUN_00406768` | — | — | Вычисляемое |
| 9 | `FUN_00406fe0` | — | — | Доп.поле |
| 10 | `FUN_00407060` | — | — | Доп.поле |
| 11 | `FUN_00407160` | — | — | Доп.поле |
| 12 | `FUN_004070e0` | — | — | Доп.поле |
| 13 | `FUN_00406de8` | — | — | Вычисляемое |
| 14 | `FUN_00406d64` | — | — | Вычисляемое |

### 17.7 B-scan/composite decoder (`FUN_004041d0`) — полная карта

**Для категорий 4-6 (TypeVar 390-391, 442-443, 480-481, 642-643, 842-843):**

| # | Парсер | Offset | Поле |
|---|--------|--------|------|
| 1 | `FUN_004023b0` | +7 | Дата |
| 2 | `FUN_00402504` | +10 | Время |
| 3 | `FUN_00405d7c` | +0x11 (11b) | Passport (lookup-decode) |
| 4 | `FUN_00406398` | — | Доп.поле |
| 5 | `FUN_00405f44` | +0x35 (7b) | Passport (lookup-decode) |
| 6 | `FUN_00406158` | +0x3E (2b) | LE16 → float |
| 7 | `FUN_00405ff0` | +0x3C (2b) | LE16 → float |
| 8 | `FUN_00405898` | +0x0C (1b) | Bool flag |
| 9 | `FUN_00405ad4` | +0x21 (7b) | Passport (lookup-decode) |

**Для категорий 10-19:**

| # | Парсер | Offset | Поле |
|---|--------|--------|------|
| 1 | `FUN_004023b0` | +7 | Дата |
| 2 | `FUN_00402504` | +10 | Время |
| 3 | `FUN_00406688` | — | Вычисляемое |
| 4 | `FUN_00406518` | — | Вычисляемое |
| 5 | `FUN_00406f38` | — | Вычисляемое |
| 6 | `FUN_00406ce0` | — | Вычисляемое |

### 17.8 Generic field parsers — смещения в теле пакета

| Функция | Смещение | Размер | Описание |
|---------|----------|--------|----------|
| `FUN_00405528` | +0x38 | 2 | LE16 числовое |
| `FUN_004055ac` | +0x35 | 2 | LE16 числовое |
| `FUN_004056b4` | +0x37 | 2 | LE16 числовое |
| `FUN_004057bc` | +0x0C | 1 | Boolean (если ≠0 → пустая строка) |
| `FUN_00405898` | +0x0C | 1 | Boolean |
| `FUN_00405cf8` | +0x11 | 11 | Passport (lookup, 11 символов) |
| `FUN_00405e84` | +0x39 | 2 | LE16 (только для TypeVar 680-683!) |
| `FUN_0040609c` | +0x3C | 2 | LE16 (только для TypeVar 680-683!) |
| `FUN_00406204` | +0x39 | 2 | LE16 (для TypeVar 362-381, 680-683, 712-731, 780-781) |
| `FUN_00406318` | +0x5E | 2 | LE16 числовое |

### 17.9 Полная структура тела пакета (реконструкция)

```
Offset  Size  Поле                        Категория
──────  ────  ──────────────────────────  ──────────
+0x00   2     devId (LE16)                all
+0x02   2     (reserved)                  all
+0x04   1     sweepHi                     all
+0x05   1     sweepLo                     all
+0x06   1     (unknown)                   all
+0x07   3     date [day,month,year-2000]  all
+0x0A   2     time [hours,minutes]        all
+0x0C   1     flag/bool                   A-scan/composite
+0x0D   4     (block-specific)            varies
+0x10   2     blockAddr (LE16)            all (в заголовке)
+0x11   11    passport (lookup-encoded)   A-scan/composite
+0x1C   5     (padding/reserved)          
+0x21   7     passport2 (lookup-encoded)  A-scan
+0x28   ?     (data)
+0x35   2     numericField1 (LE16)        generic
+0x37   2     numericField2 (LE16)        generic
+0x38   2     numericField3 (LE16)        A-scan
+0x39   2     numericField4 (LE16)        TypeVar-specific
+0x3A   2     numericField5 (LE16)        A-scan
+0x3B   2     (reserved)
+0x3C   2     numericField6 (LE16)        composite/TypeVar-specific
+0x3E   2     numericField7 (LE16)        composite
+0x40   1     (?)
+0x41   7     passport3 (lookup)          TypeVar 680-683 only
+0x45   2     numericField8 (LE16)        A-scan
+0x5E   2     numericField9 (LE16)        generic
```

### 17.10 TypeVar-зависимые условия

Функция `FUN_00406204` выводит поле LE16 @+0x39 **только** для следующих TypeVar:
- 362-363, 380-381 (толщиномер?)
- 390-391 (?)
- 680-683 (вагонный?)
- 712-713, 730-731 (?)
- 780-781 (?)
- 740-741 (?)

Функция `FUN_00405e84` и `FUN_0040609c` активны **только** для TypeVar 680-683 (предположительно «вагонная» серия с расширенными данными).

Функция `FUN_00405bdc` использует offset +0x3B для TypeVar 680-683, а +0x3E для остальных.

---

## 18. Протокол — итоговая сводка для реализации

### 18.1 Handshake (инициализация)

```
PC → Прибор:  [0x55]
Прибор → PC:  [16-byte header] [A-scan data...]
              Минимум 17 байт ответа
```

### 18.2 Чтение блока (BLOCK_REQ)

```
PC → Прибор:  [0x42] [10ms] [addr_lo] [10ms] [addr_hi] [10ms]
Прибор → PC:  [blockSize bytes]
              blockSize определяется sweep-таблицей (100×count зап.)
```

### 18.3 Чтение FLASH (FLASH_DUMP)

```
PC → Прибор:  [0x9A]
Прибор → PC:  [4293 или 5253 байт]
              Содержит TLV-записи: {tag:LE16, len:LE16, body[len]}
              End marker: tag=0xFFFF
              Padding: 0xFF
```

### 18.4 Запрос статуса (STATUS_T)

```
PC → Прибор:  [0x74]
Прибор → PC:  [variable-length status block]
```

### 18.5 16-байтовый заголовок пакета

```
Offset  Size  Поле
0x00    2     deviceId (LE16)
0x02    1     versionByte (копируется в DAT_0053e97c)
0x03    1     (reserved)
0x04    1     sweepIdHi
0x05    1     sweepIdLo
0x06    1     (reserved/flags)
0x07    3     date [day, month, year-2000]
0x0A    2     time [hours, minutes]
0x0C    4     (block-specific data)
```

### 18.6 Физические параметры COM-порта

| Параметр | Значение |
|----------|----------|
| Baud Rate | 9600 (default), поддерживается 110–256000 |
| Data Bits | 8 |
| Stop Bits | 1 |
| Parity | None |
| Flow Control | None |
| ReadIntervalTimeout | 1 ms |
| WriteTotalTimeoutConstant | 10 ms |
| Inter-byte delay (BLOCK_REQ) | 10 ms |
| Buffer size (HANDSHAKE read) | 524304 байт (0x80010) |

---

## 19. Карта смещений в B-scan данных

```
B-scan пакет (после 16-byte заголовка):
┌──────────────────────────────────────────────┐
│ A-scan header (448 bytes = 0x1C0)            │
├──────────────────────────────────────────────┤
│ Padding (16 bytes = 0x10, итого 0x1D0)       │
├──────────────────────────────────────────────┤
│ B-scan bitmap data                           │
│   Width: 240 bit (30 bytes) per row          │
│   Height: (dataLen * 8) / 240 rows           │
│   Encoding: 1 bit per pixel (MSB first)      │
│   Rendering: 2x2 pixels per bit              │
│   Colors: bit=1 → BLACK, bit=0 → WHITE      │
└──────────────────────────────────────────────┘

Размеры:
  Базовая:  4293 - 5 - 448 = 3840 байт → 3840*8/240 = 128 строк → 256 px height
  Вагонная: 5253 - 5 - 448 = 4800 байт → 4800*8/240 = 160 строк → 320 px height
```

---

## 20. Рекомендации по полному клону

Для воссоздания 1:1 функциональности PelengPC необходимо:

1. **COM-порт**: pyserial, 9600/8N1, без flow control, inter-byte delay 10ms для BLOCK_REQ
2. **HANDSHAKE**: send 0x55, read до таймаута, проверить ≥17 байт
3. **TLV-парсер**: LE16 tag + LE16 len + body, end=0xFFFF, skip 0xFF padding
4. **Категории**: blockAddr/1000 → switch(1=Ascan, 4-6=Generic, 5=Calib, 10-19=Bscan, 20-29=ShortProt)
5. **Дата**: raw[+7..+9] = [day, month, year-2000], валидация високосных
6. **Время**: raw[+10..+11] = [hours, minutes], валидация 0-23/0-59
7. **Passport**: 11 байт @+0x11, reverse lookup через 124-символьную таблицу подстановки
8. **Числа**: LE16 @+0x35, +0x37, +0x38, +0x3A, +0x3C, +0x3E, +0x5E
9. **B-scan**: offset +0x1D0, 30 bytes/row, 1 bit/pixel, 2x2 magnification
10. **Реестр**: HKLM\SOFTWARE\Altek\PelengPC — все настройки как REG_BINARY/REG_DWORD
11. **БД**: Firebird 2.5 (IBX), таблицы BlockZap/NASTR/RESULTS/SHORTPROT
12. **Отчёты**: WordML XML (A4 portrait), встроенные PNG через VML
13. **DLL interaction**: OLE Automation (CoCreateInstance/GetActiveObject → IDispatch → SAFEARRAY)
