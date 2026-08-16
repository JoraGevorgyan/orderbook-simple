#include <iostream>
#include "module1/greet.hpp"
#include <fmt/core.h>

int main(int argc, char** argv) {
  std::string name = (argc > 1) ? argv[1] : "World";
  auto message = module1::greet(name);
  std::cout << message << std::endl;
  fmt::print("(fmt) Program finished.\n");
  return 0;
}
