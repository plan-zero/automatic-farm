import serial.tools.list_ports
import serial, time
import sys
import os

VERSION = "1.0.0"

comPorts = []
hexFileData = []
ser = serial.Serial()

HEX_LINE_LENGHT = 16  # Length in bytes

CMD_PREFIX = "<CMD>"
CMD_CRLF = "\r"
CMD_TX_ADDR_DEFAULT = "A05ABCDE"
CMD_INIT_NRF = "C00"
CMD_START_WRITE = "D01w"
CMD_16BIT_OF_PAGE = "D17e"
CMD_R = "D01r"
CMD_WRITE_NEXT_PAGE = "D01t"
CMD_STOP_WRITE_PAGE = "D01y"
CMD_STOP_FLASH_CONFIRM = "D01u"



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

	ser.port = "COM12"
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
	except Exception as e:
		print ("error communicating..." + e)

	if ser.isOpen():

		try:
			ser.flushInput() #flush input buffer, discarding all its contents
			ser.flushOutput()#flush output buffer, aborting current output 
			print("Serial buffers flushed")
		except Exception as e:
			print ("error communicating..." + e)



def read_flash_data(hexFilePath):

	with open("blink_led.hex",'rb') as f:
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

	dummyLinesToAdd = len(hexFileData) % 8

	# 8 lines into a page
	# 16 bytes into a line
	for index in range(8 - dummyLinesToAdd):
		hexFileData.append("FF" * 16)		

	for line in hexFileData:
		print(line)



def send_command(command):
	if ser.isOpen():

		try:
			#write data
			ser.write(str.encode(command))
			ser.write('\r'.encode())
			print(f"write data: {command}")

			noOfRetries = 5
			countRetries = 0

			while countRetries < noOfRetries:
				
				response = ser.read()
				time.sleep(1)  #give the serial port sometime to receive the data
				response += ser.read(ser.in_waiting)
				response = response.decode("utf-8")
				if response != "":
					print(str(response))
					return response
				countRetries += 1
			else:
				print("no response")

		except Exception as e:
			print ("error communicating..." + str(e))

	else:
		print (f"cannot open {ser.port} serial port")
	
	return "NRC"
	
		
		
def send_TX_Address():
	command = CMD_PREFIX + CMD_TX_ADDR_DEFAULT
	resp = send_command(command)	
	
	if "<SET_TX:" in resp and "<EXECUTE_CMD:0x41>" in resp:
		return 0

	return 1



def send_Init_NRF():
	command = CMD_PREFIX + CMD_INIT_NRF 
	resp = send_command(command)	
	
	if "<EXECUTE_CMD:0x43>" in resp and "<NRF_CONFIG:STARTING>" in resp and "<NRF_CONFIG:DONE>" in resp:
		return 0

	return 1



def send_Start_Write():
	command = CMD_PREFIX + CMD_START_WRITE 
	resp = send_command(command)	
	
	if "<EXECUTE_CMD:0x44>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_Write_Next_Page():
	command = CMD_PREFIX + CMD_WRITE_NEXT_PAGE 
	resp = send_command(command)	
	
	if "<EXECUTE_CMD:0x44>" in resp and "<RX_DATA:G0>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_Write_In_Flash():
	command = CMD_PREFIX + CMD_R 
	resp = send_command(command)	
	
	if "<EXECUTE_CMD:0x44>" in resp and "<RX_DATA:F8>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_Stop_Write_Page():
	command = CMD_PREFIX + CMD_STOP_WRITE_PAGE 
	resp = send_command(command)	
	
	if "<EXECUTE_CMD:0x44>" in resp and "<RX_DATA:G1>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_Stop_Flash_Confirm():
	command = CMD_PREFIX + CMD_STOP_FLASH_CONFIRM 
	resp = send_command(command)	
	
	if "<EXECUTE_CMD:0x44>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_HEX_Data():
	counterLinesOfPage = 0
	for data in hexFileData:
		#data = ''.join([chr(int(''.join(c), 16)) for c in zip(data[0::2],data[1::2])])
		data = '1234567891234567'
		command = CMD_PREFIX + CMD_16BIT_OF_PAGE + data
		resp = send_command(command)

		if "<EXECUTE_CMD:0x44>" not in resp and "<SEND_TX:ACK>" not in resp:
			print("no more ack: " + resp)
			return 1
		
		counterLinesOfPage += 1

		if counterLinesOfPage % 8 == 0:
			resp = send_Write_In_Flash()
			if resp != 0:
				return 1
			if (counterLinesOfPage != len(hexFileData)):	
				resp = send_Write_Next_Page()
				if resp != 0:
					return 1
	
	resp = send_Stop_Write_Page()
	if resp == 0:
		return 0
	
	return 1
		
def flash_data(state):
	read_flash_data("")

	while (state != 99):
		if (state == 1):		# set TX address
			retValue = send_TX_Address()
			if (retValue == 0):
				state = 2
			else:
				state = 99

		if (state == 2):		# init NRF
			retValue = send_Init_NRF()
			if (retValue == 0):
				state = 3
			else:
				state = 99

		if (state == 3):		# start write command
			retValue = send_Start_Write()
			if (retValue == 0):
				state = 4
			else:
				state = 99

		if (state == 4):		# write hex file
			retValue = send_HEX_Data()
			if (retValue == 0):
				state = 5
			else:
				state = 99

		if (state == 5):		# stop flash
			retValue = send_Stop_Flash_Confirm()
			if (retValue == 0):
				state = 99
			else:
				state = 99


def main(argv):	

	# TODO:4
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

			
help_string = """
-P : serial port, e.g -P COM1
-B : serial baudrate, the value must be in 0-250K interval
-H : HEX path, use the absolute path of file
-T : target address, the value must be less or equal to 5 e.g ABCDE
-h : help, prints this message
--v : tool version
-V : verbose mode, prints debugging messages during script execution """
_commands = {
	"-P" : ["",		lambda x: "COM" == x[:3] and x[3:].isdigit(),		"-P: The supported port is COMx where x must be an unsigned int"],
	"-B" : ["",     lambda x: int(x,10) > 0 and int(x,10) <= 250000, 	"-B: The baud rate should be in 0-250000 range" ],
	"-H" : ["", 	lambda x: x != "" and os.path.isfile(x), 			"-H: File not found"],
	"-T" : ["",		lambda x: x != "" and len(x) <= 5,					"-T: The length of TX/RX address must be less or equal to 5"],
	"-h" : ["-",	lambda x: True,										help_string],
	"--v": ["-",	lambda x: True,                                    	"--v: Tool version is: " + VERSION],
	"-V" : ["-",	lambda x: False,									"-V: Run in verbose mode"]
}			

if __name__== "__main__":
    command_err = 0
    print(sys.argv)
    for arg in sys.argv:
        if arg in _commands:
            try:
                if _commands[arg][0] == "-":
                    if _commands[arg][1](0) == True:
                        print(_commands[arg][2])
                    _commands[arg][0] = "*"
                else:
                    idx = sys.argv.index(arg)
                    value = sys.argv[idx + 1]
                    if _commands[arg][1](value) == False:
                        print(_commands[arg][2])
                        command_err = 1
                    else:
                        _commands[arg][0] = value
            except Exception as e:
                print(_commands[arg][0] + ":command error, use -h to see the correct usage")
                command_err = 1
                print(e)
	
    #print(_commands)
    for cmd in _commands:
        if _commands[cmd][0] == "":
            print("The " + cmd + " parameter is mandatory!")
            command_err = 1

    if command_err == 0:
        main(_commands)
    else:
        print("Invalid command, please use -h to see the correct usage")

