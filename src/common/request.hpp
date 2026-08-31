#pragma once
#include "./types.hpp"

namespace ob {
namespace request {

// N, userId, symbol, price, quantity, side, userOrderId
struct NewOrder {
	id_t u_id;
	str21_t symbol;
	price_t price;
	quantity_t quantity;
	Side side;
	id_t o_id;
};

// C, userId, userOrderId
struct CancelOrder {
	id_t u_id;
	id_t o_id;
};

// F
struct Flush {};

struct Invalid {
	std::string reason;
};

using cmd_t = std::variant<NewOrder, CancelOrder, Flush, Invalid>;

} // namespace request
} // namespace ob
