/**
 * PelengPC.exe ver 1.2 — Полная декомпиляция (псевдокод)
 * Дата: 2026-05-12
 * Компилятор оригинала: Borland C++Builder 5
 * Метод: ручной анализ asm (objdump) + реконструкция C
 * 
 * Глобальные переменные:
 *   0x53AF64 = FormMain*
 *   0x53AF68 = FormProgress*
 *   0x53AF6C = FormNastr*
 *   0x53AF70 = FormPassWord*
 *   0x53AF74 = FormReadData*
 *   0x53AF84 = FormConfirm*
 *   0x53AF88 = FormRaport*
 *   0x53AF8C = FormScreen*
 *   0x53E948 = TCOMPort comPort (глобальный объект порта)
 *   0x5BE994 = pfn_Form_View (указатель на DLL функцию)
 *   0x5BE998 = pfn_SortBufData
 *   0x5BE99C = pfn_FreeBuffer
 */

#include <windows.h>

/* ============================================================
 *  СТРУКТУРЫ
 * ============================================================ */

// Реконструировано из инициализации @ 0x4239C8
typedef struct {
    HANDLE  hFile;      // +0x00: дескриптор COM-порта (INVALID_HANDLE_VALUE = закрыт)
    char*   portName;   // +0x04: AnsiString "\\.\COMn"
    BYTE    baudIndex;  // +0x08: индекс скорости (0-14), default=6
    DWORD   baudRate;   // +0x0C: скорость (bps), default=9600
    BYTE    dataBits;   // +0x10: биты данных (3 = 8 бит, т.к. +5)
    BYTE    stopBits;   // +0x11: стоп-биты (0=1, 1=1.5, 2=2)
    BYTE    parity;     // +0x12: четность (0=none, 1=odd, 2=even)
    BYTE    isOpen;     // +0x13: флаг "порт открыт"
    DWORD   timeout;    // +0x14: таймаут (мс), default=1000
} TCOMPort;

// FormProgress layout (частично)
typedef struct {
    // ... VCL TForm fields ...
    BYTE    active;     // +0x2F8: флаг активности приёма
    DWORD   expectedSize; // +0x2FC: ожидаемый размер блока
    DWORD   bufferPtr;  // +0x300: указатель на буфер приёма
    DWORD   receivedSize; // +0x304: фактически принято байт
} TFormProgress;

/* ============================================================
 *  COM-ПОРТ: Инициализация
 *  Адрес: 0x004239C8
 * ============================================================ */

TCOMPort* __fastcall TCOMPort_Create(TCOMPort* self)
{
    // self передаётся в EAX (Borland __fastcall)
    self->portName  = NULL;        // +0x04 = 0
    self->hFile     = INVALID_HANDLE_VALUE; // +0x00 = 0xFFFFFFFF
    self->baudIndex = 6;           // +0x08 = 6 (9600 baud)
    self->baudRate  = 0x2580;      // +0x0C = 9600
    self->dataBits  = 3;           // +0x10 = 3 (ByteSize = 3+5 = 8)
    self->stopBits  = 0;           // +0x11 = 0 (ONESTOPBIT)
    self->parity    = 0;           // +0x12 = 0 (NOPARITY)
    self->timeout   = 0x3E8;       // +0x14 = 1000 мс
    return self;
}

/* ============================================================
 *  COM-ПОРТ: Установка Baud Rate
 *  Адрес: 0x00423B4C
 * ============================================================ */

void __fastcall TCOMPort_SetBaudRate(TCOMPort* self, DWORD rate)
{
    // self в EAX, rate в EDX
    self->baudRate = rate;          // [self+0x0C] = rate

    // Switch по значению rate → установка baudIndex
    switch (rate) {
        case 110:    self->baudIndex = 0;  break;
        case 300:    self->baudIndex = 1;  break;
        case 600:    self->baudIndex = 2;  break;
        case 1200:   self->baudIndex = 3;  break;
        case 2400:   self->baudIndex = 4;  break;
        case 4800:   self->baudIndex = 5;  break;
        case 9600:   self->baudIndex = 6;  break;
        case 14400:  self->baudIndex = 7;  break;
        case 19200:  self->baudIndex = 8;  break;
        case 38400:  self->baudIndex = 9;  break;
        case 56000:  self->baudIndex = 10; break;
        case 57600:  self->baudIndex = 11; break;
        case 115200: self->baudIndex = 12; break;
        case 128000: self->baudIndex = 13; break;
        case 256000: self->baudIndex = 14; break;
        default:     self->baudIndex = 6;  break; // fallback = 9600
    }
}

/* ============================================================
 *  COM-ПОРТ: Установка DataBits / StopBits / Parity
 *  Адреса: 0x423CC4 / 0x423CE0 / 0x423CFC
 * ============================================================ */

void __fastcall TCOMPort_SetDataBits(TCOMPort* self, BYTE val)  { self->dataBits = val; }
void __fastcall TCOMPort_SetStopBits(TCOMPort* self, BYTE val)  { self->stopBits = val; }
void __fastcall TCOMPort_SetParity(TCOMPort* self, BYTE val)    { self->parity = val; }

/* ============================================================
 *  COM-ПОРТ: Получение Timeout / BaudRate
 *  Адреса: 0x423B20 (SetTimeout), 0x423B3C (GetBaudRate)
 * ============================================================ */

void __fastcall TCOMPort_SetTimeout(TCOMPort* self, DWORD ms) { self->timeout = ms; }
DWORD __fastcall TCOMPort_GetBaudRate(TCOMPort* self) { return self->baudRate; }

/* ============================================================
 *  COM-ПОРТ: Открытие порта
 *  Адрес: 0x00423D18
 * ============================================================ */

BOOL __fastcall TCOMPort_Open(TCOMPort* self)
{
    if (self->isOpen)
        return TRUE;    // уже открыт — выходим

    // Формирование строки "\\.\COMn" из self->portName
    char portPath[64];
    sprintf(portPath, "\\\\.\\%s", self->portName ? self->portName : "");

    // Открытие через CreateFileA
    self->hFile = CreateFileA(
        portPath,
        GENERIC_READ | GENERIC_WRITE,   // 0xC0000000
        0,                               // не разделяем
        NULL,                            // без безопасности
        OPEN_EXISTING,                   // = 3
        0,                               // без флагов
        NULL                             // без шаблона
    );

    // Проверка результата
    self->isOpen = (self->hFile != INVALID_HANDLE_VALUE) ? 1 : 0;

    if (self->isOpen) {
        // Получаем таймауты через GetCommTimeouts (call 0x543380)
        COMMTIMEOUTS timeouts;
        GetCommTimeouts(self->hFile, &timeouts);

        // Устанавливаем свои таймауты
        timeouts.ReadIntervalTimeout         = 1;
        timeouts.ReadTotalTimeoutMultiplier  = 0;
        timeouts.ReadTotalTimeoutConstant    = 1;
        timeouts.WriteTotalTimeoutMultiplier = 0;
        timeouts.WriteTotalTimeoutConstant   = 10;

        // Применяем (call 0x54354B = SetCommTimeouts)
        SetCommTimeouts(self->hFile, &timeouts);

        // Пауза 100мс после открытия (push 0x64; call Sleep)
        Sleep(100);
    }

    return self->isOpen;
}

/* ============================================================
 *  COM-ПОРТ: Закрытие порта
 *  Адрес: 0x00423E28
 * ============================================================ */

void __fastcall TCOMPort_Close(TCOMPort* self)
{
    if (self->isOpen) {
        CloseHandle(self->hFile);    // call 0x543358
    }
    self->hFile  = INVALID_HANDLE_VALUE;
    self->isOpen = 0;
}

/* ============================================================
 *  COM-ПОРТ: Запись буфера (WriteBytes)
 *  Адрес: 0x00423E58
 * ============================================================ */

DWORD __fastcall TCOMPort_WriteBytes(TCOMPort* self, BYTE* buffer, DWORD size)
{
    DWORD bytesWritten = 0;
    DWORD startTick = GetTickCount();       // call 0x5434E1
    DWORD deadline  = startTick + self->timeout;

    if (!self->isOpen)
        return 0;

    // Цикл записи с таймаутом
    while (TRUE) {
        DWORD written = 0;
        BOOL ok = WriteFile(self->hFile, buffer, size, &written, NULL); // call 0x54368B

        if (!ok || written == 0) {
            // Проверка таймаута
            if (GetTickCount() >= deadline)
                break;
            continue;
        }

        bytesWritten += written;
        size -= written;
        buffer += written;

        if (size <= 0)
            break;

        // Обновляем deadline
        deadline = GetTickCount() + self->timeout;
    }

    if (bytesWritten == 0) {
        // Ошибка: показываем сообщение "ERROR !!! TimeOut..."
        ShowError("ERROR !!! TimeOut waiting for response.");
    }

    return bytesWritten;
}

/* ============================================================
 *  COM-ПОРТ: Запись одного байта (WriteByte)
 *  Адрес: 0x00423F0C
 * ============================================================ */

DWORD __fastcall TCOMPort_WriteByte(TCOMPort* self, BYTE* pByte)
{
    if (!self->isOpen)
        return 0;
    // Пишем ровно 1 байт (ecx=1)
    return TCOMPort_WriteBytes(self, pByte, 1);  // call 0x44738E
}

/* ============================================================
 *  COM-ПОРТ: Количество байт во входном буфере (BytesAvail)
 *  Адрес: 0x00423F40
 * ============================================================ */

DWORD __fastcall TCOMPort_BytesAvail(TCOMPort* self)
{
    COMSTAT comStat;
    DWORD   errors;
    ClearCommError(self->hFile, &errors, &comStat);  // call 0x54346B
    return comStat.cbInQue;     // +0x04 в COMSTAT
}

/* ============================================================
 *  COM-ПОРТ: Чтение буфера (ReadBytes)
 *  Адрес: 0x00423F64
 * ============================================================ */

DWORD __fastcall TCOMPort_ReadBytes(TCOMPort* self, BYTE* buffer, DWORD maxSize)
{
    DWORD totalRead = 0;
    DWORD bytesRead = 0;
    DWORD remaining = maxSize;
    BYTE* ptr = buffer;

    if (!self->isOpen)
        return 0;

    DWORD deadline = GetTickCount() + self->timeout;

    while (remaining > 0) {
        // Проверяем сколько доступно
        DWORD avail = TCOMPort_BytesAvail(self);
        if (avail <= 0) {
            // Проверка таймаута
            if (GetTickCount() > deadline)
                break;
            continue;
        }

        // Читаем не больше чем доступно и чем нужно
        DWORD toRead = (remaining < avail) ? remaining : avail;

        BOOL ok = ReadFile(self->hFile, ptr, toRead, &bytesRead, NULL); // call 0x543711

        totalRead += bytesRead;
        remaining -= bytesRead;
        ptr       += bytesRead;

        // Обновляем deadline
        deadline = GetTickCount() + self->timeout;
    }

    return totalRead;
}

/* ============================================================
 *  COM-ПОРТ: Применение настроек (SetCommState)
 *  Адрес: 0x00424020
 * ============================================================ */

BOOL __fastcall TCOMPort_ApplySettings(TCOMPort* self)
{
    BOOL result = FALSE;

    if (!TCOMPort_IsOpen(self))    // call 0x44734D
        return FALSE;

    DCB dcb;
    if (!GetCommState(self->hFile, &dcb))  // call 0x5435FD
        return FALSE;

    // Заполняем DCB из полей объекта
    dcb.BaudRate = self->baudRate;          // [self+0x0C]
    dcb.Parity   = self->parity;           // [self+0x12]
    dcb.ByteSize = self->dataBits + 5;     // [self+0x10] + 5
    dcb.StopBits = self->stopBits;         // [self+0x11]

    // Сброс всех flow control битов
    dcb.fOutxCtsFlow  = 0;  // AND 0xFE
    dcb.fOutxDsrFlow  = 0;  // AND 0xFD
    dcb.fDtrControl   = 0;  // AND 0xFB
    dcb.fDsrSensitivity = 0; // AND 0xF7
    dcb.fTXContinueOnXoff = 0; // AND 0xCF (2 bits)
    dcb.fOutX         = 0;  // AND 0xBF
    dcb.fInX          = 0;  // AND 0x7F
    dcb.fRtsControl   = 0;  // AND 0xFE (next byte)
    // ... все flow control выключены

    result = SetCommState(self->hFile, &dcb);  // call 0x54380F

    if (!result) {
        TCOMPort_Close(self);  // call 0x447500
    }

    return result;
}

/* ============================================================
 *  ПРОТОКОЛ: FlashDump (опкод 0x9A)
 *  Адрес: 0x00424BB0
 * ============================================================ */

DWORD __fastcall Protocol_FlashDump(TCOMPort* port, BYTE opcode, 
                                     BYTE* outBuf, DWORD expectedSize)
{
    // port в EAX, opcode в DL, outBuf в ECX, expectedSize на стеке [ebp+8]
    DWORD result = 0;

    if (!TCOMPort_IsOpen(port))
        goto error_not_open;

    // Отправляем опкод (0x9A)
    BYTE cmd = opcode;
    TCOMPort_WriteByte(port, &cmd);      // call 0x4480FB

    // Настраиваем FormProgress
    TFormProgress* fp = *((TFormProgress**)0x53AF68);  // глобальный указатель
    fp->active       = 1;           // +0x2F8 = 1
    fp->bufferPtr    = (DWORD)outBuf;  // +0x300
    fp->expectedSize = expectedSize;   // +0x2FC

    // Показываем форму прогресса (вызов виртуального метода ShowModal)
    fp->vtable->ShowModal(fp);       // call [edx+0xD8]

    // Получаем результат
    result = fp->receivedSize;       // +0x304

    if (result != expectedSize) {
        // Ошибка: не все данные получены
        ShowError("Ошибка приёма Flash-блока");
        TCOMPort_Close(port);
        return result;
    }

    return result;

error_not_open:
    ShowError("Порт не открыт!");
    return 0;
}

/* ============================================================
 *  ПРОТОКОЛ: BlockRequest (опкод 0x42)
 *  Адрес: 0x00424CC0
 * ============================================================ */

DWORD __fastcall Protocol_BlockRequest(TCOMPort* port, WORD blockAddr,
                                        BYTE* outBuf, DWORD expectedSize)
{
    // port в EAX, blockAddr в DX, outBuf в ECX, expectedSize на стеке
    DWORD result = 0;

    if (!TCOMPort_IsOpen(port))
        goto error_not_open;

    // 1. Отправляем команду 0x42
    BYTE cmd = 0x42;
    TCOMPort_WriteByte(port, &cmd);

    // 2. Задержка 10 мс
    TCOMPort_Delay(port, 10);        // call 0x447D62

    // 3. Отправляем младший байт адреса
    cmd = (BYTE)(blockAddr & 0xFF);
    TCOMPort_WriteByte(port, &cmd);

    // 4. Задержка 10 мс
    TCOMPort_Delay(port, 10);

    // 5. Отправляем старший байт адреса
    cmd = (BYTE)(blockAddr >> 8);
    TCOMPort_WriteByte(port, &cmd);

    // 6. Задержка 10 мс
    TCOMPort_Delay(port, 10);

    // 7. Запускаем FormProgress для приёма
    TFormProgress* fp = *((TFormProgress**)0x53AF68);
    fp->active       = 1;
    fp->bufferPtr    = (DWORD)outBuf;
    fp->expectedSize = expectedSize;
    fp->vtable->ShowModal(fp);

    result = fp->receivedSize;

    // Ещё одна задержка 10мс после приёма
    TCOMPort_Delay(port, 10);

    if (result != expectedSize) {
        ShowError("Ошибка приёма блока");
        TCOMPort_Close(port);
    }

    return result;

error_not_open:
    ShowError("Порт не открыт!");
    return 0;
}

/* ============================================================
 *  ПРОТОКОЛ: Handshake (опкод 0x55)
 *  Адрес: 0x00424A50 (примерно)
 * ============================================================ */

BOOL __fastcall Protocol_Handshake(TCOMPort* port, BYTE* headerBuf)
{
    BYTE cmd = 0x55;
    BYTE rxBuf[0x80010];  // 512KB + 16
    BOOL success = FALSE;

    TCOMPort_WriteByte(port, &cmd);

    DWORD received = TCOMPort_ReadBytes(port, rxBuf, 0x80010);

    if (received < 0x11) {  // менее 17 байт = ошибка
        ShowError("Handshake failed: too short response");
        return FALSE;
    }

    // Копируем 16-байтовый заголовок
    memcpy(headerBuf, rxBuf, 0x10);

    // Извлекаем флаги версии прибора (байт 2)
    // DAT_0053E97C = rxBuf[2]

    // Вычисляем количество элементов A-scan
    // numElements = (received - 0x10) / 2 - 1

    success = TRUE;
    return success;
}

/* ============================================================
 *  COM-ПОРТ: Задержка (busy-wait через GetTickCount)
 *  Адрес: 0x00423A56 (Delay function)
 * ============================================================ */

void __fastcall TCOMPort_Delay(TCOMPort* self, DWORD milliseconds)
{
    // self в EAX, milliseconds в EDX
    DWORD start = GetTickCount();
    while (GetTickCount() < start + milliseconds) {
        // busy-wait
    }
}

/* ============================================================
 *  АДРЕСАЦИЯ БЛОКОВ
 * ============================================================ */

// Проверка: является ли адрес sweep-адресом (0x0041C7C0)
BOOL IsSweepAddress(int addr)
{
    return (addr > 9999 && addr < 30000) ? TRUE : FALSE;
}

// Базовый адрес (0x0041C7F0)
int GetBaseAddr(int addr) { return (addr / 100) * 100; }

// Подблок (0x0041C810)
int GetSubBlock(int addr) { return (addr % 10000) / 100; }

// Индекс внутри подблока (0x0041C838)
int GetIndex(int addr) { return addr % 100; }

/* ============================================================
 *  DLL: Загрузка dbdll.dll
 * ============================================================ */

typedef int*  (__cdecl *pfn_SortBufData_t)(BYTE* rawPacket, UINT* pSize);
typedef void  (__cdecl *pfn_FreeBuffer_t)(int allocatedPtr);
typedef void  (__cdecl *pfn_Form_View_t)(void* param1, BYTE* data, ULONG size);

pfn_SortBufData_t  pSortBufData;   // @ 0x5BE998
pfn_FreeBuffer_t   pFreeBuffer;    // @ 0x5BE99C
pfn_Form_View_t    pForm_View;     // @ 0x5BE994

void LoadDbDll(const char* dllPath)
{
    HMODULE hMod = LoadLibraryA(dllPath);   // "dbdll.dll"
    if (hMod) {
        pSortBufData = (pfn_SortBufData_t)GetProcAddress(hMod, "_SortBufData");
        pFreeBuffer  = (pfn_FreeBuffer_t)GetProcAddress(hMod, "_FreeBuffer");
        pForm_View   = (pfn_Form_View_t)GetProcAddress(hMod, "_Form_View");
    }
}

/* ============================================================
 *  РЕЕСТР: Чтение настроек
 * ============================================================ */

// Ключ: HKLM\SOFTWARE\Altek\PelengPC
// Значения: Firma, SubFirma, Label, TekTable, Numbers, CountTables

void ReadRegistry(void)
{
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, 
                      "SOFTWARE\\Altek\\PelengPC", 
                      0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        // Чтение Firma, SubFirma, Label...
        // Чтение TekTable (текущая таблица)
        // Чтение Numbers (массив номеров)
        RegCloseKey(hKey);
    }
}

/* ============================================================
 *  B-SCAN: Рендеринг (FormScreen)
 *  Адрес: 0x004235F8
 * ============================================================ */

void __fastcall FormScreen_RenderBscan(TFormScreen* self, BYTE* rawData, int rawLen)
{
    int bscanLen = rawLen - 0x1C0;  // 448 байт = заголовок A-scan
    
    // Копируем тело B-scan в буфер формы
    memcpy(&self->bscanData[0], rawData + 0x1D0, bscanLen);
    
    int rowBytes = 0x1E;   // 30 байт на строку = 240 бит
    int rows = (bscanLen * 8) / (rowBytes * 8);  // = bscanLen / 30
    
    // Создаём Bitmap: width=480 (240*2), height=rows*2
    self->bitmap->Width  = 0x1E0;  // 480
    self->bitmap->Height = rows * 2;
    
    // Рендеринг: каждый бит = пиксель 2x2
    for (int row = 0; row <= rows; row++) {
        for (int col = 0; col <= rowBytes; col++) {
            BYTE val = self->bscanData[col + row * rowBytes];
            for (int bit = 0; bit < 8; bit++) {
                COLORREF color;
                if ((val >> bit) & 1)
                    color = RGB(0, 0, 0);      // чёрный
                else
                    color = RGB(255, 255, 255); // белый
                
                int x = (col * 8 + bit) * 2;
                int y = row * 2;
                // FillRect 2x2 пикселя
                SetPixel(self->bitmap->Canvas->Handle, x,   y,   color);
                SetPixel(self->bitmap->Canvas->Handle, x+1, y,   color);
                SetPixel(self->bitmap->Canvas->Handle, x,   y+1, color);
                SetPixel(self->bitmap->Canvas->Handle, x+1, y+1, color);
            }
        }
    }
}

/* ============================================================
 *  ГЛАВНАЯ ТАБЛИЦА: SQL-операции с BlockZap
 * ============================================================ */

// SELECT * FROM BlockZap
// SELECT * FROM BlockZap WHERE Number=:Number
// UPDATE BlockZap SET Number=:Number, BlockLen=:BlockLen, Block=:Block WHERE Number=:OLD_Number
// INSERT INTO BlockZap (Number, BlockLen, Block) VALUES (:Number, :BlockLen, :Block)
// DELETE FROM BlockZap WHERE Number=:Number
// SELECT * FROM BlockZap ORDER BY Number

void SaveBlockToDatabase(int number, BYTE* blockData, int blockLen)
{
    // IBTransactionW->StartTransaction()
    // IBDataSetW->ParamByName("Number")->AsInteger = number
    // IBDataSetW->ParamByName("BlockLen")->AsInteger = blockLen  
    // IBDataSetW->ParamByName("Block")->AsBlob = blockData (blockLen bytes)
    // IBDataSetW->ExecSQL()
    // IBTransactionW->Commit()
}

/* ============================================================
 *  ТОЧКА ВХОДА: WinMain (VCL Application)
 *  Entry point: 0x00401000
 * ============================================================ */

// Borland VCL стандартная инициализация:
// 1. BCB runtime init
// 2. Application->Initialize()
// 3. Application->CreateForm(TFormMain, &FormMain)
// 4. Application->CreateForm(TFormProgress, &FormProgress)
// 5. Application->CreateForm(TFormNastr, &FormNastr)
// ... (все 12 форм)
// 6. Application->Run()  // цикл сообщений

/* ============================================================
 *  ОСНОВНЫЕ ОБРАБОТЧИКИ СОБЫТИЙ (FormMain)
 * ============================================================ */

// NReadDataClick — чтение данных из прибора
// NCOMPortClick  — настройка COM порта
// NTestPortClick — тест COM порта (Handshake 0x55)
// NSaveConfigClick — сохранение конфигурации
// NReadZapClick — чтение записи
// NReadDocClick — чтение документа/отчёта
// NDelZapClick — удаление записи
// ToolButtonRaportClick — генерация рапорта (вызов zapis2.exe)
// ToolButtonFromFileClick — загрузка из файла (.fla/.plg)
// NSortirClick — сортировка таблицы
// NViborClick — выборка по критерию
// NSearchClick — поиск
// NSaveExcelClick — экспорт в Excel
// NPrintTableClick — печать таблицы
// NPassWordClick — смена пароля

/* ============================================================
 *  КОНЕЦ ФАЙЛА
 * ============================================================ */
