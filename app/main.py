from app.serial_reader import read_serial
from app.logger import write_log
import configparser

def main() :

    config = configparser.ConfigParser()
    config.read("config.ini")
    
    usb_port = config.get('Settings', 'usb_port')
    baudrate = config.getint('Settings', 'baudrate')
    log_path = config.get('Settings', 'log_path')

    for line in read_serial(port=usb_port, baudrate=baudrate) : 
        write_log(msg=line, file_path=log_path)
        print(line)