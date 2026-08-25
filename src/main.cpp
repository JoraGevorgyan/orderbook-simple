#include <exception>
#include <iostream>

#include "executor/Executor.hpp"
#include "listener/Listener.hpp"
#include "reporter/Reporter.hpp"

int main()
{
	try {
		executor::Executor::start();
		reporter::Reporter::start();
		listener::Listener::start();
	}
	catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}
