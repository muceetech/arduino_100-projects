from machine import UART
from time import sleep

# connect the following between ESP32C3 mini and Arduino uno (through a voltage level converter)
#  for avoiding confusion , Software serial is used in arduino code
#  ESP32C3_pin   Arduino_pin
#  5V            5V
#  GND           GND
#  GPIO 21(tx)   10 (rx in software serial)
#  GPIO 20(rx)   11 (tx in software serial)

from machine import UART,Pin
from time import sleep

uart1 = UART(0, baudrate=9600, tx=21, rx=20)
P0 = Pin(0, Pin.IN,Pin.PULL_UP)     # create input pin on GPIO0

while True:
    print(P0.value())
    if P0.value() == 1:
        uart1.write("led on\n")  
        sleep(0.2)
    if P0.value() == 0:
        uart1.write("led off\n") 
        sleep(0.2)
    

