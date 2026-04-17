from app.serial_reader import read_serial
from app.logger import write_log

def main() :
    for line in read_serial() : 
        write_log(line)
        print(line)