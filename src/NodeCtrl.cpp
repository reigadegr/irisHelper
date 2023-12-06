#include "include/LOG.h"
#include "include/LockValue.h"
#include "include/Path.h"
#include "include/UnLockValue.h"
#include "include/irisConfig.h"

static inline void defaultConfig(const std::string &Cmd)
{
    std::string fullCmd =
        "nohup /odm/bin/irisConfig \"" + Cmd + "\"" + " >/dev/null 2>&1 &";
    std::system(fullCmd.c_str());
}

void ihelper_default(const struct FeasPath *p)
{
    defaultConfig("47 1 0");
    defaultConfig("258 1 0");
    defaultConfig("267 2 3 0");
    defaultConfig("273 1 0");

    Unlock_val("", "/data/system/mcd/df");

    Unlock_val(0, (p->enable).c_str());
    Unlock_val(-1, (p->fps).c_str());
    Unlock_val("N", (p->powersave).c_str());
}
static inline auto params_run(const std::string param) -> std::string
{
    if (param == "") {
        return {};
    }
    std::system(
        ("nohup /odm/bin/irisConfig \"" + param + "\" >/dev/null 2>&1 &")
            .c_str());
    return {};
}
auto opt_on(const struct irisConfig *o, const struct FeasPath *p) -> bool
{
    // df
    if (o->df != "") {
        lock_val(o->app + " " + o->df, "/data/system/mcd/df");
        std::system("stop mcd_init mcd_service; start mcd_init mcd_service");
    }

    // feas
    lock_val(o->perfmgr_enable, p->enable.c_str());
    lock_val(o->fixed_target_fps, p->fps.c_str());
    lock_val(o->perfmgr_powersave, p->powersave.c_str());

    // irisConfig params
    params_run(o->params_a);
    params_run(o->params_b);
    params_run(o->params_c);
    params_run(o->params_d);
    return true;
}
