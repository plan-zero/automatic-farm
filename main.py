import sys,time,os
sys.path.insert(0, '/home/pi/farmview/python_application')

import serial_module as Serial
import battery_module as Battery
import commands_module as Commands
import gsm_module as Gsm
import log_module as Logger
import reboot_module as Reboot
import util_module as Util
import postdata_module as PostData




appInit = False
modemInit = False
phonebook = ["0743566434"]


def HandlerCommand_getBattery(arg):
	bat = Battery.getBatteryData()
	Logger.logMessage("Send battery status to " + arg[0])
	number = Gsm.formatNumber(arg[0])
	Gsm.sendSMS(number, "Battery level is the following " + str(bat[0]) + " V, " + str(bat[1]) + "%" )

def BatteryStatusHandler(warning, critical):
	Logger.logMessage("BatteryStatusHandler called warning:" + str(warning) + " critical:" + str(critical))


Logger.logMessage("Application: initialization phase")

while modemInit == False:
	output = Util.executeShellScript('get_modem_state.sh',['modem_main_state'])
	value = Util.extractResponse(output, 'modem_main_state')
	Logger.logMessage("Modem state is: " + str(value) )
	if value == 'modem_init_complete':
		modemInit = True
	time.sleep(1)
Logger.logMessage("The modem is fully initialized")
Logger.logMessage("Application: Sending greeting message")

for number in phonebook:
#	output = Gsm.sendSMS(number, "FarmView system is starting...")
	value = Util.extractResponse(output,'result')
	if value != 'success':
		Logger.logMessage("Application: can't send the message due an internal error")

appInit = True
Logger.logMessage("Register battery handler to the serial module")
if Serial.registerSerialCallback(Battery.readBatteryDataHandler) != 1:
	Logger.logMessage("Register of battery handler failed!")
	appInit = False
Logger.logMessage("Register battery status handler")
if Battery.registerStatusBatteryCallback(BatteryStatusHandler) != 1:
	Logger.logMessage("Register of status battery handler failed!")
	appInit = False

Commands.registerCommandHandler("getBattery",HandlerCommand_getBattery)

Gsm.addAdministrator("0743566434")
Gsm.addAdministrator("+40743566434")

while modemInit == True and appInit == True:


########Read serial data:
	Serial.readSerialData()
########Calculate battery voltage:
	raw = Battery.getBatteryRawData()
	bat_v = Battery.calculateBatteryVoltage(raw)
	bat_p = Battery.calculateBatteryLevel(bat_v)
	Logger.logMessage("Battery is " + str(bat_v) + "V , " + str(bat_p))
########Post data
	PostData.m_post_bat(bat_v, bat_p)
########Check messages and execute commands
	cmd_messages = Gsm.checkMessages()
	Commands.executeCommand(cmd_messages)
####################################################################


	Logger.logMessage("5 seconds passed, going to sleep")
	time.sleep(5)
