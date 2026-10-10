#pragma once

#include "common/request.hpp"

namespace core {

// Interface of a single-symbol order book. Implementations are not required
// to be thread-safe; BookManager drives each instance from one thread.
class OrderBookBase {
public:
	OrderBookBase() = default;
	OrderBookBase(const OrderBookBase&) = delete;
	OrderBookBase& operator=(const OrderBookBase&) = delete;
	virtual ~OrderBookBase() = default;

	virtual void new_order(const ob::request::NewOrder& order) = 0;
	// Returns false if the order is not resting in this book.
	virtual bool cancel_order(const ob::request::CancelOrder& order) = 0;
	virtual void flush() = 0;
};

} // namespace core
