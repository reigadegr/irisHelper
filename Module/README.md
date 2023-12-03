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

甚至是，第一列内容不规范的写法也行，例如:

$(&+#appa#)+' com.miHoYo.hkrpg
_("?'!params_axj 258 4 40 -1 2 -1 3 0
!'!$(2_params_bfhf 47 1 13
$('!!'params_cxjzj_&-+(@_" 273 2 1 3
_('39!params_dxhzj 267 2 1 3
ssdfccperfmgr_enablensns 1
jsjsbsfixed_target_fps 60
_\_=perfmgr_powersavehshs Y

逻辑是只要第一列的字符串包含指定的子串就可以被识别
当然，也只有第一列内容可以乱写
如果写冒号，不要把冒号左边加空格!这样会把空格也读到配置里
如果写了多个相同包名的配置块，则在运行时只会以先写的为准
如果连续写了多个相同的参数，则以最后的写的参数为准
