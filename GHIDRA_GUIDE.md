# Улучшение анализа PelengPC.exe в Ghidra — Пошаговая инструкция

## 0. Подготовка

### Распаковка инсталлятора

```
innoextract PelengPC_ver1_2.exe
```
Если `innoextract` не работает — скачай с https://constexpr.org/innoextract/  
Результат: папка `app/` с `PelengPC.exe`, `dbdll.dll` и т.д.

### Импорт в Ghidra

1. File → Import File → `PelengPC.exe`
2. Язык: **x86:LE:32:default** (определится автоматически)
3. При вопросе «Analyze?» → **Yes**
4. В окне Analysis Options **включи дополнительно**:
   - ✅ Aggressive Instruction Finder
   - ✅ Decompiler Parameter ID
   - ✅ Windows x86 PE Exception Handling
   - ✅ Scalar Operand References
   - ✅ Condense Filler Bytes
5. Нажми **Analyze** → подожди 3-10 минут

---

## 1. Применение известных типов (Data Type Manager)

### 1.1 Импорт Windows API типов

1. **Window → Data Type Manager**
2. В дереве найди **BuiltInTypes** → кликни правой → **Apply Function Data Types**
3. Открой архив: **File → Open Data Type Archive** → выбери:
   - `windows_vs12_32.gdt` (лежит в `Ghidra/Features/Base/data/typeinfo/win32/`)
   
Это даст Ghidra правильные сигнатуры для `CreateFileA`, `SetCommState`, `WriteFile` и т.д.

### 1.2 Создание структуры TCOMPort

1. **Window → Data Type Manager** → правый клик на Program → **New → Structure**
2. Имя: `TCOMPort`
3. Добавь поля:

| Offset | Имя | Тип | Размер |
|--------|-----|-----|--------|
| 0x00 | hFile | HANDLE (dword) | 4 |
| 0x04 | portName | char* | 4 |
| 0x08 | baudIndex | byte | 1 |
| 0x09 | _pad1 | byte[3] | 3 |
| 0x0C | baudRate | DWORD | 4 |
| 0x10 | dataBits | byte | 1 |
| 0x11 | stopBits | byte | 1 |
| 0x12 | parity | byte | 1 |
| 0x13 | isOpen | byte | 1 |
| 0x14 | timeout | DWORD | 4 |

4. Нажми **Save** (дискета)

### 1.3 Применение структуры к глобальной переменной

1. В Listing View: нажми **G** → введи `0053e948`
2. Правый клик → **Data → TCOMPort**
3. Теперь вся область будет размечена как структура

---

## 2. Переименование функций (самое важное!)

### Навигация: нажми G, введи адрес

Переименуй каждую функцию (клик на имя → **L** или правый клик → Rename):

| Адрес | Текущее имя | Переименуй в |
|-------|-------------|--------------|
| `004239c8` | FUN_004239c8 | `TCOMPort_Create` |
| `00423b4c` | FUN_00423b4c | `TCOMPort_SetBaudRate` |
| `00423cc4` | FUN_00423cc4 | `TCOMPort_SetDataBits` |
| `00423ce0` | FUN_00423ce0 | `TCOMPort_SetStopBits` |
| `00423cfc` | FUN_00423cfc | `TCOMPort_SetParity` |
| `00423d18` | FUN_00423d18 | `TCOMPort_Open` |
| `00423e28` | FUN_00423e28 | `TCOMPort_Close` |
| `00423e58` | FUN_00423e58 | `TCOMPort_WriteBytes` |
| `00423f0c` | FUN_00423f0c | `TCOMPort_WriteByte` |
| `00423f40` | FUN_00423f40 | `TCOMPort_BytesAvail` |
| `00423f64` | FUN_00423f64 | `TCOMPort_ReadBytes` |
| `00424020` | FUN_00424020 | `TCOMPort_ApplySettings` |
| `00423a56` | FUN_00423a56 | `TCOMPort_Delay` |
| `00424bb0` | FUN_00424bb0 | `Protocol_FlashDump` |
| `00424cc0` | FUN_00424cc0 | `Protocol_BlockRequest` |
| `0041b43c` | FUN_0041b43c | `ReadBlock_Dispatcher` |
| `0041c7c0` | FUN_0041c7c0 | `IsSweepAddress` |
| `0041c7f0` | FUN_0041c7f0 | `GetBaseAddr` |
| `0041c810` | FUN_0041c810 | `GetSubBlock` |
| `0041c838` | FUN_0041c838 | `GetIndex` |
| `004235f8` | FUN_004235f8 | `FormScreen_RenderBscan` |
| `0041b008` | FUN_0041b008 | `ParseFlashDump_TLV` |
| `00407f40` | FUN_00407f40 | `FormMain_NHelpProgClick` |
| `00408184` | FUN_00408184 | `FormMain_NHelpAutorClick` |
| `00408428` | FUN_00408428 | `FormMain_NCOMPortClick` |
| `00408564` | FUN_00408564 | `FormMain_NTestPortClick` |
| `0040884c` | FUN_0040884c | `FormMain_NSaveConfigClick` |
| `004088d8` | FUN_004088d8 | `FormMain_FormClose` |
| `004088f4` | FUN_004088f4 | `FormMain_NReadDataClick` |
| `00408928` | FUN_00408928 | `FormMain_ToolButtonFromFileClick` |
| `00408978` | FUN_00408978 | `FormMain_CBTypeChange` |
| `004089f0` | FUN_004089f0 | `FormMain_NViborClick` |
| `0040ad38` | FUN_0040ad38 | `FormMain_NSortirClick` |
| `0040b4f4` | FUN_0040b4f4 | `FormMain_NReadZapClick` |
| `0040b518` | FUN_0040b518 | `FormMain_NDelZapClick` |
| `0040bccc` | FUN_0040bccc | `FormMain_FormCreate` |
| `0040c16c` | FUN_0040c16c | `FormMain_NResetTableClick` |
| `0040c184` | FUN_0040c184 | `FormMain_NOperaClick` |
| `0040c284` | FUN_0040c284 | `FormMain_NReadDocClick` |
| `0040c9cc` | FUN_0040c9cc | `FormMain_NAboutClick` |
| `0040f270` | FUN_0040f270 | `FormMain_NSearchClick` |
| `0040f35c` | FUN_0040f35c | `FormMain_FormShow` |
| `0040f4c0` | FUN_0040f4c0 | `FormMain_NSaveExcelClick` |
| `0040fb14` | FUN_0040fb14 | `FormMain_NFirmaClick` |
| `0040fc94` | FUN_0040fc94 | `FormMain_NSelectAllClick` |
| `0040fe9c` | FUN_0040fe9c | `FormMain_NNumbersClick` |
| `004100b8` | FUN_004100b8 | `FormMain_NPassWordClick` |
| `00410734` | FUN_00410734 | `FormMain_FormActivate` |
| `00410758` | FUN_00410758 | `FormMain_NPrintTableClick` |
| `00410934` | FUN_00410934 | `FormMain_NReadFilesClick` |
| `00410960` | FUN_00410960 | `FormMain_NExitClick` |
| `00410ac0` | FUN_00410ac0 | `FormMain_ToolButtonRaportClick` |
| `00410af4` | FUN_00410af4 | `FormMain_NDeviceClick` |
| `00411c6c` | FUN_00411c6c | `FormNastr_ButtonOKClick` |
| `004123e0` | FUN_004123e0 | `FormNastr_FormShow` |
| `00416a08` | FUN_00416a08 | `FormNastr_ButtonPortViewClick` |
| `00417c78` | FUN_00417c78 | `FormNastr_ButtonTestClick` |
| `0041b408` | FUN_0041b408 | `FormReadData_ButtonReadDirClick` |
| `0041bbe0` | FUN_0041bbe0 | `FormReadData_ButtonViewClick` |
| `0041bce0` | FUN_0041bce0 | `FormReadData_ButtonSendToBaseClick` |
| `0041c530` | FUN_0041c530 | `FormReadData_FormShow` |
| `00422764` | FUN_00422764 | `FormRaport_LoadFlashClick` |
| `004231b8` | FUN_004231b8 | `FormRaport_ShowRaport` |
| `00423824` | FUN_00423824 | `FormScreen_FormPaint` |

---

## 3. Разметка глобальных переменных

Нажми **G** → введи адрес → правый клик → **Rename** (или **L**):

| Адрес | Переименуй в | Тип (правый клик → Data → ...) |
|-------|-------------|------|
| `0053af64` | g_FormMain | pointer |
| `0053af68` | g_FormProgress | pointer |
| `0053af6c` | g_FormNastr | pointer |
| `0053af70` | g_FormPassWord | pointer |
| `0053af74` | g_FormReadData | pointer |
| `0053af84` | g_FormConfirm | pointer |
| `0053af88` | g_FormRaport | pointer |
| `0053af8c` | g_FormScreen | pointer |
| `0053e948` | g_comPort | TCOMPort (создай тип выше) |
| `0053e96a` | g_frameHeader | byte[16] |
| `0053e97c` | g_deviceFlags | byte |
| `005be994` | g_pfn_Form_View | pointer |
| `005be998` | g_pfn_SortBufData | pointer |
| `005be99c` | g_pfn_FreeBuffer | pointer |
| `005be9c8` | g_readerInstance | pointer |

---

## 4. Исправление calling convention (КРИТИЧНО для BCB!)

Borland C++Builder использует **`__fastcall`** — первые 3 параметра в `EAX`, `EDX`, `ECX`.

Для КАЖДОЙ функции из списка выше:

1. Открой функцию (двойной клик или **G** → адрес)
2. В Decompiler View (справа): правый клик на имя функции → **Edit Function Signature**
3. Измени Calling Convention: выбери **`__fastcall`** (или `__thiscall` для методов с self)
4. Первый параметр — это `self` (EAX): тип `TCOMPort*` (или `TFormMain*`)

**Пример для TCOMPort_Open:**
```
bool __fastcall TCOMPort_Open(TCOMPort * self)
```

**Пример для TCOMPort_SetBaudRate:**
```
void __fastcall TCOMPort_SetBaudRate(TCOMPort * self, DWORD rate)
```
(self в EAX, rate в EDX)

**Пример для Protocol_BlockRequest:**
```
int __fastcall Protocol_BlockRequest(TCOMPort * port, WORD blockAddr, byte * outBuf, int expectedSize)
```
(port=EAX, blockAddr=EDX, outBuf=ECX, expectedSize=stack)

---

## 5. Установка типов параметров в Decompiler View

В окне Decompiler (правая панель):

1. **Правый клик на переменную** → **Retype Variable**
2. Или кликни на тип и нажми **Ctrl+L**

Ключевые типы для переназначения:

| В функции | Переменная | Назначь тип |
|-----------|-----------|-------------|
| TCOMPort_Open | local_40 (COMMTIMEOUTS) | `COMMTIMEOUTS` |
| TCOMPort_ApplySettings | local_24 (DCB) | `DCB` |
| TCOMPort_BytesAvail | local_14 (COMSTAT) | `COMSTAT` |
| Protocol_BlockRequest | param_3 (outBuf) | `byte *` |
| FormScreen_RenderBscan | param_2 (rawData) | `byte *` |

---

## 6. Добавление комментариев

В Listing или Decompiler:
- **;** (точка с запятой) — добавить EOL-комментарий
- **/** — добавить Plate-комментарий (над функцией)
- **Ctrl+;** — Pre-комментарий

Ключевые места для комментариев:

```
@ 0x423d18:  // TCOMPort_Open: CreateFileA("\\.\COMn"), SetCommTimeouts, Sleep(100)
@ 0x424020:  // TCOMPort_ApplySettings: GetCommState → set BaudRate/Parity/ByteSize/StopBits → SetCommState. Flow control OFF.
@ 0x424cc0:  // Protocol_BlockRequest: send 0x42, delay(10), addr_lo, delay(10), addr_hi, delay(10) → receive via FormProgress
@ 0x424bb0:  // Protocol_FlashDump: send opcode (0x9A), receive fixed-size block via FormProgress
@ 0x4235f8:  // B-scan render: 30 bytes/row, 1 bit = 1 pixel (2x2 on screen), width=480px
@ 0x41b43c:  // Main read dispatcher: addr==60000 → FlashDump(0x9A); addr 10000-29999 → BlockRequest(0x42); else → normal
```

---

## 7. Создание Bookmarks (закладки)

**Ctrl+D** → добавить закладку в текущей позиции

Создай закладки на все ключевые функции — потом легко найдёшь через **Window → Bookmarks**.

---

## 8. Экспорт результата

### Вариант 1: Экспорт всей программы как C

**File → Export Program** → Format: **C/C++** → Save

### Вариант 2: Копирование из Decompiler View

1. Открой нужную функцию
2. В Decompiler View: **Ctrl+A** (выделить всё) → **Ctrl+C** (копировать)
3. Вставь в текстовый файл

### Вариант 3: Сохранение проекта Ghidra

Просто закрой Ghidra — проект сохранится автоматически. Потом можешь прислать мне `.c` файлы.

---

## 9. Быстрая навигация (горячие клавиши)

| Клавиша | Действие |
|---------|----------|
| **G** | Перейти по адресу |
| **L** | Переименовать |
| **T** | Изменить тип данных |
| **F5** | Открыть Decompiler (если закрыт) |
| **Ctrl+E** | Edit Function Signature |
| **X** | Показать все ссылки на (Xrefs) |
| **Ctrl+Shift+F** | Поиск строк |
| **;** | Добавить комментарий |
| **D** | Определить как Data |
| **P** | Определить как функцию (если Ghidra не нашла) |
| **Backspace** | Назад (история навигации) |

---

## 10. Что прислать мне после

После того как сделаешь шаги 1-6, дай мне **любой из вариантов**:

**Лучший вариант:** Экспортируй C-код (File → Export → C/C++) и залей в репозиторий:
```bash
git add PelengPC_ghidra_decompiled.c
git commit -m "Ghidra decompilation with renamed functions and types"
git push
```

**Минимальный вариант:** Скопируй из Decompiler View только эти 5 функций и вставь в чат:
- `TCOMPort_Open` (0x423D18)
- `TCOMPort_ApplySettings` (0x424020)
- `Protocol_BlockRequest` (0x424CC0)
- `ReadBlock_Dispatcher` (0x41B43C)
- `FormMain_FormCreate` (0x40BCCC)

---

## 11. Частые проблемы

### «Ghidra показывает мусор в Decompiler»
→ Причина: неправильный calling convention. Исправь на `__fastcall` (см. шаг 4).

### «Функция не определена (серые адреса)»
→ Встань на начало функции → нажми **P** (Define Function).

### «Параметры выглядят неправильно»
→ Правый клик на имя функции в Decompiler → **Edit Function Signature** → исправь типы вручную.

### «Строки не видны»
→ **Window → Defined Strings** — покажет все строки. Или: **Search → For Strings** → Min Length=6.

### «Не вижу DFM-формы»
→ В дереве слева раскрой **Resources** → **RCDATA** — там лежат DFM-формы в бинарном виде.

---

## 12. Дополнительно: dbdll.dll

Повтори те же шаги для `dbdll.dll`. Ключевые адреса для переименования:

| Адрес | Имя |
|-------|-----|
| `0040131c` | `_Form_View` |
| `00401454` | `_SortBufData` |
| `0040193c` | `_FreeBuffer` |
| `00407de5` | `GetFormatByCategory` |
| `00402a34` | `CalcHeaderLength` |
| `00402238` | `FillFieldOffsets` |
| `00403647` | `DecodeAscan` |
| `0040397c` | `DecodeGeneric` |
| `00403c4a` | `DecodeCalibration` |
| `00403e38` | `DecodeBscan` |
| `004040f4` | `DecodeShortProtocol` |
| `004022c8` | `DispatchByTypeVar` |
| `004023b0` | `DecodeDate` |
| `00402504` | `DecodeTime` |
