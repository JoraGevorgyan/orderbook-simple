#include "../Parser.hpp"

namespace parser {

std::string greet(const std::string& name)
{
	static_assert(std::same_as<decltype(name), const std::string&>);
	if (name.empty()) {
		return "it was empty";
	}
	return "Heeeeello, " + name + "!";
}

} // namespace parser
