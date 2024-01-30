#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <thread>
#include <vector>

#include "include/LOG.h"
#include "include/LockValue.h"
#include "include/Path.h"
#include "include/irisConfig.h"
auto printCurrentTime() -> std::string;
auto getTopApp() -> std::string;
auto opt_on(const struct irisConfig *o, const struct FeasPath *p) -> bool;
void ihelper_default(const struct FeasPath *p);
auto profileMonitor(const char *dic, const char *profile,
                    std::vector<irisConfig> &conf, FeasPath &feaspath) -> int;
static inline auto RunMain(std::vector<irisConfig> &conf,
                           std::string &now_package, FeasPath &feaspath) -> bool
{
    // 获取TopApp name
    std::string const TopApp = getTopApp();

    // 包名与上次相同则直接返回
    if (TopApp == now_package) {
        return true;
    }

    now_package = TopApp;

    // LOG("时间: ", printCurrentTime());
    // 打印包名
    for (const auto &game : conf) {
        if (TopApp.find(game.app) != std::string::npos) {
            // LOG("检测到列表应用:   ", game.app, "\n");
            SPDLOG_INFO("检测到列表应用: {}\n", game.app);
            // LOG(game);
            opt_on(&game, &feaspath);
            return true;
        }
    }

    // LOG("检测到非列表应用: ", TopApp, "\n");
    SPDLOG_INFO("检测到非列表应用: {}\n", TopApp);
    ihelper_default(&feaspath);
    return true;
}

static inline auto RunStart(std::vector<irisConfig> &conf,
                            std::string &now_package, FeasPath &feaspath)
    -> bool
{
    pthread_setname_np(pthread_self(), "HeavyThread");
    while (true) {
        RunMain(conf, now_package, feaspath);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    return false;
}

auto runThread(std::vector<irisConfig> &conf, std::string &now_package,
               const char *dic, const char *profile, FeasPath &feaspath) -> bool
{
    std::thread HeavyThread(RunStart, std::ref(conf), std::ref(now_package),
                            std::ref(feaspath));

    std::thread profileMonitorThread(profileMonitor, dic, profile,
                                     std::ref(conf), std::ref(feaspath));
    // HeavyThread.detach();
    profileMonitorThread.detach();

    HeavyThread.join();
    profileMonitorThread.join();

    return false;
}
