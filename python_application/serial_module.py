# Copyright (C) 2020 Coding Night Romania
# 
# This file is part of automatic-farm.
# 
# automatic-farm is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# automatic-farm is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with automatic-farm.  If not, see <http://www.gnu.org/licenses/>.


import serial
import sys,os
import time

ser = serial.Serial ()
ser_status = 0
ser.port = "/dev/ttyUSB0"
ser.baudrate = 576000
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE_POINT_FIVE #number of stop bits
ser.timeout = 1          #block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write

ser.open()
time.sleep(0.5)
if(ser.isOpen() == False):
    raise Exception("Can't open the port!")
ser.flush()

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
		#print("here" + str(received_data))
		received_data = received_data.decode('cp437', errors='ignore')
		for f in serialCallbacks:
			if received_data != "":
				f(received_data)
	except:
		e = sys.exc_info()
		print(e)
		return -1

def writeCommand(cmd, timeout):
    
    ser.write((cmd).encode('cp437').strip())
    ser.write("\r".encode('cp437'))

    response = ser.read()
    time.sleep(timeout)  #give the serial port sometime to receive the data
    response += ser.read(ser.in_waiting)
    #print(response)
    response = response.decode('cp437', errors='ignore')
    print("RES:" + response)
    return response
def closeConnection():
	ser.close()

def main():
	print("Testing serial module")
	def testing(data):
		print ("Received:" + str(data))
	registerSerialCallback(testing)
	while True:
		readSerialData()
if __name__=="__main__":
	main()
