#include "../Listener.hpp"

#include <algorithm>
#include <exception>
#include <iostream>

#include "executor/Executor.hpp"
#include "parser/Parser.hpp"

namespace listener {

void Listener::start()
{
	std::cout << "listener started" << std::endl;
	const auto msg = "a message from listener to parser";
	parser::greet(msg);
}

} // namespace listener
