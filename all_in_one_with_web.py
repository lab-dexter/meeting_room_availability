import RPi.GPIO as GPIO
import time
import requests
from variables import *

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

#define sound sensor PIN connection and mode
SOUND_PIN = sound_pin_number
GPIO.setup(SOUND_PIN, GPIO.IN)

#define PIR sensor PIN connection and mode
PIR_PIN = pir_pin_number
GPIO.setup(PIR_PIN, GPIO.IN)

#defined LED PIN connection and mode
LED_PIN = led_pin_number
GPIO.setup(LED_PIN,GPIO.OUT,initial=GPIO.LOW)

#define NodeMCU IP address variable
MCU_IP = nodemcu_IP_address
turnedoff = True
start = time.time()

#LED turn on function
def LED_ON():
        global start
        global turnedoff
        start = time.time()
        if turnedoff:
                print "Turned LED on: ", time.strftime("%H:%M"), "\n"
                GPIO.output(LED_PIN,GPIO.HIGH)
                QUERY_NodeMCU('in_use')
                turnedoff = False
        
#function to send request to NodeMcu esp8266 and change LCD screen text
def QUERY_NodeMCU(text):
        try:
                r = requests.get('http://'+MCU_IP+'/'+text)
        except Exception as err:
                print "Something went wrong with connecting to NodeMcu webserver.\nError code: "
                print err


#Sound sensor event detect callback function. It will call LED_ON function
def SOUND(SOUND_PIN): 
        print "Sound Detected: ", time.strftime("%H:%M"), "\n"
        LED_ON()

#PIR sensor event detect callback function. It will call LED_ON function
def MOTION(PIR_PIN): 
        print "Motion Detected: ", time.strftime("%H:%M"), "\n"
        LED_ON()

#-------MAIN PROGRAM STARTS HERE---------
print "Meeting Room Availability Detection System Starting (CTRL+C to exit)"
time.sleep(5)
print "Ready. Monitoring..."

try:
        GPIO.add_event_detect(SOUND_PIN, GPIO.FALLING, callback=SOUND)
        GPIO.add_event_detect(PIR_PIN, GPIO.RISING, callback=MOTION)
        
        GPIO.output(LED_PIN,GPIO.LOW)
        while 1:
                time.sleep(60)
                if time.time() - start > 90 and not turnedoff:
                        print "Turned LED off: ", time.strftime("%H:%M"), "\n"
                        GPIO.output(LED_PIN,GPIO.LOW)
                        QUERY_NodeMCU('free')
                        turnedoff = True
                else:
                        GPIO.output(LED_PIN,GPIO.HIGH)

except KeyboardInterrupt:
        print "Quit"
        GPIO.cleanup()
