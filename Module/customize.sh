SKIPUNZIP=0
MODDIR=${0%/*}
echo "配置文件在/data/adb/modules/irisHelper_cpp/config/iriscfgcustomize.conf"
echo "与Hydro_BrÛleur配置文件通用，可以把其配置文件覆盖到本模块的对应位置"
echo "必须对配置文件内的app进行至少一项的参数配置"
echo "目前版本尝试使用inotifyd监控配置文件"

if [ ! -f /odm/bin/irisConfig ] && [ ! -f /sys/module/*/*/perfmgr_enable ]; then
    echo "你的设备既没有x7，又没有feas功能，不需要刷啦~"
    nohup rm -rf /data/adb/modules/irisHelper* >/dev/null 2>&1 &
    exit 0
fi

if [ -f /data/adb/modules/irisHelper*/config/iriscfgcustomize.conf ]; then
    cp -af /data/adb/modules/irisHelper*/config/iriscfgcustomize.conf $MODPATH/config/iriscfgcustomize.conf
fi

nohup rm -rf /data/adb/modules/irisHelper_cpp >/dev/null 2>&1 &

cat $MODPATH/README.md | while read row; do
    echo $row
    sleep 0.01
done
