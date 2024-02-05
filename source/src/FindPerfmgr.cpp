#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>

#include <filesystem>

#include "include/LOG.h"
#include "include/Path.h"
namespace fs = std::filesystem;
//  参数分别为:
//  目录名，目标文件名，裁剪的字符串
#if 0
    参数说明:
    $1:搜索范围 $2:搜索的目标文件 $3:要开始裁剪字符串的标志(从第一个字符"/"开始裁剪)
    本例中接收FindPerfmgrName函数返回值即可获取perfmgr节点名称
    例如：std::string perfmgrName=FindPerfmgrName("/sys/module", "perfmgr_enable", "/parameters");
#endif

auto FindPerfmgrName(std::string_view dir, const std::string_view &str,
                     const std::string_view &reduceStr) -> std::string
{
    for (const auto &entry : fs::recursive_directory_iterator(dir)) {
        std::string const path = entry.path().string();

        if (path.find(str) != std::string::npos) {
            //$1: 起始位置; $2: 裁剪的长度
            std::string PerfmgrName = path.substr(
                dir.size() + 1, path.find(reduceStr) - dir.size() - 1);
            return PerfmgrName;
        }
    }
    return "UnSupport";
}
static auto whetherPerfmgrExists() -> std::string
{
    // 先检测perfmgr_mtk节点
    if (!fs::exists("/sys/module/perfmgr_mtk")) {
        SPDLOG_INFO("没刷YuniKernel");
        static std::string perfmgrName =
            FindPerfmgrName("/sys/module", "perfmgr_enable", "/parameters");
        if (perfmgrName == "UnSupport") {
            SPDLOG_INFO("feas尊嘟不支持你的设备");
            return "UnSupport";
        }
        return perfmgrName;
    }
    return "perfmgr_mtk";
}
void initFeasPath(struct FeasPath *p)
{
    std::string perfmgrName = whetherPerfmgrExists();
    if (perfmgrName == "UnSupport") [[unlikely]] {
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
