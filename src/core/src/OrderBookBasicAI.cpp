#include "../OrderBookBasicAI.hpp"

#include <algorithm>
#include <iostream>

namespace core {
namespace {

template <typename Map>
bool erase_from(Map& levels, ob::price_t price, ob::id_t u_id, ob::id_t o_id)
{
	const auto level_it = levels.find(price);
	if (level_it == levels.end()) {
		return false;
	}
	auto& level = level_it->second;
	const auto it = std::ranges::find_if(
		level, [&](const auto& r) { return r.u_id == u_id && r.o_id == o_id; });
	if (it == level.end()) {
		return false;
	}
	level.erase(it);
	if (level.empty()) {
		levels.erase(level_it);
	}
	return true;
}

} // namespace

uint64_t OrderBookBasicAI::key(ob::id_t u_id, ob::id_t o_id)
{
	return (static_cast<uint64_t>(u_id) << 32U) | o_id;
}

void OrderBookBasicAI::new_order(const ob::request::NewOrder& order)
{
	std::cout << "A, " << order.u_id << ", " << order.o_id << '\n';

	const bool is_buy = order.side == ob::Side::BUY;
	const ob::quantity_t left =
		is_buy ? match(order, _asks) : match(order, _bids);

	const bool is_market = order.price == 0;
	if (left > 0 && !is_market) {
		rest(order, left);
	}
}

template <typename Opposite>
ob::quantity_t OrderBookBasicAI::match(
	const ob::request::NewOrder& order, Opposite& opposite)
{
	const bool is_buy = order.side == ob::Side::BUY;
	const bool is_market = order.price == 0;
	ob::quantity_t left = order.quantity;

	while (left > 0 && !opposite.empty()) {
		auto level_it = opposite.begin();
		const ob::price_t level_price = level_it->first;
		const bool crosses = is_market || (is_buy ? level_price <= order.price
																							: level_price >= order.price);
		if (!crosses) {
			break;
		}

		auto& level = level_it->second;
		while (left > 0 && !level.empty()) {
			auto& maker = level.front();
			const ob::quantity_t traded = std::min(left, maker.quantity);
			if (is_buy) {
				std::cout << "T, " << order.u_id << ", " << order.o_id << ", "
									<< maker.u_id << ", " << maker.o_id << ", " << level_price
									<< ", " << traded << '\n';
			} else {
				std::cout << "T, " << maker.u_id << ", " << maker.o_id << ", "
									<< order.u_id << ", " << order.o_id << ", " << level_price
									<< ", " << traded << '\n';
			}
			left -= traded;
			maker.quantity -= traded;
			if (maker.quantity == 0) {
				_index.erase(key(maker.u_id, maker.o_id));
				level.pop_front();
			}
		}
		if (level.empty()) {
			opposite.erase(level_it);
		}
	}
	return left;
}

void OrderBookBasicAI::rest(
	const ob::request::NewOrder& order, ob::quantity_t quantity)
{
	const Resting entry{order.u_id, order.o_id, quantity};
	if (order.side == ob::Side::BUY) {
		_bids[order.price].push_back(entry);
	} else {
		_asks[order.price].push_back(entry);
	}
	_index[key(order.u_id, order.o_id)] = Location{order.side, order.price};
}

bool OrderBookBasicAI::cancel_order(const ob::request::CancelOrder& order)
{
	const auto idx = _index.find(key(order.u_id, order.o_id));
	if (idx == _index.end()) {
		return false;
	}
	const Location loc = idx->second;
	const bool erased = loc.side == ob::Side::BUY
												? erase_from(_bids, loc.price, order.u_id, order.o_id)
												: erase_from(_asks, loc.price, order.u_id, order.o_id);
	_index.erase(idx);
	if (erased) {
		std::cout << "C, " << order.u_id << ", " << order.o_id << '\n';
	}
	return erased;
}

void OrderBookBasicAI::flush()
{
	_bids.clear();
	_asks.clear();
	_index.clear();
}

} // namespace core
