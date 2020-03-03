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


from time import gmtime, strftime, localtime


def logMessage(msg):
    timestamp = strftime("%Y-%m-%d %H:%M:%S", localtime())
    with open("messages.txt",'a') as file:
        file.write(timestamp + " " + msg + "\n")
        file.close()
    print(timestamp + " " + msg)
	
	
def main():
	logMessage("This is a test")

if __name__=="__main__":
	main()