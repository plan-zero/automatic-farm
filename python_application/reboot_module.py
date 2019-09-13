import sys,os
import time

BAT_WARNING = 0
BAT_CRITICAL = 0
battery_low_counter = 0
triggerReboot = 0
rebootCount = 0

shutdownCallback = []
preshutdownCallback = []

COUNTER_LIMIT1 = 10
COUNTER_LIMIT2 = 20
REBOOT_TIMEOUT = 10

def unregisterShutdownCallback(handler):
	global shutdownCallback
	try:
		if handler in shutdownCallback:
			shutdownCallback.remove(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)
def unregisterPreShutdownCallback(handler):
	global preshutdownCallback
	try:
		if handler in preshutdownCallback:
			preshutdownCallback.remove(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)

def registerShutdownCallback(handler):
	global shutdownCallback
	try:
		global shutdownCallback
		if handler not in shutdownCallback:
			shutdownCallback.append(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)
def registerPreShutdownCallback(handler):
	global preshutdownCallback
	try:
		global preshutdownCallback
		if handler not in preshutdownCallback:
			preshutdownCallback.append(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)

def postBatteryStatus(bat_warning, bat_critical):
	try:
		global BAT_WARNING, BAT_CRITICAL
		BAT_WARNING = bat_warning
		BAT_CRITICAL = bat_critical
	except:
		e = sys.exc_info()
		print(e)

def triggerReboot(reboot):
	try:
		global triggerReboot
		triggerReboot = reboot
	except:
		e = sys.exc_info()
		print(e)

def shutdownSystem():
	try:
		while True:
			time.sleep(60) #prevent dead-loop in case of malfunction
			os.system('sudo shutdown now')
			print("Waiting shutdown")
			time.sleep(1)
	except:
		e = sys.exc_info()
		print(e)


def rebootSystem():
	try:
		while True:
			os.system('sudo shutdown -r now')
			print("Rebooting ...")
			time.sleep(1)
	except:
		e = sys.exc_info()
		print(e)

def manageLowBatteryShutdown():
	global BAT_WARNING
	global BAT_CRITICAL
	global battery_low_counter
	global ack_ok
	global COUNTER_LIMIT1, COUNTER_LIMIT2
	global shutdownCallback, preshutdownCallback
	try:
		if BAT_WARNING == 1 and BAT_CRITICAL == 0:
			battery_low_counter += 1

			if battery_low_counter == COUNTER_LIMIT1:
				for f in preshutdownCallback:
					f()
			elif battery_low_counter == COUNTER_LIMIT2:
				for f in shutdownCallback:
					f()
				shutdownSystem()
		elif BAT_CRITICAL == 1:
			for f in shutdownCallback:
				f()
			shutdownSystem()
		else:
			battery_low_counter = 0
	except:
		e = sys.exc_info()
		print(e)

def manageRebootEvent():
	global rebootCount
	global triggerReboot
	if triggerReboot:
		rebootCount += 1
		if rebootCount == 1:
			for f in preshutdownCallback:
				f()
		elif rebootCount == REBOOT_TIMEOUT:
			for f in shutdownCallback:
				f()
			rebootSystem()
	else:
		rebootCount = 0


def main():
	print("testing area")

	def preshutdownHandler():
		print("System pre-shutdown handler 1")
	def preshutdownHandler2():
		print("System pre-shutdown handler 2")
	def shutdownHandler():
		print("System shutdown handler")
	registerShutdownCallback(shutdownHandler);
	registerPreShutdownCallback(preshutdownHandler);
	registerPreShutdownCallback(preshutdownHandler2);
	postBatteryStatus(1,0)
	count = 0
	triggerReboot(0)

	while True:
		manageRebootEvent()
		manageLowBatteryShutdown()
		time.sleep(1)
		print("Main routine 1 second passed")


if __name__=="__main__":
	main()
