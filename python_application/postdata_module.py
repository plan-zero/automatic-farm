import os,sys
import subprocess

post_bat = 0
post_bat_interval = 120



def m_post_bat(bat, voltage):
    global post_bat
    try:
        if post_bat == post_bat_interval:
           post_bat = 0
           tmp = 0
           subprocess.call(['sh','post_bat',str(bat), str(voltage)], stdout= tmp )
        post_bat = post_bat + 1
    except:
        e = sys.exc_info()
        print("bat post error")
        print(e)


def main():
	print ("testing")
	post_bat = 60
	m_post_bat(13,100)


if __name__=="__main__":
	main()
