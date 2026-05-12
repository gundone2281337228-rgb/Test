# PELENG — Реверс графиков и формул (zapis2.exe)

**Дата:** 2026-05-12  
**Источник:** `zapis2.c` (133634 строк, Ghidra decompile zapis2.exe — MFC/VC++ 2003)

---

## 1. Архитектура визуализации A-scan в zapis2.exe

### 1.1 Объект CZapisView (this)

```c
struct CZapisView {
    /* ... MFC CScrollView ... */
    /* +0x24 */ int      dataOffset;       // смещение в глобальном буфере DAT_0051cb58
    /* +0x60 */ void*    dgsWidget;        // DGS-виджет (CFormView?)
    /* +0x84 */ CDC*     memDC;            // off-screen DC
    /* +0x88 */ CBitmap* memBitmap;        // off-screen bitmap
    /* +0x8c */ int      viewHeight;       // высота виджета (px) = 200
    /* +0x90 */ int      viewWidth;        // ширина виджета (px) = 0x118 = 280
    /* +0x98 */ void*    dataDescriptor;   // указатель на структуру данных/настроек
    /* +0x9c */ byte     dualChannel;      // 0=одноканальный, 1=двухканальный
    /* +0xa0 */ float    xScale;           // = viewWidth / 244.0
    /* +0xa4 */ float    yScale;           // = viewHeight / 140.0
    /* +0xac */ float    dgsMultiplier;    // 1.0 (обычный) или 2.0 (вагонный/двухканальный)
};
```

### 1.2 Инициализация масштабов (`FUN_00404000`, строка 6912)

```c
this->viewWidth  = 0x118;           // 280 пикселей
this->viewHeight = 200;             // 200 пикселей
this->xScale = (float)this->viewWidth / 244.0f;   // ≈ 1.1475
this->yScale = (float)this->viewHeight / 140.0f;  // ≈ 1.4286
```

**Почему 244?** — это количество сэмплов A-scan (0xF4 = 244 байта).  
**Почему 140?** — это 127 (max amplitude 0x7F) + 13 (отступ снизу).

### 1.3 Глобальный буфер данных

```
DAT_0051cb58 — указатель на начало сырых данных пакета (body)
```


Данные A-scan: `*(this->dataDescriptor + 4)` → массив 244 байта (амплитуды 0..255).
Данные B-scan: `*(this->dataDescriptor)` → смещение в DAT_0051cb58.

### 1.4 Глобальные параметры оси времени

```c
DAT_0051cb34 = LE16 @ offset 0x14 в пакете;  // TypeVar / tcode прибора
DAT_0051cb30 = byte[0x12] + byte[0x13]*256;  // флаги версии (бит 2 = вагонная, бит 3 = ???)
DAT_0051cb44 = *(float*)(dataDescriptor->dgsParams + 0x14);  // период дискретизации (мкс/отсч.)
_DAT_0051cb3c = (float)(int)LE16(dataDescriptor + 0x40);     // range_start (начало развёртки)
_DAT_0051cb38 = (float)(int)LE16(dataDescriptor + 0x44);     // range_end (конец развёртки)  
_DAT_0051cb40 = (float)(int)byte(dataDescriptor + 0x3c);     // задержка в призме
```

---

## 2. Режимы отображения A-scan (`FUN_00404bb0` — OnDraw)

### 2.1 Диспатчер режимов

```c
// FUN_00404bb0 — CZapisView::OnDraw(CDC* pDC)
int displayMode = 0;
if (*(this->dataDescriptor + 0x1660) != NULL) {
    displayMode = (*(code*)(this->dataDescriptor + 0x1660))(this->dataOffset);
}

switch (displayMode) {
    case 0:  FUN_00405236(this, pDC, 0xd);  break;  // ОБЫЧНАЯ развёртка
    case 1:  FUN_00404fd3(this, pDC, 0xd);  break;  // ОГИБАЮЩАЯ (peak detect)
    case 2:
    case 3:
    case 4:
    case 5:  FUN_00404d59(this, pDC, displayMode, 0xd);  break;  // СТРОБИРОВАННАЯ
}
```

| Код | Режим | Описание |
|-----|-------|----------|
| 0 | Обычная | Линейная развёртка с заполнением (гистограмма или линия) |
| 1 | Огибающая | Пиковый детектор со сглаживанием |
| 2,3 | Стробированная (4 шага) | Шаг сканирования = 4 пикселя |
| 4,5 | Стробированная (2 шага) | Шаг сканирования = 2 пикселя |

---

## 3. Обычная развёртка — `FUN_00405236`

### 3.1 Алгоритм (полная реконструкция)

```c
void CZapisView::DrawNormal(CDC* pDC, int yOffset) {
    byte renderMode = ReadByte(*(dataDescriptor->dgsParams + 8));  // [+8] в DGS
    // renderMode & 1: 0 = линейная (LineTo), 1 = прямоугольная (FillSolidRect)
    
    MoveTo(pDC, 0, ftol(0 * yScale));  // начальная точка
    
    for (int i = 0; i < 0xF3; i++) {  // 243 итерации (сэмплы 0..242)
        int x = round((float)i * this->xScale);
        byte amp = data[i];  // сэмпл (0..255, используется как 0..127 для отрисовки)
        int y = round((float)amp * this->yScale) - this->viewHeight + yOffset;
        
        if ((renderMode & 1) == 0) {
            // ЛИНЕЙНЫЙ режим: LineTo(x, y)
            LineTo(pDC, x, y);
        } else {
            // ПРЯМОУГОЛЬНЫЙ режим: FillSolidRect серым (0x808080)
            int nextX = round((float)(i+1) * xScale);
            int barWidth = abs(x - nextX);
            int barHeight = -y - viewHeight + yOffset;
            CRect rect(x, y, barWidth, barHeight);
            FillSolidRect(pDC, &rect, 0x808080);  // серый
        }
    }
}
```

### 3.2 Формула координат

```
pixel_x = round(sample_index × xScale)
pixel_y = round(amplitude × yScale) - viewHeight + yOffset

где:
  xScale = 280 / 244 ≈ 1.1475
  yScale = 200 / 140 ≈ 1.4286
  viewHeight = 200
  yOffset = 13 (0xd)
  amplitude = byte value from A-scan buffer (0..127 значимых)
```

### 3.3 Стробы (АСД — автоматический сигнализатор дефекта)

После отрисовки A-scan проверяются 2 строба:
```c
// Строб 1: из dataDescriptor->dgsParams
char strobName1 = strings[dgsParams->nameIndex1 + 0x56c + channelIdx*4];
if (strobName1 != "" && bscanWidget != NULL) {
    if (ReadByte(*bscanWidget) == 3) {
        // Рисуем DGS-кривую: FUN_00405c4c(...)
        // Параметры: gate_start, gate_end, strob_mode, channel, dgsWidget
    } else {
        // Рисуем горизонтальную линию порога: FUN_00416bd9(...)
    }
}

// Строб 2: аналогично из dgsParams->nameIndex2 + 0x5bc
```

### 3.4 Зона разметки (горизонтальная линия)

```c
short zoneStart = ReadShort(dataDescriptor + 0x28);  // начало зоны (отсчёты)
short zoneEnd   = ReadShort(dataDescriptor + 0x2c);  // конец зоны (отсчёты)
if (zoneStart - zoneEnd < -1) {
    // Рисуем пунктирную линию (pen: width=1, style=3=PS_DASHDOT, color=0)
    int x1 = round(zoneStart * xScale);
    int x2 = round(zoneEnd * xScale);
    int y_mid = -(viewHeight - yOffset) / 2;
    MoveTo(pDC, x1, y_mid);
    LineTo(pDC, x2, y_mid);
}
```



---

## 4. Огибающая (Peak Detect) — `FUN_00404fd3`

### 4.1 Алгоритм

```c
void CZapisView::DrawEnvelope(CDC* pDC, int yOffset) {
    // Два канала: channel_A = data[1..0xF0], channel_B = data[0xF1..0x1E0]
    // Сглаживание: 3-точечный фильтр (prev, current, next)
    
    int prev_a=0, prev_b=0, curr_a=0, curr_b=0, next_a=0, next_b=0;
    int smooth_a, smooth_b;
    
    for (int i = 0; i < 0xF0; i++) {  // 240 итераций
        prev_a = curr_a;   prev_b = curr_b;
        curr_a = next_a;   curr_b = next_b;
        
        if (i != 0xEF) {  // не последний
            next_a = (int)(char)ReadByte(*dataDescriptor + 1 + i);   // канал A
            next_b = (int)(char)ReadByte(*dataDescriptor + 0xF1 + i); // канал B
        }
        
        // Сглаживание канала A (пиковый детектор):
        smooth_a = curr_b;  // default = current канала B
        if (prev_a < curr_b) {
            smooth_a = (prev_a + curr_b) >> 1;   // среднее с предыдущим
        } else if (next_a < curr_b) {
            smooth_a = (next_a + curr_b) >> 1;   // среднее со следующим
        }
        
        // Сглаживание канала B (аналогично):
        smooth_b = curr_a;
        if (curr_a < prev_b) {
            smooth_b = (prev_b + curr_a) >> 1;
        } else if (curr_a < next_b) {
            smooth_b = (next_b + curr_a) >> 1;
        }
        
        // Координаты:
        int x = round((float)i * xScale);
        int y_a = round((float)(0x7F - smooth_a) * yScale) - viewHeight + yOffset;
        int y_b = round((float)(0x7F - smooth_b) * yScale) - viewHeight + yOffset;
        
        // Рисуем точку/линию
        MoveTo(pDC, x, y_a);
        if (smooth_a == smooth_b) {
            // Одна точка (SetPixel)
            SetPixel(pDC, x, y_a, 0);
        } else {
            // Линия от y_a до y_b
            LineTo(pDC, x, y_b);
        }
    }
}
```

### 4.2 Ключевые моменты

- **Два канала данных:** A (offset 1..240) и B (offset 241..480)
- **Сглаживание:** если текущий пик **ниже** соседей → берётся среднее `(сосед + текущий) / 2`
- **Инверсия Y:** `0x7F - amplitude` (127 = нулевая линия, 0 = максимум)
- **Количество точек:** 240 (0xF0), не 244 — последние 4 отбрасываются

---

## 5. Стробированная развёртка — `FUN_00404d59`

### 5.1 Алгоритм

```c
void CZapisView::DrawGated(CDC* pDC, int mode, int yOffset) {
    int step;
    if (mode == 3 || mode == 2) {
        step = 4;   // крупный шаг (4 пикселя)
    } else {
        step = 2;   // мелкий шаг (2 пикселя)
    }
    
    // Цикл 1: Основная кривая (0..0xF4 = 244 точек)
    MoveTo(pDC, 0, 0);
    for (int i = 0; i < 0xF4; i++) {
        int x = ftol(...);
        int y = ftol(...);
        LineTo(pDC, x, y);
    }
    
    // Расчёт порога (threshold line):
    int threshold_y = step - 0x8C + yOffset;  // 0x8C = 140
    ReadShort(dataDescriptor + 0x10);  // параметр порога
    
    // Цикл 2: Строб (шаг 4 пикселя, 0..0xF2 = 242 точек)
    for (int i = 0; i < 0xF2; i += 4) {
        int x = ftol(...);
        int y = ftol(...);
        MoveTo(pDC, x, y);  // пунктир: рисуем отрезками
        LineTo(pDC, next_x, next_y);
    }
    
    // Цикл 3: Порог (шаг 12 пикселей, 0..0xEE)
    for (int i = 0; i < 0xEE; i += 0xC) {
        // Пунктирная горизонтальная линия порога
        MoveTo(pDC, x1, y);
        LineTo(pDC, x2, y);
    }
}
```

---

## 6. АРД-диаграмма (DGS) — формулы

### 6.1 Обзор

АРД (Амплитуда-Расстояние-Диаметр) / DGS (Distance-Gain-Size) — ключевая
диаграмма для оценки эквивалентного размера дефекта. В zapis2.exe реализована
через цепочку функций:

```
FUN_00405c4c (рендеринг кривых)
  └── FUN_00405bff (dB → линейная амплитуда)
       └── FUN_00405b78 (нормированная АРД с коррекцией)
            └── FUN_00405abc (абсолютное затухание)
                 └── FUN_004058df (кусочная АРД-формула ближней/дальней зоны)
                      └── FUN_004058a3 (граница зон Френеля)
```

### 6.2 Глобальные переменные DGS

| Адрес | Тип | Имя (реконструкция) | Формула |
|-------|-----|---------------------|---------|
| `DAT_0051c684` | float | `N_eff` | Эффективная длина ближней зоны = `D² × K × λ_rel / 4` |
| `DAT_0051c680` | float | `D_lambda` | `D × √(λ_rel)` — диаметр × корень длины волны |
| `DAT_0051c688` | float | `S_eff` | `S_refl × λ_rel²` — эффективная площадь отражателя |
| `DAT_0051c690` | float | `sample_period` | Период дискретизации (мс/отсч) = LE16(param_6+4)/1000 |
| `DAT_0051c694` | float | `S_scaled` | `attenuation × S_eff` |
| `DAT_0051c698` | float | `S_refl` | Площадь отражателя (мм²): круг=`D×π/4×D`, эллипс=`D×K` |
| `DAT_0051c68c` | float | `ARD_at_gate` | `FUN_004058df(0, gate_pos, N_eff)` |
| `DAT_0051c670` | float | `zone_far` | `FUN_004058a3(3.0, N_eff)` — начало дальней зоны |
| `DAT_0051c66c` | float | `zone_mid` | `FUN_004058a3(1.5, N_eff)` |
| `DAT_0051c674` | float | `zone_near` | `FUN_004058a3(0.7, N_eff)` |
| `DAT_0051c668` | float | `zone_min` | `FUN_004058a3(0.2, N_eff)` |
| `DAT_0051c678` | float | `corr_far` | Коррекция для дальней зоны |
| `DAT_0051c664` | float | `corr_mid` | Коррекция для средней зоны |
| `DAT_0051c660` | float | `corr_near` | Коррекция для ближней зоны |
| `DAT_0051c67c` | int | `param6_ptr` | Указатель на текущую DGS-настройку |

### 6.3 Формула границы зон Френеля — `FUN_004058a3`

```c
float zone_boundary(float factor, float N_eff) {
    // Граница зоны = factor × N_eff, но не менее чем gate_position
    float boundary = factor * N_eff;
    if (boundary < gate_position) {
        return gate_position;   // не меньше позиции строба
    }
    return boundary - gate_position;
}
```

**Физический смысл:** Зоны Френеля делят поле преобразователя на области:
- **Ближняя зона** (r < 0.7·N): интерференционное поле, амплитуда осциллирует
- **Переходная** (0.7·N < r < 3·N): переход от интерференции к дифракции
- **Дальняя зона** (r > 3·N): монотонное убывание по закону 1/r

### 6.4 Кусочная АРД-формула — `FUN_004058df`

```c
float ARD_piecewise(int sub_flag, float distance, float N_eff) {
    float result;
    float r_norm = (distance + N_eff) / N_eff;   // нормированное расстояние
    float D_ratio = 2.0 * sqrt(attenuation) / D_lambda;
    
    if (distance > zone_mid || (distance == zone_mid && sub_flag != 0)) {
        // === ДАЛЬНЯЯ ЗОНА (r > 1.5·N) ===
        result = log10(distance) * 20.0;    // 20·lg(r)
        
        if (distance <= zone_far || (distance == zone_far && sub_flag == 0)) {
            // Промежуточная (1.5N..3N): коррекция
            result = r_norm * (-1.43) + 3.86 +
                     (result - (r_norm * (-10.04) + 29.83) * D_ratio);
        }
        // Если r > 3N — чистый 20·lg(r)
    } else {
        // === БЛИЖНЯЯ ЗОНА (r < 1.5·N) ===
        if (distance < zone_near || (distance == zone_near && sub_flag == 0)) {
            // Совсем близко (r < 0.7·N):
            result = D_ratio * 159.0 +
                     (r_norm * 19.1 * r_norm - r_norm * 7.2) -
                     D_ratio * 176.0 * D_ratio - 49.4;
        } else {
            // Переходная (0.7N..1.5N):
            result = D_ratio * 180.0 +
                     (r_norm * 35.8 + r_norm * (-17.1) * r_norm) -
                     D_ratio * 214.0 * D_ratio - 63.5;
        }
    }
    return result;
}
```

### 6.5 Физическая интерпретация

| Зона | Условие | Формула | Смысл |
|------|---------|---------|-------|
| Дальняя | r > 3N | `20·lg(r)` | Классическое затухание в дальней зоне |
| Промежуточная | 1.5N < r < 3N | Полином + `D_ratio` коррекция | Переход от ближней к дальней |
| Переходная | 0.7N < r < 1.5N | Квадратичный полином | Зона максимума давления |
| Ближняя | r < 0.7N | Другой полином с D² | Осцилляции в ближнем поле |

**Коэффициенты полиномов** (железобетонно из дизасма):
- Дальняя промежуточная: `-1.43·r + 3.86`, `(-10.04·r + 29.83)·D_ratio`
- Переходная: `35.8·r - 17.1·r² + 180·D - 214·D² - 63.5`
- Ближняя: `19.1·r² - 7.2·r + 159·D - 176·D² - 49.4`



### 6.6 Абсолютное затухание — `FUN_00405abc`

```c
float ARD_absolute(float distance, int sub_flag, float gate_pos, float speed, float N_eff) {
    float path = (distance - gate_pos) * speed;
    exp(path);  // FUN_00431b40 = exp() — затухание в материале
    
    float result;
    if (distance > zone_far || (distance == zone_far && sub_flag != 0)) {
        // Дальняя зона: 20·lg(distance)
        result = log10(distance) * 20.0;
    } else {
        // Ближняя/переходная: кусочная формула
        log10(distance);  // не используется напрямую (?)
        float ard = ARD_piecewise(sub_flag, distance, N_eff);  // FUN_004058df
        result = (float)distance - ard;  // ???? (возможно distance_dB - correction)
    }
    return result;
}
```

### 6.7 Нормированная АРД с коррекцией — `FUN_00405b78`

```c
float ARD_normalized(float distance, float gate_pos, float speed, float N_eff) {
    float raw = ARD_absolute(distance, 0, gate_pos, speed, N_eff);
    
    // Кусочная коррекция по зонам:
    if (distance > zone_near) {       // r > 0.7·N
        if (distance > zone_mid) {    // r > 1.5·N
            if (distance <= zone_far) {  // 1.5N < r < 3N
                raw -= corr_far;     // DAT_0051c678
            }
            // r > 3N — без коррекции
        } else {
            raw -= corr_mid;          // DAT_0051c664
        }
    } else {
        raw -= corr_near;             // DAT_0051c660
    }
    return raw;
}
```

### 6.8 дБ → линейная амплитуда — `FUN_00405bff`

```c
void dB_to_linear(float distance, float gate_pos, float speed, float N_eff) {
    float dB = ARD_normalized(distance, gate_pos, speed, N_eff);
    exp(dB * 0.115129f);   // 0.115129 = ln(10)/20
    // Результат: 10^(dB/20) — преобразование дБ в линейную амплитуду
}
```

**Формула:** `amplitude_linear = 10^(dB/20) = exp(dB × ln(10)/20) = exp(dB × 0.115129)`

---

## 7. Рендеринг DGS-кривых — `FUN_00405c4c`

### 7.1 Входные параметры (param_6 = DGS-настройка)

```c
struct DGS_Settings {          // param_6 в FUN_00405c4c
    /* +0x00 */ ...
    /* +0x04 */ int16  sample_rate_raw;   // LE16: период дискрет. ×1000
    /* +0x08 */ float  sqrt_lambda;       // √(длина волны)
    /* +0x0C */ float  half_wavelength;   // λ/2
    /* +0x10 */ int16  gate_period;       // период строба (отсчёты)
    /* +0x14 */ int16  sweep_length;      // длина развёртки (отсчёты)
    /* +0x18 */ int16  gate_start;        // начало строба (отсчёты)
    /* +0x1C */ int16  division_mode;     // режим деления (1,2,4,8)
    /* +0x20 */ byte   reflector_shape;   // 0=круг, ≠0=эллипс
    /* +0x24 */ byte   reflector_diam;    // диаметр отражателя D (мм)
    /* +0x28 */ byte   reflector_diam2;   // второй диаметр (для эллипса)
    /* +0x2C */ float  attenuation_coeff; // коэф. затухания (дБ/мм?)
    /* +0x30 */ byte   channel_index;     // индекс канала
    /* +0x34 */ float  unknown_34;        // доп. параметр (расчётный)
    /* +0x38 */ int16  ARD_range;         // диапазон АРД (отсчёты)
    /* +0x3C */ void*  callback_func;     // указатель на callback
    /* +0x40 */ code*  compute_func;      // вызов: (*compute_func)()
    /* +0x56C+i*4 */ char* strobNames1[]; // имена стробов канал 1
    /* +0x5BC+i*4 */ char* strobNames2[]; // имена стробов канал 2
    /* +0x66C */ byte   version_flag;     // 0x02 = вагонная (×2 масштаб)
};
```

### 7.2 Инициализация DGS (полный алгоритм)

```c
void CZapisView::InitDGS(CDC* pDC, int gate_start, int gate_end,
                          int strob_mode, int channel, int* dgsParams) {
    
    // 1. Сохраняем глобальный указатель
    DAT_0051c67c = dgsParams;  // param_6
    
    // 2. Читаем параметры преобразователя
    float D = (float)(int)ReadByte(dgsParams + 0x30);    // диаметр (мм)
    float sample_period = (float)ReadShort(dgsParams + 4) / 1000.0f;  // мс/отсч
    
    // 3. Вычисляем длину волны
    float sqrt_lam = ReadFloat6(dgsParams + 0x0C);  // 6-byte Delphi float!
    float wavelength = sqrt_lam + sqrt_lam;  // λ = 2 × (λ/2)
    
    // 4. Вычисляем ближнюю зону (формула Бергмана)
    //    N_eff = f × S × λ_inv / (4π × С_scale)
    float gate_sweep = (float)ReadShort(dgsParams + 0x10);
    float N_eff_raw = ReadFloat6(dgsParams + 8) * gate_sweep * 7.129f;
    N_eff_raw = N_eff_raw / wavelength;
    N_eff_raw = N_eff_raw / DAT_0051cb44;     // делим на период дискрет.
    float N_eff = N_eff_raw * this->dgsMultiplier;  // ×2 для вагонной
    
    // 5. Позиция строба (gate position в пикселях)
    float gate_pos = ((float)ReadShort(dgsParams + 0x38) * sample_period / 2.0f)
                     / DAT_0051cb44 * this->dgsMultiplier;
    
    // 6. Начало развёртки (в пикселях)
    short div_mode = ReadShort(dgsParams + 0x1C);
    float sweep_start = (((float)ReadShort(dgsParams + 0x14) * sample_period)
                         / (float)(8 >> (3 - (div_mode & 0x1f)))
                         / 2.0f) / DAT_0051cb44 * this->dgsMultiplier;
    
    // 7. Диаметр отражателя и форм-фактор
    float reflD = (float)(int)ReadByte(dgsParams + 0x24);
    byte shape = ReadByte(dgsParams + 0x20);
    float K;  // форм-фактор
    
    if (shape == 0) {
        // КРУГ: S = π/4 × D²
        K = 1.0f;
        S_refl = D * 0.785398f * D;   // π/4 ≈ 0.785398
    } else {
        // ЭЛЛИПС: с коррекцией по соотношению сторон
        float D2 = (float)(int)ReadByte(dgsParams + 0x28);
        float ratio;
        if (reflD <= D2) {
            ratio = reflD / D2;
        } else {
            ratio = D2 / reflD;
        }
        // Аппроксимация коэф. формы эллипса:
        K = ratio * 0.6f * ratio * ratio +
            (ratio * 0.07f + 1.0f) - ratio * 0.37f * ratio;
        S_refl = reflD * K;
    }
    
    // 8. Вычисляем промежуточные глобальные
    DAT_0051c690 = sample_period;
    float lambda_rel = (*dgsParams->compute_func)();  // вызов callback
    float sqrt_wavelength = sqrt(lambda_rel);
    DAT_0051c680 = sqrt_wavelength * D;                          // D·√λ
    lambda_rel = lambda_rel / sample_period;                      // λ_rel
    DAT_0051c688 = S_refl * lambda_rel * lambda_rel;             // S×λ²
    DAT_0051c684 = (D * D * K * lambda_rel) / 4.0f;             // N_eff
    
    // 9. Границы зон Френеля
    zone_far  = zone_boundary(3.0, N_eff);   // 3·N
    zone_mid  = zone_boundary(1.5, N_eff);   // 1.5·N
    zone_near = zone_boundary(0.7, N_eff);   // 0.7·N
    zone_min  = zone_boundary(0.2, N_eff);   // 0.2·N
    
    // 10. Коэффициент затухания
    float atten = ReadFloat6(dgsParams + 0x34);
    DAT_0051c694 = atten * S_eff;
    
    // 11. Коррекции для зон
    DAT_0051c68c = ARD_piecewise(0, gate_pos, N_eff);
    float speed = ReadFloat6(dgsParams + 0x2C);
    
    float raw_far  = ARD_absolute(zone_far, 0, gate_pos, speed, N_eff);
    float raw_far1 = ARD_absolute(zone_far, 1, gate_pos, speed, N_eff);
    corr_far = raw_far - raw_far1;
    
    float raw_mid  = ARD_absolute(zone_mid, 0, gate_pos, speed, N_eff);
    float raw_mid1 = ARD_absolute(zone_mid, 1, gate_pos, speed, N_eff);
    corr_mid = (raw_mid + corr_far) - raw_mid1;
    
    float raw_near  = ARD_absolute(zone_near, 0, gate_pos, speed, N_eff);
    float raw_near1 = ARD_absolute(zone_near, 1, gate_pos, speed, N_eff);
    corr_near = (raw_near + corr_mid) - raw_near1;
    
    // 12. Нормировка
    float base_val = ARD_normalized(gate_pos, gate_pos, speed, N_eff);
    corr_far  += base_val;
    corr_mid  += base_val;
    corr_near += base_val;
    
    // 13. Минимальная амплитуда (для отсечки шумов)
    float min_amp = dB_to_linear(zone_min, gate_pos, speed, N_eff);
    
    // 14. Позиция начала строба в пикселях
    short gs = ReadShort(dgsParams + 0x18);
    short sl = ReadShort(dgsParams + 0x14);
    short dm = ReadShort(dgsParams + 0x1C);
    float strob_start = ((float)(gs + (gate_end * sl) / (8 >> (3-(char)dm & 0x1f)))
                         * sample_period / 2.0f) / DAT_0051cb44 * this->dgsMultiplier;
    
    // 15. РИСОВАНИЕ КРИВОЙ (от gate_end вниз до gate_start)
    float step = sweep_start;  // шаг (пиксели за отсчёт)
    
    for (int i = gate_end; i >= gate_start; i--) {
        float amplitude;
        if (strob_start >= zone_min) {
            amplitude = dB_to_linear(strob_start, gate_pos, speed, N_eff);
        } else {
            amplitude = min_amp;  // ограничение снизу
        }
        
        int pixel_y = ftol(...);   // преобразование в координаты
        if (pixel_y > -1) pixel_y = -1;  // ограничение сверху
        
        int pixel_x = ftol(...);
        int pixel_y2 = ftol(...);
        
        if (i < gate_end) {
            LineTo(pDC, pixel_x, pixel_y2);
        } else {
            MoveTo(pDC, pixel_x, pixel_y2);
        }
        
        strob_start -= step;  // движемся назад по времени
    }
    
    // 16. Стрелки на концах кривой (маркеры начала/конца)
    switch (strob_mode) {  // param_5
        case 0: case 3: case 4:
            // Стрелки на обоих концах (◄►)
            DrawArrow(pDC, gate_start, y_start, -4, ±4);
            DrawArrow(pDC, gate_end, y_end, +3, ±4);
            break;
        case 1: case 2:
            // Стрелки в обратном порядке (►◄)
            DrawArrow(pDC, gate_end, y_end, -4, ±4);
            DrawArrow(pDC, gate_start, y_start, +3, ±4);
            break;
    }
}
```



---

## 8. B-scan рендеринг в zapis2.exe

### 8.1 Функция `FUN_00408... (строка ~8920)` — OnDraw для B-scan

```c
void CZapisBScanView::OnDraw(CDC* pDC) {
    // Битовая маска для извлечения бит из байта:
    byte mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    
    // Двойной цикл: 240 столбцов × 120 строк
    for (int col = 0; col < 0xF0; col++) {       // 240 бит по горизонтали
        for (int row = 0; row < 0x78; row++) {   // 120 строк
            // Читаем байт из B-scan данных:
            int byteOffset = (col + ((col >> 31) & 7)) >> 3;  // col / 8 (с учётом знака)
            byte val = ReadByte(*dataDescriptor + byteOffset + row * 0x1E);  // 30 байт/строка
            
            // Извлекаем нужный бит:
            int bitIdx = col & 7;  // col % 8
            if (bitIdx < 0) bitIdx = (bitIdx - 1 | ~7) + 1;  // коррекция отрицательных
            
            COLORREF color;
            if ((val & mask[bitIdx]) == 0) {
                color = 0xFFFFFF;  // WHITE — нет эхо
            } else {
                color = 0x000000;  // BLACK — есть эхо
            }
            
            // Рисуем пиксель (с масштабированием xScale × yScale):
            int x1 = ftol(col * xScale);
            int y1 = ftol(row * yScale_b);
            int x2 = ftol((col+1) * xScale);
            int y2 = ftol((row+1) * yScale_b);
            CRect rect(x1, y1, x2, y2);
            FillSolidRect(pDC, &rect, color);
        }
    }
    
    // Рамка (белая линия по периметру):
    SelectObject(pDC, WHITE_PEN);
    // ... MoveTo/LineTo по 5 точкам (прямоугольник)
}
```

### 8.2 Параметры B-scan

| Параметр | Значение | Источник |
|----------|----------|----------|
| Ширина данных | 30 байт/строка (0x1E) | Из дизасма: `row * 0x1E` |
| Бит/пиксель | 1 бит = 1 точка | Маска `mask[col%8]` |
| Столбцов | 240 (0xF0) | Внешний цикл |
| Строк | 120 (0x78) | Внутренний цикл |
| Цвета | BLACK=эхо, WHITE=пусто | `0x000000` / `0xFFFFFF` |
| Масштаб | FillSolidRect с xScale×yScale | Пиксели > 1:1 |

**Примечание:** В zapis2.exe B-scan рисуется 240×120, а не 240×128/160 как в PelengPC.exe. Это связано с тем, что zapis2 показывает только «рабочую область» без заголовка.

---

## 9. Отображение строба — `FUN_00416bd9`

### 9.1 Алгоритм (горизонтальная линия + стрелки)

```c
void DrawGateLine(CDC* pDC, int param_start, int param_end, int param_level, int param_dir) {
    // Читаем координаты из DGS-параметров:
    int x_start = ftol(ReadShort(param_start) * xScale);
    int x_end   = ftol(ReadShort(param_end) * xScale);
    int y_level = ftol(ReadByte(param_level) * yScale);  // уровень строба
    
    // Рисуем горизонтальную линию строба:
    MoveTo(pDC, x_start, y_level);
    LineTo(pDC, x_end, y_level);
    
    // Стрелки на концах (direction = ReadByte(param_dir)):
    switch (direction) {
        case 0: case 3: case 4:
            // Стрелки ◄► (начало влево, конец вправо)
            // Начало:
            MoveTo(pDC, x_start, y_level);
            LineTo(pDC, x_start - 4, y_level - 4);
            MoveTo(pDC, x_start, y_level);
            LineTo(pDC, x_start - 4, y_level + 4);
            // Конец:
            MoveTo(pDC, x_end - 1, y_level);  // -1 для точного позиционирования
            LineTo(pDC, x_end + 3, y_level - 4);
            MoveTo(pDC, x_end - 1, y_level);
            LineTo(pDC, x_end + 3, y_level + 4);
            break;
            
        case 1: case 2:
            // Стрелки ►◄ (начало вправо, конец влево)
            // Конец:
            MoveTo(pDC, x_end, y_level);
            LineTo(pDC, x_end - 4, y_level - 4);
            MoveTo(pDC, x_end, y_level);
            LineTo(pDC, x_end - 4, y_level + 4);
            // Начало:
            MoveTo(pDC, x_start - 1, y_level);
            LineTo(pDC, x_start + 3, y_level - 4);
            MoveTo(pDC, x_start - 1, y_level);
            LineTo(pDC, x_start + 3, y_level + 4);
            break;
    }
}
```

---

## 10. Вспомогательные функции

### 10.1 `FUN_00406920` — round(float → int)

```c
int round_float(float x) {
    int trunc = (int)x;              // отбрасываем дробную часть
    float frac = fabs(x - trunc);    // FUN_0043185f = fabs()
    if (frac > 0.5f) {
        if (trunc < 1) trunc--;      // отрицательные: округляем вниз
        else           trunc++;      // положительные: округляем вверх
    }
    return trunc;
}
```

### 10.2 `FUN_00423253` — 6-byte Delphi Real48 → IEEE double

```c
double ReadFloat6(int offset) {
    // Delphi Real48 format (6 bytes):
    // byte[0] = exponent (biased by 129)
    // byte[1..5] = 40-bit mantissa (MSB in byte[5] bit 7 = sign)
    
    byte data[6];
    memcpy(data, DAT_0051cb58 + offset, 6);
    
    if (all_zero(data)) return 0.0;
    
    // Преобразование в IEEE 754 double:
    uint sign = (data[5] & 0x80) << 24;
    uint exponent = (data[0] - 2) * 0x100000;  // re-bias: Real48 bias=129, double bias=1023
    // ... сборка мантиссы из байт 1..5 ...
    
    return (double)(sign | exponent | mantissa);
}
```

**Ключевое:** Настройки DGS хранятся в формате Delphi Real48 (6 байт), а не IEEE float/double!

### 10.3 `FUN_004231f3` — ReadByte из буфера

```c
byte ReadByte(int offset) {
    byte result;
    memcpy(&result, DAT_0051cb58 + offset, 1);
    return result;
}
```

### 10.4 `FUN_004231c9` — ReadShort (LE16) из буфера

```c
short ReadShort(int offset) {
    short result;
    memcpy(&result, DAT_0051cb58 + offset, 2);
    return result;  // Little-Endian
}
```

### 10.5 `FUN_00431a84` — log10(x)

CRT `log10()` — используется в АРД для расчёта затухания в дБ.

### 10.6 `FUN_004319c4` — sqrt(x)

CRT `sqrt()` — используется для вычисления `D × √λ`.

### 10.7 `FUN_00431b40` — exp(x)

CRT `exp()` — используется в `dB_to_linear`: `exp(dB × 0.115129)` = `10^(dB/20)`.

---

## 11. Карта данных A-scan в памяти

### 11.1 Структура dataDescriptor (this+0x98)

```c
struct AScanData {
    /* +0x00 */ int      bodyOffset;        // смещение тела в DAT_0051cb58
    /* +0x04 */ byte*    samples;           // массив 244 байт (амплитуды A-scan)
    /* +0x08 */ int16    gate1_start;       // начало строба 1 (отсчёты)
    /* +0x0C */ int16    gate1_end;         // конец строба 1
    /* +0x10 */ int16    gate1_level;       // уровень строба 1 (0..127)
    /* +0x14 */ byte     gate1_channel;     // канал строба 1
    /* +0x18 */ int16    gate2_start;       // начало строба 2
    /* +0x1C */ int16    gate2_end;         // конец строба 2
    /* +0x20 */ int16    gate2_level;       // уровень строба 2
    /* +0x24 */ byte     gate2_channel;     // канал строба 2
    /* +0x28 */ int16    zone_start;        // начало зоны разметки (отсчёты)
    /* +0x2C */ int16    zone_end;          // конец зоны разметки
    /* +0x38 */ int16    ARD_range;         // диапазон АРД-кривой
    /* +0x3C */ byte     prism_delay;       // задержка в призме
    /* +0x40 */ int16    range_start;       // начало развёртки (мкс)
    /* +0x44 */ int16    range_end;         // конец развёртки (мкс)
    /* +0x163C */ void*  dgsParams;         // указатель на DGS_Settings (672 байт)
    /* +0x1630 */ int    hasBscan;          // флаг B-scan данных
    /* +0x165C */ void*  bscanWidget;       // указатель на B-scan рендерер
    /* +0x1660 */ code*  getDisplayMode;    // callback: определяет режим отображения
};
```

### 11.2 Формат сэмплов

- **Размер:** 244 байта (0xF4)
- **Диапазон значений:** 0..255 (unsigned byte)
- **Для отрисовки:** используются как 0..127 (0x7F = максимум экрана)
- **Инверсия Y:** `pixel_y = (0x7F - amplitude) × yScale`
- **Два канала** (в режиме огибающей): канал A @ offset 1, канал B @ offset 0xF1

---

## 12. Сводка новых находок (раунд 4 — графики)

| # | Находка | Статус | Источник |
|---|---------|--------|----------|
| 45 | A-scan: 244 сэмпла, viewWidth=280px, viewHeight=200px | 🔒 Железобетонно | FUN_00404000 |
| 46 | xScale=280/244≈1.15, yScale=200/140≈1.43 | 🔒 Железобетонно | строка 6914 |
| 47 | 5 режимов отображения: 0=обычный, 1=огибающая, 2-5=стробированный | 🔒 Железобетонно | FUN_00404bb0 |
| 48 | Обычный режим: 2 варианта (линейный LineTo / прямоугольный FillSolidRect 0x808080) | 🔒 Железобетонно | FUN_00405236 |
| 49 | Огибающая: 3-точечный сглаживатель (prev,curr,next), 240 точек, 2 канала | 🔒 Железобетонно | FUN_00404fd3 |
| 50 | Стробированная: шаг 4px (mode 2,3) или 2px (mode 4,5), пунктирный порог | 🔒 Железобетонно | FUN_00404d59 |
| 51 | АРД: кусочная формула с 4 зонами (0.2N, 0.7N, 1.5N, 3N) | 🔒 Железобетонно | FUN_004058df |
| 52 | АРД коэффициенты: -1.43, 3.86, -10.04, 29.83, 19.1, -7.2, 159, -176, -49.4, 35.8, -17.1, 180, -214, -63.5 | 🔒 Железобетонно | FUN_004058df |
| 53 | dB→linear: exp(dB×0.115129) = 10^(dB/20) | 🔒 Железобетонно | FUN_00405bff |
| 54 | Площадь отражателя: круг=π/4×D², эллипс=D×K(ratio) | 🔒 Железобетонно | FUN_00405c4c |
| 55 | Форм-фактор эллипса: K = 0.6r³ + (0.07r+1) - 0.37r² | 🔒 Железобетонно | FUN_00405c4c |
| 56 | N_eff = f×S×7.129/λ/period × dgsMultiplier | 🔒 Железобетонно | FUN_00405c4c |
| 57 | Настройки DGS в Real48 (6 байт Delphi), НЕ IEEE float | 🔒 Железобетонно | FUN_00423253 |
| 58 | dgsMultiplier: 1.0 (обычный) / 2.0 (вагонный, флаг 0x66C==2) | 🔒 Железобетонно | FUN_00404000 |
| 59 | B-scan в zapis2: 240×120 точек, 30 байт/строка, 1 бит/пиксель | 🔒 Железобетонно | ~строка 8920 |
| 60 | Строб: горизонтальная линия + стрелки ◄► (4px), direction-dependent | 🔒 Железобетонно | FUN_00416bd9 |
| 61 | Зона разметки: пунктирная линия (PS_DASHDOT) по середине высоты | 🔒 Железобетонно | FUN_00405236 |
| 62 | round_float: отбрасываем дробь, если >0.5 → ceiling/floor | 🔒 Железобетонно | FUN_00406920 |

---

## 13. Что ещё осталось непокрыто

1. **Callback `getDisplayMode` (dataDescriptor+0x1660)** — что определяет режим 0..5? Вероятно связано с TypeVar.
2. **Полные DGS-таблицы** из param_6 — нужно реверсить вызов `(*compute_func)()` по адресу param_6+0x40.
3. **CxImage рендеринг** — как генерируется PNG для WordML из off-screen DC.
4. **Печать/экспорт** — как A-scan конвертируется в `protPict.png` для отчёта.
5. **Полная карта DGS_Settings** (672+ байт) — есть ещё поля после +0x66C.



---

## 14. Декодер значений полей отчёта — `FUN_0042413f`

### 14.1 Структура записи поля (stride = 28 байт = 7 int'ов)

```c
struct FieldDescriptor {       // param_2 + param_3 * 0x1C
    /* +0x00 */ int count;      // количество полей (только в [0])
    /* +0x04 */ char* label;    // [i*7+1] имя метки ("Дата:", "Время:", ...)
    /* +0x08 */ int   offset;   // [i*7+2] смещение данных в буфере (или packed: HIWORD|LOWORD)
    /* +0x0C */ byte  type;     // [i*7+3] тип поля (код декодирования)
    /* +0x10 */ char* editName; // [i*7+4] имя CEdit-контрола ("no_edit", "defekt_edit", ...)
    /* +0x14 */ code* transform;// [i*7+5] callback-трансформация (или NULL)
    /* +0x18 */ code* validate; // [i*7+6] callback-валидация (или NULL)
    /* +0x1C */ code* visible;  // [i*7+7] callback-видимости (если !=0 → вызывается)
};
```

### 14.2 Полная таблица типов полей (из switch в FUN_0042413f)

| Тип (hex) | Чтение | Формат вывода | Описание |
|-----------|--------|---------------|----------|
| 0x02 | ReadFloat6(offset) | "%f" | Дробное число (Real48) |
| 0x03 | ReadFloat6(offset) | "%f" (формат 2) | Дробное число (альт. формат) |
| 0x04 | ReadShort(offset) | "%d" | LE16 целое (с optional callback) |
| 0x06 | ReadByte[5](offset) | "%2d.%2d.%2d %2d:%2d" | Дата+время (5 байт) |
| 0x0D | ReadByte(offset) | "%d" | Byte целое (с optional callback) |
| 0x10 | ReadShort(offset) | (формат из DAT) | LE16 (формат 3) |
| 0x11 | ReadShort(offset) | (формат из DAT) | LE16 (формат 4) |
| 0x16 | ReadFloat6(offset) → sqrt() | (формат из DAT) | √(Real48) |
| 0x24 | GetEditText(editName) | строка | Обычное текстовое поле |
| 0x25 | GetEditText(editName) | строка | Поле-заголовок |
| 0x26 | GetEditText(editName) | строка | Поле с валидацией |
| 0x27 | ReadByte(offset) → callback() | строка | Lookup по таблице (callback → string) |
| 0x28 | FUN_0042409a(offset, 7) | passport | Passport-decode (7 байт, reverse-LUT) |
| 0x2A | FUN_0042409a(offset, 6) | passport | Passport-decode (6 байт) |
| 0x2B | ReadByte(offset) & 8 | "Да"/"Нет" | Boolean flag (бит 3) |
| 0x2C | GetEditText(editName) | строка | Комбо/список (CComboBox) |
| 0x2D | ReadShort(offset) | "%d" | LE16 целое (формат 5) |
| 0x30 | FUN_0042390e(offset) | дата (packed) | Packed date (20-bit decompose) |
| 0x31 | ReadByte(HI) + ReadByte(LO) | условный | Двухбайтовое с порогом (0x7F/0x10) |
| 0x35 | ReadShort(offset) | "%d" | LE16 → int (формат 6) |
| 0x36 | GetEditText(editName) | строка | Числовое поле (CEdit) |
| 0x38 | ReadByte(HIWORD) | byte с инверсией | Если >0x7F → 256-val (знаковый) |
| 0x39 | FUN_0042409a(offset, 12) | passport | Passport-decode (12 байт!) |
| 0x3A | GetEditText(editName) | строка | Расширенное поле |
| 0x3B | FUN_0042409a(offset, 11) | passport | Passport-decode (11 байт) |
| 0x3E | ReadShort(LO) + ReadByte(HI) → callback | float/string | Составное поле (2 источника) |
| 0x3F | ReadByte(LO) + ReadByte(HI) → callback | float/string | Двухбайт с callback |
| 0x42 | ReadFloat6(LO) + ReadByte(HI) | float + условие | Float с порогом (0x10/0x7E) |
| 0x43 | ReadFloat6(LO) → 2×√val | float | Двойной корень Real48 (диаметр?) |

### 14.3 Passport LUT в zapis2.exe (FUN_00423a6b) — **подтверждение**

| Индекс | CP1251 | Символ | Совпадение с DLL |
|--------|--------|--------|-----------------|
| 0x00 | 0x30 | '0' | DLL pos 10 |
| 0x01 | 0x31 | '1' | DLL pos 1 |
| 0x02 | 0x32 | '2' | DLL pos 2 |
| ... | ... | ... | ... |
| 0x09 | 0x39 | '9' | DLL pos 9 |
| 0x0A | 0x20 | ' ' | DLL pos 11 |
| 0x0B | 0x41 | 'A' | DLL pos 81 |
| 0x0C | 0xC1 | 'Б' | DLL pos 16 |
| 0x0D | 0x42 | 'B' | DLL pos 82 |
| 0x0E | 0xC3 | 'Г' | DLL pos 18 |
| 0x0F | 0xC4 | 'Д' | DLL pos 19 |
| 0x10 | 0x45 | 'E' | DLL pos — |
| 0x11 | 0xC6 | 'Ж' | DLL pos 22 |
| 0x12 | 0xC7 | 'З' | DLL pos 23 |
| 0x13 | 0xC8 | 'И' | DLL pos 24 |
| 0x14 | 0xC9 | 'Й' | DLL pos 25 |
| 0x15 | 0x4B | 'K' | DLL pos — |
| 0x16 | 0xCB | 'Л' | DLL pos 27 |
| 0x17 | 0x4D | 'M' | DLL pos — |
| 0x18 | 0x48 | 'H' | DLL pos — |
| 0x19 | 0x4F | 'O' | DLL pos — |
| 0x1A | 0xCF | 'П' | DLL pos 31 |
| 0x1B | 0x50 | 'P' | DLL pos — |
| 0x1C | 0x43 | 'C' | DLL pos — |
| 0x1D | 0xD2 | 'Т' | DLL pos 34 |
| 0x1E | 0xD3 | 'У' | DLL pos 35 |
| 0x1F | 0xD7 | 'Ч' | DLL pos 39 |
| 0x20 | 0xD4 | 'Ф' | DLL pos 36 |
| 0x21 | 0xD5 | 'Х' | DLL pos 37 |
| 0x22 | 0xD6 | 'Ц' | DLL pos 38 |
| 0x23 | 0xD8 | 'Ш' | DLL pos 40 |
| 0x24 | 0xD9 | 'Щ' | DLL pos 41 |
| 0x25 | 0xDA | 'Ъ' | DLL pos 42 |
| 0x26 | 0xDB | 'Ы' | DLL pos 43 |
| 0x27 | 0xDC | 'Ь' | DLL pos 44 |

**Вывод:** zapis2.exe использует **свою** нумерацию LUT (отличную от DLL), но конвертирует в те же CP1251-символы. Смесь латиницы и кириллицы — для «международных» серийных номеров.

---

## 15. Инициализация документа — `FUN_0040da36`

### 15.1 Диспатч функции отображения (callback param_1+0x528)

```c
void InitDocument(int dataDesc) {
    // Выбор функции getDisplayMode (DGS-режим):
    if ((flags & 2)==0 && (flags & 0x40)==0 && tcode!=0x5006 && tcode!=0x5106) {
        dataDesc->getModeFn = FUN_00408f24;  // обычный режим
    } else {
        dataDesc->getModeFn = FUN_00408f89;  // расширенный режим
    }
    
    // Выбор функции рендеринга (callback param_1+0x528):
    if ((flags & 0x40)==0 || (flags & 4)!=0) {
        if ((flags & 0x10)==0) {
            if ((flags & 0x40)==0) {
                switch (tcode & 0xFF) {
                    case 1:  fn = FUN_00409213; break;  // одноканальный A-scan
                    case 3:
                    case 7:  fn = FUN_004090f3; break;  // огибающая
                    case 4:
                    case 6:
                    case 8:  fn = FUN_00409147; break;  // стробированный
                }
            } else {
                if ((tcode & 0xFF) == 3 || (tcode & 0xFF) == 7) {
                    fn = FUN_004090f3;
                } else {
                    fn = FUN_00409147;
                }
            }
        } else if ((flags & 0x20)==0) {
            fn = FUN_0040900f;  // B-scan простой
        } else {
            fn = FUN_00409093;  // B-scan двойной
        }
    } else {
        fn = FUN_0040919b;      // специальный (вагонная?)
    }
    dataDesc->renderFn = fn;
}
```

### 15.2 Карта TypeCode → режим рендеринга

| tcode & 0xFF | Режим | Функция |
|--------------|-------|---------|
| 1 | A-scan одноканальный | `FUN_00409213` |
| 3, 7 | Огибающая (peak detect) | `FUN_004090f3` |
| 4, 6, 8 | Стробированный | `FUN_00409147` |
| (flags & 0x10) без 0x20 | B-scan простой | `FUN_0040900f` |
| (flags & 0x10) с 0x20 | B-scan двойной | `FUN_00409093` |
| (flags & 0x40) без бит 4 | Специальный | `FUN_0040919b` |

### 15.3 Флаги версии (DAT_0051cb30)

| Бит | Маска | Значение |
|-----|-------|----------|
| 1 | 0x02 | Режим «расширенный DGS» |
| 2 | 0x04 | Вагонная версия (multiplier=2.0) |
| 4 | 0x10 | B-scan данные присутствуют |
| 5 | 0x20 | Двойной B-scan (2 канала) |
| 6 | 0x40 | Специальный режим (тестовый?) |
| 16 | 0x10000 | B-scan поля в отчёте (показывать секцию BScan) |

---

## 16. Генерация отчёта WordML (реконструкция)

### 16.1 Структура XML-документа

```xml
<?xml version="1.0"?>
<?mso-application progid='Word.Document'?>
<w:wordDocument ...>
  <w:body>
    <wx:sect>
      <w:sectPr>
        <w:type w:val="continuous"/>
        <w:cols w:num="..."/>
        <w:pgSz w:w="11906" w:h="16838"/>  <!-- A4 portrait -->
      </w:sectPr>
      <w:pict>
        <w:binData w:name="wordml://protPict.png">
          <!-- base64 PNG image of A-scan/B-scan -->
        </w:binData>
        <v:shape style="width:240pt;height:120pt">
          <v:imagedata src="wordml://protPict.png"/>
        </v:shape>
      </w:pict>
    </wx:sect>
  </w:body>
</w:wordDocument>
```

### 16.2 Размеры изображения в отчёте

| Условие | Стиль | Позиционирование |
|---------|-------|-----------------|
| Нет B-scan (`hasBscan==0` и нет флага 0x10000) | `width:240pt;height:120pt` | Обычное (inline) |
| Есть B-scan | `width:240pt;height:120pt;position:absolute` | Абсолютное (плавающее) |

### 16.3 Генерация PNG

```c
// Рендеринг off-screen → CxImage → PNG:
CxImage::Save(bitmap, "temp.png", 2);  // тип 2 = PNG
// После вставки в XML:
DeleteFileA("temp.png");               // удаляем временный файл
```

---

## 17. Обработка полей формы — `FUN_004168de`

### 17.1 Поля, заполняемые в форме

```c
void CZapisView::FillFormFields(int* param_1) {
    // 1. Ограничение высоты:
    if (this->maxHeight < DAT_0051cb28) {
        DAT_0051cb28 = this->maxHeight;
    }
    
    // 2. Поле "imhoOper" (оператор):
    piVar1 = FindField(this, "imhoOper");
    if (piVar1 != NULL) {
        DrawLabel(param_1, x, y, "imhoOper_label");  // метка
        PlaceEdit(this, "imhoOper", position, param_1);  // CEdit
        AdvanceY(this, height + 5);
    }
    
    // 3. Разделительная линия
    DrawLine(param_1, x, y, "---");
    
    // 4. Копируем DGS-параметры (672 байт = 0x19C dwords = 412 int)
    memcpy(auStack_6d0, dataDescriptor->dgsParams, 0x19C * 4);
    
    // 5. Вызов рендеринга DGS-виджета:
    FUN_0040afd1(this->dgsWidget, param_1);  // рисует настройки
    
    // 6. Поле "about" (описание дефекта):
    PlaceEdit(this, "about", position, param_1);
    
    // 7. Если режим печати (не экран):
    if (isPrinting) {
        SetFieldText(this, "imhoOper", param_1);
        SetFieldText(this, "about", param_1);
    }
}
```

### 17.2 Ключевые имена полей формы

| Строковый ID | Назначение |
|-------------|-----------|
| `"numZap"` | Номер записи (протокола) |
| `"imhoOper"` | Имя оператора |
| `"about"` | Описание / заключение по дефекту |

---

## 18. Формула расчёта глубины — `FUN_004238ec`

```c
float depth_from_time(float time_samples) {
    return (_DAT_00515ea0 * time_samples * _DAT_0051cb38) / 2000.0f;
}
```

**Где:**
- `_DAT_00515ea0` — скорость звука в материале (м/с ÷ 1000 = мм/мкс)
- `time_samples` — время пробега (в отсчётах)
- `_DAT_0051cb38` — период дискретизации (мкс/отсч)
- `2000.0` — двукратный путь (туда-обратно) × 1000 (мм→мкм)

**Итоговая формула:** `depth_mm = v_sound × t_samples × dt / 2000`

---

## 19. Packed Date decoder — `FUN_0042390e`

```c
void DecodeDatePacked(uint packed) {
    // Формат packed: bits [19:8]=month+day, bits [7:0]=extra, bits [31:20]=year
    int mid_field = (packed & 0xFFFFF) >> 8;      // биты 8..19
    int low_field = packed & 0xFF;                  // биты 0..7
    int high_field = packed >> 20;                  // биты 20..31
    
    short val1 = ReadShort(mid_field);              // ?
    short val2 = ReadShort(mid_field + low_field);  // ?
    short val3 = ReadShort(high_field);             // ?
    // → sprintf("%d.%d.%d", ...) через __ftol
}
```

---

## 20. Сводка новых находок (раунд 5 — отчёты/TLV)

| # | Находка | Статус | Источник |
|---|---------|--------|----------|
| 63 | Декодер полей: 26+ типов (0x02..0x43), stride 28 байт | 🔒 Железобетонно | FUN_0042413f |
| 64 | Passport в zapis2: своя LUT (отличается от DLL!), смесь lat+cyr | 🔒 Железобетонно | FUN_00423a6b |
| 65 | Passport lengths: 5, 6, 7, 11, 12 байт (разные поля) | 🔒 Железобетонно | switch case 0x28/0x2A/0x39/0x3B |
| 66 | Real48 (6-byte Delphi float) используется для типов 0x02, 0x03, 0x16, 0x42, 0x43 | 🔒 Железобетонно | FUN_00423253 calls |
| 67 | Boolean: тип 0x2B = ReadByte & 0x08 (бит 3) → "Да"/"Нет" | 🔒 Железобетонно | case 0x2B |
| 68 | Тип 0x38: знаковый byte (>0x7F → 256-val) | 🔒 Железобетонно | case 0x38 |
| 69 | Тип 0x43: 2×√(Real48) — двойной корень (= диаметр из площади?) | 🔒 Железобетонно | case 0x43 |
| 70 | Глубина: `v × t × dt / 2000` | 🔒 Железобетонно | FUN_004238ec |
| 71 | WordML: CxImage→temp.png→base64→XML, затем DeleteFile | 🔒 Железобетонно | строки 20433/20455 |
| 72 | Поля формы: "numZap", "imhoOper", "about" — 3 ключевых + массив из dataDescriptor | 🔒 Железобетонно | FUN_004168de |
| 73 | InitDoc: tcode&0xFF → render callback (1=A-scan, 3/7=peak, 4/6/8=gated) | 🔒 Железобетонно | FUN_0040da36 |
| 74 | Флаги: бит1=DGS, бит2=вагонная, бит4=BScan, бит5=dual-BScan, бит6=special | 🔒 Железобетонно | FUN_0040da36 |
| 75 | DGS-параметры: 672 байт (0x19C×4) копируются в стек перед рендером | 🔒 Железобетонно | FUN_004168de |
| 76 | Нет SQL в zapis2.exe — только WordML отчёты, нет Firebird | 🔒 Железобетонно | grep подтвердил |
| 77 | Packed date (тип 0x30): 3 поля из 32-бит числа (bits 0-7, 8-19, 20-31) | 🔒 Железобетонно | FUN_0042390e |

---

## 21. Что осталось непокрыто (обновлённый список)

1. **Полная LUT zapis2** (все 40+ символов `FUN_00423a6b`) — нужно дочитать switch до конца
2. **Callbacks FUN_00408f24..FUN_00409213** — точная логика определения displayMode
3. **FUN_0040afd1** — рендеринг DGS-виджета (отрисовка настроек в форме)
4. **FUN_00421f16** — чтение текста из CEdit по имени (для типов 0x24-0x36)
5. **Полный список строк полей** из dataDescriptor+0x48 и +0xB3C (DFM-ресурсы)
6. **FUN_0042371b** — инициализация шрифта отчёта (Times New Roman, 7pt × 16pt)



---

## 22. Таблицы периодов дискретизации (мкс/отсч)

### 22.1 Полная карта callback'ов (по tcode)

Callback `dataDescriptor+0x528` возвращает период дискретизации по индексу канала:

| Функция | tcode & 0xFF | Режим | Таблица значений (мкс) |
|---------|-------------|-------|------------------------|
| `FUN_00409213` | 1 | A-scan одноканальный | 0→1.25, 1→2.5, 2→5.0, 3→0.4 |
| `FUN_004090f3` | 3, 7 | Огибающая | 0→1.25, 1→1.8, 2→2.5, 3→5.0 |
| `FUN_00409147` | 4, 6, 8 | Стробированный | 0→0.4, 1→1.25, 2→2.5, 3→5.0 |
| `FUN_0040900f` | (flags&0x10) | B-scan простой | 0→0.1, 1→0.4, 2→0.62, 3→1.25, 4→1.8, 5→2.5, 6→5.0, 7→10.0 |
| `FUN_00409093` | (flags&0x10&0x20) | B-scan двойной | 0→1.25, 1→1.8, 2→2.5, 3→5.0, 4→10.0 |
| `FUN_0040919b` | (flags&0x40) | Специальный | 0→0.4, 1→0.62, 2→1.25, 3→1.8, 4→2.5, 5→5.0, 6→10.0 |

### 22.2 Именовательные callback'и

`FUN_00408f24` (обычный режим, по индексу 0..5+):
- Возвращает строку-имя режима из массива `DAT_004aa420..DAT_004aa440`

`FUN_00408f89` (расширенный режим, по индексу 0..8+):
- Возвращает строку-имя из массива `DAT_004aa444..DAT_004aa474`

---

## 23. Декодер DGS-полей — `FUN_0040b7fd` (32 типа)

### 23.1 Структура записи DGS-поля (stride = 12 байт)

```c
struct DGS_FieldEntry {
    /* +0x18 */ char* label;       // имя поля (строка)
    /* +0x1C */ int   dataOffset;  // смещение данных в буфере
    /* +0x20 */ int   fieldType;   // тип декодирования (0..0x20+)
};
// Массив начинается с param_2 + 0x60, записи по stride 0x0C (12 байт)
```

### 23.2 Полная таблица типов DGS-полей

| Тип | Чтение | Результат | Семантика |
|-----|--------|-----------|-----------|
| 0 | строка (label) | spacer every 2 chars | Заголовок (разделитель) |
| 1 | ReadFloat6(offset) | "%.Xf" | Скорость звука (мм/мкс) |
| 2 | ReadFloat6(offset) | "%.Xf" | Частота (МГц) |
| 3 | ReadFloat6(offset) | "%.Xf" | Длина волны (мм) |
| 4 | ReadShort(offset) | "%d" | Целое (отсчёты) |
| 7 | ReadByte(offset) → switch(0,1,2) | строка | Тип преобразователя (0="Круг",1="Прямоуг",2="Эллипс") |
| 8 | FUN_0042409a(offset, 8) | passport | Серийный номер (8 байт LUT) |
| 9 | ReadByte(offset) → switch(0,1) | строка | Boolean ("Вкл"/"Выкл") |
| 0x0A | ReadByte → callback → match | "1.25 МГц"/"X МГц" | Частота по таблице (1.25/0.62 → особый формат) |
| 0x0B | ReadByte(offset) → lookup[+0x4CC] | строка | Имя строба канал 1 |
| 0x0C | ReadByte(offset) → lookup[+0x51C] | строка | Имя строба канал 2 |
| 0x0D | ReadByte(offset) | "%d" | Числовой byte |
| 0x0E | ReadByte → switch(0..3) | строка | Тип отражателя (0/1/2/3 → 4 варианта) |
| 0x0F | ReadByte(offset) | "%d" | Числовой byte (формат 2) |
| 0x10 | ReadShort(offset) | "%d" | Целое LE16 |
| 0x11 | ReadShort(offset) / 10 | "%.1f" | LE16 ÷ 10 (= дробное с 1 дес.знаком!) |
| 0x12 | ReadShort → `f = val × (0xF0 >> mode) / (mult × dt)` → `depth_mm` | "%.Xf мм / %.Xf мм" | **Начало развёртки (мм)** — ключевая формула! |
| 0x13 | ReadShort → `val / dt` → `depth_mm` | "%.Xf мм / %.Xf мм" | **Конец развёртки (мм)** |
| 0x14 | ReadByte → switch(0..4) | строка | Режим деления (×1,×2,×4,×8,×16) |
| 0x15 | ReadByte → switch(0,1) | строка | Режим (Прямой/Теневой) |
| 0x16 | ReadFloat6 → √val | "%.Xf" | √(Real48) — диаметр |
| 0x17 | ReadByte → switch(0,1) | строка | АСД (Вкл/Выкл) |
| 0x18 | ReadByte → switch(0..7) | строка | Тип сканирования (8 вариантов) |
| 0x19 | ReadByte → switch(0..3) | строка | Тип контроля (4 варианта) |
| 0x1A | ReadBit(flags, bitmask) | "Да"/"Нет" | Флаг из byte (bit test) |
| 0x1B | ReadBit(flags2, bitmask) | "Да"/"Нет" | Другой флаг (bit test) |
| 0x1C | строка (label) + spacer | (section header) | Разделитель секции (сдвиг +0x96) |
| 0x1D | ReadByte → switch(0..4) | строка | Тип B-scan ("BC1","BC2","BC1+BC2",...) |
| 0x1E | ReadShort(offset) | "%d" | Целое LE16 (формат 3) |
| 0x1F | ReadByte(offset) | "%d" | Числовой byte (формат 3) |
| 0x20 | ReadBit(flags3, bitmask) | "Да"/"Нет" | Третий набор битовых флагов |

### 23.3 Ключевые формулы декодирования

**Тип 0x12 — начало развёртки (мм):**
```c
short raw = ReadShort(offset);
byte div_mode = ReadByte(DAT_0051cb58 + param_2[0x660]);  // режим деления из буфера
int divisor = 0xF0 >> (div_mode & 0x1F);  // 240, 120, 60, 30, 15...
float time_us = (float)(raw * divisor) / (_DAT_00515ea0 * _DAT_0051cb44);
float depth_mm = FUN_004238ec(time_us);  // = v × t × dt / 2000
```

**Тип 0x13 — конец развёртки (мм):**
```c
short raw = ReadShort(offset);
float time_us = (float)raw / _DAT_0051cb44;
float depth_mm = FUN_004238ec(time_us);
```

**Тип 0x11 — дробное ÷10:**
```c
short raw = ReadShort(offset);
double result = (double)(raw / 10);  // целочисленное деление!
```

---

## 24. Конструктор DGS-виджета — `FUN_0040b6ff`

```c
void* DGSWidget_Init(void* this) {
    // Копируем 0x19C (412) dword'ов из стека:
    memcpy(this + 0x18, stack_data, 0x19C * 4);  // 1648 байт!
    
    // Извлекаем глобальные параметры оси:
    _DAT_0051cb3c = (float)ReadShort(this[0x1B1]);  // range_start
    _DAT_0051cb38 = (float)ReadShort(this[0x1B2]);  // range_end
    _DAT_0051cb40 = (float)ReadShort(this[0x1B0]);  // prism_delay
    _DAT_0051cb44 = this[0x1D];                      // sample_period (уже float!)
    
    // Физические размеры виджета:
    this[6] = 300;        // ширина (px)
    this[0x16] = 50;      // отступ X (0x32)
    this[0x15] = 30;      // отступ Y (0x1E)
    
    SetHeight(this, 0x438);  // высота = 1080 px (?!)
}
```

**Ключевые смещения в DGS-виджете:**
- `this[0x1B0]` = prism_delay (задержка в призме)
- `this[0x1B1]` = range_start (начало развёртки)
- `this[0x1B2]` = range_end (конец развёртки)
- `this[0x1D]` = sample_period (период дискретизации как float)

---

## 25. Функция `FUN_004085b0` — форматирование двухбайтового поля (тип 0x31)

```c
char* FormatDualByte(int threshold_byte, uint value_byte) {
    // Инверсия: если >0x7F, то знаковое отрицательное
    value_byte &= 0xFF;
    if (value_byte > 0x7F) {
        value_byte = -(0x100 - value_byte);  // → отрицательное
    }
    
    // Форматирование зависит от threshold:
    if (threshold_byte < 0x10) {
        sprintf(result, format_short, ...);    // DAT_004aa360: короткий формат
    } else if ((int)value_byte < 0x7F) {
        sprintf(result, format_normal, ...);   // DAT_004aa370: обычный
    } else {
        sprintf(result, format_over, ...);     // DAT_004aa368: превышение
    }
    
    // Результат записывается в глобальный буфер DAT_0051c760 (макс. 8 символов)
    return &DAT_0051c760;
}
```

---

## 26. Формула depth_from_time (подтверждение)

```c
// FUN_004238ec:
float depth_mm = (_DAT_00515ea0 * time_samples * _DAT_0051cb38) / 2000.0f;
```

**Где:**
- `_DAT_00515ea0` = 1 (обычный) или 2 (вагонная версия) — **множитель для двупутного пробега!**
- `time_samples` = время в отсчётах
- `_DAT_0051cb38` = период дискретизации (мкс/отсч)
- `2000.0` = конверсия мкс→мм (скорость×2 для двукратного пути встроена в множитель)

**Физическая интерпретация:**
```
depth_mm = multiplier × samples × dt / 2000
         = multiplier × t_us / 2000
```
Для вагонной версии `multiplier=2` означает **удвоенный путь** (эхо-метод), т.е. реальная глубина = `t × v / 2` где v≈3000 м/с для стали.

---

## 27. Сводка новых находок (раунд 6)

| # | Находка | Статус | Источник |
|---|---------|--------|----------|
| 78 | 6 таблиц периодов дискретизации: 0.1..10.0 мкс, по 4-8 значений каждая | 🔒 Железобетонно | FUN_0040900f..FUN_00409213 |
| 79 | Декодер DGS-полей: 32+ типа (0x00..0x20), stride 12 байт | 🔒 Железобетонно | FUN_0040b7fd |
| 80 | Тип 0x07: "Круг"/"Прямоуг"/"Эллипс" (3 варианта) | 🔒 Железобетонно | case 7 |
| 81 | Тип 0x0B/0x0C: lookup по таблицам strobNames (+0x4CC, +0x51C) | 🔒 Железобетонно | case 0x0B/0x0C |
| 82 | Тип 0x11: LE16÷10 — дробное с 1 десятичным знаком | 🔒 Железобетонно | case 0x11 |
| 83 | Тип 0x12: **формула начала развёртки** `raw×(240>>mode)/(mult×dt)` → depth_mm | 🔒 Железобетонно | case 0x12 |
| 84 | Тип 0x14: режим деления (×1..×16) — 5 вариантов | 🔒 Железобетонно | case 0x14 |
| 85 | Тип 0x18: 8 типов сканирования | 🔒 Железобетонно | case 0x18 |
| 86 | Тип 0x1D: "BC1"/"BC2"/"BC1+BC2" — 5 вариантов B-scan | 🔒 Железобетонно | case 0x1D |
| 87 | DGS-виджет: 1648 байт данных, range@[0x1B0..0x1B2], dt@[0x1D] | 🔒 Железобетонно | FUN_0040b6ff |
| 88 | `_DAT_00515ea0` = 1 (обычный) / 2 (вагонная) — НЕ dgsMultiplier, а **путь**! | 🔒 Железобетонно | FUN_0040afd1 |
| 89 | FormatDualByte: 3 формата (short/normal/over), знаковая инверсия >0x7F | 🔒 Железобетонно | FUN_004085b0 |
| 90 | Тип 0x4A/0x4B: LE16 → формат (последние 2 типа в switch) | 🔒 Железобетонно | FUN_0042413f end |
| 91 | Passport decode в zapis2 (FUN_0042409a): обратный цикл + FUN_00423a6b для каждого байта | 🔒 Железобетонно | FUN_0042409a |
| 92 | Реестр: `SOFTWARE\Altek\PelengPC` читается из `FUN_00408834` (RegOpenKeyExA) | 🔒 Железобетонно | FUN_00408834 |

---

## 28. Что осталось непокрыто (обновлённый)

1. **Содержимое строковых DAT** (DAT_004aa420..474, DAT_004b24d8..26FC) — имена режимов, отражателей, стробов
2. **FUN_00411799** — инициализация дополнительных данных для DGS
3. **FUN_0042873e** — привязка CEdit к полю (layout в форме)
4. **Полная карта DGS_Settings** из dataDescriptor+0x163C (0x19C×4=1648 байт)
5. **Шрифт отчёта**: Times New Roman, `local_20=7, local_1c=0x10` (7pt × 16pt leading)
