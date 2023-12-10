#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <vector>

#if 0
    基于shadow3aaa的版本，微调
    使用说明：直接接收getTopApp()函数的返回值即可获取包名
    例如：std::string TopApp = getTopApp();
#endif
static inline auto getTopAppShell() -> std::string;
static inline auto execCmdSync(std::string command,
                               const std::vector<std::string> &args)
    -> std::string
{
    // 将命令和参数拼接为一个字符串
    for (const auto &arg : args) {
        command += " ";
        command += arg;
    }
    // 执行命令并获取输出
    FILE *pipe = popen(command.c_str(), "r");
    if (pipe == nullptr) {
        return {};
    }
    char buffer[2];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

static inline auto Testfile(const char *location)
{
    return access(location, F_OK) == 0;
}
auto getTopApp() -> std::string
{
    if (Testfile("/sys/kernel/gbe/gbe2_fg_pid")) {
        std::string pid;

        std::ifstream f_pid("/sys/kernel/gbe/gbe2_fg_pid");
        if (!f_pid.is_open()) {
            chmod("/sys/kernel/gbe/gbe2_fg_pid", 0666);
            return getTopAppShell();
        }
        f_pid >> pid;
        f_pid.close();
        if (pid == "0") {
            return getTopAppShell();
        }
        std::ifstream app("/proc/" + pid + "/cmdline");

        if (!app.is_open()) {
            chmod(("/proc/" + pid + "/cmdline").c_str(), 0666);
            return getTopAppShell();
        }
        std::string name;
        std::getline(app, name, '\0');
        app.close();
        return name;
        //  return checkSymbol(name);
    }
    return getTopAppShell();
}
/*
static inline auto getTopAppShell() -> std::string
{
    std::string name;
    const std::string str =
        execCmdSync("/system/bin/dumpsys", {"window", "visible-apps"});

    const auto pkgPos = str.find("package=") + 8;

    name = str.substr(pkgPos, str.find(' ', pkgPos) - pkgPos - 0);

    const auto first = name.find_first_not_of(' ');
    const auto last = name.find_last_not_of(' ');
    name = name.substr(first, last - first + 1);
    return name;
    // return checkSymbol(name);
}
*/

static inline auto getTopAppShell() -> std::string
{
    std::string name = execCmdSync("/system/bin/dumpsys", {"activity", "lru"});
    /*
        name.erase(name.find('/'));

            size_t pos;
            while ((pos = name.find(':')) != std::string::npos) {
                name.erase(0, pos + 1);
            }
        */

    const auto pkgPos = name.find(" TOP") + 4;
    // find第二个参数:从指定的位置开始搜索
    name = name.substr(pkgPos, name.find('/', pkgPos) - pkgPos);
    size_t pos;
    if ((pos = name.find(":")) != std::string::npos) {
        name.erase(0, pos + 1);  // 删除冒号及其前面的内容
    }

    return name;

    // return checkSymbol(name);
}

// 暂时不用
/*
auto checkSymbol(std::string &name) -> std::string {
    if (auto colonPos = name.find(':'); colonPos != std::string::npos) {
        // 截取冒号前边的部分
        name = name.substr(0, colonPos);
    }
    return name;
}
*/
