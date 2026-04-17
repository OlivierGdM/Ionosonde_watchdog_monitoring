import serial

def read_serial(port="/dev/ttyUSB0", baudrate=9600) :
    """Arduino output logs queue generator"""
    ser = serial.Serial(port, baudrate, timeout=1)

    while True :
        # Read the oldest line in the USB driver buffer
        line = ser.readline().decode(errors="ignore").strip()

        # Adds a line to the queue if there exists one, otherwise wait for a new one
        if line :
            yield line
