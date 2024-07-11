import serial
import time
import cv2
from time import sleep
from picamera import PiCamera
import numpy as np
import io

class MosquitoScanner:
    def __init__(self, port='/dev/ttyUSB0', baudrate=9600, timeout=1):
        self.ser = serial.Serial(port, baudrate, timeout=timeout)
        time.sleep(2) #give time to arduino to reset if needed
        self.camera = PiCamera()
        self.angle_h = 0  # Horizontal angle (Motor 1)
        self.angle_v = 0  # Vertical angle (Motor 2)
        time.sleep(1)

    def send_command(self, command):
        self.ser.write((command+"\n").encode("utf-8"))
        sleep(1) #Give arduino time to process command
        
        # To print command each time it is send to and received by Arduino
        while self.ser.in_waiting:
                response = self.ser.readline().decode().strip()
                if response:
                        print(f"{response}")

    def rotate_motor_1(self):
        command = "rotatem"  # Rotate Motor 1 by 11.25 degrees
        self.send_command(command)
        self.angle_h += 11.25
        if self.angle_h >= 360:
            self.angle_h -= 360

    def rotate_motor_2_clockwise(self):
        command = "rotatemm"  # Rotate Motor 2 by 5 degrees clockwise
        self.send_command(command)
        self.angle_v += 5

    def rotate_motor_2_anticlockwise(self):
        command = "rotatemmm"  # Rotate Motor 2 by 5 degrees anti-clockwise
        self.send_command(command)
        self.angle_v -= 5

    def capture_image(self):
        filename = f'image_v{int(self.angle_v)}_h{int(self.angle_h)}.jpg'
        self.camera.resolution = (1920, 1088)
        sleep(2)  # Allow time for the camera to adjust
        stream = io.BytesIO()
        self.camera.capture(stream, format='jpeg')
        stream.seek(0)
        image = np.frombuffer(stream.getvalue(), dtype=np.uint8)
        image = cv2.imdecode(image, cv2.IMREAD_COLOR)
        cv2.imwrite(filename, image)
        print(f'Image captured: {filename}')
        sleep(2)  # Allow time for the camera to reset

    def scan(self):
        # Move Motor 2 to -20 degrees (4 steps anti-clockwise : 0 -> -5 -> -10 -> -15 -> -20)
        for p in range(4):
            self.rotate_motor_2_anticlockwise()
        
        # Scan process
        vertical_steps = 9  # From -20 to 20 degrees in 5-degree increments (-20 -> -15 -> -10 -> -5 -> 0 -> 5 -> 10 -> 15 -> 20) )
        horizontal_steps = 32  # Full 360-degree rotation with 11.25-degree steps (0 -> 11.25 -> 22.5 -> 33.75 -> 45 -> .... -> 360)
        
        for q in range(vertical_steps):
            for r in range(horizontal_steps):
                self.rotate_motor_1()
                self.capture_image()
            if q < vertical_steps - 1:  # Avoid extra step after last vertical position ( that is at 20 , we don't need one more 5 degree rotation of motor2 )
                self.rotate_motor_2_clockwise()
        
        # Reset Motor 2 to horizontal position (20 degrees anti-clockwise : 20 -> 15 -> 10 -> 5 -> 0 )
        for s in range(4):
            self.rotate_motor_2_anticlockwise()
            
            
    def close (self) :
            self.camera.close() # to release camera resources
            self.ser.close() # to close the serial conncection

if __name__ == "__main__":
    scanner = MosquitoScanner()
    try :
            scanner.scan()
    finally :
            scanner.close()
