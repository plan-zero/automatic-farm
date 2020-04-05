import serial
import sys,os
import time
import re

ser = serial.Serial ()

ser_status = 0
ser.port = "/dev/ttyUSB0"
ser.baudrate = 115200
ser.bytesize = serial.EIGHTBITS #number of bits per bytes
ser.parity = serial.PARITY_NONE #set parity check: no parity
ser.stopbits = serial.STOPBITS_ONE_POINT_FIVE #number of stop bits
ser.timeout = None          #block read
ser.xonxoff = False     #disable software flow control
ser.rtscts = False     #disable hardware (RTS/CTS) flow control
ser.dsrdtr = False       #disable hardware (DSR/DTR) flow control
ser.writeTimeout = 2     #timeout for write


ser.open()
time.sleep(0.5)
if(ser.isOpen() == False):
    raise Exception("Can't open the port!")
ser.flush()


def serial_write_cmd(cmd, timeout):
    
    ser.write((cmd).encode('cp437').strip())
    ser.write("\r".encode('cp437'))

    response = ser.read()
    time.sleep(timeout)  #give the serial port sometime to receive the data
    response += ser.read(ser.in_waiting)
    #print(response)
    response = response.decode('cp437', errors='ignore')
    print("RES:" + response)
    return response

nrf24_cmds = {
    "set_tx_addr" : lambda x,y: serial_write_cmd("<CMD>A05" + str(x),y),
    "set_rx_addr" : lambda x,y: serial_write_cmd("<CMD>B05" + str(x),y),
    "set_broadcast_addr" : lambda x,y: serial_write_cmd("<CMD>B05" + str(x),y),
    "set_rx_mode" : lambda x: serial_write_cmd("<CMD>C01L",x),
    "set_tx_mode" : lambda x: serial_write_cmd("<CMD>C00",x),
    "send_data" : lambda x,y: serial_write_cmd("<CMD>D{:02d}".format(len(x)) + str(x), y)
}

try:
    broadcast_address = "AAAAA"
    rx_address = "11223"
    tx_address = ""
    salve_new_tx = "AAA01"

    res = nrf24_cmds["set_broadcast_addr"](broadcast_address, 0.1)
    #wait at least 2 seconds for incoming msg
    res = nrf24_cmds["set_rx_mode"](2)
    #extract the address
    if len(res) >= 6 and "<RX_DATA:B" in res:
        addr = re.findall(r"<RX_DATA:[A-Za-z0-9]+>", res)
        if len(addr) and addr[0] != "":
            addr[0] = re.sub(r"<RX_DATA:B",'',addr[0])
            tx_address = re.sub(r">",'',addr[0])
            print("Extact tx:" + tx_address)
        else:
            raise Exception("Get TX:Invalid format or no response")
    else:
        raise Exception("Get TX:Invalid format or no response")
    res = nrf24_cmds["set_tx_addr"](tx_address, 0)
    res = nrf24_cmds["set_tx_mode"](0)
    res = nrf24_cmds["send_data"]("P" + rx_address + tx_address + "0001", 0.1)
    #switch back to rx
    res = nrf24_cmds["set_rx_addr"](rx_address,0.001)
    res = nrf24_cmds["set_rx_mode"](3)
    if not tx_address + rx_address + "000OK" in res:
        raise Exception("The slave refused the connection")
    #go back in tx mode
    res = nrf24_cmds["set_tx_addr"](tx_address, 0.1)
    res = nrf24_cmds["set_tx_mode"](0.1)
    #start the pairing
    res = nrf24_cmds["send_data"]("P" + rx_address + tx_address + "0002R", 1)
    res = nrf24_cmds["send_data"]("P" + rx_address + salve_new_tx + "0002P", 0.5)
    if not "OK" + rx_address in res:
        raise Exception("Slave refused the pairing!")
    res = nrf24_cmds["set_tx_addr"](salve_new_tx,1)
    res = nrf24_cmds["send_data"]("P" + rx_address + salve_new_tx + "0003C", 0.1)
    res = nrf24_cmds["send_data"]("P" + rx_address + salve_new_tx + "0003D", 1)
    if not "OK" + rx_address in res:
        raise Exception("Connection test failed!")
    res = nrf24_cmds["set_rx_addr"](rx_address,0.01)
    res = nrf24_cmds["set_rx_mode"](0.1)
    print("Successfully connected with " + salve_new_tx)
    
except Exception as e:
    print(str(e))
    exc_type, exc_obj, exc_tb = sys.exc_info()
    fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
    print(exc_type, fname, exc_tb.tb_lineno)
    res = nrf24_cmds["set_tx_mode"](0.01)
    ser.close()

ser.close()