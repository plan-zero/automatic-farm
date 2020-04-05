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



import serial, time
import sys
import os
import subprocess
import re

PLATFORM_SYS = "UNDEFINED"
from sys import platform
if platform == "linux" or platform == "linux2" or platform == "Linux":
    PLATFORM_SYS = "LINUX"
elif platform == "darwin":
    PLATFORM_SYS = "OSX"
elif platform == "win32":
	PLATFORM_SYS = "WIN"

VERSION = "1.0.0"

comPorts = []
ser = serial.Serial()

HEX_LINE_LENGHT = 16  # Length in bytes

CMD_PREFIX = "<CMD>"
CMD_ABORT = "D01o"
CMD_CRLF = "\r"
CMD_TX_ADDR_DEFAULT = "A05"
CMD_RX_ADDR_DEFAULT = "B05"
CMD_INIT_NRF_TX = "C00"
CMD_INIT_NRF_RX = "C01L"
CMD_START_WRITE = "D01w"
CMD_16BIT_OF_PAGE = "D17e"
CMD_R = "D01r"
CMD_WRITE_NEXT_PAGE = "D01t"
CMD_VERIFY_CHECKSUM = "D01y"
CMD_STOP_FLASH_CONFIRM = "D01u"
CMD_SEND_CHECKSUM = "E05y"
CMD_SEND_16B_ASCII = "E33e"

RX_ADDRESS = "11223"

SLEEP_TIME_SERIAL_DEFAULT = 0.04
SLEEP_TIME_SERIAL_NRF_INIT = 0.1
SLEEP_TIME_SERIAL_CRC = 0.5
SLEEP_TIME_SERIAL_BOOTLOADER = 1

APP_MAX_SIZE = 10240

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
	ser.timeout = 2          #block read
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
	hexFileData = []
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
	line_data = ""
	line_byte_count = 0
	
	output = []
	all_bytes = 0
	for line in hexFileData:
		print_message(str(line_byte_count) + "  " + line, DEBUG)
		for byte in line:
			all_bytes += 1
			if line_byte_count < 32:
				line_data += str(byte)
				line_byte_count += 1
			else:
				output.append(line_data)
				line_data = "" +  str(byte)
				line_byte_count = 1
				counterLine += 1
	
	while line_byte_count < 32:
		line_data += "F"
		line_byte_count += 1
		all_bytes += 1
	output.append(line_data)
	counterLine += 1
	
	#if (all_bytes/2) < APP_MAX_SIZE:
	print_message("Preparing " + str(all_bytes) + " B to write", INFO)
	#else:
	#	print_message("The hex file is to big: " + str(all_bytes) + ", the maximum size is 10kB", ERROR)
	#	return []
	#print_message("output len is: " + str(len(output)), DEBUG)
	dummyLinesToAdd = len(output) % 8

	# 8 lines into a page
	# 16 bytes into a line
	if dummyLinesToAdd != 0:
		for index in range(8 - dummyLinesToAdd):
			output.append("FF" * 16)		

	for line in output:
		print_message(line, DEBUG)
	return output

def wait_command(sleeptime):
	response = ser.read()
	time.sleep(sleeptime)  #give the serial port sometime to receive the data
	response += ser.read(ser.in_waiting)
	response = response.decode('cp437', errors='ignore')
	return response

def send_command(command, sleeptime):
	if ser.isOpen():

		try:
			#write data
			ser.write(str.encode(command))
			ser.write('\r'.encode('cp437'))
			print_message(f"write data: {command}", DEBUG)

			noOfRetries = 5
			countRetries = 0

			while countRetries < noOfRetries:
				
				response = ser.read()
				time.sleep(sleeptime)  #give the serial port sometime to receive the data
				response += ser.read(ser.in_waiting)
				response = response.decode('cp437', errors='ignore')
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

def send_bootloader_key(key, tx):
	print_message("Put target in programming mode:", INFO)
	print_message("==> Request default address", INFO)
	command = CMD_PREFIX + "D15K" + RX_ADDRESS + tx + "000" + "R"
	resp = send_command(command, 0.04)
	#set RX address
	if send_RX_Address(RX_ADDRESS) == 1:
		print("can't set rx mode")
		return 1
	#go in rx mode
	print_message("wait rx...", DEBUG)
	if send_Init_NRF("RX") == 1:
		return 1
	#wait 2 seconds to get the answer
	res = wait_command(2)
	print_message("received:" + res, DEBUG)
	if "OTAOK" not in res:
		return 1
	idx = res.find("OTAOK")
	if idx == -1:
		return 1
	_rx_def = res[idx+5:idx+10].replace(">","")
	print_message("default rx: " + _rx_def, DEBUG)
	if send_Init_NRF("TX") == 1:
		return 1
	command = CMD_PREFIX + "D24K" + RX_ADDRESS + tx + "000" + key
	resp = send_command(command, SLEEP_TIME_SERIAL_BOOTLOADER)
	#wait to enter in bootloader
	send_TX_Address(_rx_def)
	
	return 0
		
		
def send_TX_Address(Tx_addr):
	command = CMD_PREFIX + CMD_TX_ADDR_DEFAULT + Tx_addr
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<SET_TX:" in resp and "<EXECUTE_CMD:41>" in resp:
		return 0

	return 1
def send_RX_Address(Rx_addr):
	command = CMD_PREFIX + CMD_RX_ADDR_DEFAULT + Rx_addr
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)	
	
	if "<SET_RX:" in resp and "<EXECUTE_CMD:42>" in resp:
		return 0

	return 1



def send_Init_NRF(mode):
	if mode == "TX":
		command = CMD_PREFIX + CMD_INIT_NRF_TX
		resp = send_command(command, SLEEP_TIME_SERIAL_NRF_INIT)	
		
		if "<EXECUTE_CMD:43>" in resp and "<NRF_CONFIG:STARTING>" in resp and "<NRF_CONFIG:DONE>" in resp:
			return 0
	elif mode == "RX":
		command = CMD_PREFIX + CMD_INIT_NRF_RX
		resp = send_command(command, SLEEP_TIME_SERIAL_NRF_INIT)	
		
		if "<EXECUTE_CMD:43>" in resp and "<NRF_CONFIG:STARTING>" in resp and "<NRF_CONFIG:DONE_RX>" in resp:
			return 0

	return 1



def send_Start_Write():
	command = CMD_PREFIX + CMD_START_WRITE 
	resp = send_command(command, SLEEP_TIME_SERIAL_BOOTLOADER)	
	
	if "<EXECUTE_CMD:44>" in resp and "<SEND_TX:ACK>" in resp and "<RX_DATA:S" in resp:
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
	print_message("CRC verification and erase", INFO)
	command = CMD_PREFIX + CMD_SEND_CHECKSUM + checksum
	resp = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)
	
	if "<EXECUTE_CMD:45>" in resp and "<SEND_TX:ACK>" in resp and "<RX_DATA:G" in resp:
		return 0
	return 1

def send_verify_checksum():
	#wait until the erase is performed
	time.sleep(1)
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
	
def send_abort_command():
	command = CMD_PREFIX + CMD_ABORT
	resp = command = send_command(command, SLEEP_TIME_SERIAL_DEFAULT)
	
	if "<EXECUTE_CMD:44>" in resp and "<SEND_TX:ACK>" in resp:
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

def send_HEX_Data(crc, hexFileData):
	counterLinesOfPage = 0
	t = len(hexFileData)
	c = 0
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
		
def flash_data(tx, hex_file, crc, key):
	
	out = read_flash_data(hex_file)
	state = 1
	tries = 5

	while (state != 99 and state != 98 and tries != 0):
		if (state == 1):		# set TX address
			retValue = send_TX_Address(tx)
			if (retValue == 0):
				state = 2
			else:
				print_message("Failed to set TX address", ERROR)
				state = 99

		if (state == 2):		# init NRF
			retValue = send_Init_NRF("TX")
			if (retValue == 0):
				state = 0
			else:
				print_message("Failed to init NRF", ERROR)
				state = 99
		if (state == 0): #sent bootloader key
			if key != "+" and tries == 5:
				retValue = send_bootloader_key(key, tx)
			else:
				retValue = 0
			if (retValue == 0):
				state = 3
			else:
				print_message("Target not responding", ERROR)
				state = 99
		if (state == 3):		# start write command
			retValue = send_Start_Write()
			if (retValue == 0):
				state = 4
			else:
				print_message("Target can't enter in programming mode", ERROR)
				state = 99

		if (state == 4):		# write hex file
			retValue = send_HEX_Data(crc, out)
			if (retValue == 0):
				state = 5
			else:
				print_message("Failed to write data", ERROR)
				state = 99

		if (state == 5):		# stop flash
			retValue = send_Stop_Flash_Confirm()
			if (retValue == 0):
				state = 98
			else:
				print_message("Fail to program target, aborting", ERROR)
				state = 99
				abort = 1
		if state == 99:
			tries -= 1
			retValue = send_abort_command()
			if retValue == 0:
				print_message("Abort command sent", INFO)
				time.sleep(1) #wait until the micro is restarting
				print_message("Restart flashing, try no: " + str( (5 - tries) ), INFO)
				state = 1
			else:
				print_message("Can't restart procedure, stop flash", ERROR)
				tries = 0
			
	return state

def main(argv):	
	global VERBOSE_MODE, PLATFORM_SYS

	if argv["-V"][0] == "*":
		VERBOSE_MODE = 1
	port = argv["-P"][0]
	baud = argv["-B"][0]
	tx_addr = argv["-T"][0]
	hex_file = argv["-H"][0]
	key = argv["-K"][0]
	
	ret = connect_to_com_port(port,baud)
	#calculate CRC here, use relative path to this script
	
	_crc_valid = 0
	CRC = 0
	try:
		if PLATFORM_SYS == "LINUX":
			if os.path.isabs(os.path.dirname(sys.argv[0])):
				crc_cmd = os.path.dirname(sys.argv[0]) + "/../crc/crc16_linux"
			else:
				crc_cmd = os.path.dirname(sys.argv[0]) + "./../crc/crc16_linux"
		elif PLATFORM_SYS == "WIN":
			crc_cmd = os.path.dirname(sys.argv[0]) + "\\..\\crc\\crc16"
		crc_cmd = crc_cmd + " -x " + hex_file
		p = subprocess.Popen(crc_cmd, shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, close_fds=True)
		output = p.stdout.read()
		res = re.findall(r"<[0-9]+>", output.decode())
		crc_str = res[0].replace('>','')
		crc_str = crc_str.replace('<','')
		CRC = "{0:X}".format(int(crc_str,10))
		if len(CRC) == 3:
			CRC = "0" + CRC
		print_message("CRC calculated:" + CRC, INFO)
		_crc_valid = 1
	except Exception as e:
		_crc_valid = 0
		print_message("CRC16 failed: " + str(e), ERROR)
	
	if ret == 1 and _crc_valid == 1:
		procedure = flash_data(tx_addr, hex_file, CRC, key)
		if procedure == 99:
			print_message("Flashing procedure failed: internal error code returned", ERROR)
		elif procedure == 98:
			print_message("Succesfully flashing", INFO)
	else:
		print_message("Flash failed: can't open COM port or CRC16 failed", ERROR)
			
help_string = """
-K : bootloader key, the key that puts the application in bootloader e.g Aa51cVvM7z 
-P : serial port, e.g -P COM1
-B : serial baudrate, the value must be in 0-250K interval
-H : HEX path, use the absolute path of file
-T : target address, the value must be less or equal to 5 e.g ABCDE
-h : help, prints this message
--v : tool version
-V : verbose mode, prints debugging messages during script execution """
_commands = {
	"-K" : ["+",		lambda x: len(str(x)) == 10, 					"-K: The key must be 10 char long(alpha-numeric ASCII)"],
	"-P" : ["",		lambda x: ("COM" == x[:3] and x[3:].isdigit()) or (x[:-4] == "/dev/tty"),		
																		"-P: The supported port is COMx where x must be an unsigned int or /dev/ttyXXX"],
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

