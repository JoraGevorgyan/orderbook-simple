#include "../Parser.hpp"

#include <iostream>

namespace parser {

void greet(const std::string& msg)
{
	static_assert(std::same_as<decltype(msg), const std::string&>);
	if (msg.empty()) {
		std::cout << "it was empty";
	}
	std::cout << "got msg: " + msg << std::endl;
}

} // namespace parser
