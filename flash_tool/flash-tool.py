import serial.tools.list_ports
import serial, time
import sys

comPorts = []
hexFileData = []
ser = serial.Serial()

HEX_LINE_LENGHT = 16  # Length in bytes

CMD_PREFIX = "<CMD>"
CMD_CRLF = b"\x0d"
CMD_TX_ADDR_DEFAULT = "A05ABCDE"
CMD_INIT_NRF = "C00"
CMD_START_WRITE = "D01w"
CMD_16BIT_OF_PAGE = "D17e"
CMD_R = "D01r"
CMD_WRITE_NEXT_PAGE = "D01t"
CMD_STOP_WRITE_PAGE = "D01y"


def display_com_ports():
	comlist = serial.tools.list_ports.comports()
	for element in comlist:
		comPorts.append(element.device)
	print("Available COM ports: " + str(comPorts))


def connect_to_com_port(comPort):
	
	#initialization and open the port

	#possible timeout values:
	#    1. None: wait forever, block call
	#    2. 0: non-blocking mode, return immediately
	#    3. x, x is bigger than 0, float allowed, timeout block call

	ser.port = "COM5"
	#ser.port = comPort
	ser.baudrate = 250000
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
		print(f"Serial connection with {ser.port} is open")
	except:
		print (f"Error open {ser.port} serial port")

	if ser.isOpen():

		try:
			ser.flushInput() #flush input buffer, discarding all its contents
			ser.flushOutput()#flush output buffer, aborting current output 
			print("Serial buffers flushed")
		except Exception as e:
			print ("error communicating..." + e)


def read_flash_data(hexFilePath):

	with open("programmer.hex",'rb') as f:
		for line in f:
			line = line[1:(len(line)-2)]
			hexFileData.append(line.decode("utf-8"))
			
	for line in hexFileData:
		pass
		#print(line)

	counterLine = 0
	for line in hexFileData:
		line = line[8:]
		line = line[:-2]
		hexFileData[counterLine] = line
		counterLine += 1
		
	for line in hexFileData:
		pass
		#print(line)
	
	counterLine = 0
	for line in hexFileData:
		lineLenght = len(line)
		if (HEX_LINE_LENGHT * 2 > lineLenght):
			line = line + ("F" * (HEX_LINE_LENGHT * 2 - lineLenght))
		if (hexFileData[counterLine] == ""):
			hexFileData.remove("")
		else:
			hexFileData[counterLine] = line
		counterLine += 1
	
	for line in hexFileData:
		print(line)
		
def send_command(command):
	if ser.isOpen():

		try:
			#write data
			ser.write(str.encode(command))
			print(f"write data: {command}")

			noOfRetries = 5
			countRetries = 0

			while countRetries < noOfRetries:
				
				response = ser.read()
				time.sleep(0.1)  #give the serial port sometime to receive the data
				response += ser.read(ser.in_waiting)
				response = response.decode("utf-8")
				if response != "":
					print(str(response))
					return response
				countRetries += 1
			else:
				print("no response")

		except Exception as e:
			print ("error communicating..." + e)

	else:
		print (f"cannot open {ser.port} serial port")
	
	return "NRC"
	
		
		
def send_TX_address():
	command = CMD_PREFIX + CMD_TX_ADDR_DEFAULT + CMD_CRLF
	resp = send_command(command)	
	
	if "<SET_TX:" in resp and "<EXECUTE_CMD:0x41>" in resp:
		return 0

	return 1
		
def flash_data(state):
	read_flash_data("")

	while (state != 99):
		if (state == 1):		# set TX address
			retValue = send_TX_address()
			if (retValue == 0):
				state = 99
			else:
				state = 99

		if (state == 2):		# set TX address
			retValue = send_TX_address()
			if (retValue == 0):
				state = 99
			else:
				state = 99


def main():	

	# TODO:
	# 	Option 2: Open COM port selected (not hardcoded)
	#			  Store into a global variable the comm port
	#	Option 3: Implement file to selecte
	#			  Read data & create data structure for flash protocol
	#			  Implement flashing SW


	while (1):
		print('''
		1. Show all COM ports
		2. Connect to COM port
		3. Flash device with hex file
		4. Exit
		''')
		
		option = input("Opt: ")
		
		if (option == "1"):
			display_com_ports()
			
		if (option == "2"):
			comSelect = ""
			#comSelect = input("Write COM (eg. COM3): ")
			connect_to_com_port(str(comSelect))
			
		if (option == "3"):
			flash_data(1)
			
		if (option == "4"):
			exit()

			

if __name__== "__main__":
  main()