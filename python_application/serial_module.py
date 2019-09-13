import serial
import sys,os
import time

ser = serial.Serial ("/dev/ttyS0", 250000, timeout = 5, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)
ack_ok = 0

serialCallbacks = []

def unregisterSerialCallback(handler):
	global serialCallbacks
	try:
		if handler in serialCallbacks:
			serialCallbacks.remove(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)	

def registerSerialCallback(handler):
	global serialCallbacks
	try:
		if handler not in serialCallbacks:
			serialCallbacks.append(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)	

def readSerialData():
	global ack_ok
	global serialCallbacks
	try:
		#read serial port
		received_data = ser.read()
		time.sleep(0.03)
		data_left = ser.inWaiting()
		received_data += ser.read(data_left)
		#print(received_data)

		for f in serialCallbacks:
			f(received_data)
	except:
		e = sys.exc_info()
		print(e)
		return -1

def main():
	print("Testing serial module")
	def testing(data):
		print ("Received:" + str(data))
	registerSerialCallback(testing)
	while True:
		readSerialData()
if __name__=="__main__":
	main()
