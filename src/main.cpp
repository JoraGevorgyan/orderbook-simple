#include <iostream>
#include <exception>
#include "executor/Executor.hpp"
#include "reporter/Reporter.hpp"
#include "listener/Listener.hpp"

int main() {
	try {
		executor::Executor::start();
		reporter::Reporter::start();
		listener::Listener::start();
	} catch (const std::exception& exception) {
		std::cerr << exception.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}
