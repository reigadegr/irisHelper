SKIPUNZIP=0
MODDIR=${0%/*}
echo "配置文件在/data/adb/modules/irisHelper_cpp/config/iriscfgcustomize.conf"
echo "与Hydro_BrÛleur配置文件通用，可以把其配置文件覆盖到本模块的对应位置"
echo "必须对配置文件内的app进行至少一项的参数配置"
echo "目前版本修改完配置文件需要重新执行service.sh以更新"
if [ -f /data/adb/modules/irisHelper_cpp/config/iriscfgcustomize.conf ]; then
    cp -af /data/adb/modules/irisHelper_cpp/config/iriscfgcustomize.conf /data/adb/modules_update/irisHelper_cpp/config/iriscfgcustomize.conf
fi

cat $MODPATH/README.md | while read row; do
    echo $row
    sleep 0.12
done
