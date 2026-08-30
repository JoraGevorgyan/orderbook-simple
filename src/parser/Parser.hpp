#pragma once

#include <string_view>

#include "common/request.hpp"

namespace parser {

ob::request::cmd_t parse_csv(std::string_view data);

} // namespace parser
