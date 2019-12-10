import serial.tools.list_ports
import serial, time
import sys

comPorts = []
hexFileData = []


HEX_LINE_LENGHT = 16  # Length in bytes

CMD_PREFIX = "<CMD>"
CMD_CRLF = "\r\n"
CMD_TX_ADDR_DEFAULT = "A05ABCDE"
CMD_INIT_NRF = "C00"
CMD_START_WRITE = "D01w"
CMD_16BIT_OF_PAGE = "D17e"
CMD_R = "D01r"
CMD_WRITE_NEXT_PAGE = "D01t"
CMD_STOP_WRITE_PAGE = "D01y"


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


def read_flash_data(hexFilePath):

	with open("c:\\Users\\Teban\\Desktop\\programmer.hex",'rb') as f:
		for line in f:
			line = line[1:(len(line)-2)]
			hexFileData.append(line.decode("utf-8"))
			
	for line in hexFileData:
		print(line)

	counterLine = 0
	for line in hexFileData:
		line = line[10:]
		hexFileData[counterLine] = line
		counterLine += 1
		
	for line in hexFileData:
		print(line)
	
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
	""
	# need to implement this
	
		
		
def send_TX_address():
	command = CMD_PREFIX + CMD_TX_ADDR_DEFAULT + CMD_CRLF
	send_command(command)	
	# async wait
	
	return 0
		
def flash_data(state):
	read_flash_data("")
	
	while (state != 0):
		if (state == 1):		# set TX address
			retValue = send_TX_address()
			if (retValue != 0):
				state = 0
				break
		

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
			comPorts = []
			display_com_ports()
			
		if (option == "2"):
			comSelect = ""
			#comSelect = input("Write COM (eg. COM3): ")
			connect_to_com_port(str(comSelect))
			
		if (option == "3"):
			hexFilePath = ""
			flash_data(1)
			
		if (option == "4"):
			break

			

if __name__== "__main__":
  main()