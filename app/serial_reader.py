import serial
import time

def read_serial(port="/dev/ttyACM0", baudrate=9600) :
    """Arduino output logs queue generator"""
    flag = True

    while True :    # Serial connection loop
        try :
            ser = serial.Serial(port, baudrate, timeout=1)
            yield "Serial connection established"

            while True :  
                try :
                    # Read the oldest line in the USB driver buffer
                    print(1)
                    line = ser.readline().decode(errors="ignore").strip()

                    # Adds a line to the queue if there exists one, otherwise wait for a new one
                    if line :
                        yield line

                except serial.SerialException as err : # Loop ends when connection is lost and buffer is empty
                    print(2)
                    yield "Serial connection cut."
                    yield f"{err}"
                    break

        except serial.SerialException as err :
            if flag :
                # flag = False
                yield f"Serial connection failed with port {port} and baudrate {baudrate}."
                yield f"{err}"
            time.sleep(2)
            print("again")
    print("fin")

