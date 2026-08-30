#pragma once

#include <array>
#include <cstdint>
#include <map>
#include <vector>

namespace ob {

using price_t = uint64_t; // may be a class later
using quantity_t = uint64_t;
using order_id_t = uint64_t;
using trade_id_t = uint64_t;
using timestamp_t = uint64_t;

inline constexpr uint8_t max_str_len = 11;
using str_t = std::array<char, max_str_len>;

inline constexpr uint8_t max_cmd_len = 7; // new order expects 7
using cmd_fields_t = std::array<str_t, max_cmd_len>;

enum class Side : uint8_t { BUY = 0, SELL };

} // namespace ob
