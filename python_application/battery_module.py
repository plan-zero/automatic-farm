import sys,os
import time
import inspect

bat_v = 0
bat_p = 0

batteryRaw = 0
BAT_HIGH = 13
BAT_LOW = 11.9
BAT_WARNING = 0
BAT_CRITICAL = 0
BAT_MED = 12.15


batteryStatusCallback = []

def getBatteryData():
	global bat_v, bat_p
	return [bat_v, bat_p]

def getBatteryRawData():
	global batteryRaw
	return batteryRaw

def readBatteryDataHandler(received_data):
	global batteryRaw
	state = 0
    	count = 0
    	all_val = 0
	#print("Received data " + str(received_data))
	for _char in received_data:
		if state == 0:
			if _char == 'B':
				state = 1
			else:
				state = 0
		elif state == 1:
			if _char == 'A':
				state = 2
			else:
				state = 0
		elif state == 2:
			if _char == 'T':
				state = 3
			else:
				state = 0
		elif state == 3:
			#get data here
			raw_value = ord(_char) << 8
			state = 4
		elif state == 4:
			raw_value = raw_value | ord(_char)
			state = 0
			# print(raw_value)
			all_val = all_val + raw_value
			count = count + 1
	batteryRaw = round(all_val / count)

def _checkArgumentsOfFunction(handler):
	try:
		if sys.version_info[0] < 3:
			return len(inspect.getargspec(handler).args)
		return len(inspect.getfullargspec(handler).args)
	except:
		e = sys.exc_info()
		print(e)	

def unregisterStatusBatteryCallback(handler):
	try:
		global batteryStatusCallback
		if handler in batteryStatusCallback:
			batteryStatusCallback.remove(handler)
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)

def registerStatusBatteryCallback(handler):
	try:
		global batteryStatusCallback
		if handler not in batteryStatusCallback:
			if _checkArgumentsOfFunction(handler) == 2:
				batteryStatusCallback.append(handler)
			else:
				print("The handler must have 2 arguments")
			return 1
		return 0
	except:
		e = sys.exc_info()
		print(e)


def calculateBatteryVoltage(raw):
    try:
	global bat_v
        K = 8.5  #K = 8.05 #resistor raport
        DV = 0.4 #diode (nmosfet) offset
        v_ref = 2.56 #adc reference

        v_adc = (raw * v_ref) / 1023
        _v_bat = round(v_adc * K + DV,2)
        #print("RAW value is: " + str(raw))
        if raw != -1:
            bat_v = _v_bat
            return _v_bat
        else:
            return -1
    except:
        e = sys.exc_info()
        print("calculateBatteryVoltage: error during conversion")
        print(e)
        return -1

def calculateBatteryLevel(bat_v):
    global BAT_HIGH
    global BAT_LOW
    global BAT_MED
    global BAT_WARNING
    global BAT_CRITICAL
    global bat_p
    try:
		if bat_v == -1:
			return -1
		else:
			if bat_v < BAT_MED and bat_v > BAT_LOW:
				level = int( (bat_v  - BAT_LOW) / (BAT_HIGH - BAT_LOW) * 100 )
				#print("WARNING: calculateBatteryLevel: BAT_V is " + str(bat_v) + " procentage is: " + str(level))
				BAT_WARNING = 1
				for f in batteryStatusCallback:
					f(BAT_WARNING, BAT_CRITICAL)
                                bat_p = level
				return level
			if bat_v <= BAT_HIGH and bat_v >= BAT_MED:
				level = int( (bat_v - BAT_LOW) / (BAT_HIGH - BAT_LOW) * 100)
				#print("calculateBatteryLevel: BAT_V is " + str(bat_v) + " procentage is: " + str(level))
				BAT_CRITICAL = 0
				BAT_WARNING = 0
                                bat_p = level
				return level
			elif bat_v < BAT_LOW:
				level = 0
				#print("calculateBatteryLevel: CRITICAL battery level is too low: "+ str(bat_v) +", shutdown is required")
				BAT_CRITICAL = 1
				for f in batteryStatusCallback:
					f(BAT_WARNING, BAT_CRITICAL)
                                bat_p = level
				return level
			elif bat_v > BAT_HIGH:
				level = 100
				#print("battery level is full, this can be caused by battery charging: " + str(bat_v))
				bat_p = level
                                return level
    except:
        e = sys.exc_info()
        print("calculateBatteryVoltage: error during conversion")
        print(e)
        return -1


def main():
	print("testing battery module")
	def batteryHandler(bat_w, bat_c):
		print("The battery status is " + str(bat_w) + " , " + str(bat_c))
	registerStatusBatteryCallback(batteryHandler)
	b_v = calculateBatteryVoltage(550)
	b_p = calculateBatteryLevel(b_v)
	print("Battery is " + str(b_v) + " , " + str(b_p) )

if __name__=="__main__":
	main()
