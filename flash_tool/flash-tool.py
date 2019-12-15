import serial.tools.list_ports
import serial, time
import sys
import os
import subprocess
import re

VERSION = "1.0.0"

comPorts = []
hexFileData = []
ser = serial.Serial()

HEX_LINE_LENGHT = 16  # Length in bytes

CMD_PREFIX = "<CMD>"
CMD_CRLF = "\r"
CMD_TX_ADDR_DEFAULT = "A05"
CMD_INIT_NRF = "C00"
CMD_START_WRITE = "D01w"
CMD_16BIT_OF_PAGE = "D17e"
CMD_R = "D01r"
CMD_WRITE_NEXT_PAGE = "D01t"
CMD_VERIFY_CHECKSUM = "D01y"
CMD_STOP_FLASH_CONFIRM = "D01u"
CMD_SEND_CHECKSUM = "E05y"
CMD_SEND_16B_ASCII = "E33e"

SLEEP_TIME_SERIAL_DEFAULT = 0.07
SLEEP_TIME_SERIAL_NRF_INIT = 0.1
SLEEP_TIME_SERIAL_CRC = 0.1

VERBOSE_MODE = 0
DEBUG = 1
ERROR = 2
INFO  = 3

_previous_progress = -1
_progress_res = 10

def print_message(msg, dbg_level):
	
	if(VERBOSE_MODE):
		if dbg_level == ERROR:
			print("ERR:" + str(msg))
		elif dbg_level == INFO:
			print("INFO:" + str(msg))
		elif dbg_level == DEBUG:
			print("DEBUG:" + str(msg))
	else: #print just the error messages and info
		if dbg_level == ERROR:
			print("ERR:" + str(msg))
		elif dbg_level == INFO:
			print("INFO:" + str(msg))

def connect_to_com_port(comPort, baud):
	
	#initialization and open the port

	#possible timeout values:
	#    1. None: wait forever, block call
	#    2. 0: non-blocking mode, return immediately
	#    3. x, x is bigger than 0, float allowed, timeout block call
	ser_status = 0
	ser.port = comPort
	ser.baudrate = baud
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
		print_message(f"Serial connection with {ser.port} is open", INFO)
	except Exception as e:
		print_message("error open serial port: " + str(e), ERROR)
		return 0
	if ser.isOpen():

		try:
			ser.flushInput() #flush input buffer, discarding all its contents
			ser.flushOutput()#flush output buffer, aborting current output
			
			print_message("Serial buffers flushed", DEBUG)
			return 1
		except Exception as e:
			print_message("error communicating..." + str(e), ERROR)
			return 0
	return 0

def read_flash_data(hexFilePath):

	with open(hexFilePath,'rb') as f:
		for line in f:
			#line = line[1:(len(line)-2)]
			hexFileData.append(line.decode("utf-8"))
			
	for line in hexFileData:
		pass
		#print(line)

	counterLine = 0
	for line in hexFileData:
		line = line.replace('\r', '')
		line = line.replace('\n', '')
		line = line[9:-2]
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
		print_message(line, DEBUG)



def send_command(command, sleeptime):
	if ser.isOpen():

		try:
			#write data
			ser.write(str.encode(command))
			ser.write('\r'.encode())
			print_message(f"write data: {command}", DEBUG)

			noOfRetries = 5
			countRetries = 0

			while countRetries < noOfRetries:
				
				response = ser.read()
				time.sleep(sleeptime)  #give the serial port sometime to receive the data
				response += ser.read(ser.in_waiting)
				response = response.decode("utf-8")
				if response != "":
					print_message(str(response), DEBUG)
					return response
				countRetries += 1
			else:
				print_message("no response", DEBUG)

		except Exception as e:
			print_message("error communicating..." + str(e), DEBUG)

	else:
		print_message(f"cannot open {ser.port} serial port", DEBUG)
	
	return "NRC"
	
		
		
def send_TX_Address(Tx_addr):
	command = CMD_PREFIX + CMD_TX_ADDR_DEFAULT + Tx_addr
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<SET_TX:" in resp and "<EXECUTE_CMD:41>" in resp:
		return 0

	return 1



def send_Init_NRF():
	command = CMD_PREFIX + CMD_INIT_NRF 
	resp = send_command(command, SLEEP_TIME_SERIAL_NRF_INIT)	
	
	if "<EXECUTE_CMD:43>" in resp and "<NRF_CONFIG:STARTING>" in resp and "<NRF_CONFIG:DONE>" in resp:
		return 0

	return 1



def send_Start_Write():
	command = CMD_PREFIX + CMD_START_WRITE 
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<EXECUTE_CMD:44>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_Write_Next_Page():
	command = CMD_PREFIX + CMD_WRITE_NEXT_PAGE 
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<EXECUTE_CMD:44>" in resp and "<RX_DATA:G" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1



def send_Write_In_Flash():
	command = CMD_PREFIX + CMD_R 
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<EXECUTE_CMD:44>" in resp and "<RX_DATA:F8>" in resp and "<SEND_TX:ACK>" in resp:
		return 0

	return 1


def send_checksum(checksum):
	command = CMD_PREFIX + CMD_SEND_CHECKSUM + checksum
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)
	
	if "<EXECUTE_CMD:45>" in resp and "<SEND_TX:ACK>" in resp and "<RX_DATA:G1>" in resp:
		return 0
	return 1

def send_verify_checksum():
	command = CMD_PREFIX + CMD_VERIFY_CHECKSUM 
	resp = send_command(command, SLEEP_TIME_SERIAL_CRC)	
	
	if "<EXECUTE_CMD:44>" in resp and "<SEND_TX:ACK>" in resp and "<RX_DATA:H0>" in resp:
		return 0

	return 1



def send_Stop_Flash_Confirm():
	command = CMD_PREFIX + CMD_STOP_FLASH_CONFIRM 
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<EXECUTE_CMD:44>" in resp and "<SEND_TX:ACK>" in resp and "<RX_DATA:J0>" in resp:
		return 0

	return 1

def show_progress(current, total):
	global _previous_progress
	try:
		#we don't show the progress in verbose mdoe
		if VERBOSE_MODE == 0:
			progress = (current * 100) / total
			if (progress - _previous_progress) > _progress_res or _previous_progress == -1 or progress == 100:
				print_message("DOWNLOAD: " + str(progress) + "%", INFO)
				_previous_progress = progress
	except Exception as e:
		print_message(str(e), ERROR)

def send_HEX_Data(crc):
	counterLinesOfPage = 0
	t = len(hexFileData)
	c = 0;
	for data in hexFileData:
		command = CMD_PREFIX + CMD_SEND_16B_ASCII + data
		resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)

		if "<EXECUTE_CMD:45>" not in resp and "<SEND_TX:ACK>" not in resp:
			print_message("no more ack: " + resp, DEBUG)
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
		c = c + 1
		show_progress(c,t)
	resp = send_checksum(crc)
	resp = send_verify_checksum()
	if resp == 0:
		return 0
	
	return 1
		
def flash_data(state, tx, hex_file, crc):
	
	read_flash_data(hex_file)

	while (state != 99 and state != 98):
		if (state == 1):		# set TX address
			retValue = send_TX_Address(tx)
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
			retValue = send_HEX_Data(crc)
			if (retValue == 0):
				state = 5
			else:
				state = 99

		if (state == 5):		# stop flash
			retValue = send_Stop_Flash_Confirm()
			if (retValue == 0):
				state = 98
			else:
				state = 99
	return state

def main(argv):	
	global VERBOSE_MODE
	# TODO:4
	# 	Option 2: Open COM port selected (not hardcoded)
	#			  Store into a global variable the comm port
	#	Option 3: Implement file to selecte
	#			  Read data & create data structure for flash protocol
	#			  Implement flashing SW
	if argv["-V"][0] == "*":
		VERBOSE_MODE = 1
	port = argv["-P"][0]
	baud = argv["-B"][0]
	tx_addr = argv["-T"][0]
	hex_file = argv["-H"][0]
	ret = connect_to_com_port(port,baud)
	#calculate CRC here, use relative path to this script
	
	_crc_valid = 0
	CRC = 0
	try:
		crc_cmd = os.path.dirname(sys.argv[0]) + "\\..\\tools\\crc\\crc16"
		crc_cmd = crc_cmd + " -x " + hex_file
		p = subprocess.Popen(crc_cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
		output = p.stdout.read()
		res = re.findall(r"<[0-9]+>", output.decode())
		crc_str = res[0].replace('>','')
		crc_str = crc_str.replace('<','')
		CRC = "{0:X}".format(int(crc_str,10))
		print_message("CRC calculated:" + CRC, INFO)
		_crc_valid = 1
	except Exception as e:
		_crc_valid = 0
		print_message("CRC16 failed: " + str(e), ERROR)
	
	if ret == 1 and _crc_valid == 1:
		procedure = flash_data(1,tx_addr, hex_file, CRC)
		if procedure == 99:
			print_message("Flashing procedure failed: internal error code returned", ERROR)
		elif procedure == 98:
			print_message("Succesfully flashing", INFO)
	else:
		print_message("Flash failed: can't open COM port or CRC16 failed", ERROR)
			
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
    print_message(sys.argv, DEBUG)
    for arg in sys.argv:
        if arg in _commands:
            try:
                if _commands[arg][0] == "-":
                    if _commands[arg][1](0) == True:
                        print_message(_commands[arg][2], ERROR)
                    _commands[arg][0] = "*"
                else:
                    idx = sys.argv.index(arg)
                    value = sys.argv[idx + 1]
                    if _commands[arg][1](value) == False:
                        print_message(_commands[arg][2], ERROR)
                        command_err = 1
                    else:
                        _commands[arg][0] = value
            except Exception as e:
                print_message(_commands[arg][0] + ":command error, use -h to see the correct usage", ERROR)
                command_err = 1
                print_message(str(e), ERROR)
	
    #print(_commands)
    for cmd in _commands:
        if _commands[cmd][0] == "":
            print_message("The " + cmd + " parameter is mandatory!", ERROR)
            command_err = 1

    if command_err == 0:
        main(_commands)
    else:
        print_message("Invalid command, please use -h to see the correct usage", ERROR)

