import serial

def read_serial(port="/dev/ttyUSB0", baudrate=9600) :
    ser = serial.Serial(port, baudrate, timeout=1)

    while True :
        # Read the oldest line in the USB driver buffer
        line = ser.readline().decode(errors="ignore").strip()

        # Create a file generator
        if line :
            yield line
