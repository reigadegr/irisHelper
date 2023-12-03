#include <filesystem>
// #include "include/LOG.h"
namespace fs = std::filesystem;
//  参数分别为:
//  目录名，目标文件名，裁剪的字符串
#if 0
    参数说明:
    $1:搜索范围 $2:搜索的目标文件 $3:要开始裁剪字符串的标志(从第一个字符"/"开始裁剪)
    本例中接收FindPerfmgrName函数返回值即可获取perfmgr节点名称
    例如：std::string perfmgrName=FindPerfmgrName("/sys/module", "perfmgr_enable", "/parameters");
#endif

auto FindPerfmgrName(const std::string &dir, const std::string &str,
                     const std::string &reduceStr) -> std::string
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
