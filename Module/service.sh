#!/bin/sh
wait_until_login() {
    # in case of /data encryption is disabled
    while [ "$(getprop sys.boot_completed)" != "1" ]; do
        sleep 1
    done

    # in case of the user unlocked the screen
    while [ ! -d "/sdcard/Android" ]; do
        sleep 1
    done
}
wait_until_login

MODDIR=${0%/*}
FileName="irisHelper"

if [ -f $MODDIR/$FileName.log ]; then
    mv -f $MODDIR/$FileName.log $MODDIR/$FileName.log.bak
fi
touch $MODDIR/$FileName.log
#rm -rf /data/adb/modules/scene_systemless/iris

chown 0:0 $MODDIR/$FileName
chmod +x $MODDIR/$FileName
killall -15 $FileName iris_helper
nohup $MODDIR/$FileName $MODDIR/config/iriscfgcustomize.conf >$MODDIR/$FileName.log 2>&1 &
