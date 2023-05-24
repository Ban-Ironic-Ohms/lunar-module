import serial
from data_sort import parse
import time
import sys

# baud = 9600
# port = '/dev/ttyACM0'
# ser = serial.Serial(port, baud, timeout=1)

def run():
    ser = serial.Serial(port=sys.argv[1],baudrate=int(sys.argv[2]))
    # ser = serial.Serial(port, baud, timeout=1)
    while True:
        try:
            while ser.in_waiting:
                data = ser.readline().decode('utf-8')[:-1]
                print(data)
                if data[:8] == "LM-TIDE:":
                    parse(data)
        except Exception as e:
            print(e)
            print("oops! Somethign went wrong. Waiting 10 seconds and retrying")
            time.sleep(10)
            run()
        # print("oops! Somethign went wrong. Waiting 10 seconds and retrying")
        # time.sleep(10)
        # run()

        
if __name__ == "__main__":
    run()