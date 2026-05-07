# config.py

# Webcam
CAMERA_INDEX = 0   # 0 = integrated webcam, 1 = webcam USB

# Hand tracking parameters
MAX_HANDS = 2
MIN_DET_CONF = 0.6
MIN_TRACK_CONF = 0.5

# OSC
OSC_IP = "127.0.0.1"
OSC_PORT = 9000

# Arduino (optional)
USE_ARDUINO = False
ARDUINO_PORT = "COM3"      # change only if you use Arduino
ARDUINO_BAUDRATE = 9600