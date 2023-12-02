#include "include/LockValue.h"
#include "include/UnLockValue.h"
#include "include/irisConfig.h"

void ihelper_default()
{
    // oo的作用：防止constchar *类型数据直接相加出错
    std::string oo = " ";
    std::system(
        ("/odm/bin/irisConfig \"47 1 0\"" + oo + "2>/dev/null").c_str());
    std::system(
        ("/odm/bin/irisConfig \"258 1 0\"" + oo + "2>/dev/null").c_str());

    std::system(
        ("/odm/bin/irisConfig \"267 2 3 0\"" + oo + "2>/dev/null").c_str());
    std::system(
        ("/odm/bin/irisConfig \"273 1 0\"" + oo + "2>/dev/null").c_str());

    Unlock_val("", "/data/system/mcd/df");
    Unlock_val(0, "/sys/module/perfmgr_mtk/parameters/perfmgr_enable");
    Unlock_val(-1, "/sys/module/perfmgr_mtk/parameters/fixed_target_fps");
    Unlock_val("N", "/sys/module/perfmgr_mtk/parameters/powersave");
}
static inline auto params_run(std::string param) -> std::string
{
    if (param == "") {
        return {};
    }
    std::system(("/odm/bin/irisConfig \"" + param + "\" 2>/dev/null").c_str());
    return {};
}
auto opt_on(const struct irisConfig *o) -> bool
{
    std::string oo = " ";
    lock_val(o->app + oo + o->df, "/data/system/mcd/df");

    lock_val(o->perfmgr_enable,
             "/sys/module/perfmgr_mtk/parameters/perfmgr_enable");

    lock_val(o->fixed_target_fps,
             "/sys/module/perfmgr_mtk/parameters/fixed_target_fps");
    lock_val(o->perfmgr_powersave,
             "/sys/module/perfmgr_mtk/parameters/powersave");
    params_run(o->params_a);
    params_run(o->params_b);
    params_run(o->params_c);
    params_run(o->params_d);
    return true;
}
