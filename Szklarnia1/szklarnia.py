import machine
import network
import utime
from umqtt.simple2 import MQTTClient

# Define LED pin
led_pin = machine.Pin(20, machine.Pin.OUT)

# Define Wi-Fi settings
wifi_ssid = "wojtek"
wifi_password = "staniszewski123456"

# Define MQTT settings
mqtt_broker = "194.183.54.87"
mqtt_user = "wojtek"
mqtt_password = "69AGjfsuWm8aMviMtTfdMPCpjz68mj"
mqtt_topic_analog = "szklarnia/analog"
mqtt_topic_cyrkulacja1 = "szklarnia/cyrkulacja1"

# Connect to Wi-Fi
wifi = network.WLAN(network.STA_IF)
wifi.active(True)
wifi.connect(wifi_ssid, wifi_password)

# Wait until connected to Wi-Fi
while not wifi.isconnected():
    utime.sleep(1)

print("Connected to Wi-Fi")
print("IP address:", wifi.ifconfig()[0])

# Define MQTT callback function
def mqtt_callback(topic, msg):
    print("Received message:", msg)
    if topic.decode() == mqtt_topic_cyrkulacja1:
        if msg == b"on":
            led_pin.on()
            print("LED switched ON")
        elif msg == b"off":
            led_pin.off()
            print("LED switched OFF")

# Connect to MQTT broker
mqtt_client = MQTTClient("pico_client", mqtt_broker, user=mqtt_user, password=mqtt_password)
mqtt_client.set_callback(mqtt_callback)
mqtt_client.connect()

print("Connected to MQTT broker")

# Subscribe to MQTT topic
mqtt_client.subscribe(mqtt_topic_cyrkulacja1)
print("Subscribed to topic:", mqtt_topic_cyrkulacja1)

# Main loop
while True:
    mqtt_client.check_msg()  # Check for incoming MQTT messages

    # Read A0 value
    adc = machine.ADC(machine.Pin(26))
    a0_value = adc.read()

    # Publish A0 value on MQTT topic
    mqtt_client.publish(mqtt_topic_analog, str(a0_value))
    print("Published A0 value:", a0_value)

    utime.sleep(1)
