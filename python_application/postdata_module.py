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
