
import network
import time
from machine import Pin,UART
from umqtt.simple import MQTTClient

# Setup Wi-Fi and MQTT
# Connect to WiFi

WIFI_SSID = ""       # Enter wifi SSID name 
WIFI_PASS = ""       # Enter wifi password 
MQTT_BROKER = "broker.hivemq.com"
MQTT_CLIENT_ID = "esp32-traffic"
MQTT_TOPIC = b"esp/led"

# Pins for Red, Yellow, Green LEDs
led = Pin(0, Pin.OUT)
uart1 = UART(0, baudrate=9600, tx=21, rx=20)

led.value(0)     # LED off in ESP32 initially

def connect_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    wlan.connect(WIFI_SSID, WIFI_PASS)
    while not wlan.isconnected():
        time.sleep(0.2)
    print("WiFi Connected!")

def handle_message(topic, msg):
    value1 = msg.decode()
    print("value", value1)
    led.value(0)
    if value1 == "on":
        led.value(1)     # LED connected to GPIO0 in ESP32C3 is on when mqtt message is received "on"
        uart1.write("led on\n")   # send message from esp32c3 to arduino through serial  
        
    elif value1 == "off":
        led.value(0)    # LED connected to GPIO0 in ESP32C3 is off when mqtt message is received "off"
        uart1.write("led off\n")   # send message from esp32c3 to arduino through serial  

def main():
    connect_wifi()
    client = MQTTClient(MQTT_CLIENT_ID, MQTT_BROKER)
    client.set_callback(handle_message)
    client.connect()
    client.subscribe(MQTT_TOPIC)
    print("Connected to MQTT broker")

    while True:
        client.check_msg()
        time.sleep(0.1)

main()

