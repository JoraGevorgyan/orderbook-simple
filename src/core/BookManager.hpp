#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

#include "common/AsyncService.hpp"
#include "common/request.hpp"
#include "core/OrderBookBase.hpp"

namespace core {

using book_command_t = std::
	variant<ob::request::NewOrder, ob::request::CancelOrder, ob::request::Flush>;

// Singleton service (start/stop/queue/worker come from ob::AsyncService) that
// routes order commands to a per-symbol order book.
// Producers (e.g. listeners) use the static API and never need the instance.
class BookManager : public ob::AsyncService<BookManager, book_command_t> {
public:
	// Thread-safe, non-blocking. Return false if the manager has been stopped.
	static bool new_order(const ob::request::NewOrder& order);
	static bool cancel_order(const ob::request::CancelOrder& order);
	static bool flush();

	~BookManager();

private:
	friend class ob::AsyncService<BookManager, book_command_t>;

	BookManager() = default;

	// Command handlers, executed on the worker thread only.
	void handle(const book_command_t& cmd);
	void handle(const ob::request::NewOrder& order);
	void handle(const ob::request::CancelOrder& order);
	void handle(const ob::request::Flush& flush);

	// Touched by the worker thread only.
	std::unordered_map<ob::str21_t, std::unique_ptr<OrderBookBase>> _books;
};

} // namespace core
