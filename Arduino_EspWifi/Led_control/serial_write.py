from machine import UART
from time import sleep

# connect the following between ESP32C3 mini and Arduino uno (through a voltage level converter)
#  for avoiding confusion , Software serial is used in arduino code
#  ESP32C3_pin   Arduino_pin
#  5V            5V
#  GND           GND
#  GPIO 21(tx)   10 (rx in software serial)
#  GPIO 20(rx)   11 (tx in software serial)

uart1 = UART(0, baudrate=9600, tx=21, rx=20)
while True:
    uart1.write("hello\n")  # write 5 bytes
    sleep(1)
