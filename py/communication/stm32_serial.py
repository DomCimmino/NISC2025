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
        """Send cube state to STM32 with detailed debugging"""
        if not self.connected:
            raise Exception("Not connected to STM32")
        
        try:
            # Clear buffers before sending
            self.serial_port.reset_input_buffer()
            self.serial_port.reset_output_buffer()
            print("Buffers cleared")
            
            # Get cube data as bytes
            cube_data = cube_state.to_serial_format()
            print(f"Sending cube data ({len(cube_data)} bytes): {cube_data}")
            
            # Send protocol with markers
            protocol_data = b'CUBE_START|' + cube_data + b'|CUBE_END'
            print(f"Full protocol data: {protocol_data}")
            
            # Send all data at once
            bytes_written = self.serial_port.write(protocol_data)
            print(f"Sent {bytes_written} bytes")
            
            # Wait for acknowledgment with detailed output
            print("Waiting for STM32 response (timeout: 5 seconds)...")
            print("=" * 50)
            
            start_time = time.time()
            timeout = 5.0
            ack_received = False
            
            while time.time() - start_time < timeout and not ack_received:
                if self.serial_port.in_waiting > 0:
                    try:
                        # Read whatever data is available
                        raw_data = self.serial_port.readline()
                        
                        # Try to decode as text first
                        try:
                            decoded = raw_data.decode().strip()
                            print(f"STM32 → {decoded}")
                            
                            if decoded == 'ACK':
                                print("✓ ACK received!")
                                ack_received = True
                            elif 'ACK' in decoded:
                                print("✓ ACK found in message!")
                                ack_received = True
                                
                        except UnicodeDecodeError:
                            # Print hex if not decodable as text
                            hex_data = raw_data.hex()
                            print(f"STM32 → [HEX] {hex_data}")
                            
                            # Check if hex contains ACK pattern (41 43 4B)
                            if '41434b' in hex_data.lower():
                                print("✓ ACK pattern found in hex data!")
                                ack_received = True
                                
                    except Exception as e:
                        print(f"Error processing response: {e}")
                        break
                else:
                    # Show we're still waiting
                    if int((time.time() - start_time) * 2) % 2 == 0:  # Blink every 0.5s
                        print("Waiting...", end='\r')
                    time.sleep(0.1)
            
            print("=" * 50)
            
            if ack_received:
                print("✓ Cube state sent successfully!")
                return True
            else:
                print("✗ No ACK received within timeout")
                print("Possible issues:")
                print("- STM32 not running correct firmware")
                print("- Baud rate mismatch")
                print("- Serial connection issues")
                print("- Protocol mismatch")
                return False
                
        except Exception as e:
            print(f"Error sending data: {e}")
            import traceback
            traceback.print_exc()
            return False
    
    def receive_data(self):
        """Receive data from STM32"""
        if self.connected:
            try:
                return self.serial_port.readline().decode().strip()
            except:
                return None
        return None