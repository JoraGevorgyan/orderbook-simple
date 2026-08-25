#include <iostream>
#include "module1/greet.hpp"

int main(int argc, char** argv) {
	std::string name = (argc > 1) ? argv[1] : "World";
	auto message = module1::greet(name);
	std::cout << message << std::endl;
	std::cout << "Program finished." << std::endl;
	return 0;
}
