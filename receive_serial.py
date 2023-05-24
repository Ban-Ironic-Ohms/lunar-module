from subprocess import Popen, PIPE

com_ports = ['COM1', 'COM2', 'COM3', 'COM4', 'COM5', 'COM6', 'COM7']
processes = []

for i in com_ports:
    processes.append(Popen(['python3', 'serial_to_csv.py', i, "9600"], stdin=PIPE, stdout=PIPE, stderr=PIPE))
    
# print(processes)