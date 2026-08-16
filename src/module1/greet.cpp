#include "module1/greet.hpp"

namespace module1 {

std::string greet(const std::string& name) {
  return "Hello, " + name + "!";
}

} // namespace module1
