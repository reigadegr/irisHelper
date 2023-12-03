#include <filesystem>

#include "include/LOG.h"
#include "include/Path.h"
auto FindPerfmgrName(const std::string &dir, const std::string &str,
                     const std::string &reduceStr) -> std::string;

static auto whetherPerfmgrExists() -> std::string
{
    // 先检测perfmgr_mtk节点
    if (!std::filesystem::exists("/sys/module/perfmgr_mtk")) {
        LOG("没刷YuniKernel");
        static std::string perfmgrName =
            FindPerfmgrName("/sys/module", "perfmgr_enable", "/parameters");
        if (perfmgrName == "UnSupport") {
            LOG("feas尊嘟不支持你的设备");
            return "UnSupport";
        }
        return perfmgrName;
    }
    return "perfmgr_mtk";
}
void initFeasPath(struct FeasPath *p)
{
    std::string perfmgrName = whetherPerfmgrExists();
    if (perfmgrName == "UnSupport") {
        p->enable = "";
        p->fps = "";
        return;
    }
    p->enable = ("/sys/module/" + perfmgrName + "/parameters/perfmgr_enable");
    p->fps =
        ("/sys/module/" + perfmgrName + "/parameters/fixed_target_fps").c_str();
    p->powersave =
        ("/sys/module/" + perfmgrName + "/parameters/powersave").c_str();
}
