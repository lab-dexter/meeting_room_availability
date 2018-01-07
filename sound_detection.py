import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)
PIR_PIN = 17
GPIO.setup(PIR_PIN, GPIO.IN)

def SOUND(PIR_PIN): 
	print "Sound Detected!\n------------------"
	time.sleep(2)
	
print "Sound Module Test(CTRL+C to exit)"
time.sleep(5)
print "Ready"

try:
	GPIO.add_event_detect(PIR_PIN, GPIO.FALLING, callback=SOUND)
	while 1:
		time.sleep(100)        
		
except KeyboardInterrupt:
	print "Quit"
	GPIO.cleanup()
