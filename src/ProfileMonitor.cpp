//
// Created by lixiaoqing on 2022/7/12.
//
#include <errno.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <mutex>
#include "include/irisConfig.h"
#include "include/LOG.h"
static std::mutex confMutex;

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))
auto readProfile(const char *profile, std::vector<irisConfig> &conf) -> bool;
auto printCurrentTime() -> std::string;
auto profileMonitor(const char *dic, const char *profile,
		    std::vector<irisConfig> &conf) -> int
{
	pthread_setname_np(pthread_self(), "profileMonitor");
	// inotify 初始化
	int fd = inotify_init();
	int wd = inotify_add_watch(fd, dic, IN_MODIFY | IN_CREATE | IN_DELETE);
	// 监听指定目录下的修改、创建、删除事件

	// 创建一个 epoll 句柄
	int epfd = epoll_create(256);
	struct epoll_event ev;
	ev.data.fd = fd; // 设置要处理的事件相关的文件描述符
	ev.events = EPOLLIN | EPOLLET; // 设置要处理的事件类型

	// 注册 epoll 事件
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

	// 循环监听事件
	char buffer[BUF_LEN];
	struct epoll_event events[20]; // 存储从内核得到的事件集合
	while (true) {
		// 等待事件发生。返回需要处理的事件数目
		int nfds = epoll_wait(epfd, events, 20, 500);
		for (int i = 0; i < nfds; ++i) {
			/**
             * epoll_wait 会一直阻塞直到下面2种情况：
             *   1. 一个文件描述符触发了事件。
             *   2. 被一个信号处理函数打断，或者 timeout 超时。
             * 所以下面需要对 fd 进行过滤，判断是否是我们需要的 fd 产生了事件
             */
			if (events[i].data.fd != fd) {
				continue;
			}
			int length = read(fd, buffer, BUF_LEN);
			if (length < 0) {
				perror("read");
			}
			int pos = 0;
			while (pos < length) {
				struct inotify_event *event =
					(struct inotify_event *)&buffer[pos];
				if (event->len) {
					/////////
					if (event->mask & IN_MODIFY) {
						/*
						if (event->mask & IN_ISDIR) {
							printf("The directory %s was modified.\n",
							       event->name);
						} else {
							printf("The file %s was modified.\n",
							       event->name);
						}
						*/
						LOG("时间: ",
						    printCurrentTime());
						printf("文件:%s 被修改辣!\n",
						       event->name);
						std::lock_guard<std::mutex> lock(
							confMutex);
						readProfile(profile, conf);
					}
					//////
				}
				pos += EVENT_SIZE + event->len;
			}
		}
	}

	inotify_rm_watch(fd, wd);
	close(epfd);
	close(fd);
	return 0;
}
/*
void profileMonitorThread(const char *dic, const char *profile,
			  std::vector<irisConfig> &conf)
{
	std::thread pfmt(profileMonitor, dic, profile, std::ref(conf));
	pfmt.join();
}
*/