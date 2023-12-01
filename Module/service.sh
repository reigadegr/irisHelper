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
initfile(){
    [ ! -d $(dirname "$1") ] && mkdir -p $(dirname "$1")
    chattr -R -i "$1"
    touch "$1"
    chmod 0644 "$1"
}
init_filesystem(){
    df_file="/data/system/mcd/df"        
    initfile "$df_file"
}
MODDIR=${0%/*}
FileName="irisHelper"

if [ -f $MODDIR/$FileName.log ]; then
    mv -f $MODDIR/$FileName.log $MODDIR/$FileName.log.bak
fi
touch $MODDIR/$FileName.log
init_filesystem
chown 0:0 $MODDIR/$FileName
chmod +x $MODDIR/$FileName
killall -15 $FileName iris_helper
nohup $MODDIR/$FileName $MODDIR/config/iriscfgcustomize.conf >$MODDIR/$FileName.log 2>&1 &
