# Декомпиляция PelengPC_ver1_2.exe

**Дата:** 2026-05-12  
**Инструменты:** objdump, Python (lzma, struct), ручной анализ PE  
**Среда:** Linux sandbox (без Ghidra/IDA — только статический анализ)

---

## 1. Общая информация

| Параметр | Значение |
|----------|----------|
| **Тип файла** | PE32 executable (GUI) Intel 80386, for MS Windows |
| **Размер** | 6,583,064 байт (6.3 МБ) |
| **Формат** | Inno Setup 5.4.2 installer |
| **Содержимое** | Самораспаковывающийся установщик с LZMA-сжатием |
| **Размер payload (распакован)** | 15,327,351 байт (14.6 МБ) |

---

## 2. Структура инсталлятора

### 2.1 PE-заголовок (launcher)

```
Секции:
  CODE     0x9364   @ 0x401000  (код загрузчика Inno Setup)
  DATA     0x024C   @ 0x40B000  (данные: "Inno Setup Setup Data (5.4.2)")
  .idata   0x0950   @ 0x40D000  (импорт)
  .rsrc    0x2C00   @ 0x411000  (ресурсы, иконка)

Entry point: 0x00409C40
```

### 2.2 Импорты загрузчика

- **kernel32.dll** — файловые операции, память, процессы
- **user32.dll** — MessageBoxA, окна
- **oleaut32.dll** — VARIANT-операции
- **advapi32.dll** — реестр, привилегии (SeShutdownPrivilege)
- **comctl32.dll** — InitCommonControls

### 2.3 LZMA-данные

Сжатый payload начинается по смещению **0xD404** (после маркера `zlb\x1a`):
- LZMA props: `5D 00 00 80 00` (lc=3, lp=0, pb=2, dict=8MB)
- Распакованный размер: **15,327,351 байт**

---

## 3. Содержимое инсталлятора (19 PE-файлов)

| # | Offset | SizeOfImage | Идентификация |
|---|--------|-------------|---------------|
| 0 | 0x2472D | 2000 KB | DLL (вспомогательная) |
| 1 | 0x20C72D | 24 KB | DLL (Firebird-related) |
| 2 | 0x20DD2D | 1088 KB | DLL (большая, возможно fbembed.dll) |
| 3 | 0x31DD2D | 196 KB | DLL |
| 4 | 0x34ED2D | 536 KB | DLL |
| 5 | 0x3DAC82 | 492 KB | DLL |
| 6 | 0x454C82 | 344 KB | DLL |
| 7 | 0x557C82 | 24 KB | DLL (Firebird-related) |
| 8 | 0x55BA43 | 668 KB | DLL |
| 9 | 0x602A43 | 232 KB | **dbdll.dll** (102_203DLL.dll) — содержит _SortBufData, _Form_View |
| **10** | **0x636E43** | **2116 KB** | **PelengPC.exe** — основное приложение (VCL, Borland C++Builder 5) |
| 11 | 0x7D8505 | 1208 KB | **zapis2.exe** — генератор отчётов (MFC, CZapisDoc/CZapisView) |
| 12 | 0x902757 | 40 KB | DLL |
| 13 | 0x909557 | 120 KB | DLL |
| 14 | 0xA413E9 | 120 KB | DLL |
| 15 | 0xB00012 | 284 KB | DLL |
| 16 | 0xC9F077 | 808 KB | Вспомогательный EXE (VCL, Delphi-like, TDefVersionType) |
| 17 | 0xD5CA77 | 1212 KB | DLL |
| 18 | 0xE89E77 | 88 KB | Малый PE |

### 3.1 Файлы, упоминаемые в установщике

**Исполняемые:**
- `PelengPC.exe` — основная программа
- `zapis2.exe` — генератор отчётов WordML
- `SetUp_PelengPC.exe` — дополнительный установщик
- `DotNetInstaller.exe` — .NET prerequisite installer

**DLL:**
- `dbdll.dll` (102_203DLL.dll) — TLV-парсер данных от прибора
- `fbembed.dll` — Firebird Embedded DB engine
- `gds32.dll` — InterBase/Firebird client
- `ib_util.dll`, `fbintl.dll`, `fbudf.dll` — Firebird utilities
- `icudt30.dll`, `icuin30.dll`, `icuuc30.dll` — ICU (Unicode)
- `MSVCP71.dll`, `MSVCR71.dll` — MS Visual C++ runtime

**Базы данных:**
- `PelengPC.fdb` — основная БД Firebird
- `database.fdb` — дополнительная БД
- `security.fdb`, `security2.fdb` — безопасность Firebird
- `PelengPCtest.dal` — тестовый файл данных

**Конфигурация:**
- `firebird.conf` — конфиг Firebird
- `aliases.conf` — алиасы БД
- `firebird.msg` — сообщения

---


## 4. PelengPC.exe — Декомпиляция основного приложения

### 4.1 Метаданные

| Параметр | Значение |
|----------|----------|
| **Компилятор** | Borland C++Builder 5 (VCL) |
| **Image Base** | 0x00400000 |
| **Entry Point** | 0x00401000 |
| **Размер .text** | 0x11FA00 (1.14 МБ кода) |
| **Экспортов** | 1283 (C++ mangled names) |
| **Путь сборки** | `D:\PROGRAM FILES\BORLAND\CBUILDER5\...` |
| **Исходники** | `C:\program\union\pelengpc_fb_no_password\fb_test\` |
| **Версия** | PelengPC Version 1.1 |
| **COM ID** | `PelengPC.IData` (OLE Automation Server) |
| **Help файл** | `PelengPC.hlp`, `PelengPCHelp.hlp` |

### 4.2 Импорты

| DLL | Функции |
|-----|---------|
| KERNEL32.DLL | CreateFileA, ClearCommError, GetCommState, SetCommState, GetCommTimeouts, SetCommTimeouts, WriteFile, ReadFile, CloseHandle, Sleep, GetTickCount, CreateThread, CreateMutexA, VirtualAlloc/Free, LoadLibraryA, GetProcAddress... |
| USER32.DLL | CreateWindowExA, SetWindowLongA, PeekMessageA, TranslateMessage, DispatchMessageA, MessageBoxA, LoadStringA... |
| GDI32.DLL | CreateDC, CreateBitmap, BitBlt, SetPixel, SelectObject... |
| ADVAPI32.DLL | RegOpenKeyExA, RegQueryValueExA, RegSetValueExA, RegCreateKeyA/ExA, RegCloseKey, RegDeleteKeyA, RegEnumKeyExA |
| OLE32.DLL | CoInitialize, CoCreateInstance, GetActiveObject... |
| OLEAUT32.DLL | SafeArrayCreateVector, SafeArrayAccessData, SysAllocString, VariantInit... |
| SHELL32.DLL | ShellExecuteA, SHGetPathFromIDList... |
| SETUPAPI.DLL | SetupDiGetClassDevsA, SetupDiEnumDeviceInfo, SetupDiOpenDevRegKey, SetupDiDestroyDeviceInfoList |
| COMCTL32.DLL | InitCommonControlsEx |
| COMDLG32.DLL | GetOpenFileNameA, GetSaveFileNameA |
| VERSION.DLL | GetFileVersionInfoA, VerQueryValueA |

### 4.3 Секции PE

```
.text   0x11FA00 @ 0x401000  — код (1,175 KB)
.data   0x01AA00 @ 0x521000  — данные (106 KB)
.tls    0x000200 @ 0x5C3000  — Thread Local Storage
.rdata  0x000200 @ 0x5C4000  — readonly data
.idata  0x003200 @ 0x5C5000  — imports
.edata  0x010600 @ 0x5C9000  — exports (1283 entries)
.rsrc   0x021A00 @ 0x5DA000  — resources (DFM forms, icons)
.reloc  0x014E00 @ 0x5FC000  — relocations
```

---

## 5. Формы (VCL DFM Resources)

### 5.1 Список форм (12 штук)

| Класс | Имя объекта | Назначение |
|-------|-------------|------------|
| **TFormMain** | FormMain | Главное окно: DBGrid, toolbar, main menu |
| **TFormNastr** | FormNastr | Настройки (COM порт, baud, операторы) |
| **TFormPassWord** | FormPassWord | Диалог пароля |
| **TFormReadData** | FormReadData | Чтение данных из прибора |
| **TFormScreen** | FormScreen | Визуализация B-scan |
| **TFormProgress** | FormProgress | Прогресс-бар приёма |
| **TFormConfirm** | FormConfirm | Диалог подтверждения |
| **TFormRaport** | FormRaport | Просмотр/генерация рапортов |
| **TDriveForm** | DriveForm | Выбор диска/каталога |
| **TFCreate** | FCreate | Создание БД |
| **TKForm** | KForm | Вспомогательная форма |
| **TLoginDialog** | LoginDialog | Авторизация в БД |
| **TPasswordDialog** | PasswordDialog | Пароль БД |

### 5.2 FormMain — Структура главного окна

**Компоненты интерфейса:**
```
TFormMain
├── TStatusBar (StatusBar)
├── TPanel (PanelMain)
│   ├── TDBGrid (таблица BlockZap)
│   └── TPanel (PanelHead)
│       ├── TLabel (LabelType)
│       └── TLabel (Label1)
├── TToolBar (ToolBar)
│   ├── ToolButtonRaport      → ToolButtonRaportClick
│   ├── ToolButtonReadData    → NReadDataClick
│   ├── ToolButtonFromFile    → ToolButtonFromFileClick
│   ├── ToolButtonSortir      → NSortirClick
│   ├── ToolButtonVibor       → NViborClick
│   ├── ToolButtonSearch      → NSearchClick
│   ├── ToolButtonReadZap     → NReadZapClick
│   ├── ToolButtonReadOtchet  → NReadDocClick
│   ├── ToolButtonDelZap      → NDelZapClick
│   ├── ToolButtonOpera       → NOperaClick
│   ├── ToolButtonSave        → NSaveConfigClick
│   └── ToolButtonHelp        → NHelpProgClick
├── TMainMenu (MainMenu)
│   ├── [Файл]
│   │   ├── NReadData (Чтение) → NReadDataClick
│   │   ├── NFromFile → ToolButtonFromFileClick
│   │   ├── NReadZap → NReadZapClick
│   │   ├── NReadDoc → NReadDocClick
│   │   └── [Выход] → NExitClick
│   ├── NTable [Таблица]
│   │   ├── [Сортировка] → NSortirClick
│   │   ├── [Выборка] → NViborClick
│   │   ├── [Поиск] → NSearchClick
│   │   ├── [Выбрать все] → NSelectAllClick
│   │   ├── NResetTable → NResetTableClick
│   │   ├── NSaveExcel → NSaveExcelClick
│   │   ├── [Предпросмотр] → NPreViewClick
│   │   └── NPrintTable → NPrintTableClick
│   ├── [Настройки]
│   │   ├── [Операторы] → NOperaClick
│   │   ├── [Фирма] → NFirmaClick
│   │   ├── NCOMPort ("COM &Port") → NCOMPortClick
│   │   ├── NTestPort → NTestPortClick
│   │   ├── NSaveConfig → NSaveConfigClick
│   │   └── [Устройство] → NDeviceClick
│   └── [Помощь]
│       ├── [Справка] → NHelpProgClick
│       ├── [О программе] → NAboutClick
│       ├── [Номера] → NNumbersClick
│       ├── [Пароль] → NPassWordClick
│       └── [Автор] → NHelpAutorClick
├── TIBDatabase (IBDatabase1)
├── TIBTransaction (IBTransactionR) — чтение
├── TIBTransaction (IBTransactionW) — запись
├── TIBDataSet (IBDataSetR) — SELECT * FROM BlockZap
├── TIBDataSet (IBDataSetW) — UPDATE BlockZap
├── TIBDataSet (IBDataSetS) — вспомогательный
├── TDataSource (DataSource1) → IBDataSetR
├── TOpenDialog (OpenDialog)
└── TSaveDialog (SaveDialog)
```

### 5.3 SQL-запросы (из DFM и .data)

```sql
-- Чтение
SELECT * FROM BlockZap
SELECT * FROM BlockZap WHERE Number=:Number
SELECT * FROM BlockZap WHERE Number=:NumZap

-- Запись
UPDATE BlockZap SET Number=:Number, BlockLen=:BlockLen, Block=:Block 
  WHERE Number=:OLD_Number
INSERT INTO ...

-- Удаление
DELETE FROM BlockZap WHERE Number=...

-- Сортировка
SELECT * FROM ... ORDER BY Number
```

**Таблица `BlockZap`:**
| Поле | Тип | Описание |
|------|-----|----------|
| Number | INTEGER | Номер записи (PK) |
| BlockLen | INTEGER | Длина блока данных |
| Block | BLOB | Сырые данные от прибора |

### 5.4 FormNastr — Компоненты настроек

```
TFormNastr
├── TabSheetPort
│   ├── GroupBoxPort
│   │   └── ComboBoxPort      — выбор COM-порта
│   ├── GroupBoxSubPort
│   │   └── ComboBoxBaud      — выбор скорости
│   └── ButtonPortView        → ButtonPortViewClick
├── PanelOperTable             — таблица операторов
│   ├── SpeedButtonAdd        → SpeedButtonAddClick
│   ├── SpeedButtonDel        → SpeedButtonDelClick
│   ├── SpeedButtonUp         → SpeedButtonUpClick
│   ├── SpeedButtonDown       → SpeedButtonDownClick
│   └── SpeedButtonClear      → SpeedButtonClearClick
└── Events: FormShow, FormClose
```

### 5.5 FormScreen — B-scan визуализация

Из SaveDialog в FormScreen:
```
Filter = "PLG Files|*.plg|FLASH Files|*.fla"
```

---


## 6. COM-порт — Полная реконструкция

### 6.1 Класс TCOMPort

Обнаружен по RTTI: `TCOMPort *` (строка @ 0x02304D в .text)

**Структура объекта (реконструирована из инициализации @ 0x4239C8):**

```c
struct TCOMPort {
    /* 0x00 */ HANDLE  hFile;       // INVALID_HANDLE_VALUE (-1) при создании
    /* 0x04 */ char*   portName;    // NULL при создании (AnsiString)
    /* 0x08 */ BYTE    baudIndex;   // 6 = default (9600)
    /* 0x0C */ DWORD   baudRate;    // 0x2580 = 9600 default
    /* 0x10 */ BYTE    dataBits;    // 3 → ByteSize = dataBits + 5 = 8
    /* 0x11 */ BYTE    stopBits;    // 0 = 1 stop bit
    /* 0x12 */ BYTE    parity;      // 0 = no parity
    /* 0x14 */ DWORD   timeout;     // 0x3E8 = 1000 ms
};
// Итого: ~24 байта
```

**Конфигурация по умолчанию: 9600 baud, 8N1, timeout=1000ms**

### 6.2 Таблица Baud Rate (функция @ 0x423B4C)

Switch-case устанавливает `baudIndex` в зависимости от `baudRate`:

| baudRate (hex) | baudRate (dec) | baudIndex |
|----------------|----------------|-----------|
| 0x6E | 110 | 0 |
| 0x12C | 300 | 1 |
| 0x258 | 600 | 2 |
| 0x4B0 | 1200 | 3 |
| 0x960 | 2400 | 4 |
| 0x12C0 | 4800 | 5 |
| **0x2580** | **9600** | **6 (DEFAULT)** |
| 0x3840 | 14400 | 7 |
| 0x4B00 | 19200 | 8 |
| 0x9600 | 38400 | 9 |
| 0xDAC0 | 56000 | 10 |
| 0xE100 | 57600 | 11 |
| 0x1C200 | 115200 | 12 |
| 0x1F400 | 128000 | 13 |
| 0x3E800 | 256000 | 14 |

### 6.3 Открытие порта

```c
// Формирование имени порта: "\\.\COMn"
CreateFileA("\\\\.\\COMn", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

// Установка таймаутов:
COMMTIMEOUTS timeouts;
timeouts.ReadIntervalTimeout        = 1;
timeouts.ReadTotalTimeoutMultiplier = 0;
timeouts.ReadTotalTimeoutConstant   = 1;
timeouts.WriteTotalTimeoutMultiplier= 0;
timeouts.WriteTotalTimeoutConstant  = 10;
SetCommTimeouts(hFile, &timeouts);

Sleep(100);  // пауза после открытия
```

### 6.4 Применение настроек

```c
DCB dcb;
GetCommState(hFile, &dcb);
dcb.BaudRate  = port->baudRate;    // из поля 0x0C
dcb.Parity    = port->parity;      // 0 = NOPARITY
dcb.ByteSize  = port->dataBits + 5; // 3+5 = 8
dcb.StopBits  = port->stopBits;    // 0 = ONESTOPBIT
dcb.fOutxCtsFlow = 0;              // нет hardware flow control
dcb.fOutxDsrFlow = 0;
SetCommState(hFile, &dcb);
```

**Итог: 8N1, без аппаратного управления потоком.**

### 6.5 Функции ввода-вывода

| Адрес | Функция | Описание |
|-------|---------|----------|
| OpenPort | `CreateFileA` + timeouts + `Sleep(100)` |
| ClosePort | `CloseHandle` + handle = -1 |
| WriteByte | `WriteFile(hFile, &byte, 1, &written, NULL)` |
| WriteBytes | `WriteFile` в цикле с проверкой `GetTickCount` timeout |
| ReadBytes | `ReadFile` в цикле с timeout |
| BytesAvail | `ClearCommError` → `COMSTAT.cbInQue` |
| Delay | `GetTickCount` busy-wait loop |

### 6.6 Обнаружение COM-портов (через SetupAPI)

Используется `SETUPAPI.DLL`:
```c
SetupDiGetClassDevsA(&GUID_DEVCLASS_PORTS, NULL, NULL, DIGCF_PRESENT);
// Перебор устройств:
SetupDiEnumDeviceInfo(hDevInfo, index, &devInfoData);
SetupDiOpenDevRegKey(hDevInfo, &devInfoData, DICS_FLAG_GLOBAL, 0, DIREG_DEV, KEY_READ);
// Чтение "PortName" из реестра устройства
RegQueryValueExA(hKey, "PortName", ...);
```

---

## 7. Протокол связи с прибором

### 7.1 Опкод 0x55 — HANDSHAKE

```c
byte cmd = 0x55;
WriteByte(port, &cmd);                    // Отправка 0x55
int received = ReadBytes(port, buf, 0x80010);  // Приём (до 512KB+16)
if (received < 0x11) { ERROR; }           // Минимум 17 байт
// buf[0..0x0F] = 16-байтовый заголовок → копируется в глобальные
// buf[2] = флаги версии прибора
// Количество элементов A-scan = (received - 0x10) >> 1 - 1
```

### 7.2 Опкод 0x42 — BLOCK_REQ (запрос блока)

```c
byte cmd = 0x42;
WriteByte(port, &cmd);      // команда
Delay(port, 10);            // 10 мс пауза
byte addr_lo = (byte)(blockAddr & 0xFF);
WriteByte(port, &addr_lo);  // младший байт адреса
Delay(port, 10);
byte addr_hi = (byte)(blockAddr >> 8);
WriteByte(port, &addr_hi);  // старший байт адреса  
Delay(port, 10);
// Ожидание ответа: FormProgress отображает прогресс
```

**Протокол: `[0x42] [10ms] [addr_lo] [10ms] [addr_hi] [10ms] → ответ`**

### 7.3 Опкод 0x9A — FLASH_DUMP

```c
byte cmd = 0x9A;
WriteByte(port, &cmd);
// Приём фиксированного блока:
//   Базовая версия: 4293 байт (0x10C5)
//   Вагонная версия: 5253 байт (0x1485)
```

### 7.4 Опкод 0x74 — STATUS

Запрашивает текущее состояние прибора (температура, батарея, настройки).

### 7.5 Сообщения об ошибках

```
"ERROR !!! TimeOut waiting for response."   @ 0x52F7B3
"Settings-OK!"                              @ 0x52F8C4  (?)
"Settings-BAD! DisConnect!"                 @ 0x52F8D2  (?)
```

---

## 8. База данных

### 8.1 Firebird Embedded

- **Движок:** fbembed.dll (Firebird 2.x Embedded)
- **Основная БД:** `PelengPC.fdb`
- **Путь по умолчанию:** `C:\DataBase`
- **Зеркало:** `D:\Mirror`
- **IBX компоненты:** TIBDatabase, TIBTransaction, TIBDataSet

### 8.2 IBX-транзакции в FormMain

```
IBDatabase1        — соединение с PelengPC.fdb
IBTransactionR     — транзакция чтения (DefaultDatabase = IBDatabase1)
IBTransactionW     — транзакция записи (DefaultDatabase = IBDatabase1)
IBDataSetR         — SELECT * FROM BlockZap (readonly)
IBDataSetW         — UPDATE/INSERT (для записи)
IBDataSetS         — вспомогательный (сортировка/фильтр)
DataSource1        → IBDataSetR (для DBGrid)
```

### 8.3 Реестр

| Ключ | Содержание |
|------|-----------|
| `SOFTWARE\Altek\PelengPC` | Основные настройки (Firma, SubFirma, Label, TekTable, Numbers) |
| `SOFTWARE\Altek\PelengPC\Versions` | Список версий (Count, Version) |
| `SOFTWARE\Altek\PelengPC\Versions\<Name>` | DllFile, CountTables, Table1..N |
| `SOFTWARE\Altek\Operators` | Operator1..N (список операторов) |

### 8.4 BDE (Legacy)

Обнаружены строки BDE (Borland Database Engine):
```
Software\Borland\Database Engine
DLLPATH, DLLINITFUNCTION, DLLEXITFUNCTION
IDAPI32.DLL → DbiInitFn, DbiExit
```

**Вывод:** Поддерживаются два движка — IBX/Firebird (актуальный) и BDE (legacy).

---

## 9. Динамическая загрузка dbdll.dll

### 9.1 Загрузка

```c
HMODULE hModule = LoadLibraryA("dbdll.dll");

// Глобальные указатели:
typedef int* (__cdecl *pfn_SortBufData)(byte* rawPacket, uint* pSize);
typedef void (__cdecl *pfn_Form_View)(param1, byte* data, ULONG size);
typedef void (__cdecl *pfn_FreeBuffer)(int allocatedPtr);

pfn_SortBufData pSortBufData = (pfn_SortBufData)GetProcAddress(hModule, "_SortBufData");
pfn_FreeBuffer  pFreeBuffer  = (pfn_FreeBuffer)GetProcAddress(hModule, "_FreeBuffer");
pfn_Form_View   pForm_View   = (pfn_Form_View)GetProcAddress(hModule, "_Form_View");
```

Строка `"_Form_View"` найдена @ 0x52466F в .data.
Строка `"_SortBufData"` найдена @ 0x5243AD.
Строка `"_FreeBuffer"` найдена @ 0x5244B3.

### 9.2 Использование

- `_SortBufData` вызывается при получении блока данных от прибора для его разбора по полям
- `_FreeBuffer` освобождает буфер, аллоцированный в `_SortBufData`
- `_Form_View` передаёт данные через OLE Automation обратно в PelengPC

---

## 10. Адресация блоков

### 10.1 Sweep-адреса

Диапазон **10000–29999** — sweep-данные (подтверждено из функции IsSweepAddress).

### 10.2 Вспомогательные функции

```c
int GetBaseAddr(int addr) { return (addr / 100) * 100; }
int GetSubBlock(int addr) { return (addr % 10000) / 100; }
int GetIndex(int addr)    { return addr % 100; }
```

### 10.3 Специальный адрес 60000

Адрес `60000` = запрос полного FLASH-дампа (опкод 0x9A вместо 0x42).

---

## 11. B-scan визуализация (FormScreen)

### 11.1 Формат данных

```
Пакет от прибора:
[0x00..0x0F]   — 16-байт заголовок фрейма
[0x10..0x1CF]  — заголовок A-scan (448 байт, 0x1C0)
[0x1D0..]      — тело B-scan (битовая карта)
```

### 11.2 Параметры рендеринга

- **Ширина строки:** 30 байт = 240 бит
- **1 бит = 1 пиксель** (чёрный/белый)
- **Масштаб:** 2×2 пикселя на экране
- **Ширина изображения:** 480 px (240 × 2)
- **Высота:** зависит от размера данных

### 11.3 Размеры пакетов

| Версия | Полный пакет | Тело B-scan |
|--------|-------------|-------------|
| Базовая | 4293 байт | 4288 байт (4293-5) |
| Вагонная | 5253 байт | 5248 байт (5253-5) |

### 11.4 Сохранение

Поддерживаемые форматы:
- **PLG Files** (`*.plg`) — собственный формат
- **FLASH Files** (`*.fla`) — дамп flash-памяти
- **BMP** (`*.bmp`) — изображение B-scan

---

## 12. OLE Automation Server

### 12.1 Регистрация

```
CLSID: Pelengpc_tlb::CLSID_IData
IID:   Pelengpc_tlb::IID_IIData
LIBID: Pelengpc_tlb::LIBID_PelengPC (ver 1.0)
ProgID: "PelengPC.IData"
ThreadingModel: (из реестра)
```

### 12.2 Интерфейс IIData

Реализован через ATL:
```cpp
class TIDataImpl : 
    public ATL::CComCoClass<TIDataImpl, &Pelengpc_tlb::CLSID_IData>,
    public ATL::IDispatchImpl<Pelengpc_tlb::IIData, 
                              &Pelengpc_tlb::IID_IIData,
                              &Pelengpc_tlb::LIBID_PelengPC, 1, 0,
                              ATL::CComTypeInfoHolder>
{
    // Методы: ShowWindow, PAKIET (приём данных через SAFEARRAY)
};
```

### 12.3 Взаимодействие с dbdll.dll

`dbdll.dll → _Form_View`:
1. `GetActiveObject` или `CoCreateInstance` для PelengPC.IData
2. Вызов метода `ShowWindow` через `IDispatch::Invoke`
3. Данные передаются как `SAFEARRAY(VT_UI1)`

---

## 13. Контакты разработчика

```
Компания: Altek (ООО "Алтек")
Сайт:     www.altek.info
E-mail:   mail@altek.info
          progr@altek.info
          kag@altek.info
          alexer@altek.info
```

---

## 14. Встроенный конфиг Firebird (начало payload)

Первые данные в распакованном payload — `firebird.conf`:
```
# Firebird configuration file
# DefaultDbCachePages = 2048
# (стандартный конфиг Firebird 2.x)
```

---

## 15. Псевдокод основных функций

### 15.1 Инициализация COM-порта (0x4239C8)

```c
TCOMPort* TCOMPort_Create() {
    TCOMPort* port = new TCOMPort;
    port->hFile    = INVALID_HANDLE_VALUE;
    port->portName = NULL;
    port->baudIndex = 6;           // 9600
    port->baudRate  = 9600;
    port->dataBits  = 3;           // 8 бит (3+5)
    port->stopBits  = 0;           // 1 stop
    port->parity    = 0;           // none
    port->timeout   = 1000;        // 1 сек
    return port;
}
```

### 15.2 Установка Baud Rate (0x423B4C)

```c
void TCOMPort_SetBaudRate(TCOMPort* port, DWORD rate) {
    port->baudRate = rate;
    switch (rate) {
        case 110:    port->baudIndex = 0;  break;
        case 300:    port->baudIndex = 1;  break;
        case 600:    port->baudIndex = 2;  break;
        case 1200:   port->baudIndex = 3;  break;
        case 2400:   port->baudIndex = 4;  break;
        case 4800:   port->baudIndex = 5;  break;
        case 9600:   port->baudIndex = 6;  break;
        case 14400:  port->baudIndex = 7;  break;
        case 19200:  port->baudIndex = 8;  break;
        case 38400:  port->baudIndex = 9;  break;
        case 56000:  port->baudIndex = 10; break;
        case 57600:  port->baudIndex = 11; break;
        case 115200: port->baudIndex = 12; break;
        case 128000: port->baudIndex = 13; break;
        case 256000: port->baudIndex = 14; break;
        default:     /* оставить как есть */ break;
    }
}
```

### 15.3 Запрос блока данных

```c
int RequestBlock(TCOMPort* port, WORD blockAddr, byte* outBuf, int expectedSize) {
    byte cmd;
    
    if (blockAddr == 60000) {
        // FLASH dump
        cmd = 0x9A;
        WriteByte(port, &cmd);
        return ReadBytes(port, outBuf, expectedSize);
    }
    
    // Обычный запрос
    cmd = 0x42;
    WriteByte(port, &cmd);
    Delay(port, 10);
    
    byte lo = (byte)(blockAddr & 0xFF);
    WriteByte(port, &lo);
    Delay(port, 10);
    
    byte hi = (byte)(blockAddr >> 8);
    WriteByte(port, &hi);
    Delay(port, 10);
    
    // Приём с отображением прогресса
    return ReadBlockWithProgress(port, outBuf, expectedSize);
}
```

### 15.4 Обработка принятых данных

```c
void ProcessReceivedBlock(byte* rawData, int rawSize) {
    uint outputSize = rawSize;
    
    // Разбор через DLL
    int* parsed = pSortBufData(rawData, &outputSize);
    
    if (parsed) {
        int category = parsed[0];  // 1..4
        int numFields = parsed[1];
        
        // Сохранение в БД
        IBDataSetW->ParamByName("Number")->AsInteger = blockNumber;
        IBDataSetW->ParamByName("BlockLen")->AsInteger = rawSize;
        IBDataSetW->ParamByName("Block")->AsBlob = rawData;
        IBDataSetW->ExecSQL();
        IBTransactionW->Commit();
        
        // Освобождение
        pFreeBuffer((int)parsed);
    }
}
```

---

## 16. Сводка архитектуры

```
┌─────────────────────────────────────────────────────┐
│                  PelengPC.exe                         │
│  (C++Builder 5, VCL, ATL COM Server)                │
├──────────┬──────────┬───────────┬───────────────────┤
│ FormMain │FormNastr │FormScreen │  FormReadData     │
│ (DBGrid) │(Settings)│ (B-scan)  │  (Read device)    │
├──────────┴──────────┴───────────┴───────────────────┤
│              TCOMPort (RS-232)                        │
│    Open/Close/Read/Write via Win32 API               │
│    Protocol: 0x55=Handshake, 0x42=Block, 0x9A=Flash │
├──────────────────────────────────────────────────────┤
│         dbdll.dll (TLV Parser)                       │
│    _SortBufData → parse raw packet by category       │
│    _Form_View → OLE Automation → PelengPC.IData      │
│    _FreeBuffer → cleanup                             │
├──────────────────────────────────────────────────────┤
│         Firebird Embedded (fbembed.dll)              │
│    PelengPC.fdb → Table: BlockZap(Number,BlockLen,Block)│
├──────────────────────────────────────────────────────┤
│         zapis2.exe (Report Generator)                │
│    MFC Doc/View → WordML (Word 2003 XML)            │
│    Реестр: SOFTWARE\Altek\Operators                  │
└──────────────────────────────────────────────────────┘
```

---

## 17. Ограничения данной декомпиляции

1. **Нет Ghidra/IDA** — анализ выполнен только через `objdump` и ручной разбор hex/asm
2. **Нет innoextract** — файлы из инсталлятора извлечены через LZMA-распаковку, но без разделения по файлам (монолитный blob)
3. **Нет полного CFG** — граф вызовов восстановлен частично по `call` инструкциям
4. **DFM в бинарном формате** — названия компонентов извлечены, но не все свойства (Caption, размеры и т.д.)
5. **Точная семантика STATUS (0x74)** — формат ответа не определён
6. **16-байтовый заголовок пакета** — известны: байты 0-1 (devId), 2 (флаги), 4-5 (sweepId), 0x10-0x11 (blockAddr). Остальные не идентифицированы.

---

## 18. Рекомендации

1. Для полной декомпиляции запустить **Ghidra** или **IDA Pro** на `/tmp/extracted_pe_10.exe` (PelengPC.exe, 1555 KB)
2. Извлечь DFM-ресурсы через **Resource Hacker** для получения полных свойств форм
3. Использовать **innoextract** на локальной машине для разделения файлов установщика
4. Декомпилировать dbdll.dll (PE#9 @ offset 0x602A43) для полного понимания TLV-протокола
