#include <concepts>
#include <iostream>
#include "module1/greet.hpp"

int main(int argc, char** argv)
{
	try {
		static_assert(std::same_as<decltype(42), int>);
		const std::string name = (argc > 1) ? argv[1] : "World";
		auto message = module1::greet(name);
		std::cout << message << std::endl;
		std::cout << "C++20 check: passed" << std::endl;
	}
	catch (...) {
		std::cerr << "unkhandled exception" << std::endl;
	}
	return 0;
}
