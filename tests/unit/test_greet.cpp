#include <gtest/gtest.h>
#include "module1/greet.hpp"

TEST(GreetTest, ReturnsGreeting) {
  EXPECT_EQ(module1::greet("Alice"), "Hello, Alice!");
}
