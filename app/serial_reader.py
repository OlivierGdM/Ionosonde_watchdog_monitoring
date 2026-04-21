import serial
import time

def read_serial(port="/dev/ttyACM0", baudrate=9600) :
    """Arduino output logs queue generator"""
    flag = True

    # Serial connection loop
    while True :    
        try :
            ser = serial.Serial(port, baudrate, timeout=1)
            yield "Serial connection established"
        
        # Catches an error when the serial connection attemp fails
        except serial.SerialException as err :
            # Yield error message once to avoid flooding
            if flag :
                flag = False
                yield f"Serial connection failed with port {port} and baudrate {baudrate}. Trying again..."
                yield f"{err}"
            # Wait and initiate a new connection attemp
            time.sleep(2)
            continue


        # Serial reader loop
        while True :  
            try :
                # Read the oldest line in the USB driver buffer
                line = ser.readline().decode(errors="ignore").strip()
                # Adds a line to the queue if there exists one, otherwise wait for a new one
                if line :
                    yield line
            
            # Catches an error when the usb is disconnected and its buffer is empty
            except serial.SerialException as err : 
                ser.close()
                yield "Serial connection cut."
                yield f"{err}"

                # Break reading loop. Initiate reconnection
                flag = True
                break 