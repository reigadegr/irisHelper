#include <iostream>
template <typename... Args> static void LOG(const Args &...args)
{
	const auto printArg = [&](const auto &arg) { std::cout << arg; };
	(printArg(args), ...);
	std::cout << std::endl;
}