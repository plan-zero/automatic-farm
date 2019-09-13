#!/bin/sh


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

