#pragma once
#include <iostream>
struct irisConfig {
	const std::string app;
	const std::string params_a;
	const std::string params_b;
	const std::string params_c;
	const std::string params_d;
	const std::string df;
	const std::string perfmgr_enable;
	const std::string fixed_target_fps;
} __attribute__((aligned(64)));
