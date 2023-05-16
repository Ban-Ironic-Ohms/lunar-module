import json
from datetime import datetime
import time
import random

# RECIVING DATA IN FOLLOWING FORMAT:
"""
LM-TIDE:GROUPNAME:TEMP:HUMID:ECO2:TVOC:H2:ETHANOL
"""

# Somethign something something, the data is recived. Passing to new function

def parse(data):
    # data comes in as a string
    if data[:8] != "LM-TIDE:":
        print("ERROR: Data not in correct format")
        print(f"header --{data[:8]}-- not correct")
        return False
    data = data[8:].split(":")
    
    if len(data) != 7:
        print("ERROR: Data not in correct format")
        print(f"Data length is {len(data)}")
        return False

    push_json(data)

def push_json(data):
    filename = f"{data[0]}.json"
    
    with open (filename, "r") as f:
        decoder =  json.JSONDecoder()
        file = decoder.decode(f.read())
        
    now = str(datetime.now())
    print(now)
    data = [float(i) for i in data[1:]]
    file[now] = data
    
    with open(filename, "w") as f:
        json.dump(file, f, indent=4)

def push_csv(data):
    filenae = f"{data[0]}.csv"
    
    # write to the csv file where there is a new line for each data point, and the headers are the first line
    # headers will be: time, temp, humid, eco2, tvoc, h2, ethanol
    with open(filename, "a") as f:
        f.write(f"{data[0]}, {data[1]}, {data[2]}, {data[3]}, {data[4]}, {data[5]}, {data[6]}\n")
        
    
    
# while True:
#     time.sleep(.1)
#     parse(f"LM-TIDE:group{random.randint(1,10)}:{random.randrange(0, 35)}:{random.randrange(0,100)}:{random.randrange(300, 900)}:{random.randrange(300, 900)}:{random.randrange(300, 900)}:{random.randrange(300, 900)}")
    