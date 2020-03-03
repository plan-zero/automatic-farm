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


import re
import sys,os
import subprocess

scriptsPath="/home/pi/farmview/4g_modem/"

def executeShellScript(scriptName,Parameter):
	output = ''
	global scriptsPath
	try:
		temp_msg = open("tmp.txt", 'w')
		print("Executing: " + scriptsPath + scriptName )
		if Parameter != '':
			cmd = []
			cmd.append('sh')
			cmd.append(str(scriptsPath + scriptName))
			for p in Parameter:
				cmd.append(p)
			subprocess.call(cmd, stdout=temp_msg)
		else:
			subprocess.call(['sh',scriptsPath + scriptName], stdout=temp_msg)
		with open("tmp.txt",'r') as file:
			output = file.read()
			file.close()
		temp_msg.close()
		return output
	except:
		e = sys.exc_info()
		print("executeShellScript: error during script execution")
		print(e)
		return ''

def extractResponse(curlraw, keyword):
    try:
        output = re.findall(r"\"" + keyword + "\"\:\"([A-Za-z0-9_+]+)\"", curlraw)
        if len(output) > 0:
            return output[0]
        return ''
    except:
        e = sys.exc_info()
        print("extractResponse: error during curl value extraction")
        print(e)
        return ''
		
def main():
	print("Testing")
	executeShellScript("test.sh",'')
	
	
if __name__=="__main__":
	main()
