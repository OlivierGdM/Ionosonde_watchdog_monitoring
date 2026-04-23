import serial
import time
import configparser
from pathlib import Path
from datetime import datetime, timezone



# This function is the main loop of the programme.
# The idea is to generate a queue of mesages from which we pull the oldest, timestamp it and then print it.
#
# It first repeatedly tries to establish the serial connection with the arduino.
# Once the connection is established, it pulls the information from the USB buffer and adds it to the queue.
# If the serial connection is cut, we break the reading loop and go back to the connection loop
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
            # Yield error message once only to avoid flooding
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


# A simple function to timestamp and write a line in the log file
def write_log(msg:str, file_path="logs/arduino.log") :
    timestamp = datetime.now().astimezone(timezone.utc)
    with open(file_path, 'a') as f :
        f.write(f"{timestamp.isoformat()} - {msg}\n") 



if __name__ == "__main__" :
    config = configparser.ConfigParser()
    # Check if config.ini exists
    if not config.read("config.ini") : 
        raise FileNotFoundError(f"Could not find config.ini in {Path.cwd()} directory, with monitor.py.")

    # Check for formatting errors
    else :
        baudrate = config.get('Settings', 'baudrate')
        usb_port = config.get('Settings', 'usb_port')
        log_path = config.get('Settings', 'log_path')

        if not Path(usb_port).is_absolute() :
            raise ValueError("usb_port was not defined as an absolute path in config.ini")
        if not Path(usb_port).is_symlink() :
            raise ValueError("usb_port path was not defined as a symlink path in config.ini")
        
        if not Path(log_path).is_absolute() :
            raise ValueError("log_path was not defined as an absolute path in config.ini")
        if not Path(log_path).is_file() :
            raise ValueError("log_path was not defined as a file path in config.ini")
        
        if not baudrate.isdigit() :
            raise ValueError("baudrate was not defined as an integer in config.ini")
    
    for line in read_serial(port=usb_port, baudrate=int(baudrate)) : 
        write_log(msg=line, file_path=log_path)
        print(line)