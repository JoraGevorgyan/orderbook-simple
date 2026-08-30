#pragma once

#include <boost/static_string/static_string.hpp>

namespace ob {

using price_t = uint64_t; // may be a class later
using quantity_t = uint64_t;
using id_t = uint32_t;
using trade_id_t = uint64_t;
using timestamp_t = uint64_t;

inline constexpr uint8_t max_str_len = 21; // suppose we don't need more
using str_t = boost::static_string<max_str_len>;

enum class Side : uint8_t { BUY = 0, SELL };

} // namespace ob
