#include "include/LOG.h"
#include "include/LockValue.h"
#include "include/irisConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
auto RunStart(std::vector<irisConfig> &conf, std::string &now_package) -> bool;
auto readProfile(const char *profile, std::vector<irisConfig> &conf) -> bool;

// 删除第一列
void GetSecondArg(std::string &buf)
{
	size_t pos = buf.find(' ');
	if (pos != std::string::npos) {
		buf.erase(0, pos + 1); // 删除空格和左边的所有内容
	}
}

void ihelper_default()
{
	std::string oo = " ";
	std::system(
		("/odm/bin/irisConfig \"47 1 0\"" + oo + "2>/dev/null").c_str());
	std::system(("/odm/bin/irisConfig \"258 1 0\"" + oo + "2>/dev/null")
			    .c_str());

	std::system(("/odm/bin/irisConfig \"267 2 3 0\"" + oo + "2>/dev/null")
			    .c_str());
	std::system(("/odm/bin/irisConfig \"273 1 0\"" + oo + "2>/dev/null")
			    .c_str());

	lock_val("", "/data/system/mcd/df");
	lock_val(0, "/sys/module/perfmgr_mtk/parameters/perfmgr_enable");
	lock_val(-1, "/sys/module/perfmgr_mtk/parameters/fixed_target_fps");
}
void print_struct(std::vector<irisConfig> &conf)
{
	for (const auto &tmp : conf) {
		LOG("包名: ", tmp.app);
		LOG("参数a: ", tmp.params_a);
		LOG("参数b: ", tmp.params_b);
		LOG("参数c: ", tmp.params_c);
		LOG("参数d: ", tmp.params_d);
		LOG("df: ", tmp.df);
		LOG("perfmgr是否开启: ", tmp.perfmgr_enable);
		LOG("目标fps: ", tmp.fixed_target_fps);
		LOG("\n");
	}
}
void initProfile(std::string argv1)
{
	// 嘻嘻
	std::system(
		("sed -i 's/pfmgr_enable/perfmgr_enable/g' " + argv1).c_str());
	std::system(("sed -i 's/f_t_fps/fixed_target_fps/g' " + argv1).c_str());
}
auto main(int argc, char **argv) -> int
{
	if (argv[1] == nullptr) {
		LOG("没填写argv[1]？");
		return 1;
	}
	initProfile(argv[1]);
	ihelper_default();

	std::vector<irisConfig> conf;

	if (!readProfile(argv[1], conf)) {
		LOG("文件都打不开，argv[1]填对了么？");
		return 2;
	}

	print_struct(conf);

	// 记录当前包名
	std::string now_package = "";
	RunStart(conf, now_package);
}
