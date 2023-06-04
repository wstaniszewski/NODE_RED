import machine
import time

# Define LED pins
led_pin_1 = machine.Pin(20, machine.Pin.OUT)
led_pin_2 = machine.Pin(21, machine.Pin.OUT)

# Blink the LEDs
while True:
    led_pin_1.on()  # Turn on LED 1
    led_pin_2.off()  # Turn off LED 2
    time.sleep(1)  # Wait for 1 second

    led_pin_1.off()  # Turn off LED 1
    led_pin_2.on()  # Turn on LED 2
    time.sleep(1)  # Wait for 1 second

# add code for blinking of internal leds



