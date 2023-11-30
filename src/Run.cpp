#include "include/LOG.h"
#include "include/LockValue.h"
#include "include/irisConfig.h"
#include <sstream>
#include <thread>
#include <vector>
auto opt_off() -> bool;
auto printCurrentTime() -> std::string;
auto getTopApp() -> std::string;
auto opt_on(const struct irisConfig *o) -> bool;
auto RunMain(std::vector<irisConfig> &conf, std::string &now_package) -> bool
{
	// 获取TopApp name
	std::string const TopApp = getTopApp();

	// 包名与上次相同则直接返回
	if (TopApp == now_package) {
		return true;
	}

	now_package = TopApp;

	LOG("时间: ", printCurrentTime());
	// 打印包名
	for (const auto &game : conf) {
		if (TopApp.find(game.app) != std::string::npos) {
			LOG("检测到列表应用:   ", game.app, "\n");
			// LOG(game);
			opt_on(&game);
			return true;
		}
	}

	LOG("检测到非列表应用: ", TopApp, "\n");
	opt_off();
	return true;
}

auto RunStart(std::vector<irisConfig> &conf, std::string &now_package) -> bool
{
	while (true) {
		RunMain(conf, now_package);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}