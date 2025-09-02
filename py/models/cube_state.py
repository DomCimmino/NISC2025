import numpy as np
from enum import Enum

class FaceColor(Enum):
    WHITE = 0
    YELLOW = 1
    RED = 2
    ORANGE = 3
    BLUE = 4
    GREEN = 5

class CubeState:
    def __init__(self):
        self.faces = {
            'U': np.full((3, 3), FaceColor.WHITE.value),    
            'D': np.full((3, 3), FaceColor.YELLOW.value),  
            'F': np.full((3, 3), FaceColor.RED.value),   
            'B': np.full((3, 3), FaceColor.ORANGE.value),  
            'L': np.full((3, 3), FaceColor.BLUE.value),   
            'R': np.full((3, 3), FaceColor.GREEN.value) 
        }
    
    def set_face(self, face_name, face_data):
        """Set the state of a specific face"""
        if face_name in self.faces and face_data.shape == (3, 3):
            self.faces[face_name] = face_data
    
    def get_face(self, face_name):
        """Get the state of a specific face"""
        return self.faces.get(face_name, None)
    
    def to_serial_format(self):
        """Convert cube state to serial format for STM32"""
        serial_data = []
        
        for face in ['U', 'D', 'F', 'B', 'L', 'R']:
            face_array = self.faces[face].flatten()
            serial_data.extend(face_array.tolist())
        
        return bytes(serial_data)
    
    def from_serial_format(self, data):
        """Load cube state from serial data"""
        if len(data) != 54:
            return False
        
        index = 0
        for face in ['U', 'D', 'F', 'B', 'L', 'R']:
            face_data = np.array(data[index:index+9]).reshape(3, 3)
            self.faces[face] = face_data
            index += 9
        
        return True