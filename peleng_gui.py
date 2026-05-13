#!/usr/bin/env python3
"""
PelengPC GUI — Графический интерфейс для работы с дефектоскопом УД2-102
Двойной клик по протоколу → дешифровка + график А-развёртки
Двойной клик по отчёту → дешифровка всех полей

Требования: PyQt6 (или PyQt5 как fallback)
"""

import sys
import os
import struct

# Add current dir to path
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from peleng_reader import (
    PelengCOM, parse_tlv, decode_protocol, decode_passport,
    TLVRecord, DecodedReport, print_report, render_ascan_text,
    SERIES_NAMES, MODE_NAMES, OBJECT_TYPES, SIDE_NAMES, NECK_TYPES
)

# Try to import GUI framework
try:
    from PyQt6.QtWidgets import (QApplication, QMainWindow, QTableWidget, 
                                  QTableWidgetItem, QVBoxLayout, QHBoxLayout,
                                  QWidget, QPushButton, QLabel, QComboBox,
                                  QTextEdit, QSplitter, QMessageBox, QDialog,
                                  QDialogButtonBox, QFormLayout, QLineEdit)
    from PyQt6.QtCore import Qt, QTimer
    from PyQt6.QtGui import QPainter, QPen, QColor, QPixmap
    HAS_QT = True
    QT_VERSION = 6
except ImportError:
    try:
        from PyQt5.QtWidgets import (QApplication, QMainWindow, QTableWidget,
                                      QTableWidgetItem, QVBoxLayout, QHBoxLayout,
                                      QWidget, QPushButton, QLabel, QComboBox,
                                      QTextEdit, QSplitter, QMessageBox, QDialog,
                                      QDialogButtonBox, QFormLayout, QLineEdit)
        from PyQt5.QtCore import Qt, QTimer
        from PyQt5.QtGui import QPainter, QPen, QColor, QPixmap
        HAS_QT = True
        QT_VERSION = 5
    except ImportError:
        HAS_QT = False
        QT_VERSION = 0


# ============================================================
#  A-SCAN WIDGET (рисует развёртку)
# ============================================================

if HAS_QT:
    class AScanWidget(QWidget):
        """Виджет отображения А-развёртки"""
        
        def __init__(self, parent=None):
            super().__init__(parent)
            self.waveform_ch1 = bytes(240)
            self.waveform_ch2 = bytes(240)
            self.gate1_start = 0
            self.gate1_end = 0
            self.gate1_level = 50
            self.gate2_start = 0
            self.gate2_end = 0
            self.gate2_level = 50
            self.setMinimumSize(500, 250)
        
        def set_data(self, report: DecodedReport):
            """Загружает данные из отчёта"""
            if report.waveform_ch1:
                self.waveform_ch1 = report.waveform_ch1
            if report.waveform_ch2:
                self.waveform_ch2 = report.waveform_ch2
            self.gate1_level = report.gate_level
            self.update()
        
        def paintEvent(self, event):
            """Рисует А-развёртку"""
            p = QPainter(self)
            p.setRenderHint(QPainter.RenderHint.Antialiasing)
            
            w, h = self.width(), self.height()
            margin = 30
            plot_w = w - 2 * margin
            plot_h = h - 2 * margin
            
            # Background
            p.fillRect(0, 0, w, h, QColor(255, 255, 255))
            
            # Plot area border
            p.setPen(QPen(QColor(0, 0, 0), 1))
            p.drawRect(margin, margin, plot_w, plot_h)
            
            # Grid
            p.setPen(QPen(QColor(200, 200, 200), 1))
            for i in range(1, 10):
                x = margin + int(plot_w * i / 10)
                p.drawLine(x, margin, x, margin + plot_h)
                y = margin + int(plot_h * i / 10)
                p.drawLine(margin, y, margin + plot_w, y)
            
            # Waveform Channel 1 (blue)
            if self.waveform_ch1:
                p.setPen(QPen(QColor(0, 0, 200), 1))
                samples = len(self.waveform_ch1)
                if samples > 0:
                    scale_x = plot_w / samples
                    scale_y = plot_h / 255.0
                    prev_x, prev_y = margin, margin + plot_h
                    for i in range(samples):
                        x = margin + int(i * scale_x)
                        amp = self.waveform_ch1[i]
                        y = margin + plot_h - int(amp * scale_y)
                        if i > 0:
                            p.drawLine(prev_x, prev_y, x, y)
                        prev_x, prev_y = x, y
            
            # Gate 1 (green dashed)
            if self.gate1_level > 0:
                p.setPen(QPen(QColor(0, 180, 0), 2, Qt.PenStyle.DashLine))
                gate_y = margin + plot_h - int(self.gate1_level * plot_h / 100)
                p.drawLine(margin, gate_y, margin + plot_w, gate_y)
            
            # Labels
            p.setPen(QPen(QColor(0, 0, 0), 1))
            p.drawText(5, margin + plot_h // 2, "дБ")
            p.drawText(margin + plot_w // 2, h - 5, "мкс")
            p.drawText(margin, margin - 5, "А-развёртка")
            
            p.end()


    # ============================================================
    #  MAIN WINDOW
    # ============================================================

    class PelengMainWindow(QMainWindow):
        """Главное окно приложения"""
        
        def __init__(self):
            super().__init__()
            self.setWindowTitle("PelengPC Reader — УД2-102")
            self.setMinimumSize(900, 600)
            self.records = []
            self.com = None
            self.init_ui()
        
        def init_ui(self):
            central = QWidget()
            self.setCentralWidget(central)
            layout = QVBoxLayout(central)
            
            # Toolbar
            toolbar = QHBoxLayout()
            self.port_combo = QComboBox()
            self.port_combo.addItems([f"COM{i}" for i in range(1, 20)])
            self.port_combo.setCurrentText("COM3")
            toolbar.addWidget(QLabel("Порт:"))
            toolbar.addWidget(self.port_combo)
            
            self.btn_connect = QPushButton("Подключить")
            self.btn_connect.clicked.connect(self.on_connect)
            toolbar.addWidget(self.btn_connect)
            
            self.btn_read = QPushButton("Читать FLASH")
            self.btn_read.clicked.connect(self.on_read_flash)
            self.btn_read.setEnabled(False)
            toolbar.addWidget(self.btn_read)
            
            self.btn_demo = QPushButton("Демо (из файла)")
            self.btn_demo.clicked.connect(self.on_demo)
            toolbar.addWidget(self.btn_demo)
            
            toolbar.addStretch()
            self.status_label = QLabel("Не подключен")
            toolbar.addWidget(self.status_label)
            layout.addLayout(toolbar)
            
            # Splitter: table + detail
            splitter = QSplitter(Qt.Orientation.Vertical)
            
            # Table of records
            self.table = QTableWidget()
            self.table.setColumnCount(6)
            self.table.setHorizontalHeaderLabels([
                "№", "Адрес", "Категория", "Размер", "Дата", "Тип"
            ])
            self.table.doubleClicked.connect(self.on_double_click)
            self.table.setSelectionBehavior(QTableWidget.SelectionBehavior.SelectRows)
            splitter.addWidget(self.table)
            
            # Detail area
            detail_widget = QWidget()
            detail_layout = QHBoxLayout(detail_widget)
            
            self.text_detail = QTextEdit()
            self.text_detail.setReadOnly(True)
            self.text_detail.setFontFamily("Courier New")
            detail_layout.addWidget(self.text_detail)
            
            self.ascan_widget = AScanWidget()
            detail_layout.addWidget(self.ascan_widget)
            
            splitter.addWidget(detail_widget)
            splitter.setSizes([300, 300])
            
            layout.addWidget(splitter)
        
        def on_connect(self):
            """Подключение к COM-порту"""
            port = self.port_combo.currentText()
            self.com = PelengCOM(port, debug=True)
            if self.com.connect():
                if self.com.test_port():
                    self.status_label.setText(
                        f"Подключен: {port}, payload={self.com.payload_len}, "
                        f"{'Вагонная' if self.com.is_wagon else 'Базовая'}")
                    self.btn_read.setEnabled(True)
                else:
                    self.status_label.setText(f"Ошибка handshake на {port}")
                    self.com.disconnect()
            else:
                self.status_label.setText(f"Не удалось открыть {port}")
        
        def on_read_flash(self):
            """Чтение FLASH-дампа"""
            if not self.com:
                return
            self.status_label.setText("Чтение данных...")
            
            try:
                # Сначала пробуем полное чтение (handshake → данные уже в ответе)
                data = self.com.read_all_data()
                if data and len(data) > 16:
                    self.status_label.setText(f"Данные: {len(data)} байт (из handshake)")
                    # Parse TLV after 16-byte header
                    records = parse_tlv(data[16:])
                    if records:
                        self.load_records(records)
                        return
                    # Если TLV не нашли — попробуем с начала (без header skip)
                    records = parse_tlv(data)
                    if records:
                        self.load_records(records)
                        return
                    # Нет TLV — покажем raw
                    self.status_label.setText(f"Данные: {len(data)} байт, TLV не найдены")
                    return
                
                # Если не получилось — пробуем Flash dump (0x9A)
                self.status_label.setText("Чтение Flash (0x9A)...")
                data = self.com.read_flash()
                if data:
                    self.status_label.setText(f"FLASH: {len(data)} байт")
                    if len(data) > 16:
                        records = parse_tlv(data[16:])
                        if not records:
                            records = parse_tlv(data)
                        if records:
                            self.load_records(records)
                    else:
                        records = parse_tlv(data)
                        if records:
                            self.load_records(records)
                else:
                    self.status_label.setText("Ошибка чтения данных")
            except Exception as e:
                self.status_label.setText(f"Ошибка: {e}")
                import traceback
                traceback.print_exc()
        
        def on_demo(self):
            """Загрузка демо-данных (создаёт тестовые записи)"""
            import struct as st
            # Create sample records for demo
            demo_records = []
            for i in range(5):
                body = bytearray(200)
                st.pack_into('<H', body, 0x00, 1182)
                body[0x07] = 10; body[0x08] = 6; body[0x09] = 16
                body[0x0A] = 14; body[0x0B] = 13 + i
                st.pack_into('<H', body, 0x14, 731)
                st.pack_into('<H', body, 0x15D, 50 + i)
                body[0x48] = i % 3
                body[0x4B] = i % 4
                # Some waveform data
                for j in range(min(150, len(body) - 0x61)):
                    body[0x61 + j] = int(127 + 100 * abs(
                        __import__('math').sin(j * 0.1 + i)))
                
                cat = [1, 4, 10, 20, 5][i % 5]
                demo_records.append(TLVRecord(
                    tag=cat * 1000 + 100 + i,
                    offset=i * 200,
                    size=len(body),
                    body=bytes(body),
                    category=cat
                ))
            
            self.load_records(demo_records)
            self.status_label.setText(f"Демо: {len(demo_records)} записей")
        
        def load_records(self, records: list):
            """Загружает записи в таблицу"""
            self.records = records
            self.table.setRowCount(len(records))
            for i, rec in enumerate(records):
                self.table.setItem(i, 0, QTableWidgetItem(str(i + 1)))
                self.table.setItem(i, 1, QTableWidgetItem(f"{rec.tag}"))
                self.table.setItem(i, 2, QTableWidgetItem(rec.category_name))
                self.table.setItem(i, 3, QTableWidgetItem(f"{rec.size} байт"))
                # Try to decode date
                date_str = ""
                try:
                    if len(rec.body) > 0x09:
                        date_str = f"{rec.body[0x07]:02d}.{rec.body[0x08]:02d}.{rec.body[0x09]+2000}"
                except Exception:
                    date_str = "?"
                self.table.setItem(i, 4, QTableWidgetItem(date_str))
                # Type
                type_str = ""
                try:
                    if len(rec.body) > 0x15:
                        tv = struct.unpack_from('<H', rec.body, 0x14)[0]
                        series = (tv >> 8) & 0xFF
                        type_str = SERIES_NAMES.get(series, f"0x{tv:04X}")
                except Exception:
                    type_str = "?"
                self.table.setItem(i, 5, QTableWidgetItem(type_str))
            
            self.table.resizeColumnsToContents()
        
        def on_double_click(self, index):
            """Обработка двойного клика по записи"""
            row = index.row()
            if row >= len(self.records):
                return
            
            rec = self.records[row]
            report = decode_protocol(rec.body)
            
            # Fill detail text
            lines = []
            lines.append(f"{'═' * 50}")
            lines.append(f"  ПРОТОКОЛ № {report.record_number}")
            lines.append(f"  {report.device_name} № {report.device_id}")
            lines.append(f"  {report.date} {report.time}")
            lines.append(f"{'═' * 50}")
            lines.append(f"")
            lines.append(f"  Категория:  {rec.category_name}")
            lines.append(f"  Адрес:      {rec.tag}")
            lines.append(f"  Размер:     {rec.size} байт")
            lines.append(f"")
            lines.append(f"  ─── ОБЪЕКТ ───")
            lines.append(f"  Тип объекта:    {report.object_type}")
            lines.append(f"  Номер объекта:  {report.object_number}")
            lines.append(f"  Плавка:         {report.smelting}")
            lines.append(f"  Завод (ЗД):     {report.factory}")
            lines.append(f"  Сторона:        {report.side}")
            lines.append(f"  Шейка:          {report.neck}")
            lines.append(f"  Обод:           {report.object_number}")
            lines.append(f"  Обточка:        {report.machining}")
            lines.append(f"  Наплавка:       {'Да' if report.surfacing else 'Нет'}")
            lines.append(f"  Дефект:         {'Да' if report.defect_flag else 'Нет'}")
            lines.append(f"")
            lines.append(f"  ─── ИЗМЕРЕНИЯ ───")
            lines.append(f"  Глубина Y, мм:    {report.depth_mm}")
            lines.append(f"  Скорость, м/с:    {report.velocity_mps}")
            lines.append(f"  Амплитуда, дБ:    {report.amplitude_db}")
            lines.append(f"  Толщина, мм:      {report.thickness_mm:.2f}")
            lines.append(f"  Длина объекта:    {report.total_length}")
            lines.append(f"  TypeVar:          {report.typevar} (0x{report.typevar:04X})")
            lines.append(f"")
            lines.append(f"  ─── WAVEFORM ───")
            lines.append(f"  Канал 1: {len(report.waveform_ch1)} сэмплов")
            lines.append(f"  Канал 2: {len(report.waveform_ch2)} сэмплов")
            lines.append(f"")
            
            # Add text A-scan
            if report.waveform_ch1:
                lines.append(render_ascan_text(report, width=50, height=12))
            
            self.text_detail.setPlainText('\n'.join(lines))
            
            # Draw A-scan widget
            self.ascan_widget.set_data(report)


# ============================================================
#  MAIN
# ============================================================

def main():
    if not HAS_QT:
        print("PyQt6/PyQt5 не найден!")
        print("Установите: pip install PyQt6")
        print()
        print("Запуск CLI режима:")
        print("  python peleng_reader.py --demo-xml")
        print("  python peleng_reader.py --demo-decode")
        return 1
    
    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    
    window = PelengMainWindow()
    window.show()
    
    return app.exec() if QT_VERSION == 6 else app.exec_()


if __name__ == "__main__":
    sys.exit(main())
