from data_sort import parse
import sys

def run(filename):
    with open(filename, "r") as f:
        data = f.readlines()
        for line in data:
            print(line[:8])
            if line[:8] == "LM-TIDE:":
                parse(line[:-1])
    
        
if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("ERROR: No file given")
        print("Usage: python file_to_csv.py <filename>")
    else:
        run(sys.argv[1])