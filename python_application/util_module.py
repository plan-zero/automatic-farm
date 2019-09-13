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
