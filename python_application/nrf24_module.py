import sys,os
import time
import re

sys.path.insert(0, '/home/ame/git_projects/automatic-farm/python_application')
import serial_module as Serial

CMD_DEF_TIMEOUT = 0.001

cmds = {
    "set_tx_addr" : lambda x,y: Serial.writeCommand("<CMD>A05" + str(x),y),
    "set_rx_addr" : lambda p,x,y: "<EXECUTE_CMD:42>" in  Serial.writeCommand("<CMD>B06" + str(p) + str(x),y),
    "set_broadcast_addr" : lambda x,y: Serial.writeCommand("<CMD>B06" + str(x),y),
    "set_rx_mode" : lambda x: "<EXECUTE_CMD:43>" and "<NRF_CONFIG:DONE_RX>" in Serial.writeCommand("<CMD>C01L",x),
    "set_tx_mode" : lambda x: Serial.writeCommand("<CMD>C00",x),
    "send_data" : lambda x,y: Serial.writeCommand("<CMD>D{:02d}".format(len(x)) + str(x), y),
    "reset" : lambda x: "<EXECUTE_CMD:52>" in Serial.writeCommand("<CMD>R00",x),
    "read_data" :  lambda x: Serial.writeCommand("",x)
}