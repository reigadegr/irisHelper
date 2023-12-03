#!/bin/sh
chmod +x $(pwd)/irisHelper
chown 0:0 $(pwd)/irisHelper
#killall -15 irisHelper
$(pwd)/irisHelper $(pwd)/iriscfgcustomize.conf
