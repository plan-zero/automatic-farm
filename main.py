from time import gmtime, strftime, localtime
import sys,time,os
import subprocess
sys.path.insert(0, '/home/pi/farmview/python_application')

import serial_module as Serial
import battery_module as Battery
import commands_module as Commands
import gsm_module as Gsm
import log_module as Logger
import reboot_module as Reboot
import util_module as Util
import postdata_module as PostData

modemerr = 0
interfaceUp = 0
usbDownThr = 0
dailyRebootDone = 0
def manageDailyReboot():
	global usbDownThr
	global dailyRebootDone

	timestamp = strftime("%H:%M",localtime())
	if timestamp == "12:00" and dailyRebootDone == 0:
		try:
			dailyRebootDone = 1
            		#remove log messages
			#os.system('sudo rm /home/pi/farmview/messages.txt')
			Logger.logMessage("Daily reboot")
			Reboot.triggerReboot(1)
		except:
			e = sys.exc_info()
			print(e)
	elif timestamp == "12:05":
		os.system('sudo rm /home/pi/farmview/messages.txt')
		dailyRebootDone = 0

def checkUsbInterface():
	global interfaceUp
	###check if usb0 interface still running
	ifconf = open("tmp.txt", 'w')
	subprocess.call(['ifconfig'], stdout=ifconf)
	ifconf.close()
	with open("tmp.txt", 'r') as ifconf_out:
		if "usb0" not in  ifconf_out.read():
			usbDownThr = usbDownThr + 1
			interfaceUp = 0
			Logger.logMessage("Interface down, wait 5 minutes until reboot")
			if usbDownThr == 60:
				Logger.logMessage("Trigger reboot because usb0 is down")
				Reboot.triggerReboot(1)
		else:
			interfaceUp = 1
			Logger.logMessage("Interface up")
			usbDownThr = 0
		ifconf_out.close()


appInit = False
modemInit = False
phonebook = ["0743566434"]

def checkModemState():
	global modemerr
        output = Util.executeShellScript('get_modem_state.sh',['modem_main_state'])
        value = Util.extractResponse(output, 'modem_main_state')
	Logger.logMessage("Modem state is: " + str(value) )
        if value != 'modem_init_complete':
		modemerr = modemerr + 1
		if modemerr == 12:
			modemerr = 0
			Logger.logMessage("Trigger reboot because the modem is in error state")
			Reboot.triggerReboot(1)
	else:
		Logger.logMessage("Modem is running")
		modemerr = 0


def RebootShutdownHandler():
	Logger.logMessage("Shutdown...")

def RebootPreShutdownHandler():
	Logger.logMessage("The system will shut down in few seconds")
	for n in phonebook:
		number = Gsm.formatNumber(n)
		Gsm.sendSMS(number, "The system will shut down in few seconds")

def HandlerCommand_getBattery(arg):
	bat = Battery.getBatteryData()
	Logger.logMessage("Send battery status to " + arg[0])
	number = Gsm.formatNumber(arg[0])
	Gsm.sendSMS(number, "Battery level is the following " + str(bat[0]) + " V, " + str(bat[1]) + "%" )

def BatteryStatusHandler(warning, critical):
	Logger.logMessage("BatteryStatusHandler called warning:" + str(warning) + " critical:" + str(critical))
	Reboot.postBatteryStatus(warning, critical)

Logger.logMessage("Application: initialization phase")

timeout = 0
while interfaceUp == 0:
	time.sleep(10)
	checkUsbInterface()
	timeout = timeout + 1
	if timeout == 180: #half of hour
		os.system('sudo shutdown -r now') #reboot system since usb is down
		timeout = 0
timeout = 0
while modemInit == False:
	output = Util.executeShellScript('get_modem_state.sh',['modem_main_state'])
	value = Util.extractResponse(output, 'modem_main_state')
	Logger.logMessage("Modem state is: " + str(value) )
	if value == 'modem_init_complete':
		modemInit = True
	else:
		timeout = timeout + 1
		if timeout == 90:
			os.system('sudo shutdown -r now') #reboot system since usb is down
			timeout = 0
	time.sleep(1)
Logger.logMessage("The modem is fully initialized")
Logger.logMessage("Application: Sending greeting message")

for number in phonebook:
	output = Gsm.sendSMS(number, "FarmView system is starting...")
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
Reboot.registerShutdownCallback(RebootShutdownHandler)
Reboot.registerPreShutdownCallback(RebootPreShutdownHandler)

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
########Manage reboots and shutdown
	Reboot.manageRebootEvent()
	Reboot.manageLowBatteryShutdown()
####################################################################
	checkUsbInterface()
	checkModemState()
	manageDailyReboot()
	Logger.logMessage("5 seconds passed, going to sleep")
	time.sleep(5)
