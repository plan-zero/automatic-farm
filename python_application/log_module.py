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