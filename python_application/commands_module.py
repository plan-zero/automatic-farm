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


import log_module as logger

definedCommands = []
admin_number=""


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
	addAdministrator(admin_number)
	registerCommandHandler("getBattery", myHandler1)
	registerCommandHandler("cancelReboot", myHandler2)
	registerCommandHandler("reboot", myHandler3)
	mes = [ ["0", admin_number, "cancelReboot"], ["0", admin_number, "getBattery"] ]
	executeCommand(mes)

	print("Remove handler 2")
	unregisterCommandHandler("cancelReboot", myHandler2)
	executeCommand(mes)

	print("Remove admin")
	removeAdministrator(admin_number)
	executeCommand(mes)

if __name__ == "__main__":
	main()
