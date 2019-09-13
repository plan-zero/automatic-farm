import util_module as util
import support_gsm7 as gsm7
import time
import sys,os
import re

#admin = ["0743566434", "+40743566434", "0741457348", "+40741457348"]
phonebook = ["0743566434"] #, "0757791322", "0741457348"]


admin = []
definedCommands = []

def addAdministrator(adm):
        global admin
        admin.append(adm)
def removeAdministrator(adm):
        global admin
        if adm in admin:
                admin.remove(adm)




def readSMS():
	#check incoming messages
	output = util.executeShellScript('clear_sms_flag.sh','')
	value = util.extractResponse(output, 'sms_received_flag')

	if value > 0 and value != '':
		#print("MonitorApp: ["+ value + "] messages have been received. Going to read them!")
		output = util.executeShellScript('get_sms.sh','')
		rec_messages = decodeSMS(output)
		return rec_messages
	return []

def sendSMS(Number, Message):
	output = ''
	global scriptsPath
	try:
		_enc_message = gsm7.encodeMessage(Message)
		output = util.executeShellScript('send_sms.sh',[Number, _enc_message])
		return output
	except:
		e = sys.exc_info()
		print("sendSMS: error during script execution")
		print(e)
		return ''

def decodeSMS(msg):
	try:
		messages = []
		tokens = re.findall(r"\{([A-Za-z0-9_,\"\:\+]+)\}", msg)
		for t in tokens:
			_number = re.findall(r"\"number\"\:\"([A-Za-z0-9_+]+)\"", t)
			_content = re.findall(r"\"content\"\:\"([A-Fa-f0-9]+)\"", t)
			_id = re.findall(r"\"id\"\:\"([0-9]+)\"", t)
			temp_msg = open("tmp.txt", 'w')
			_content = gsm7.decodeMessage(_content[0])
			#print("HERE " + _content)
			messages.append([_id[0],_number[0],_content ])
		return messages
	except:
		e = sys.exc_info()
		print("decodeSMS: error during sms decoding")
		print(e)
		return ''

def formatNumber(number):
	try:
		if '+' in number:
			return number[2:]
		else:
			return number
	except:
		e = sys.exc_info()
		print("formatNumber: error during formating")
		print(e)
		return "0743566434"


def checkMessages():
	try:
		messages = readSMS()
                cmd_messages = []
		for m in messages:
			id = m[0]
			number = m[1]
			text = m[2]

			if number in admin:
				cmd_messages.append(m)
				util.executeShellScript('delete_sms.sh',[id])
			else:
				sendSMS(admin[0], "FW:" + number + ":" + text)
				util.executeShellScript('delete_sms.sh',[id])
		return cmd_messages

	except:
               e = sys.exc_info()
               print("error during msg checking")
               print(e)
               return []


def main():
	print("testing area")
	#sendSMS("0743566434", "salut andrei")
	while True:
		checkMessages()
		time.sleep(1)

if __name__=="__main__":
	main()