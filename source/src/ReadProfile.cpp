#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>
#include <vector>

#include "include/LOG.h"
#include "include/irisConfig.h"
template <typename T, typename U>
static inline void printParams(std::string_view Description, const T &params,
                               const U &default_value)
{
    if (params == default_value) [[unlikely]] {
        return;
    }
    LOG(Description.data(), params);
}
void print_struct(std::vector<irisConfig> &conf)
{
    for (const auto &tmp : conf) {
        SPDLOG_INFO("\n已成功添加: {}", tmp.app);
        printParams("参数a: ", tmp.params_a, "");
        printParams("参数b: ", tmp.params_b, "");
        printParams("参数c: ", tmp.params_c, "");
        printParams("参数d: ", tmp.params_d, "");
        printParams("df: ", tmp.df, "");
        printParams("perfmgr是否开启: ", tmp.perfmgr_enable, 0);
        printParams("目标fps: ", tmp.fixed_target_fps, -1);
        printParams("低功耗perfmgr: ", tmp.perfmgr_powersave, "N");
        LOG("---------------------");
    }
    SPDLOG_INFO("查询完毕");
}
// 删除第一列
static inline void GetSecondArg(std::string &buf)
{
    size_t pos = buf.find(' ');
    if (pos != std::string::npos) {
        buf.erase(0, pos + 1);  // 删除空格和左边的所有内容
    }
}
static inline void reduceStr(std::string &buf)
{
    while (buf.find("\"") != std::string::npos) {
        buf.erase(buf.find("\""), 1);
    }
}
static inline auto strCtrl(std::string &buf, const char *target_str,
                           std::string &arg) -> bool
{
    if (buf.find(target_str) != std::string::npos) {
        GetSecondArg(buf);
        reduceStr(buf);
        if (buf == target_str) {
            buf = "";
            return true;
        }
        arg = buf;
        return true;
    }
    return false;
}

auto readProfile(std::string_view profile, std::vector<irisConfig> &conf)
    -> bool
{
    static std::mutex confMutex;
    std::ifstream file(profile.data());
    if (!file.is_open()) [[unlikely]] {
        return false;
    }
    conf.clear();
    std::string buf;
    SPDLOG_INFO("加载配置文件...");

    std::string app = "";
    std::string params_a = "";
    std::string params_b = "";
    std::string params_c = "";
    std::string params_d = "";
    std::string df = "";
    std::string perfmgr_enable = "0";
    std::string fixed_target_fps = "-1";
    std::string perfmgr_powersave = "N";
    bool flag = false;
    while (std::getline(file, buf)) {
        if (buf[0] == '#' || buf.empty()) {
            continue;
        }

        if (buf.find("app") != std::string::npos) {
            if ((app != "") && (flag = true)) {
                if (!(params_a == "" && params_b == "" && params_c == "" &&
                      params_d == "" && df == "" && perfmgr_enable == "0" &&
                      fixed_target_fps == "-1" && perfmgr_powersave == "N")) {
                    std::lock_guard<std::mutex> lock(confMutex);
                    conf.push_back({app.c_str(), params_a.c_str(),
                                    params_b.c_str(), params_c.c_str(),
                                    params_d.c_str(), df.c_str(),
                                    atoi(perfmgr_enable.c_str()),
                                    atoi(fixed_target_fps.c_str()),
                                    perfmgr_powersave.c_str()});
                }
                // reset value
                params_a = "";
                params_b = "";
                params_c = "";
                params_d = "";
                df = "";
                perfmgr_enable = "0";
                fixed_target_fps = "-1";
                perfmgr_powersave = "N";
            }
            GetSecondArg(buf);
            reduceStr(buf);
            app = buf;
            flag = true;
            continue;
        }

        if (strCtrl(buf, "params_a", params_a)) {
            continue;
        }
        if (strCtrl(buf, "params_b", params_b)) {
            continue;
        }
        if (strCtrl(buf, "params_c", params_c)) {
            continue;
        }
        if (strCtrl(buf, "params_d", params_d)) {
            continue;
        }
        if (strCtrl(buf, "df", df)) {
            continue;
        }
        if (strCtrl(buf, "perfmgr_enable", perfmgr_enable)) {
            if (perfmgr_enable != "0" && perfmgr_enable != "1") {
                perfmgr_enable = "0";
            }
            continue;
        }
        if (strCtrl(buf, "fixed_target_fps", fixed_target_fps)) {
            if (fixed_target_fps == "") {
                fixed_target_fps = "-1";
            }
            continue;
        }
        if (strCtrl(buf, "perfmgr_powersave", perfmgr_powersave)) {
            if (perfmgr_powersave != "Y" && perfmgr_enable != "N") {
                perfmgr_enable = "N";
            }
            continue;
        }
    }

    if (!(params_a == "" && params_b == "" && params_c == "" &&
          params_d == "" && df == "" && perfmgr_enable == "0" &&
          fixed_target_fps == "-1" && perfmgr_powersave == "N")) {
        std::lock_guard<std::mutex> lock(confMutex);
        conf.push_back(
            {app.c_str(), params_a.c_str(), params_b.c_str(), params_c.c_str(),
             params_d.c_str(), df.c_str(), atoi(perfmgr_enable.c_str()),
             atoi(fixed_target_fps.c_str()), perfmgr_powersave.c_str()});
    }
    file.close();
    print_struct(conf);
    return true;
}
