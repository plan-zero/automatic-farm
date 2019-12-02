import serial.tools.list_ports
import serial, time
import sys

comPorts = []

def display_com_ports():
	comlist = serial.tools.list_ports.comports()
	connected = []
	for element in comlist:
		comPorts.append(element.device)
	print("Available COM ports: " + str(comPorts))
	
def connect_to_com_port(comPort):
	
	#initialization and open the port

	#possible timeout values:
	#    1. None: wait forever, block call
	#    2. 0: non-blocking mode, return immediately
	#    3. x, x is bigger than 0, float allowed, timeout block call

	ser = serial.Serial()
	ser.port = "COM5"
	#ser.port = comPort
	ser.baudrate = 9600
	ser.bytesize = serial.EIGHTBITS #number of bits per bytes
	ser.parity = serial.PARITY_NONE #set parity check: no parity
	ser.stopbits = serial.STOPBITS_ONE #number of stop bits
	#ser.timeout = None          #block read
	ser.timeout = 1            #non-block read
	#ser.timeout = 2              #timeout block read
	ser.xonxoff = False     #disable software flow control
	ser.rtscts = False     #disable hardware (RTS/CTS) flow control
	ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
	ser.writeTimeout = 2     #timeout for write

	try: 
		ser.open()
	except:
		print ("Error open serial port")
		exit()

	if ser.isOpen():

		try:
			ser.flushInput() #flush input buffer, discarding all its contents
			ser.flushOutput()#flush output buffer, aborting current output 
					 #and discard all that is in buffer

			#write data
			ser.write(str.encode('AT+CSQ'))
			print("write data: AT+CSQ")

			time.sleep(0.5)  #give the serial port sometime to receive the data

			numOfLines = 0

			while True:
				print("trace2")
				response = ser.readline()
				print("read data: " + response.decode("utf-8"))

				numOfLines = numOfLines + 1

				if (numOfLines >= 5):
					break

			ser.close()
		except Exception as e:
			print ("error communicating..." + e)

	else:
		print ("cannot open serial port ")

def flash_device(hexFilePath):
	buffer = []

	with open("c:\\Users\\Teban\\Desktop\\programmer.hex",'rb') as f:
	#	buff = f.read()
		for line in f:
			line = line[1:(len(line)-2)]
			print(line)
			buffer.append(line.decode("utf-8"))

	myStr = []
	for e in buffer:
		line = 0
		column = 0
		sublist = []
		while len(e) > 0:
			sublist.append(int(('0x' + e[0:2]), 16))
			e = e[2:len(e)]
		myStr.append(sublist)
		
	for l in myStr:
		hexList = []
		for i in l:
			hexList.append(hex(i))
		print(hexList)
		

def main():	
	while (1):
		print('''
		1. Show all COM ports
		2. Connect to COM port
		3. Flash device with hex file
		4. Exit
		''')
		
		option = input("Opt: ")
		
		if (option == "1"):
			comPorts = []
			display_com_ports()
			
		if (option == "2"):
			comSelect = ""
			#comSelect = input("Write COM (eg. COM3): ")
			connect_to_com_port(str(comSelect))
			
		if (option == "3"):
			hexFilePath = ""
			flash_device(hexFilePath)
			
		if (option == "4"):
			break

			

if __name__== "__main__":
  main()