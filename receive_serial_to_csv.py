import serial
from data_sort import parse
import time

baud = 9600
port = '/dev/ttyACM0'
ser = serial.Serial(port, baud, timeout=1)

def run():
    while True:
        try:
            while ser.in_waiting:
                data = ser.readline().decode('utf-8')[:-1]
                print(data)
                if data[:8] == "LM-TIDE:":
                    parse(data)
        except Exception as e:
            print(e)
            print("oops! Somethign went wrong. Waiting 30 seconds and retrying")
            time.sleep(30)
            run()
        # print("oops! Somethign went wrong. Waiting 30 seconds and retrying")
        # time.sleep(30)
        # run()

        
if __name__ == "__main__":
    run()