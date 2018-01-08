import RPi.GPIO as GPIO
import time
import requests

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

#define sound sensor PIN connection and mode
SOUND_PIN = 17
GPIO.setup(SOUND_PIN, GPIO.IN)

#define PIR sensor PIN connection and mode
PIR_PIN = 4
GPIO.setup(PIR_PIN, GPIO.IN)

#defined LED PIN connection and mode
LED_PIN = 18
GPIO.setup(LED_PIN,GPIO.OUT,initial=GPIO.LOW)

#define NodeMCU IP address variable
MCU_IP = '192.168.8.113'

#LED turn on function
def LED_ON():
        print "LED on"
        GPIO.output(LED_PIN,GPIO.HIGH)
        QUERY_NodeMCU('in_use')
        time.sleep(5)
        print "LED off\n"
        GPIO.output(LED_PIN,GPIO.LOW)
        QUERY_NodeMCU('free')

#function to send request to NodeMcu esp8266 and change LCD screen text
def QUERY_NodeMCU(text):
        r = requests.get('http://'+MCU_IP+'/'+text)


#Sound sensor event detect callback function. It will call LED_ON function
def SOUND(SOUND_PIN): 
	print "Sound Detected!\n------------------"
	LED_ON()

#PIR sensor event detect callback function. It will call LED_ON function
def MOTION(PIR_PIN): 
	print "Motion Detected!\n------------------"
	LED_ON()

#-------MAIN PROGRAM STARTS HERE---------
print "Meeting Room Availability Detection System Starting(CTRL+C to exit)"
time.sleep(5)
print "Ready. Monitoring..."

try:
	GPIO.add_event_detect(SOUND_PIN, GPIO.FALLING, callback=SOUND)
	GPIO.add_event_detect(PIR_PIN, GPIO.RISING, callback=MOTION)
	while 1:
		time.sleep(100)        
		
except KeyboardInterrupt:
	print "Quit"
	GPIO.cleanup()
