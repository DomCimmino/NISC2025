import serial
import serial.tools.list_ports
import time
from typing import Optional
import struct

class STM32Communicator:
    def __init__(self, baudrate=115200):
        self.serial_port = None
        self.baudrate = baudrate
        self.connected = False
    
    def find_stm32_port(self):
        """Find the STM32 Nucleo board COM port"""
        ports = serial.tools.list_ports.comports()
        for port in ports:
            if 'STM' in port.description or 'Nucleo' in port.description:
                return port.device
        return None
    
    def connect(self, port: Optional[str] = None):
        """Connect to the STM32 board"""
        if port is None:
            port = self.find_stm32_port()
            if port is None:
                raise Exception("STM32 board not found")
        
        try:
            self.serial_port = serial.Serial(
                port=port,
                baudrate=self.baudrate,
                bytesize=serial.EIGHTBITS,
                parity=serial.PARITY_NONE,
                stopbits=serial.STOPBITS_ONE,
                timeout=2  # Increased timeout for ACK
            )
            self.connected = True
            print(f"Connected to {port}")
            
            # Clear any existing data
            self.serial_port.reset_input_buffer()
            self.serial_port.reset_output_buffer()
            
            return True
        except Exception as e:
            print(f"Connection failed: {e}")
            return False
    
    def disconnect(self):
        """Disconnect from the STM32 board"""
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
        self.connected = False
    
    def send_cube_state(self, cube_state):
        """
        Send cube state as 54 raw chars in the fixed order:
        U(0..8), R(9..17), F(18..26), D(27..35), L(36..44), B(45..53)
        with colors mapped to letters: W,Y,R,O,B,G
        """
        if not self.connected:
            raise Exception("Not connected to STM32")
    
        try:
            self.serial_port.reset_input_buffer()
            self.serial_port.reset_output_buffer()
    
            # Fissa l'ordine delle facce
            face_order = ['U', 'R', 'F', 'D', 'L', 'B']
    
            # Mappa numeri → lettere
            color_map = {0: 'W', 1: 'Y', 2: 'R', 3: 'O', 4: 'B', 5: 'G'}
    
            # Costruisci la stringa in modo preciso
            char_data = ''
            for face in face_order:
                # Flatten row-major (riga per riga)
                char_data += ''.join(color_map[v] for v in cube_state.faces[face].flatten())
    
            if len(char_data) != 54:
                raise ValueError(f"Cube state must be 54 chars, got {len(char_data)}")
    
            print(f"Sending raw cube state: {char_data}")
            self.serial_port.write(char_data.encode())
    
        except Exception as e:
            print(f"Error sending data: {e}")
            import traceback
            traceback.print_exc()
            return False
    
        return True

    def receive_data(self):
        """Receive data from STM32"""
        if self.connected:
            try:
                return self.serial_port.readline().decode().strip()
            except:
                return None
        return None