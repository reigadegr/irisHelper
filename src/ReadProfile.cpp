#include "include/LOG.h"
#include "include/irisConfig.h"
#include <fstream>
#include <iostream>
#include <vector>
void GetSecondArg(std::string &buf);
static inline void reduceStr(std::string &buf) {
    while (buf.find("\"") != std::string::npos) {
        buf.erase(buf.find("\""), 1);
    }
}
auto strCtrl(std::string &buf, const char *target_str, std::string &arg)
    -> bool {
    if (buf.find(target_str) != std::string::npos) {
        GetSecondArg(buf);
        reduceStr(buf);
        arg = buf;
        return true;
    }
    return false;
}

auto readProfile(const char *profile, std::vector<irisConfig> &conf) -> bool {
    std::ifstream file(profile);
    if (!file.is_open()) {
        return false;
    }
    std::string buf;
    while (true) {
        std::string app = "";
        std::string params_a = "";
        std::string params_b = "";
        std::string params_c = "";
        std::string params_d = "";
        std::string df = "";
        std::string perfmgr_enable = "0";
        std::string fixed_target_fps = "-1";
        bool flag = false;
        while (std::getline(file, buf)) {
            if (buf[0] == '#' || buf.empty()) {
                continue;
            }

            if (buf.find("app") != std::string::npos) {
                if ((app != "") && (flag = true)) {
                    conf.push_back({app, params_a, params_b, params_c, params_d,
                                    df, perfmgr_enable, fixed_target_fps});
                    // reset value
                    params_a = "";
                    params_b = "";
                    params_c = "";
                    params_d = "";
                    df = "";
                    perfmgr_enable = "0";
                    fixed_target_fps = "-1";
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
                continue;
            }
        }
        if (app == "") {
            break;
        }
        conf.push_back({app, params_a, params_b, params_c, params_d, df,
                        perfmgr_enable, fixed_target_fps});
    }
    file.close();
    return true;
}
