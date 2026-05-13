#!/usr/bin/env python3
"""
PelengPC GUI v3.0 — Графический интерфейс для дефектоскопа УД2-102
Читает каталог из handshake → запрашивает блоки по 0x42 → декодирует записи.

Требования: PyQt6 (или PyQt5), pyserial
"""

import sys
import os
import struct
import time

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))

from peleng_reader import (
    PelengCOM, MeasurementRecord, decode_record, category_name,
    EMPTY_MARKERS, CATEGORY_NAMES, decode_passport
)

try:
    from PyQt6.QtWidgets import (QApplication, QMainWindow, QTableWidget,
                                  QTableWidgetItem, QVBoxLayout, QHBoxLayout,
                                  QWidget, QPushButton, QLabel, QComboBox,
                                  QTextEdit, QSplitter, QProgressBar)
    from PyQt6.QtCore import Qt, QThread, pyqtSignal
    HAS_QT = True
    QT_VERSION = 6
except ImportError:
    try:
        from PyQt5.QtWidgets import (QApplication, QMainWindow, QTableWidget,
                                      QTableWidgetItem, QVBoxLayout, QHBoxLayout,
                                      QWidget, QPushButton, QLabel, QComboBox,
                                      QTextEdit, QSplitter, QProgressBar)
        from PyQt5.QtCore import Qt, QThread, pyqtSignal
        HAS_QT = True
        QT_VERSION = 5
    except ImportError:
        HAS_QT = False
        QT_VERSION = 0


if HAS_QT:

    # ════════════════════════════════════════════════════════════
    #  WORKER THREAD — чтение записей в фоне
    # ════════════════════════════════════════════════════════════

    class ReadWorker(QThread):
        """Фоновый поток чтения записей из прибора."""
        progress = pyqtSignal(int, int, str)   # current, total, message
        finished = pyqtSignal(list)             # list of MeasurementRecord
        error = pyqtSignal(str)

        def __init__(self, port: str, baud: int):
            super().__init__()
            self.port = port
            self.baud = baud

        def run(self):
            try:
                com = PelengCOM(self.port, baudrate=self.baud, debug=False)
                if not com.connect():
                    self.error.emit(f"Не удалось открыть {self.port}")
                    return

                self.progress.emit(0, 1, "Handshake...")
                catalog = com.get_catalog()
                if not catalog:
                    self.error.emit("Каталог пуст")
                    com.disconnect()
                    return

                # Только cat 4-29 (протоколы + результаты + A-scan)
                targets = [a for a in catalog if 4000 <= a < 30000]
                self.progress.emit(0, len(targets), f"Чтение {len(targets)} блоков...")

                records = []
                for i, addr in enumerate(targets):
                    self.progress.emit(i, len(targets),
                                       f"Блок {addr} ({i+1}/{len(targets)})")

                    data = com.read_block(addr)
                    if data and not com.is_empty_block(data) and len(data) >= 85:
                        rec = decode_record(addr, data)
                        if rec:
                            records.append(rec)

                    time.sleep(0.02)

                com.disconnect()
                self.progress.emit(len(targets), len(targets), "Готово")
                self.finished.emit(records)

            except Exception as e:
                self.error.emit(f"{type(e).__name__}: {e}")


    # ════════════════════════════════════════════════════════════
    #  MAIN WINDOW
    # ════════════════════════════════════════════════════════════

    class PelengMainWindow(QMainWindow):
        def __init__(self):
            super().__init__()
            self.setWindowTitle("PelengPC Reader v3.0 — УД2-102")
            self.setMinimumSize(900, 600)
            self.records = []
            self.worker = None
            self._init_ui()

        def _init_ui(self):
            central = QWidget()
            self.setCentralWidget(central)
            layout = QVBoxLayout(central)

            # ── Toolbar ──
            toolbar = QHBoxLayout()
            self.port_combo = QComboBox()
            self.port_combo.setEditable(True)
            self.port_combo.addItems([f"COM{i}" for i in range(1, 20)])
            self.port_combo.setCurrentText("COM3")
            toolbar.addWidget(QLabel("Порт:"))
            toolbar.addWidget(self.port_combo)

            self.baud_combo = QComboBox()
            self.baud_combo.addItems(["9600", "19200", "38400", "57600", "115200"])
            self.baud_combo.setCurrentText("19200")
            toolbar.addWidget(QLabel("Скорость:"))
            toolbar.addWidget(self.baud_combo)

            self.btn_read = QPushButton("Читать все записи")
            self.btn_read.clicked.connect(self._on_read)
            toolbar.addWidget(self.btn_read)

            toolbar.addStretch()
            self.status_label = QLabel("Не подключен")
            toolbar.addWidget(self.status_label)
            layout.addLayout(toolbar)

            # ── Progress bar ──
            self.progress_bar = QProgressBar()
            self.progress_bar.setVisible(False)
            layout.addWidget(self.progress_bar)

            # ── Splitter: table + detail ──
            splitter = QSplitter(Qt.Orientation.Vertical)

            # Table
            self.table = QTableWidget()
            self.table.setColumnCount(7)
            self.table.setHorizontalHeaderLabels([
                "№", "Адрес", "Категория", "Дата", "Время", "Паспорт", "Дефект"
            ])
            self.table.setSelectionBehavior(QTableWidget.SelectionBehavior.SelectRows)
            self.table.doubleClicked.connect(self._on_double_click)
            splitter.addWidget(self.table)

            # Detail
            self.text_detail = QTextEdit()
            self.text_detail.setReadOnly(True)
            self.text_detail.setFontFamily("Courier New")
            splitter.addWidget(self.text_detail)

            splitter.setSizes([400, 200])
            layout.addWidget(splitter)

        def _on_read(self):
            """Запуск чтения в фоновом потоке."""
            if self.worker and self.worker.isRunning():
                return

            port = self.port_combo.currentText()
            baud = int(self.baud_combo.currentText())

            self.btn_read.setEnabled(False)
            self.progress_bar.setVisible(True)
            self.progress_bar.setValue(0)
            self.status_label.setText("Чтение...")

            self.worker = ReadWorker(port, baud)
            self.worker.progress.connect(self._on_progress)
            self.worker.finished.connect(self._on_finished)
            self.worker.error.connect(self._on_error)
            self.worker.start()

        def _on_progress(self, current, total, msg):
            if total > 0:
                self.progress_bar.setMaximum(total)
                self.progress_bar.setValue(current)
            self.status_label.setText(msg)

        def _on_finished(self, records):
            self.records = records
            self.btn_read.setEnabled(True)
            self.progress_bar.setVisible(False)
            self.status_label.setText(f"Прочитано: {len(records)} записей")
            self._fill_table()

        def _on_error(self, msg):
            self.btn_read.setEnabled(True)
            self.progress_bar.setVisible(False)
            self.status_label.setText(f"Ошибка: {msg}")

        def _fill_table(self):
            """Заполняет таблицу записями."""
            self.table.setRowCount(len(self.records))
            for i, rec in enumerate(self.records):
                self.table.setItem(i, 0, QTableWidgetItem(str(i + 1)))
                self.table.setItem(i, 1, QTableWidgetItem(str(rec.address)))
                self.table.setItem(i, 2, QTableWidgetItem(rec.category_name))
                self.table.setItem(i, 3, QTableWidgetItem(rec.date_str))
                self.table.setItem(i, 4, QTableWidgetItem(rec.time_str))
                self.table.setItem(i, 5, QTableWidgetItem(rec.passport_primary))
                defect_item = QTableWidgetItem("Да" if rec.has_defect else "Нет")
                self.table.setItem(i, 6, defect_item)
            self.table.resizeColumnsToContents()

        def _on_double_click(self, index):
            """Показывает детали записи."""
            row = index.row()
            if row >= len(self.records):
                return
            rec = self.records[row]

            lines = []
            lines.append(f"{'═' * 50}")
            lines.append(f"  ЗАПИСЬ #{row+1}")
            lines.append(f"{'═' * 50}")
            lines.append(f"")
            lines.append(f"  Адрес:        {rec.address}")
            lines.append(f"  Категория:    {rec.category_name} (cat={rec.category})")
            lines.append(f"  Дата:         {rec.date_str} {rec.time_str}")
            lines.append(f"  Прибор ID:    {rec.device_id}")
            lines.append(f"  TypeVar:      {rec.typevar}")
            lines.append(f"  TypeVar2:     {rec.typevar2}")
            lines.append(f"  Сторона:      {rec.side}")
            lines.append(f"  Паспорт:      '{rec.passport_primary}'")
            lines.append(f"  Дефект:       {'Да' if rec.has_defect else 'Нет'} "
                         f"(flags=0x{rec.defect_flags:04X})")
            lines.append(f"  Year field:   {rec.year_field}")
            lines.append(f"  Probe angle:  {rec.probe_angle}")
            lines.append(f"")
            lines.append(f"  ─── ИЗМЕРЕНИЯ (raw) ───")
            lines.append(f"  values: {rec.meas_values.hex(' ') if rec.meas_values else '-'}")
            lines.append(f"  extra:  {rec.extra_params.hex(' ') if rec.extra_params else '-'}")
            lines.append(f"")
            lines.append(f"  ─── RAW HEX ───")
            for off in range(0, min(len(rec.raw), 85), 16):
                chunk = rec.raw[off:off + 16]
                h = ' '.join(f'{b:02x}' for b in chunk)
                lines.append(f"  {off:04x}: {h}")

            self.text_detail.setPlainText('\n'.join(lines))


# ════════════════════════════════════════════════════════════
#  MAIN
# ════════════════════════════════════════════════════════════

def main():
    if not HAS_QT:
        print("PyQt6/PyQt5 не найден! Установите: pip install PyQt6")
        print("Используйте CLI: python peleng_reader.py COM3 19200 --all")
        return 1

    app = QApplication(sys.argv)
    app.setStyle("Fusion")
    window = PelengMainWindow()
    window.show()
    return app.exec() if QT_VERSION == 6 else app.exec_()


if __name__ == "__main__":
    sys.exit(main())
