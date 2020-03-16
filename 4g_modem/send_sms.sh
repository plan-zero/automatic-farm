#!/bin/sh
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




current_time=$(date +"%y;%m;%d;%H;%M;%S;+2")
encoded_text=$2
textlen=${#2}

if test $textlen -ge 1080; then 
	echo [-1] Sms limit is 1080
	return
fi

curl -s --header "Referer: http://192.168.0.1/index.html" \
"http://192.168.0.1/goform/goform_set_cmd_process?\
isTest=false&goformId=SEND_SMS&notCallback=true&Number=$1&sms_time=$current_time&MessageBody=$encoded_text&ID=-1&encode_type=GSM7_default"

