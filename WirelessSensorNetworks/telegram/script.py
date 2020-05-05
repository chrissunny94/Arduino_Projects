import sys
import time
import random
import datetime
import telepot
import serial

import httplib ,urllib
import psutil

ser = serial.Serial('/dev/ttyUSB5',9600)
counter=32

"""
A simple bot that accepts two commands:
- /roll : reply with a random integer between 1 and 6, like rolling a dice.
- /time : reply with the current time, like a clock.
INSERT TOKEN below in the code, and run:
$ python diceyclock.py
Ctrl-C to kill.
"""

def handle(msg):
    	chat_id = msg['chat']['id']
    	command = msg['text']
	#ser.write(str(chr(counter)))
	TMP=ser.readline(16384)
	TMP.rstrip('\n');
	TMP = TMP.split(":")
	print TMP

    	print 'Got command: %s' % command

    	if command == '/DHT':
		STR = str("DHT Humidity & Temp ") + TMP[0] +"% ," +TMP[1]
        	bot.sendMessage(chat_id, STR)
    	elif command == '/BMP':
	        STR = str("BMP Temp & Pressure ") + TMP[2] +"C ," +TMP[3] 
        	bot.sendMessage(chat_id,STR )
	elif command == '/ALTITUDE':
		bot.sendMessage(chat_id,TMP[4])
	elif command == '/GAS':
		STR = str("GAS:") + TMP[5] +"C"
		bot.sendMessage(chat_id,STR)

	elif command == '/HELP':
                bot.sendMessage(chat_id,"/DHT /BMP /ALTITUDE")

bot = telepot.Bot('216949951:AAH-ESdlr0pHlDuViVhDwnJNnlkoFuQ_Wpc')
bot.notifyOnMessage(handle)
print 'I am listening ...'

while 1:
    time.sleep(10)
