#include "module1/greet.hpp"

namespace module1 {

std::string greet(const std::string& name) {
	static_assert(std::same_as<decltype(name), const std::string&>);
	return "Hello, " + name + "!";
}

} // namespace module1
