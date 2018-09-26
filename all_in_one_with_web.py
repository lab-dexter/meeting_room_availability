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
        
#function to send request to NodeMcu esp8266 and change LCD screen text
def QUERY_NodeMCU(text):
        try:
                r = requests.get('http://'+MCU_IP+'/'+text, timeout=(5, 10))
        except Exception as err:
                print "Something went wrong with connecting to NodeMcu webserver.\nError code: "
                print err


#Sound sensor event detect callback function. It will call LED_ON function
def SOUND(SOUND_PIN): 
        print "Sound Detected"
        global inuse
        inuse = True

#PIR sensor event detect callback function. It will call LED_ON function
def MOTION(PIR_PIN): 
        print "Motion Detected"
        global inuse
        inuse = True

#-------MAIN PROGRAM STARTS HERE---------
print "Meeting Room Availability Detection System Starting (CTRL+C to exit)"
time.sleep(5)
print "Ready. Monitoring: ", time.strftime("%H:%M"), "\n"

try:
        GPIO.add_event_detect(SOUND_PIN, GPIO.FALLING, callback=SOUND)
        GPIO.add_event_detect(PIR_PIN, GPIO.RISING, callback=MOTION)
        
        GPIO.output(LED_PIN,GPIO.LOW)

        start = time.time();
        inuse  = False
        wasinuse = False;
        
        while 1:
                
                if inuse:
                        GPIO.output(LED_PIN,GPIO.HIGH)
                else:
                        GPIO.output(LED_PIN,GPIO.LOW)              
                
                #Send status change every 2 minutes, not often. Or if it was free, but... busy now!
                if time.time() - start > 120 or (inuse and not wasinuse):
                        print time.strftime("%H:%M"), ": wasinuse -", wasinuse, "| inuse -", inuse

                        #Send the request only when status changes
                        if inuse:
                                if not wasinuse:
                                        QUERY_NodeMCU('in_use')
                                        wasinuse = True
                                        print "Request sent: ",time.strftime("%H:%M"),". Room is in use\n"
                                        
                                inuse = False
                        elif wasinuse:
                                QUERY_NodeMCU('free')
                                wasinuse = False
                                print "Request sent: ",time.strftime("%H:%M"),". Room is free\n"
                                        
                        start = time.time()

                time.sleep(0.1)
                                
except KeyboardInterrupt:
        print "Quit"
        GPIO.cleanup()
