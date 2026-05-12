# PelengPC.exe ver 1.2 — Полная декомпиляция

**Дата:** 2026-05-12  
**Метод:** objdump (GNU binutils 2.41) + Python 3 (struct, lzma) + ручной анализ  
**Исходный файл:** PelengPC_ver1_2.exe (Inno Setup 5.4.2, 6.3 МБ)  
**Извлечённый бинарь:** PelengPC.exe (1555 КБ, PE32 i386, Borland C++Builder 5)

---

## 1. Таблица методов (Published Method Table)

### FormMain (TFormMain) — 45 методов

| Метод | Адрес | Описание |
|-------|-------|----------|
| NHelpProgClick | 0x00407F40 | Вызов PelengPC.hlp |
| NHelpAutorClick | 0x00408184 | Диалог "Об авторах" |
| **NCOMPortClick** | **0x00408428** | **Настройка COM → показ FormNastr → ApplySettings** |
| **NTestPortClick** | **0x00408564** | **Тест COM (Handshake 0x55)** |
| NSaveConfigClick | 0x0040884C | Сохранение конфигурации в реестр |
| FormClose | 0x004088D8 | Закрытие главной формы |
| **NReadDataClick** | **0x004088F4** | **Чтение из прибора (режим COM direct)** |
| CBTypeChange | 0x00408978 | Смена типа прибора в ComboBox |
| VersTChange | 0x004089B4 | Смена версии таблицы |
| NViborClick | 0x004089F0 | Выборка по фильтру |
| NSortirClick | 0x0040AD38 | Сортировка таблицы |
| NReadZapClick | 0x0040B4F4 | Чтение одной записи |
| NDelZapClick | 0x0040B518 | Удаление записи |
| **FormCreate** | **0x0040BCCC** | **Инициализация: БД, реестр, DLL, COM порт** |
| NResetTableClick | 0x0040C16C | Сброс таблицы |
| NOperaClick | 0x0040C184 | Управление операторами |
| NReadDocClick | 0x0040C284 | Чтение отчёта |
| NAboutClick | 0x0040C9CC | О программе |
| NSearchClick | 0x0040F270 | Поиск в таблице |
| FormShow | 0x0040F35C | Показ формы |
| NSaveExcelClick | 0x0040F4C0 | Экспорт в Excel |
| NFirmaClick | 0x0040FB14 | Настройки фирмы |
| NSelectAllClick | 0x0040FC94 | Выбрать все записи |
| DBGridCellClick | 0x0040FDA8 | Клик по ячейке |
| NNumbersClick | 0x0040FE9C | Управление номерами |
| NPassWordClick | 0x004100B8 | Смена пароля |
| DBGridTitleClick | 0x00410168 | Клик по заголовку (сортировка) |
| DBGridDblClick | 0x00410388 | Двойной клик (просмотр) |
| FormActivate | 0x00410734 | Активация формы |
| NPrintTableClick | 0x00410758 | Печать таблицы |
| NReadFilesClick | 0x00410934 | Чтение из файлов .fla/.plg |
| NExitClick | 0x00410960 | Выход |
| **ToolButtonRaportClick** | **0x00410AC0** | **Генерация рапорта (запуск zapis2.exe)** |
| NDeviceClick | 0x00410AF4 | Настройки прибора |
| **ToolButtonFromFileClick** | **0x00408928** | **Загрузка из файла (режим buffer)** |

### FormNastr (TFormNastr) — 25 методов

| Метод | Адрес | Описание |
|-------|-------|----------|
| ButtonOKClick | 0x00411C6C | Применить настройки + закрыть |
| ButtonCanselClick | 0x00412118 | Отмена |
| ButtonDefaultClick | 0x00412138 | Сброс на значения по умолчанию |
| **FormShow** | **0x004123E0** | **Заполнение ComboBoxPort и ComboBoxBaud** |
| ButtonAddClick | 0x00416198 | Добавить оператора |
| ButtonDelClick | 0x004164E8 | Удалить оператора |
| **ButtonPortViewClick** | **0x00416A08** | **Перечисление COM портов (SetupDi API)** |
| **ButtonTestClick** | **0x00417C78** | **Тест подключения к прибору** |
| SpeedButtonAddClick | 0x004173D4 | Добавить sweep-запись |
| SpeedButtonClearClick | 0x00417570 | Очистить sweep-таблицу |
| SpeedButtonDelClick | 0x004175A0 | Удалить sweep-запись |
| SpeedButtonUpClick | 0x00417B0C | Переместить запись вверх |
| SpeedButtonDownClick | 0x00417FE4 | Переместить запись вниз |

### FormReadData (TFormReadData) — 10 методов

| Метод | Адрес | Описание |
|-------|-------|----------|
| **ButtonReadDirClick** | **0x0041B408** | **Чтение каталога блоков из прибора** |
| ButtonViewClick | 0x0041BBE0 | Просмотр выбранного блока |
| ListBoxDblClick | 0x0041B420 | Двойной клик = просмотр |
| ButtonSelectAllClick | 0x0041BC3C | Выбрать все блоки |
| **ButtonSendToBaseClick** | **0x0041BCE0** | **Отправить блоки в БД** |
| CheckListBoxTypeClickCheck | 0x0041C26C | Фильтр по типу |
| FormClose | 0x0041C4CC | Закрытие |
| FormShow | 0x0041C530 | Показ формы |

### FormRaport — 11 методов

| Метод | Адрес | Описание |
|-------|-------|----------|
| LoadFlashClick | 0x00422764 | Загрузка из FLASH |
| FromPriborClick | 0x00422938 | Чтение из прибора |
| FromFileClick | 0x00422950 | Чтение из файла |
| LoadEkranClick | 0x00422E94 | Загрузка экрана B-scan |
| ShowRaport | 0x004231B8 | Отображение рапорта |

### FormScreen — 3 метода

| Метод | Адрес | Описание |
|-------|-------|----------|
| **FormPaint** | **0x00423824** | **Рендеринг B-scan bitmap** |
| N1Click | 0x00423850 | Контекстное меню |
| FormMouseUp | 0x0042387C | Обработка клика мыши |

---

## 2. Ключевые обработчики — Псевдокод

### 2.1 NCOMPortClick (0x408428) — Настройка COM-порта

```c
void __fastcall TFormMain::NCOMPortClick(TObject* Sender)
{
    // Проверяем что есть connection info  
    if (!self->connectionInfo->hasData)  // [self+0x2FC]->field+0x32
        return;

    // Показываем FormNastr модально
    TFormNastr* nastr = *FormNastr;    // 0x53AF6C
    // Передаём текущую конфигурацию COM-порта в форму
    nastr->SetPortConfig(nastr->comboPort);  // +0x2FC → +0x2E0
    int result = nastr->ShowModal();         // vtable+0xD8

    if (result == mrCancel)  // == 2
        goto update_status;

    // Применяем настройки COM порта  
    TCOMPort* port = &DAT_0053E948;
    BOOL ok = TCOMPort_ApplySettings(port);  // call 0x424020

    if (ok) {
        // StatusBar = "Settings-OK!"   (строка @ 0x523CC4)
        statusText = "Settings-OK! ";
    } else {
        // StatusBar = "Settings-BAD! DisConnect!"  (строка @ 0x523CD2)
        statusText = "Settings-BAD! DisConnect!";
    }

update_status:
    // Обновляем StatusBar
    self->StatusBar->Panels[0]->Text = "";   // [self+0x2D0]+0x1F0
    self->StatusBar->Panels[0]->Text = statusText;
}
```

### 2.2 NReadDataClick (0x4088F4) — Чтение из прибора (COM)

```c
void __fastcall TFormMain::NReadDataClick(TObject* Sender)
{
    self->readAborted = FALSE;          // [self+0x409] = 0

    // Устанавливаем режим "прямое подключение через COM-порт"
    TFormReadData* reader = *(TFormReadData**)0x5BE9C8;
    reader->connectionMode = 1;         // [reader+0x3C4] = 1 (COM direct)

    // Показываем FormReadData модально — пользователь выбирает блоки
    reader->ShowModal();                // vtable+0xD8
}
```

### 2.3 ToolButtonFromFileClick (0x408928) — Чтение из файла

```c
void __fastcall TFormMain::ToolButtonFromFileClick(TObject* Sender)
{
    self->readAborted = FALSE;          // [self+0x409] = 0

    TFormReadData* reader = *(TFormReadData**)0x5BE9C8;
    reader->connectionMode = 0;         // [reader+0x3C4] = 0 (from file/buffer)

    // Загружаем данные из файла
    reader->LoadFromFile();             // call 0x41AB20

    // Если блоки загружены — показываем форму
    if (reader->blockList != NULL)      // [reader+0x318] != 0
        reader->ShowModal();
}
```

### 2.4 Главный диспатчер чтения блока (0x41B43C)

```c
int __fastcall TFormReadData::ReadBlock(BYTE* outBuffer, TFormReadData* self)
{
    int result = 0;

    // Копируем 16-байт заголовок в глобальную переменную
    memcpy(&DAT_0053E96A, outBuffer, 16);

    // ===== СПЕЦИАЛЬНЫЙ АДРЕС 60000 = FLASH DUMP =====
    if (self->currentBlockAddr == 60000) {      // [self+0x3C8] == 0xEA60
        result = Protocol_FlashDump(&comPort, outBuffer + 16);
        *(WORD*)(outBuffer + 0x10) = 60000;     // маркер
        goto done;
    }

    // ===== SWEEP-АДРЕСА (10000-29999) =====
    if (IsSweepAddress(self->currentBlockAddr)) {
        int blockSize = GetBlockSizeForAddr(self, self->currentBlockAddr);
        result = Protocol_BlockRequest(&comPort, self->currentBlockAddr,
                                       outBuffer + 0x10, blockSize);
        if (result == 0) return 0;
        outBuffer[0x0E] = (BYTE)result;
        goto done;
    }

    // ===== ОБЫЧНЫЕ АДРЕСА =====
    int blockSize = GetBlockSizeForAddr(self, self->currentBlockAddr);
    result = Protocol_BlockRequest(&comPort, self->currentBlockAddr,
                                   outBuffer + 0x10, blockSize);
done:
    return result;
}
```

---

## 3. dbdll.dll — _SortBufData (TLV парсер)

### 3.1 Полная реконструкция (0x401454)

```c
// Экспорт [ordinal 2]: __cdecl _SortBufData(BYTE* rawPacket, UINT* pSize)
// Вход: rawPacket = сырой пакет от прибора, pSize = указатель на размер
// Выход: указатель на распарсенный буфер (или NULL)

int* __cdecl _SortBufData(BYTE* rawPacket, UINT* pSize)
{
    UINT origSize = *pSize;
    if (origSize == 0) return NULL;

    BYTE* body = rawPacket + 0x10;     // пропускаем 16-байт фрейм-заголовок

    // === Извлечение метаданных из заголовка ===
    
    // Адрес блока (LE16 из первых 2 байт тела)
    WORD blockAddr = body[0] | (body[1] << 8);
    
    // Категория = адрес / 1000
    int category = blockAddr / 1000;
    
    // Sweep ID = rawPacket[4]*100 + rawPacket[5]
    int sweepId = rawPacket[4] * 100 + rawPacket[5];
    
    // Device ID (LE16 из rawPacket[0:2])
    int devId = rawPacket[0] | (rawPacket[1] << 8);
    
    // Дополнительное поле (body[5] | body[6]<<8)
    int field_c = body[5] | (body[6] << 8);

    // === Получение формата по категории ===
    FormatDesc* fmt = GetFormatByCategory(&category);  // 0x407DE5
    int headerLen   = CalcTotalFieldLength(fmt);       // 0x402A34
    int numFields   = fmt->fieldCount * 4;             // [fmt+8] << 2

    // === Аллокация результата ===
    UINT totalSize = origSize + headerLen + numFields + 2;
    *pSize = totalSize;
    
    int* result = (int*)malloc(totalSize);
    if (!result) return NULL;

    // Копируем оригинальные данные в хвост результата
    memcpy((BYTE*)result + numFields + headerLen + 2, rawPacket, origSize);
    
    BYTE* decoded = (BYTE*)result + numFields + 2;  // начало декодированных данных

    // === ДИСПАТЧ ПО КАТЕГОРИИ ===
    switch (category) {
        case 1:         // A-scan
        case 2: case 3: // Общие данные
            result[0] = 2;
            DecodeAscan(decoded, body, fmt, &category);   // 0x403647
            break;
            
        case 4: case 6: // Настройки/Generic
            result[0] = 1;
            DecodeGeneric(decoded, body, fmt, &category); // 0x40397C
            break;
            
        case 5:         // Калибровка
            result[0] = 1;
            DecodeCalibration(decoded, body, fmt, &category); // 0x403C4A
            break;
            
        case 10: case 11: case 12: case 13:
        case 14: case 15: case 16: case 17:
        case 18: case 19:  // B-scan / Результаты
            result[0] = 3;
            DecodeBscan(decoded, body, fmt, &category);   // 0x403E38
            break;
            
        case 20: case 21: case 22: case 23:
        case 24: case 25: case 26: case 27:
        case 28: case 29:  // Короткий протокол
            result[0] = 4;
            DecodeShortProtocol(decoded, body, fmt, &category); // 0x4040F4
            break;
            
        default:
            break;
    }

    // Заполняем таблицу смещений полей
    FillFieldOffsets((BYTE*)result + 2, fmt);  // 0x402238
    result[1] = (char)fmt->fieldCount;

    return result;
}
```

### 3.2 Формат выходного буфера

```
Offset 0:    BYTE  dataType       (1=Generic, 2=Ascan, 3=Bscan, 4=ShortProtocol)
Offset 1:    BYTE  numFields      (количество полей)
Offset 2:    WORD  field_offsets[numFields]   (LUT смещений для каждого поля)
             ...decoded fields (headerLen bytes)...
             ...original raw packet (origSize bytes)...
```

### 3.3 Категории данных

| Категория | Адреса (×1000) | Тип (result[0]) | Описание |
|-----------|---------------|-----------------|----------|
| 1 | 1000-1999 | 2 | A-scan данные |
| 2-3 | 2000-3999 | 2 | Общие данные |
| 4, 6 | 4000-4999, 6000-6999 | 1 | Настройки прибора |
| 5 | 5000-5999 | 1 | Калибровка |
| 10-19 | 10000-19999 | 3 | B-scan / Результаты |
| 20-29 | 20000-29999 | 4 | Короткий протокол (ShortProt) |

---

## 4. DFM-компоненты форм

### FormReadData
```
TFormReadData (FormReadData)
├── TStatusBar (StatusBar)
├── TPanel (PanelDir)
│   ├── TLabel (LabelDir)    — "Каталог прибора"
│   └── TLabel (LabelFrom)   — "Загружено из..."
├── TPanel (PanelButton)
│   ├── TButton (ButtonReadDir)      — "Чтение каталога"
│   ├── TButton (ButtonView)         — "Просмотр"
│   ├── TButton (ButtonSendToBase)   — "В базу"
│   ├── TButton (ButtonSelectAll)    — "Выбрать все"
│   └── TButton (ButtonExit)         — "Выход"
├── TPageControl (PageControlReadData)
│   └── TListBox (ListBox)           — список блоков
├── TCheckListBox (CheckListBoxType) — фильтр по типу
├── TOpenDialog (OpenDialog)
└── TSaveDialog (SaveDialog)
```

### FormRaport
```
TFormRaport (FormRaport)
├── TToolBar (ToolBar1)
│   ├── TToolButton (LoadItem)    — загрузить
│   ├── TToolButton (SaveItem)    — сохранить
│   ├── TToolButton (UpdDev)      — обновить из прибора
│   └── TToolButton (LoadFlash)   — загрузить FLASH
├── TEdit (Edit) + TUpDown (UpDown1) — номер записи
├── TStatusBar (StatusBar)
├── TRadioGroup (Radio)           — выбор источника
├── TButton (ViewButton)          — просмотр
├── TMainMenu (RaportMenu)
│   ├── TMenuItem (FromPribor)    — из прибора
│   ├── TMenuItem (FromFile)      — из файла
│   └── TMenuItem (N11)
└── TImageList (ImageList)
```

### FormScreen
```
TFormScreen (FormScreen)
└── TPopupMenu (PopupMenu)        — контекстное меню (сохранить .plg/.fla/.bmp)
```

---

## 5. Глобальные переменные

| Адрес | Тип | Имя | Описание |
|-------|-----|-----|----------|
| 0x53AF64 | TFormMain* | FormMain | Главная форма |
| 0x53AF68 | TFormProgress* | FormProgress | Прогресс приёма |
| 0x53AF6C | TFormNastr* | FormNastr | Настройки |
| 0x53AF70 | TFormPassWord* | FormPassWord | Пароль |
| 0x53AF74 | TFormReadData* | FormReadData | Чтение данных |
| 0x53AF84 | TFormConfirm* | FormConfirm | Подтверждение |
| 0x53AF88 | TFormRaport* | FormRaport | Рапорты |
| 0x53AF8C | TFormScreen* | FormScreen | B-scan |
| 0x53B93C | ? | MainDataObj | Объект данных главной формы |
| 0x53E948 | TCOMPort | comPort | Глобальный COM-порт |
| 0x53E96A | BYTE[16] | frameHeader | Заголовок последнего фрейма |
| 0x53E97C | BYTE | deviceFlags | Флаги версии/типа прибора |
| 0x5BE994 | pfn* | pForm_View | DLL: _Form_View |
| 0x5BE998 | pfn* | pSortBufData | DLL: _SortBufData |
| 0x5BE99C | pfn* | pFreeBuffer | DLL: _FreeBuffer |
| 0x5BE9C8 | TFormReadData* | readerInstance | Экземпляр формы чтения |

---

## 6. Полный Data Flow

```
┌──────────────────────────────────────────────────────────────────┐
│  ПОЛЬЗОВАТЕЛЬ                                                     │
│  [Меню "Чтение" или кнопка "Из прибора"]                        │
└────────────────────────┬─────────────────────────────────────────┘
                         │ NReadDataClick (0x4088F4)
                         │   reader->mode = 1 (COM direct)
                         ▼
┌──────────────────────────────────────────────────────────────────┐
│  FormReadData (показ модально)                                    │
│  [ButtonReadDir] → читает каталог блоков                         │
│  [ListBox] → пользователь выбирает блоки                        │
│  [ButtonSendToBase] → отправляет в БД                           │
└────────────────────────┬─────────────────────────────────────────┘
                         │ ReadBlock (0x41B43C)
                         ▼
┌──────────────────────────────────────────────────────────────────┐
│  ПРОТОКОЛ                                                         │
│                                                                   │
│  if (addr == 60000):                                             │
│      WriteByte(0x9A)  → FLASH dump                               │
│  else:                                                           │
│      WriteByte(0x42)                                             │
│      Delay(10ms)                                                 │
│      WriteByte(addr_lo)                                          │
│      Delay(10ms)                                                 │
│      WriteByte(addr_hi)                                          │
│      Delay(10ms)                                                 │
│                                                                   │
│  → FormProgress показывает прогресс приёма                       │
│  → ReadBytes с таймаутом                                         │
└────────────────────────┬─────────────────────────────────────────┘
                         │ rawPacket (16-byte header + body)
                         ▼
┌──────────────────────────────────────────────────────────────────┐
│  dbdll.dll → _SortBufData (0x401454)                             │
│                                                                   │
│  1. Извлечение: blockAddr, category, sweepId, devId              │
│  2. Диспатч по category → формат полей                           │
│  3. Декодирование полей (число/строка/дата)                      │
│  4. Результат: [type][numFields][offsets...][decoded...][raw...] │
└────────────────────────┬─────────────────────────────────────────┘
                         │ parsed result
                         ▼
┌──────────────────────────────────────────────────────────────────┐
│  PelengPC.exe                                                     │
│                                                                   │
│  if (category == 10-19):    → FormScreen (B-scan визуализация)   │
│  else:                      → DBGrid (таблица)                   │
│                                                                   │
│  Сохранение в БД:                                                │
│    INSERT INTO BlockZap (Number, BlockLen, Block)                 │
│    VALUES (:num, :len, :rawData)                                 │
└──────────────────────────────────────────────────────────────────┘
```

---

## 7. Структура TFormReadData

```c
typedef struct TFormReadData {
    // ... VCL TForm base (0x000 - 0x2FF) ...
    
    // +0x304: результат FormProgress (принятые байты)
    // +0x314: кэш блоков (для buffer-режима)
    // +0x318: blockList (TList) — список загруженных записей
    // +0x31C: адресная таблица блоков
    // +0x320: имя текущего sweep
    // +0x324: sweep entries (массив по 24 байта)
    // +0x3B0: sweepCount
    // +0x3BC: streak-таблица
    // +0x3C0: streak-count
    // +0x3C4: connectionMode (1=COM direct, 0=file/buffer)
    // +0x3C8: currentBlockAddr (текущий запрашиваемый адрес)
    // +0x409: abortFlag
} TFormReadData;
```

---

## 8. Контакты / Метаданные

```
Компания:    ООО "Алтек"  
Сайт:        www.altek.info
E-mail:      mail@altek.info, progr@altek.info, kag@altek.info, alexer@altek.info
Реестр:      HKLM\SOFTWARE\Altek\PelengPC
Версия ПО:   PelengPC Version 1.1
Компилятор:  Borland C++Builder 5 (путь: D:\PROGRAM FILES\BORLAND\CBUILDER5\)
Исходники:   C:\program\union\pelengpc_fb_no_password\fb_test\
COM Server:  PelengPC.IData (ATL)
```
