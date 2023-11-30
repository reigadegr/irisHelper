#include "include/LockValue.h"
#include "include/UnLockValue.h"
#include "include/irisConfig.h"

void ihelper_default();
auto params_run(std::string param) -> std::string
{
	if (param == "") {
		return {};
	}
	std::system(("/odm/bin/irisConfig \"" + param + "\"" + " 2>/dev/null")
			    .c_str());
	return {};
}
auto opt_on(const struct irisConfig *o) -> bool
{
	lock_val(o->app + " " + o->df, "/data/system/mcd/df");

	lock_val(o->perfmgr_enable,
		 "/sys/module/perfmgr_mtk/parameters/perfmgr_enable");

	lock_val(o->fixed_target_fps,
		 "/sys/module/perfmgr_mtk/parameters/fixed_target_fps");
	params_run(o->params_a);
	params_run(o->params_b);
	params_run(o->params_c);
	params_run(o->params_d);
	return true;
}

auto opt_off() -> bool
{
	ihelper_default();
	lock_val("", "/data/system/mcd/df");
	Unlock_val(0, "/sys/module/perfmgr_mtk/parameters/perfmgr_enable");
	Unlock_val(-1, "/sys/module/perfmgr_mtk/parameters/fixed_target_fps");
	return true;
}
