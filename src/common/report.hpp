#pragma once

#include <variant>
#include "common/types.hpp"

namespace ob {
namespace report {

struct OrderInfo {
	id_t user_id;
	id_t order_id;
};

enum class AcknowledgmentType : u_int8_t { NEW = 0, CANCEL };

struct Acknowledgement {
	OrderInfo info;
	AcknowledgmentType type;
};

struct MatchedTrades {};

struct BookChange {};

using report_t = std::variant<Acknowledgement, MatchedTrades, BookChange>;

} // namespace report
} // namespace ob
