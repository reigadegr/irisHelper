# irisHelper
c++版irisHelper

配置文件块写法：
例如: 
app: "com.miHoYo.hkrpg"
params_a: 258 4 40 -1 2 -1 3 0
params_b: 47 1 13
params_c: 273 2 1 3
params_d: 267 2 1 3
perfmgr_enable: 1
fixed_target_fps: 60
perfmgr_powersave: N

当然，不写冒号，引号也可以，如下：

app com.miHoYo.hkrpg
params_a 258 4 40 -1 2 -1 3 0
params_b 47 1 13
params_c 273 2 1 3
params_d 267 2 1 3
perfmgr_enable 1
fixed_target_fps 60
perfmgr_powersave Y

逻辑是匹配到指定行后消除每行第一列的字符串
所以不要把冒号左边加空格!这样会把空格写到节点然后出现问题
如果写了多个相同包名的配置，则只会以先写的为准
