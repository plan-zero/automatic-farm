def main():
	buffer = []

	with open("programmer.hex",'rb') as f:
	#	buff = f.read()
		for line in f:
			line = line[1:(len(line)-2)]
			print(line)
			buffer.append(line.decode("utf-8"))
			
	'''
	for e in buffer:
		myStr = ''
		while len(e) > 0:
			myStr = myStr + e[0:2] + ' '
			e = e[2:len(e)]
		print(myStr)
	'''
	
	myStr = []
	for e in buffer:
		line = 0
		column = 0
		sublist = []
		while len(e) > 0:
			sublist.append(int(('0x' + e[0:2]), 16))
			e = e[2:len(e)]
		myStr.append(sublist)
		
	for l in myStr:
		hexList = []
		for i in l:
			hexList.append(hex(i))
		print(hexList)
	
	# primul byte e lungimea
	# urmatorii 4 sunt adresa
	# urmeaza 16 bytes, ultimul e checksum
if __name__== "__main__":
  main()