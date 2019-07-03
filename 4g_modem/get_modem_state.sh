#!/bin/sh
curl -s --header "Referer: http://192.168.0.1/index.html" "http://192.168.0.1/goform/goform_get_cmd_process?cmd=$1"