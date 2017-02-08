#!/bin/bash

IP="lyt8266.fritz.box"

curl http://$IP/unlock?password=securitybyobscurity
echo
sleep 2

FW="$(ls /tmp/arduino_build_*/*.bin)" && curl -F "image=@$FW" $IP/update
echo
