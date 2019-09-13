import log_module as logger

definedCommands = []


def registerCommandHandler(commandString, commandHandler):
	global definedCommands
	definedCommands.append([commandString, commandHandler])

def unregisterCommandHandler(commandString, commandHandler):
	global definedCommands
	for ftuple in definedCommands:
		if ftuple[0] == commandString and ftuple[1] == commandHandler:
			definedCommands.remove(ftuple)

def executeCommand(messages):
	print("messages are: " + str(messages))
	global definedCommands
	for msg in messages:
		arg = []
		arg.append(msg[1])
		arg.append(msg[2])

		#print("going to execution")
		for ftuple in definedCommands:
			if ftuple[0] == arg[1]:
				#print("These are eq:" + str(ftuple[0]) + "  " + str(arg[1]) )
				functPtr = ftuple[1]
				functPtr(arg)


def main():
	def myHandler1(arg):
		print(arg + " handler 1")

	def myHandler2(arg):
		print(arg + " handler 2")
	def myHandler3(arg):
		print(arg + " handler 3")
	addAdministrator("0743566434")
	registerCommandHandler("getBattery", myHandler1)
	registerCommandHandler("cancelReboot", myHandler2)
	registerCommandHandler("reboot", myHandler3)
	mes = [ ["0", "0743566434", "cancelReboot"], ["0", "0743566434", "getBattery"] ]
	executeCommand(mes)

	print("Remove handler 2")
	unregisterCommandHandler("cancelReboot", myHandler2)
	executeCommand(mes)

	print("Remove admin")
	removeAdministrator("0743566434")
	executeCommand(mes)

if __name__ == "__main__":
	main()
