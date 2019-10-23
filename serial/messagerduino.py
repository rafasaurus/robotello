from serial import Serial
from time import sleep

port = "COM10"
rate = 115200

s1 = Serial(port,rate)
s1.flushInput()

while True:
    if s1.inWaiting()>0:
        inputValue = s1.readline()
        print(inputValue)
        break
print("I'm Raspberry pi")
sleep(1)
while True:
    text = input("I'm ready to give comand = ")
    s1.write(str.encode(text))
    sleep(1.5)
    if s1.inWaiting()>0:
        inputValue = s1.readline()
        print(inputValue)
        #break
    else:
        print("debug")
