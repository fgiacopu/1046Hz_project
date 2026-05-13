# arduino_input.py

class ArduinoInput:
    def __init__(self, port="/dev/ttyACM0", baudrate=9600):
        self.port = port
        self.baudrate = baudrate
        self.enabled = False  # disattivato di default

    def connect(self):
        try:
            import serial
            self.serial = serial.Serial(self.port, self.baudrate, timeout=1)
            self.enabled = True
        except Exception as e:
            print("Arduino non connesso:", e)
            self.enabled = False

    def read(self):
        if not self.enabled:
            return None

        try:
            line = self.serial.readline().decode().strip()
            if line:
                return float(line)
        except:
            pass

        return None