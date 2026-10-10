#include <exception>
#include <iostream>

#include "core/BookManager.hpp"
#include "listener/Listener.hpp"
#include "reporter/ReporterBase.hpp"

int main()
{
	try {
		core::BookManager::start();
		reporter::ReporterBase::start();
		listener::start_udp();
		core::BookManager::stop();
		reporter::ReporterBase::stop();
	}
	catch (const std::exception& err) {
		std::cerr << err.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "Unknown exception" << std::endl;
		return 1;
	}
	return 0;
}
