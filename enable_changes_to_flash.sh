#!/bin/bash

IP="lyt8266.fritz.box"

if [ $# -eq 1 ]; then
	IP="$1"
fi

curl http://$IP/unlock?password=securitybyobscurity
echo