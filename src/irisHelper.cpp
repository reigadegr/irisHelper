#include "include/LOG.h"
#include "include/LockValue.h"
#include "include/irisConfig.h"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
void ihelper_default();
//auto RunStart(std::vector<irisConfig> &conf, std::string &now_package) -> bool;
auto readProfile(const char *profile, std::vector<irisConfig> &conf) -> bool;
//void print_struct(std::vector<irisConfig> &conf);
auto runThread(std::vector<irisConfig> &conf, std::string &now_package,
	       const char *dic, const char *profile) -> bool;

auto dirname(std::string path) -> std::string
{
	std::filesystem::path p(path);
	return p.parent_path();
}

static inline void initProfile(std::string argv1)
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

	// 记录当前包名
	std::string now_package = "";

	std::mutex confMutex;
	runThread(conf, now_package, (dirname(argv[1])).c_str(), argv[1]);
}
