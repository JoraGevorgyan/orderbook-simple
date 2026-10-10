#pragma once

#include <cstdint>
#include <deque>
#include <functional>
#include <map>
#include <unordered_map>

#include "core/OrderBookBase.hpp"

namespace core {

class OrderBookBasicAI final : public OrderBookBase {
public:
	void new_order(const ob::request::NewOrder& order) override;
	// Returns false if the order is not resting in this book.
	bool cancel_order(const ob::request::CancelOrder& order) override;
	void flush() override;

private:
	struct Resting {
		ob::id_t u_id;
		ob::id_t o_id;
		ob::quantity_t quantity;
	};

	using level_t = std::deque<Resting>;
	using bids_t = std::map<ob::price_t, level_t, std::greater<>>;
	using asks_t = std::map<ob::price_t, level_t, std::less<>>;

	struct Location {
		ob::Side side;
		ob::price_t price;
	};

	static uint64_t key(ob::id_t u_id, ob::id_t o_id);

	// Matches against `opposite`, returns the unfilled quantity.
	template <typename Opposite>
	ob::quantity_t match(const ob::request::NewOrder& order, Opposite& opposite);

	void rest(const ob::request::NewOrder& order, ob::quantity_t quantity);

	bids_t _bids;
	asks_t _asks;
	std::unordered_map<uint64_t, Location> _index;
};

} // namespace core
