import sys
import numpy as np
from PyQt5.QtWidgets import (QApplication, QMainWindow, QWidget, QVBoxLayout, 
                             QHBoxLayout, QPushButton, QLabel, QComboBox, 
                             QGridLayout, QGroupBox, QMessageBox, QStatusBar,
                             QSpacerItem, QSizePolicy)
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QColor, QFont
import random

from models.cube_state import FaceColor, CubeState
from communication.stm32_serial import STM32Communicator

class ColorButton(QPushButton):
    def __init__(self, color_value, parent=None):
        super().__init__(parent)
        self.color_value = color_value
        self.setFixedSize(40, 40)  # Increased size
        self.update_color()
        
    def update_color(self):
        color_map = {
            FaceColor.WHITE.value: QColor(255, 255, 255),
            FaceColor.YELLOW.value: QColor(255, 255, 0),
            FaceColor.RED.value: QColor(255, 0, 0),
            FaceColor.ORANGE.value: QColor(255, 165, 0),
            FaceColor.BLUE.value: QColor(0, 0, 255),
            FaceColor.GREEN.value: QColor(0, 255, 0)
        }
        
        color = color_map.get(self.color_value, QColor(200, 200, 200))
        self.setStyleSheet(f"""
            QPushButton {{
                background-color: {color.name()};
                border: 2px solid #333;
                border-radius: 4px;
            }}
            QPushButton:hover {{
                border: 2px solid #000;
            }}
        """)
    
    def set_color(self, color_value):
        self.color_value = color_value
        self.update_color()

class CubeFaceWidget(QWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.buttons = []
        self.init_ui()
    
    def init_ui(self):
        main_layout = QVBoxLayout()
        main_layout.setAlignment(Qt.AlignCenter)
        
        # Face label with styling
        self.face_label = QLabel('U')
        self.face_label.setAlignment(Qt.AlignCenter)
        self.face_label.setFont(QFont('Arial', 16, QFont.Bold))
        self.face_label.setStyleSheet("color: #333; margin: 5px;")
        main_layout.addWidget(self.face_label)
        
        # Cube grid layout
        grid_widget = QWidget()
        grid_layout = QGridLayout(grid_widget)
        grid_layout.setSpacing(1)  # Reduced spacing for closer buttons
        grid_layout.setContentsMargins(0, 0, 0, 0)
        
        for row in range(3):
            row_buttons = []
            for col in range(3):
                btn = ColorButton(FaceColor.WHITE.value)
                btn.clicked.connect(self.create_color_click_handler(btn))
                grid_layout.addWidget(btn, row, col)
                row_buttons.append(btn)
            self.buttons.append(row_buttons)
        
        main_layout.addWidget(grid_widget, alignment=Qt.AlignCenter)
        self.setLayout(main_layout)
    
    def set_face_name(self, face_name):
        self.face_label.setText(face_name)
    
    def create_color_click_handler(self, button):
        def handler():
            current_color = button.color_value
            new_color = (current_color + 1) % len(FaceColor)
            button.set_color(new_color)
        return handler
    
    def get_face_data(self):
        data = []
        for row in range(3):
            for col in range(3):
                data.append(self.buttons[row][col].color_value)
        return np.array(data).reshape(3, 3)
    
    def set_face_data(self, face_data):
        for row in range(3):
            for col in range(3):
                self.buttons[row][col].set_color(face_data[row, col])

class CubeUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.cube_state = CubeState()
        self.communicator = STM32Communicator()
        self.current_face = 'U'
        # Initialize current face widget FIRST
        self.current_face_widget = CubeFaceWidget()
        self.init_ui()
        self.load_face('U')
    
    def init_ui(self):
        self.setWindowTitle('Rubik\'s Cube Controller')
        self.setGeometry(100, 100, 900, 700)  # Increased height for new button
        
        # Main central widget
        central_widget = QWidget()
        main_layout = QHBoxLayout(central_widget)
        main_layout.setSpacing(20)
        main_layout.setContentsMargins(20, 20, 20, 20)
        
        # Left side - Cube faces section
        faces_group = QGroupBox("Cube Display")
        faces_group.setStyleSheet("""
            QGroupBox {
                font-weight: bold;
                border: 2px solid #ccc;
                border-radius: 8px;
                margin-top: 10px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px 0 5px;
            }
        """)
        
        faces_layout = QVBoxLayout(faces_group)
        faces_layout.setSpacing(15)
        
        # Face selection with styling
        face_select_layout = QHBoxLayout()
        face_select_layout.addWidget(QLabel("Select Face:"))
        
        self.face_combo = QComboBox()
        self.face_combo.addItems(['U', 'D', 'F', 'B', 'L', 'R'])
        self.face_combo.setCurrentText('U')
        self.face_combo.setFixedWidth(80)
        self.face_combo.setStyleSheet("""
            QComboBox {
                padding: 5px;
                border: 1px solid #ccc;
                border-radius: 4px;
            }
        """)
        
        face_select_layout.addWidget(self.face_combo)
        face_select_layout.addStretch()
        faces_layout.addLayout(face_select_layout)
        
        # Current face display - centered
        face_display_widget = QWidget()
        face_display_layout = QVBoxLayout(face_display_widget)
        face_display_layout.setAlignment(Qt.AlignCenter)
        face_display_layout.addWidget(self.current_face_widget)
        faces_layout.addWidget(face_display_widget)
        
        # Navigation buttons
        nav_layout = QHBoxLayout()
        nav_layout.setSpacing(10)
        
        self.prev_btn = QPushButton("◀ Previous Face")
        self.next_btn = QPushButton("Next Face ▶")
        
        for btn in [self.prev_btn, self.next_btn]:
            btn.setFixedHeight(35)
            btn.setStyleSheet("""
                QPushButton {
                    background-color: #f0f0f0;
                    border: 1px solid #ccc;
                    border-radius: 5px;
                    padding: 5px 10px;
                }
                QPushButton:hover {
                    background-color: #e0e0e0;
                }
            """)
        
        self.prev_btn.clicked.connect(self.previous_face)
        self.next_btn.clicked.connect(self.next_face)
        
        nav_layout.addWidget(self.prev_btn)
        nav_layout.addWidget(self.next_btn)
        faces_layout.addLayout(nav_layout)
        
        # Right side - Control panel
        control_group = QGroupBox("Controls")
        control_group.setStyleSheet("""
            QGroupBox {
                font-weight: bold;
                border: 2px solid #ccc;
                border-radius: 8px;
                margin-top: 10px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                left: 10px;
                padding: 0 5px 0 5px;
            }
        """)
        
        control_layout = QVBoxLayout(control_group)
        control_layout.setSpacing(10)
        
        # Connection controls
        self.connect_btn = QPushButton("🔌 Connect to STM32")
        self.disconnect_btn = QPushButton("🔌 Disconnect")
        self.disconnect_btn.setEnabled(False)
        
        for btn in [self.connect_btn, self.disconnect_btn]:
            btn.setFixedHeight(40)
            btn.setStyleSheet("""
                QPushButton {
                    background-color: #e3f2fd;
                    border: 1px solid #90caf9;
                    border-radius: 5px;
                    padding: 5px;
                }
                QPushButton:hover {
                    background-color: #bbdefb;
                }
                QPushButton:disabled {
                    background-color: #f5f5f5;
                    color: #999;
                }
            """)
        
        conn_layout = QHBoxLayout()
        conn_layout.addWidget(self.connect_btn)
        conn_layout.addWidget(self.disconnect_btn)
        control_layout.addLayout(conn_layout)
        
        # Action buttons
        self.send_btn = QPushButton("📤 Send to STM32")
        self.reset_btn = QPushButton("🔄 Reset Face")
        self.reset_all_btn = QPushButton("🗑️ Reset All Faces")
        self.solve_btn = QPushButton("✅ Set Face Solved")
        self.solve_all_btn = QPushButton("🏆 Set All Solved")
        self.random_btn = QPushButton("🎲 Random Solvable Config")  # New button
        
        action_buttons = [
            self.send_btn,
            self.reset_btn,
            self.reset_all_btn,
            self.solve_btn,
            self.solve_all_btn,
            self.random_btn  # Added new button
        ]
        
        for btn in action_buttons:
            btn.setFixedHeight(40)
            btn.setStyleSheet("""
                QPushButton {
                    background-color: #f5f5f5;
                    border: 1px solid #ddd;
                    border-radius: 5px;
                    padding: 5px;
                    text-align: left;
                    padding-left: 15px;
                }
                QPushButton:hover {
                    background-color: #e0e0e0;
                }
            """)
            control_layout.addWidget(btn)
        
        # Set initial states
        self.send_btn.setEnabled(False)
        
        # Connect signals
        self.connect_btn.clicked.connect(self.connect_to_stm32)
        self.disconnect_btn.clicked.connect(self.disconnect_from_stm32)
        self.send_btn.clicked.connect(self.send_cube_state)
        self.reset_btn.clicked.connect(self.reset_face)
        self.reset_all_btn.clicked.connect(self.reset_all_faces)
        self.solve_btn.clicked.connect(self.set_face_solved)
        self.solve_all_btn.clicked.connect(self.set_all_solved)
        self.random_btn.clicked.connect(self.generate_random_solvable)  # New connection
        self.face_combo.currentTextChanged.connect(self.face_changed)
        
        # Add widgets to main layout
        main_layout.addWidget(faces_group, 3)
        main_layout.addWidget(control_group, 2)
        
        self.setCentralWidget(central_widget)
        
        # Status bar with styling
        self.statusBar().setStyleSheet("""
            QStatusBar {
                background-color: #f0f0f0;
                color: #333;
                border-top: 1px solid #ccc;
            }
        """)
        self.statusBar().showMessage("Ready")
    
    def save_current_face(self):
        face_data = self.current_face_widget.get_face_data()
        self.cube_state.set_face(self.current_face, face_data)
    
    def load_face(self, face_name):
        self.current_face = face_name
        self.current_face_widget.set_face_name(face_name)
        
        face_data = self.cube_state.get_face(face_name)
        if face_data is not None:
            self.current_face_widget.set_face_data(face_data)
    
    def face_changed(self, face_name):
        self.save_current_face()
        self.load_face(face_name)
        self.statusBar().showMessage(f"Switched to {face_name} face")
    
    def connect_to_stm32(self):
        try:
            if self.communicator.connect():
                self.statusBar().showMessage("Connected to STM32")
                self.connect_btn.setEnabled(False)
                self.disconnect_btn.setEnabled(True)
                self.send_btn.setEnabled(True)
        except Exception as e:
            QMessageBox.critical(self, "Connection Error", str(e))
    
    def disconnect_from_stm32(self):
        self.communicator.disconnect()
        self.statusBar().showMessage("Disconnected")
        self.connect_btn.setEnabled(True)
        self.disconnect_btn.setEnabled(False)
        self.send_btn.setEnabled(False)
    
    def send_cube_state(self):
        self.save_current_face()
        if self.communicator.send_cube_state(self.cube_state):
            self.statusBar().showMessage("Cube state sent successfully")
        else:
            self.statusBar().showMessage("Failed to send cube state")
    
    def reset_face(self):
        self.current_face_widget.set_face_data(
            np.full((3, 3), FaceColor.WHITE.value)
        )
        self.statusBar().showMessage(f"Reset {self.current_face} face to white")
    
    def reset_all_faces(self):
        for face in ['U', 'D', 'F', 'B', 'L', 'R']:
            self.cube_state.set_face(face, np.full((3, 3), FaceColor.WHITE.value))
        self.load_face(self.current_face)
        self.statusBar().showMessage("Reset all faces to white")
    
    def set_face_solved(self):
        face_colors = {
            'U': FaceColor.WHITE.value,
            'D': FaceColor.YELLOW.value,
            'F': FaceColor.RED.value,
            'B': FaceColor.ORANGE.value,
            'L': FaceColor.BLUE.value,
            'R': FaceColor.GREEN.value
        }
        color = face_colors.get(self.current_face, FaceColor.WHITE.value)
        self.current_face_widget.set_face_data(np.full((3, 3), color))
        self.statusBar().showMessage(f"Set {self.current_face} face to solved state")
    
    def set_all_solved(self):
        face_colors = {
            'U': FaceColor.WHITE.value,
            'D': FaceColor.YELLOW.value,
            'F': FaceColor.RED.value,
            'B': FaceColor.ORANGE.value,
            'L': FaceColor.BLUE.value,
            'R': FaceColor.GREEN.value
        }
        for face, color in face_colors.items():
            self.cube_state.set_face(face, np.full((3, 3), color))
        self.load_face(self.current_face)
        self.statusBar().showMessage("Set all faces to solved state")
    
    def generate_random_solvable(self):
        """Generate a random but solvable cube configuration"""
        # Start with a solved cube
        self.set_all_solved()
        
        # Apply a series of random moves to scramble it
        moves = ["U", "U'", "D", "D'", "F", "F'", "B", "B'", "L", "L'", "R", "R'"]
        num_moves = random.randint(10, 20)  # Apply 10-20 random moves
        
        for _ in range(num_moves):
            move = random.choice(moves)
            self.apply_move(move)
        
        self.load_face(self.current_face)
        self.statusBar().showMessage(f"Generated random solvable configuration ({num_moves} moves)")
    
    def apply_move(self, move):
        """Apply a cube move to the current state"""
        # Get all face data
        faces = {}
        for face_name in ['U', 'D', 'F', 'B', 'L', 'R']:
            faces[face_name] = self.cube_state.get_face(face_name).copy()
        
        # Apply the move by rotating the appropriate face and adjacent stickers
        if move == "U":
            # Rotate U face clockwise
            faces['U'] = np.rot90(faces['U'], 3)
            # Rotate top layer of adjacent faces
            temp = faces['F'][0, :].copy()
            faces['F'][0, :] = faces['R'][0, :]
            faces['R'][0, :] = faces['B'][0, :]
            faces['B'][0, :] = faces['L'][0, :]
            faces['L'][0, :] = temp
            
        elif move == "U'":
            # Rotate U face counterclockwise
            faces['U'] = np.rot90(faces['U'], 1)
            # Rotate top layer of adjacent faces
            temp = faces['F'][0, :].copy()
            faces['F'][0, :] = faces['L'][0, :]
            faces['L'][0, :] = faces['B'][0, :]
            faces['B'][0, :] = faces['R'][0, :]
            faces['R'][0, :] = temp
            
        elif move == "D":
            # Rotate D face clockwise
            faces['D'] = np.rot90(faces['D'], 3)
            # Rotate bottom layer of adjacent faces
            temp = faces['F'][2, :].copy()
            faces['F'][2, :] = faces['L'][2, :]
            faces['L'][2, :] = faces['B'][2, :]
            faces['B'][2, :] = faces['R'][2, :]
            faces['R'][2, :] = temp
            
        elif move == "D'":
            # Rotate D face counterclockwise
            faces['D'] = np.rot90(faces['D'], 1)
            # Rotate bottom layer of adjacent faces
            temp = faces['F'][2, :].copy()
            faces['F'][2, :] = faces['R'][2, :]
            faces['R'][2, :] = faces['B'][2, :]
            faces['B'][2, :] = faces['L'][2, :]
            faces['L'][2, :] = temp
            
        elif move == "F":
            # Rotate F face clockwise
            faces['F'] = np.rot90(faces['F'], 3)
            # Rotate front layer of adjacent faces
            temp = faces['U'][2, :].copy()
            faces['U'][2, :] = np.flip(faces['L'][:, 2])
            faces['L'][:, 2] = faces['D'][0, :]
            faces['D'][0, :] = np.flip(faces['R'][:, 0])
            faces['R'][:, 0] = temp
            
        elif move == "F'":
            # Rotate F face counterclockwise
            faces['F'] = np.rot90(faces['F'], 1)
            # Rotate front layer of adjacent faces
            temp = faces['U'][2, :].copy()
            faces['U'][2, :] = faces['R'][:, 0]
            faces['R'][:, 0] = np.flip(faces['D'][0, :])
            faces['D'][0, :] = faces['L'][:, 2]
            faces['L'][:, 2] = np.flip(temp)
            
        elif move == "B":
            # Rotate B face clockwise
            faces['B'] = np.rot90(faces['B'], 3)
            # Rotate back layer of adjacent faces
            temp = faces['U'][0, :].copy()
            faces['U'][0, :] = faces['R'][:, 2]
            faces['R'][:, 2] = np.flip(faces['D'][2, :])
            faces['D'][2, :] = faces['L'][:, 0]
            faces['L'][:, 0] = np.flip(temp)
            
        elif move == "B'":
            # Rotate B face counterclockwise
            faces['B'] = np.rot90(faces['B'], 1)
            # Rotate back layer of adjacent faces
            temp = faces['U'][0, :].copy()
            faces['U'][0, :] = np.flip(faces['L'][:, 0])
            faces['L'][:, 0] = faces['D'][2, :]
            faces['D'][2, :] = np.flip(faces['R'][:, 2])
            faces['R'][:, 2] = temp
            
        elif move == "L":
            # Rotate L face clockwise
            faces['L'] = np.rot90(faces['L'], 3)
            # Rotate left layer of adjacent faces
            temp = faces['U'][:, 0].copy()
            faces['U'][:, 0] = np.flip(faces['B'][:, 2])
            faces['B'][:, 2] = np.flip(faces['D'][:, 0])
            faces['D'][:, 0] = faces['F'][:, 0]
            faces['F'][:, 0] = temp
            
        elif move == "L'":
            # Rotate L face counterclockwise
            faces['L'] = np.rot90(faces['L'], 1)
            # Rotate left layer of adjacent faces
            temp = faces['U'][:, 0].copy()
            faces['U'][:, 0] = faces['F'][:, 0]
            faces['F'][:, 0] = faces['D'][:, 0]
            faces['D'][:, 0] = np.flip(faces['B'][:, 2])
            faces['B'][:, 2] = np.flip(temp)
            
        elif move == "R":
            # Rotate R face clockwise
            faces['R'] = np.rot90(faces['R'], 3)
            # Rotate right layer of adjacent faces
            temp = faces['U'][:, 2].copy()
            faces['U'][:, 2] = faces['F'][:, 2]
            faces['F'][:, 2] = faces['D'][:, 2]
            faces['D'][:, 2] = np.flip(faces['B'][:, 0])
            faces['B'][:, 0] = np.flip(temp)
            
        elif move == "R'":
            # Rotate R face counterclockwise
            faces['R'] = np.rot90(faces['R'], 1)
            # Rotate right layer of adjacent faces
            temp = faces['U'][:, 2].copy()
            faces['U'][:, 2] = np.flip(faces['B'][:, 0])
            faces['B'][:, 0] = np.flip(faces['D'][:, 2])
            faces['D'][:, 2] = faces['F'][:, 2]
            faces['F'][:, 2] = temp
        
        # Update all faces in the cube state
        for face_name, face_data in faces.items():
            self.cube_state.set_face(face_name, face_data)
    
    def previous_face(self):
        current_index = self.face_combo.currentIndex()
        new_index = (current_index - 1) % self.face_combo.count()
        self.face_combo.setCurrentIndex(new_index)
    
    def next_face(self):
        current_index = self.face_combo.currentIndex()
        new_index = (current_index + 1) % self.face_combo.count()
        self.face_combo.setCurrentIndex(new_index)
    
    def closeEvent(self, event):
        if self.communicator.connected:
            self.communicator.disconnect()
        event.accept()

def main():
    app = QApplication(sys.argv)
    window = CubeUI()
    window.show()
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()