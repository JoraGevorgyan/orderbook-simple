#include <gtest/gtest.h>
#include "module1/greet.hpp"

TEST(Integration, GreetContainsName) {
  auto s = module1::greet("Integration");
  EXPECT_NE(s.find("Integration"), std::string::npos);
}
