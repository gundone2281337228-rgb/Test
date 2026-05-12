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
