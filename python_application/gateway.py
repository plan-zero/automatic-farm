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
import sys,time,os
sys.path.insert(0, '/home/ame/git_projects/automatic-farm/python_application')
import nrf24_module as NRF24
import re
import serial_module as Serial
from enum import Enum

class GatewayExceptionInit(Exception):
    pass
def throwException(value):
    if value == False:
        raise GatewayExceptionInit()

class ChildState(Enum):
    UNASSIGNED = 1,
    PAIRING = 2,
    PAIRED = 3,
    ERROR = 4


radio_cfg = {
    "pipe0" : "AAAAA",
    "pipe1" : "00AAA",
    "pipe2" : "10AAA",
    "pipe3" : "20AAA",
    "pipe4" : "30AAA",
    "pipe5" : "40AAA"
}

# no : pipe, rx_address, tx_address
radio_child_cfg = {
    "leaf_0" : [ChildState.UNASSIGNED, "pipe1", radio_cfg["pipe1"], ""],
    "leaf_1" : [ChildState.UNASSIGNED, "pipe2", radio_cfg["pipe2"], ""],
    "leaf_2" : [ChildState.UNASSIGNED, "pipe3", radio_cfg["pipe3"], ""],
    "leaf_3" : [ChildState.UNASSIGNED, "pipe4", radio_cfg["pipe4"], ""],
    "leaf_4" : [ChildState.UNASSIGNED, "pipe5", radio_cfg["pipe5"], ""]
}

pairing_request_count = 0
pairing_request_data = {}

base_count = 0
base_addr = ["","01AAA", "02AAA", "02AAA", "02AAA"]
def get_new_address():
    global base_count,base_addr
    base_count += 1
    return base_addr[base_count]

def broadcast_pairing_msg_check(data):
    global pairing_request_data, pairing_request_count
    #extract the address
    if len(data) >= 6 and "<RX_DATA:B" in data:
        addr = re.findall(r"<RX_DATA:[A-Za-z0-9]+>", data)
        if len(addr) and addr[0] != "":
            addr[0] = re.sub(r"<RX_DATA:B",'',addr[0])
            tx_address = re.sub(r">",'',addr[0])
            print("Extact tx:" + tx_address)
            if not tx_address in pairing_request_data.values():
                pairing_request_data[pairing_request_count] = tx_address
                pairing_request_count += 1

def serial_rec_cb(data):
    print("data:" + str(data))
    broadcast_pairing_msg_check(data)

def init():
    global pairing_request_count
    try:
        #reset module
        res = NRF24.cmds["reset"](2)
        throwException(res)
        #set broadcast
        res = NRF24.cmds["set_rx_addr"](0,radio_cfg["pipe0"],NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        #set other pipes address
        res = NRF24.cmds["set_rx_addr"](1,radio_cfg["pipe1"],NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        res = NRF24.cmds["set_rx_addr"](2,radio_cfg["pipe2"],NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        res = NRF24.cmds["set_rx_addr"](3,radio_cfg["pipe3"],NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        res = NRF24.cmds["set_rx_addr"](4,radio_cfg["pipe4"],NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        res = NRF24.cmds["set_rx_addr"](5,radio_cfg["pipe5"],NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        res = NRF24.cmds["set_rx_mode"](NRF24.CMD_DEF_TIMEOUT)
        throwException(res)
        #register serial callback
        Serial.registerSerialCallback(serial_rec_cb)
        pairing_request_count = 0
        return 0
    except Exception as e:
        print(str(e))
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)
        return -1

def pairing_procedure(rx_address, tx_address, slave_tx_address):
    try:
        salve_new_tx = slave_tx_address
        res = NRF24.cmds["set_tx_addr"](tx_address, 0)
        res = NRF24.cmds["set_tx_mode"](0)
        res = NRF24.cmds["send_data"]("P" + rx_address + tx_address + "0001", 0.1)
        #switch back to rx
        res = NRF24.cmds["set_rx_mode"](0)
        res = NRF24.cmds["read_data"](3)
        if not tx_address + rx_address + "000OK1" in res:
            raise Exception("The slave refused the connection")
        #go back in tx mode
        res = NRF24.cmds["set_tx_addr"](tx_address, 0.1)
        res = NRF24.cmds["set_tx_mode"](0.1)
        #start the pairing
        res = NRF24.cmds["send_data"]("P" + rx_address + tx_address + "0002R", 1)
        res = NRF24.cmds["send_data"]("P" + rx_address + tx_address + "0002P" + salve_new_tx, 0.5)
        if not rx_address + "000OK2" in res:
            raise Exception("Slave refused the pairing!")
        res = NRF24.cmds["set_tx_addr"](salve_new_tx,1)
        res = NRF24.cmds["send_data"]("P" + rx_address + salve_new_tx + "0003C", 0.1)
        res = NRF24.cmds["send_data"]("P" + rx_address + salve_new_tx + "0003D", 1)
        if not rx_address + "000OK3" in res:
            raise Exception("Connection test failed!")
        res = NRF24.cmds["set_rx_mode"](0.1)
        print("Successfully connected with " + salve_new_tx)
        return 1
        
    except Exception as e:
        print(str(e))
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)
        return -1

def manage_pairing():
    global radio_child_cfg,pairing_request_count,pairing_request_data
    while pairing_request_count:
        pairing_request_count -= 1
        empty_slot = 0
        for leaf, value in radio_child_cfg.items():
            if value[0] == ChildState.UNASSIGNED:
                print("Add child to the following slot: " + leaf)
                empty_slot = 1
                child_address = get_new_address()
                value[0] = ChildState.PAIRING
                print("data " + pairing_request_data[pairing_request_count])
                if pairing_procedure(value[2], pairing_request_data[pairing_request_count], child_address) == 1:
                    value[0] = ChildState.PAIRED
                    value[3] = child_address
                else:
                    value[0] = ChildState.UNASSIGNED
                    pairing_request_data[pairing_request_count] = ""
                break
        if empty_slot == 0:
            print("No more slots available")
        
        


def main():
    print("Gateway v1.0.0")
    print("Gateway starting...")
    init_done = 0
    if 0 == init():
        init_done = 1
    while(init_done):
        Serial.readSerialData()
        manage_pairing()
        time.sleep(0.5)


if __name__ == "__main__":
    main()