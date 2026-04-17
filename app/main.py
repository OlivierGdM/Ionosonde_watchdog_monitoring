from serial_reader import read_serial
from logger import write_log

def main() :
    for line in read_serial() : 
        write_log(line)