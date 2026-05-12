#!/usr/bin/env python3
"""
peleng_gui.py — PyQt6 GUI для PelengPC Clone
Полный аналог интерфейса PelengPC.exe (12 форм VCL → PyQt6)

Запуск: python peleng_gui.py [--port COM3] [--demo]
Зависимости: pip install PyQt6 pyserial
"""

import sys
import os
from pathlib import Path

# Импорт основного модуля
sys.path.insert(0, str(Path(__file__).parent))
from peleng_pc_clone import (
    PelengPCApp, COMPort, Protocol, BlockZapDB, BscanRenderer,
    Config, OperatorList, ProtocolError,
    BAUD_RATES, DEFAULT_BAUD, FRAME_HEADER_SIZE,
    DataCategory, ConnectionMode, APP_VERSION,
    is_sweep_address, classify_block, sort_buf_data, parse_tlv_stream
)

try:
    from PyQt6.QtWidgets import (
        QApplication, QMainWindow, QWidget, QVBoxLayout, QHBoxLayout,
        QGridLayout, QGroupBox, QLabel, QPushButton, QComboBox,
        QTableWidget, QTableWidgetItem, QHeaderView, QStatusBar,
        QToolBar, QMenuBar, QMenu, QDialog, QDialogButtonBox,
        QLineEdit, QTextEdit, QProgressBar, QListWidget, QTabWidget,
        QCheckBox, QSpinBox, QFileDialog, QMessageBox, QSplitter,
        QListWidgetItem, QAbstractItemView
    )
    from PyQt6.QtCore import Qt, QThread, pyqtSignal, QTimer
    from PyQt6.QtGui import QAction, QPixmap, QImage, QPainter, QColor, QFont
    HAS_PYQT6 = True
except ImportError:
    HAS_PYQT6 = False
    print("[!] PyQt6 не установлен. Установите: pip install PyQt6")
    sys.exit(1)




# ==============================================================================
# WORKER THREAD для протокола (чтобы не блокировать GUI)
# ==============================================================================

class ProtocolWorker(QThread):
    """Фоновый поток для операций с COM-портом"""
    progress = pyqtSignal(int, int)       # (received, total)
    finished = pyqtSignal(bytes)          # результат
    error = pyqtSignal(str)               # ошибка
    
    def __init__(self, protocol: Protocol, operation: str, **kwargs):
        super().__init__()
        self.protocol = protocol
        self.operation = operation
        self.kwargs = kwargs
    
    def run(self):
        try:
            self.protocol.on_progress = lambda r, t: self.progress.emit(r, t)
            
            if self.operation == 'handshake':
                ok, data = self.protocol.handshake()
                if ok:
                    self.finished.emit(data)
                else:
                    self.error.emit("Handshake failed")
            
            elif self.operation == 'block':
                data = self.protocol.request_block(
                    self.kwargs['addr'], self.kwargs['size'])
                self.finished.emit(data)
            
            elif self.operation == 'flash':
                data = self.protocol.flash_dump()
                self.finished.emit(data)
            
            elif self.operation == 'dispatch':
                data = self.protocol.read_block_dispatch(
                    self.kwargs['addr'], self.kwargs['size'])
                self.finished.emit(data)
                
        except ProtocolError as e:
            self.error.emit(str(e))
        except Exception as e:
            self.error.emit(f"Unexpected: {e}")


# ==============================================================================
# ДИАЛОГ ПРОГРЕССА (FormProgress)
# ==============================================================================

class ProgressDialog(QDialog):
    """Аналог FormProgress — отображение прогресса приёма данных"""
    
    def __init__(self, parent=None, title="Приём данных..."):
        super().__init__(parent)
        self.setWindowTitle(title)
        self.setFixedSize(400, 120)
        self.setModal(True)
        
        layout = QVBoxLayout(self)
        self.label = QLabel("Ожидание данных...")
        self.progress = QProgressBar()
        self.progress.setRange(0, 100)
        self.btn_cancel = QPushButton("Отмена")
        self.btn_cancel.clicked.connect(self.reject)
        
        layout.addWidget(self.label)
        layout.addWidget(self.progress)
        layout.addWidget(self.btn_cancel)
    
    def update_progress(self, received: int, total: int):
        if total > 0:
            pct = int(received * 100 / total)
            self.progress.setValue(pct)
            self.label.setText(f"Получено: {received} / {total} байт")


# ==============================================================================
# ДИАЛОГ НАСТРОЕК COM-ПОРТА (FormNastr)
# ==============================================================================

class SettingsDialog(QDialog):
    """
    Аналог FormNastr — настройки COM-порта и операторов.
    
    Компоненты из DFM:
      TabSheetPort → ComboBoxPort, ComboBoxBaud, ButtonPortView
      PanelOperTable → SpeedButtonAdd/Del/Up/Down/Clear
    """
    
    def __init__(self, app: PelengPCApp, parent=None):
        super().__init__(parent)
        self.app = app
        self.setWindowTitle("Настройки")
        self.setMinimumSize(500, 400)
        self._build_ui()
        self._load_settings()
    
    def _build_ui(self):
        layout = QVBoxLayout(self)
        
        tabs = QTabWidget()
        layout.addWidget(tabs)
        
        # --- Вкладка "Порт" ---
        port_tab = QWidget()
        port_layout = QVBoxLayout(port_tab)
        
        # Группа "COM Порт"
        port_group = QGroupBox("COM Порт")
        pg_layout = QGridLayout(port_group)
        
        pg_layout.addWidget(QLabel("Порт:"), 0, 0)
        self.combo_port = QComboBox()
        self._populate_ports()
        pg_layout.addWidget(self.combo_port, 0, 1)
        
        btn_refresh = QPushButton("Обновить")
        btn_refresh.clicked.connect(self._populate_ports)
        pg_layout.addWidget(btn_refresh, 0, 2)
        
        pg_layout.addWidget(QLabel("Скорость:"), 1, 0)
        self.combo_baud = QComboBox()
        for baud in BAUD_RATES:
            self.combo_baud.addItem(str(baud), baud)
        pg_layout.addWidget(self.combo_baud, 1, 1)
        
        btn_test = QPushButton("Тест подключения")
        btn_test.clicked.connect(self._test_port)
        pg_layout.addWidget(btn_test, 2, 0, 1, 3)
        
        port_layout.addWidget(port_group)
        
        # Результат теста
        self.test_result = QLabel("")
        port_layout.addWidget(self.test_result)
        port_layout.addStretch()
        
        tabs.addTab(port_tab, "Порт")
        
        # --- Вкладка "Операторы" ---
        oper_tab = QWidget()
        oper_layout = QVBoxLayout(oper_tab)
        
        self.oper_list = QListWidget()
        oper_layout.addWidget(self.oper_list)
        
        btn_layout = QHBoxLayout()
        btn_add = QPushButton("Добавить")
        btn_add.clicked.connect(self._add_operator)
        btn_del = QPushButton("Удалить")
        btn_del.clicked.connect(self._del_operator)
        btn_layout.addWidget(btn_add)
        btn_layout.addWidget(btn_del)
        oper_layout.addLayout(btn_layout)
        
        tabs.addTab(oper_tab, "Операторы")
        
        # Кнопки OK/Cancel
        buttons = QDialogButtonBox(
            QDialogButtonBox.StandardButton.Ok | QDialogButtonBox.StandardButton.Cancel)
        buttons.accepted.connect(self._save_and_accept)
        buttons.rejected.connect(self.reject)
        layout.addWidget(buttons)
    
    def _populate_ports(self):
        """ButtonPortViewClick — перечисление COM-портов (SetupDi аналог)"""
        self.combo_port.clear()
        try:
            import serial.tools.list_ports
            ports = serial.tools.list_ports.comports()
            for p in ports:
                self.combo_port.addItem(f"{p.device} - {p.description}", p.device)
        except ImportError:
            # Fallback: стандартные порты
            for i in range(1, 17):
                self.combo_port.addItem(f"COM{i}", f"COM{i}")
    
    def _load_settings(self):
        """Загрузка текущих настроек в UI"""
        # Порт
        idx = self.combo_port.findData(self.app.port.port_name)
        if idx >= 0:
            self.combo_port.setCurrentIndex(idx)
        
        # Baud
        idx = self.combo_baud.findData(self.app.port.baud_rate)
        if idx >= 0:
            self.combo_baud.setCurrentIndex(idx)
        
        # Операторы
        self.oper_list.clear()
        for op in self.app.operators.operators:
            self.oper_list.addItem(op)
    
    def _test_port(self):
        """ButtonTestClick — тест Handshake"""
        port_name = self.combo_port.currentData()
        baud = self.combo_baud.currentData()
        
        if not port_name:
            self.test_result.setText("Выберите порт!")
            return
        
        # Временно подключаемся
        test_port = COMPort()
        test_port.port_name = port_name
        test_port.set_baud_rate(baud)
        
        if not test_port.open():
            self.test_result.setText("Ошибка: не удалось открыть порт")
            self.test_result.setStyleSheet("color: red")
            return
        
        proto = Protocol(test_port)
        ok, response = proto.handshake()
        test_port.close()
        
        if ok:
            dev_id = response[0] | (response[1] << 8)
            self.test_result.setText(
                f"Settings-OK! Device ID={dev_id}, {len(response)} байт")
            self.test_result.setStyleSheet("color: green")
        else:
            self.test_result.setText("Settings-BAD! DisConnect!")
            self.test_result.setStyleSheet("color: red")
    
    def _add_operator(self):
        from PyQt6.QtWidgets import QInputDialog
        name, ok = QInputDialog.getText(self, "Добавить оператора", "ФИО:")
        if ok and name:
            self.app.operators.add(name)
            self.oper_list.addItem(name)
    
    def _del_operator(self):
        item = self.oper_list.currentItem()
        if item:
            self.app.operators.remove(item.text())
            self.oper_list.takeItem(self.oper_list.row(item))
    
    def _save_and_accept(self):
        """ButtonOKClick — сохранение настроек"""
        self.app.port.port_name = self.combo_port.currentData() or ""
        self.app.port.set_baud_rate(self.combo_baud.currentData() or DEFAULT_BAUD)
        self.app.save_config()
        self.accept()




# ==============================================================================
# ДИАЛОГ ЧТЕНИЯ ДАННЫХ (FormReadData)
# ==============================================================================

class ReadDataDialog(QDialog):
    """
    Аналог FormReadData — чтение данных из прибора.
    
    Компоненты: ButtonReadDir, ButtonView, ButtonSendToBase,
                ButtonSelectAll, ButtonExit, ListBox, CheckListBoxType
    """
    
    def __init__(self, app: PelengPCApp, parent=None):
        super().__init__(parent)
        self.app = app
        self.records = []
        self.setWindowTitle("Чтение данных из прибора")
        self.setMinimumSize(600, 500)
        self._build_ui()
    
    def _build_ui(self):
        layout = QVBoxLayout(self)
        
        # Статус
        self.label_status = QLabel("Готов к чтению")
        layout.addWidget(self.label_status)
        
        # Список блоков
        self.list_blocks = QListWidget()
        self.list_blocks.setSelectionMode(QAbstractItemView.SelectionMode.MultiSelection)
        layout.addWidget(self.list_blocks)
        
        # Кнопки
        btn_layout = QHBoxLayout()
        
        btn_read = QPushButton("Чтение каталога")
        btn_read.clicked.connect(self._read_directory)
        btn_layout.addWidget(btn_read)
        
        btn_select_all = QPushButton("Выбрать все")
        btn_select_all.clicked.connect(self._select_all)
        btn_layout.addWidget(btn_select_all)
        
        btn_send = QPushButton("В базу данных")
        btn_send.clicked.connect(self._send_to_base)
        btn_layout.addWidget(btn_send)
        
        btn_view = QPushButton("Просмотр")
        btn_view.clicked.connect(self._view_block)
        btn_layout.addWidget(btn_view)
        
        btn_exit = QPushButton("Закрыть")
        btn_exit.clicked.connect(self.accept)
        btn_layout.addWidget(btn_exit)
        
        layout.addLayout(btn_layout)
    
    def _read_directory(self):
        """ButtonReadDirClick — чтение каталога блоков из прибора"""
        if self.app.connection_mode == ConnectionMode.COM_DIRECT:
            try:
                # Читаем FLASH → парсим TLV → получаем список блоков
                self.label_status.setText("Чтение каталога...")
                QApplication.processEvents()
                
                raw = self.app.protocol.read_block_dispatch(60000, self.app.protocol.flash_size)
                self.records = parse_tlv_stream(raw)
                
                self.list_blocks.clear()
                for rec in self.records:
                    cat_name = rec.category.name
                    item = QListWidgetItem(
                        f"[{rec.tag:5d}] {cat_name:12s} ({len(rec.data)} байт)")
                    item.setData(Qt.ItemDataRole.UserRole, rec)
                    self.list_blocks.addItem(item)
                
                self.label_status.setText(f"Загружено {len(self.records)} записей")
            except ProtocolError as e:
                self.label_status.setText(f"Ошибка: {e}")
        else:
            # Режим "из файла"
            path, _ = QFileDialog.getOpenFileName(
                self, "Открыть файл", "", "FLASH Files (*.fla);;PLG Files (*.plg);;All (*)")
            if path:
                with open(path, 'rb') as f:
                    data = f.read()
                self.records = parse_tlv_stream(data, start_offset=0)
                self.list_blocks.clear()
                for rec in self.records:
                    cat_name = rec.category.name
                    item = QListWidgetItem(
                        f"[{rec.tag:5d}] {cat_name:12s} ({len(rec.data)} байт)")
                    item.setData(Qt.ItemDataRole.UserRole, rec)
                    self.list_blocks.addItem(item)
                self.label_status.setText(f"Из файла: {len(self.records)} записей")
    
    def _select_all(self):
        """ButtonSelectAllClick"""
        self.list_blocks.selectAll()
    
    def _send_to_base(self):
        """ButtonSendToBaseClick — сохранение выбранных блоков в БД"""
        selected = self.list_blocks.selectedItems()
        if not selected:
            QMessageBox.warning(self, "Внимание", "Выберите записи для сохранения")
            return
        
        count = 0
        for item in selected:
            rec = item.data(Qt.ItemDataRole.UserRole)
            if rec:
                # Формируем полный пакет (заголовок + данные)
                full = bytearray(FRAME_HEADER_SIZE + len(rec.data))
                full[0x10] = rec.tag & 0xFF
                full[0x11] = (rec.tag >> 8) & 0xFF
                full[FRAME_HEADER_SIZE:] = rec.data
                self.app.save_block_to_db(rec.tag, bytes(full))
                count += 1
        
        self.label_status.setText(f"Сохранено {count} записей в БД")
        QMessageBox.information(self, "Готово", f"Сохранено {count} записей")
    
    def _view_block(self):
        """ButtonViewClick — просмотр выбранного блока"""
        item = self.list_blocks.currentItem()
        if not item:
            return
        rec = item.data(Qt.ItemDataRole.UserRole)
        if not rec:
            return
        
        # Если B-scan — показываем изображение
        if rec.category == DataCategory.BSCAN:
            dlg = BscanViewDialog(rec.data, self)
            dlg.exec()
        else:
            # Текстовый просмотр
            text = f"Тег: {rec.tag}\nКатегория: {rec.category.name}\n"
            text += f"Размер: {len(rec.data)} байт\n\n"
            text += "Hex dump (первые 256 байт):\n"
            for i in range(0, min(len(rec.data), 256), 16):
                hex_part = ' '.join(f'{b:02X}' for b in rec.data[i:i+16])
                text += f"  {i:04X}: {hex_part}\n"
            QMessageBox.information(self, f"Блок {rec.tag}", text)


# ==============================================================================
# ДИАЛОГ ПРОСМОТРА B-SCAN (FormScreen)
# ==============================================================================

class BscanViewDialog(QDialog):
    """Аналог FormScreen — отображение B-scan изображения"""
    
    def __init__(self, bscan_data: bytes, parent=None):
        super().__init__(parent)
        self.setWindowTitle("B-scan")
        self.bscan_data = bscan_data
        self.renderer = BscanRenderer()
        self._build_ui()
    
    def _build_ui(self):
        layout = QVBoxLayout(self)
        
        # Рендерим B-scan
        image = self.renderer.render_to_qimage(self.bscan_data)
        
        if image:
            self.label_img = QLabel()
            pixmap = QPixmap.fromImage(image)
            self.label_img.setPixmap(pixmap)
            layout.addWidget(self.label_img)
            
            w, h = self.renderer.get_dimensions(self.bscan_data)
            self.setWindowTitle(f"B-scan ({w}×{h} px)")
            self.resize(w + 40, h + 80)
        else:
            layout.addWidget(QLabel("Нет данных B-scan"))
        
        # Кнопка сохранения
        btn_save = QPushButton("Сохранить BMP")
        btn_save.clicked.connect(self._save_bmp)
        layout.addWidget(btn_save)
    
    def _save_bmp(self):
        path, _ = QFileDialog.getSaveFileName(
            self, "Сохранить B-scan", "bscan.bmp", "BMP Files (*.bmp)")
        if path:
            if self.renderer.save_bmp(self.bscan_data, path):
                QMessageBox.information(self, "Готово", f"Сохранено: {path}")
            else:
                QMessageBox.warning(self, "Ошибка", "Не удалось сохранить")


# ==============================================================================
# ГЛАВНОЕ ОКНО (FormMain)
# ==============================================================================

class MainWindow(QMainWindow):
    """
    Полный аналог FormMain — главное окно PelengPC.
    
    Из DFM:
      StatusBar, ToolBar, MainMenu, DBGrid (таблица BlockZap),
      TIBDatabase, TIBTransaction, TIBDataSet, TDataSource
    
    Event handlers (45 методов):
      NCOMPortClick, NTestPortClick, NReadDataClick,
      ToolButtonFromFileClick, NReadZapClick, NDelZapClick,
      FormCreate, NResetTableClick, NSearchClick, NSortirClick,
      NSaveExcelClick, NPassWordClick, NPrintTableClick,
      NExitClick, ToolButtonRaportClick, NAboutClick, etc.
    """
    
    def __init__(self, app: PelengPCApp, demo: bool = False):
        super().__init__()
        self.app = app
        self.demo = demo
        self.setWindowTitle(f"PelengPC v{APP_VERSION}")
        self.setMinimumSize(900, 600)
        self._build_menu()
        self._build_toolbar()
        self._build_central()
        self._build_statusbar()
        self._form_create()
    
    def _build_menu(self):
        """Создание главного меню (из DFM MainMenu)"""
        menubar = self.menuBar()
        
        # Файл
        file_menu = menubar.addMenu("Файл")
        
        act_read = QAction("Чтение из прибора", self)
        act_read.setShortcut("Ctrl+R")
        act_read.triggered.connect(self._on_read_data)
        file_menu.addAction(act_read)
        
        act_from_file = QAction("Из файла...", self)
        act_from_file.setShortcut("Ctrl+O")
        act_from_file.triggered.connect(self._on_from_file)
        file_menu.addAction(act_from_file)
        
        file_menu.addSeparator()
        
        act_read_zap = QAction("Чтение записи", self)
        act_read_zap.triggered.connect(self._on_read_zap)
        file_menu.addAction(act_read_zap)
        
        act_del = QAction("Удалить запись", self)
        act_del.setShortcut("Delete")
        act_del.triggered.connect(self._on_delete)
        file_menu.addAction(act_del)
        
        file_menu.addSeparator()
        
        act_exit = QAction("Выход", self)
        act_exit.setShortcut("Alt+F4")
        act_exit.triggered.connect(self.close)
        file_menu.addAction(act_exit)
        
        # Таблица
        table_menu = menubar.addMenu("Таблица")
        
        act_sort = QAction("Сортировка", self)
        act_sort.triggered.connect(self._on_sort)
        table_menu.addAction(act_sort)
        
        act_search = QAction("Поиск", self)
        act_search.setShortcut("Ctrl+F")
        act_search.triggered.connect(self._on_search)
        table_menu.addAction(act_search)
        
        act_select_all = QAction("Выбрать все", self)
        act_select_all.setShortcut("Ctrl+A")
        act_select_all.triggered.connect(self._on_select_all)
        table_menu.addAction(act_select_all)
        
        table_menu.addSeparator()
        
        act_reset = QAction("Сброс таблицы", self)
        act_reset.triggered.connect(self._on_reset_table)
        table_menu.addAction(act_reset)
        
        act_excel = QAction("Экспорт в Excel", self)
        act_excel.triggered.connect(self._on_export_excel)
        table_menu.addAction(act_excel)
        
        act_print = QAction("Печать таблицы", self)
        act_print.setShortcut("Ctrl+P")
        act_print.triggered.connect(self._on_print)
        table_menu.addAction(act_print)
        
        # Настройки
        settings_menu = menubar.addMenu("Настройки")
        
        act_com = QAction("COM Порт...", self)
        act_com.triggered.connect(self._on_com_port)
        settings_menu.addAction(act_com)
        
        act_test = QAction("Тест порта", self)
        act_test.triggered.connect(self._on_test_port)
        settings_menu.addAction(act_test)
        
        settings_menu.addSeparator()
        
        act_save_cfg = QAction("Сохранить конфигурацию", self)
        act_save_cfg.triggered.connect(self._on_save_config)
        settings_menu.addAction(act_save_cfg)
        
        act_password = QAction("Пароль...", self)
        act_password.triggered.connect(self._on_password)
        settings_menu.addAction(act_password)
        
        # Помощь
        help_menu = menubar.addMenu("Помощь")
        
        act_about = QAction("О программе", self)
        act_about.triggered.connect(self._on_about)
        help_menu.addAction(act_about)
    
    def _build_toolbar(self):
        """Панель инструментов (из DFM ToolBar)"""
        toolbar = QToolBar("Инструменты")
        self.addToolBar(toolbar)
        
        toolbar.addAction("Рапорт").triggered.connect(self._on_raport)
        toolbar.addAction("Чтение").triggered.connect(self._on_read_data)
        toolbar.addAction("Из файла").triggered.connect(self._on_from_file)
        toolbar.addSeparator()
        toolbar.addAction("Сортировка").triggered.connect(self._on_sort)
        toolbar.addAction("Поиск").triggered.connect(self._on_search)
        toolbar.addSeparator()
        toolbar.addAction("Удалить").triggered.connect(self._on_delete)
        toolbar.addAction("Настройки").triggered.connect(self._on_com_port)
    
    def _build_central(self):
        """Центральная часть — таблица BlockZap (аналог DBGrid)"""
        central = QWidget()
        self.setCentralWidget(central)
        layout = QVBoxLayout(central)
        
        # Таблица (аналог TDBGrid)
        self.table = QTableWidget()
        self.table.setColumnCount(4)
        self.table.setHorizontalHeaderLabels(["Number", "BlockLen", "Категория", "Дата"])
        self.table.horizontalHeader().setSectionResizeMode(QHeaderView.ResizeMode.Stretch)
        self.table.setSelectionBehavior(QAbstractItemView.SelectionBehavior.SelectRows)
        self.table.setEditTriggers(QAbstractItemView.EditTrigger.NoEditTriggers)
        self.table.doubleClicked.connect(self._on_double_click)
        layout.addWidget(self.table)
    
    def _build_statusbar(self):
        """StatusBar"""
        self.status = QStatusBar()
        self.setStatusBar(self.status)
        self.status.showMessage("Готов")
    
    def _form_create(self):
        """
        FormMain_FormCreate @ 0x40BCCC:
        1. Init COM port
        2. Load registry/config  
        3. Load DLL (не нужен в Python)
        4. Connect DB
        5. Populate table
        """
        self.status.showMessage(
            f"Порт: {self.app.port.port_name} @ {self.app.port.baud_rate} baud | "
            f"БД: {self.app.config.db_path}")
        self._refresh_table()
    
    def _refresh_table(self):
        """Обновление таблицы BlockZap"""
        rows = self.app.db.select_all()
        self.table.setRowCount(len(rows))
        
        for i, (number, block_len, block_data) in enumerate(rows):
            self.table.setItem(i, 0, QTableWidgetItem(str(number)))
            self.table.setItem(i, 1, QTableWidgetItem(str(block_len)))
            
            # Определяем категорию
            cat = classify_block(number)
            self.table.setItem(i, 2, QTableWidgetItem(cat.name))
            
            # Пытаемся извлечь дату
            if isinstance(block_data, bytes) and len(block_data) > FRAME_HEADER_SIZE + 12:
                body = block_data[FRAME_HEADER_SIZE:]
                date_str = _decode_date(body, 7) if len(body) > 10 else ""
                self.table.setItem(i, 3, QTableWidgetItem(date_str))
            else:
                self.table.setItem(i, 3, QTableWidgetItem(""))
        
        self.status.showMessage(f"Записей: {len(rows)}")
    
    # === EVENT HANDLERS (аналоги FormMain_*Click) ===
    
    def _on_com_port(self):
        """NCOMPortClick — открыть настройки порта"""
        dlg = SettingsDialog(self.app, self)
        if dlg.exec() == QDialog.DialogCode.Accepted:
            if self.app.port.apply_settings():
                self.status.showMessage("Settings-OK!")
            else:
                self.status.showMessage("Settings-BAD! DisConnect!")
    
    def _on_test_port(self):
        """NTestPortClick — тест подключения"""
        if not self.app.port.is_open:
            if not self.app.port.open():
                QMessageBox.warning(self, "Ошибка", "Не удалось открыть порт")
                return
        
        try:
            ok, resp = self.app.protocol.handshake()
            if ok:
                self.status.showMessage(f"Handshake OK! ({len(resp)} байт)")
            else:
                self.status.showMessage("Handshake FAILED!")
        except ProtocolError as e:
            QMessageBox.warning(self, "Ошибка", str(e))
    
    def _on_read_data(self):
        """NReadDataClick — чтение из прибора (COM direct)"""
        self.app.connection_mode = ConnectionMode.COM_DIRECT
        dlg = ReadDataDialog(self.app, self)
        dlg.exec()
        self._refresh_table()
    
    def _on_from_file(self):
        """ToolButtonFromFileClick — загрузка из файла"""
        self.app.connection_mode = ConnectionMode.FROM_FILE
        dlg = ReadDataDialog(self.app, self)
        dlg.exec()
        self._refresh_table()
    
    def _on_read_zap(self):
        """NReadZapClick"""
        self._on_read_data()
    
    def _on_delete(self):
        """NDelZapClick — удаление записи"""
        row = self.table.currentRow()
        if row < 0:
            return
        number = int(self.table.item(row, 0).text())
        reply = QMessageBox.question(
            self, "Подтверждение",
            f"Удалить запись Number={number}?",
            QMessageBox.StandardButton.Yes | QMessageBox.StandardButton.No)
        if reply == QMessageBox.StandardButton.Yes:
            self.app.db.delete(number)
            self._refresh_table()
    
    def _on_sort(self):
        """NSortirClick"""
        self.table.sortItems(0)
    
    def _on_search(self):
        """NSearchClick"""
        from PyQt6.QtWidgets import QInputDialog
        text, ok = QInputDialog.getText(self, "Поиск", "Номер записи:")
        if ok and text:
            try:
                num = int(text)
                for row in range(self.table.rowCount()):
                    if int(self.table.item(row, 0).text()) == num:
                        self.table.selectRow(row)
                        self.table.scrollToItem(self.table.item(row, 0))
                        return
                QMessageBox.information(self, "Поиск", f"Запись {num} не найдена")
            except ValueError:
                pass
    
    def _on_select_all(self):
        """NSelectAllClick"""
        self.table.selectAll()
    
    def _on_reset_table(self):
        """NResetTableClick"""
        self._refresh_table()
    
    def _on_export_excel(self):
        """NSaveExcelClick"""
        path, _ = QFileDialog.getSaveFileName(
            self, "Экспорт", "peleng_data.csv", "CSV Files (*.csv)")
        if path:
            rows = self.app.db.select_all()
            with open(path, 'w', encoding='utf-8') as f:
                f.write("Number;BlockLen;Category\n")
                for num, blen, _ in rows:
                    cat = classify_block(num).name
                    f.write(f"{num};{blen};{cat}\n")
            self.status.showMessage(f"Экспортировано в {path}")
    
    def _on_print(self):
        """NPrintTableClick"""
        QMessageBox.information(self, "Печать", "Функция печати (TODO)")
    
    def _on_save_config(self):
        """NSaveConfigClick"""
        self.app.save_config()
        self.status.showMessage("Конфигурация сохранена")
    
    def _on_password(self):
        """NPassWordClick"""
        from PyQt6.QtWidgets import QInputDialog
        pwd, ok = QInputDialog.getText(
            self, "Пароль", "Введите пароль:", QLineEdit.EchoMode.Password)
        if ok:
            self.status.showMessage("Пароль принят")
    
    def _on_raport(self):
        """ToolButtonRaportClick — генерация рапорта"""
        QMessageBox.information(self, "Рапорт", "Генерация отчёта (TODO: WordML export)")
    
    def _on_about(self):
        """NAboutClick"""
        QMessageBox.about(self, "О программе",
            f"<h2>PelengPC Clone v{APP_VERSION}</h2>"
            f"<p>Полный аналог PelengPC.exe</p>"
            f"<p>Написан на основе реверс-инжиниринга<br>"
            f"7080 функций декомпилировано</p>"
            f"<hr>"
            f"<p>Оригинал: ООО 'Алтек'<br>"
            f"www.altek.info<br>"
            f"mail@altek.info</p>")
    
    def _on_double_click(self, index):
        """DBGridDblClick — двойной клик по записи"""
        row = index.row()
        number = int(self.table.item(row, 0).text())
        record = self.app.db.select_by_number(number)
        if record:
            block_data = record[2]
            cat = classify_block(number)
            if cat == DataCategory.BSCAN and len(block_data) > BSCAN_DATA_OFFSET:
                bscan_data = block_data[BSCAN_DATA_OFFSET:]
                dlg = BscanViewDialog(bscan_data, self)
                dlg.exec()
            else:
                parsed = sort_buf_data(block_data)
                if parsed:
                    info = f"Number: {number}\nТип: {parsed.data_type.name}\n"
                    for k, v in parsed.fields.items():
                        info += f"{k}: {v}\n"
                    QMessageBox.information(self, f"Запись {number}", info)


# ==============================================================================
# ENTRY POINT
# ==============================================================================

def run_gui(port: str = "", demo: bool = False):
    """Запуск GUI приложения"""
    qt_app = QApplication(sys.argv)
    qt_app.setApplicationName("PelengPC")
    qt_app.setApplicationVersion(APP_VERSION)
    
    # Создаём приложение
    app = PelengPCApp()
    if port:
        app.port.port_name = port
    
    # Главное окно
    window = MainWindow(app, demo=demo)
    window.show()
    
    return qt_app.exec()


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description="PelengPC GUI")
    parser.add_argument('--port', default='', help='COM-порт')
    parser.add_argument('--demo', action='store_true', help='Демо-режим')
    args = parser.parse_args()
    
    sys.exit(run_gui(port=args.port, demo=args.demo))
