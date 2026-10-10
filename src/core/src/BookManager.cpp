#include "../BookManager.hpp"

#include <memory>
#include <variant>

#include "core/OrderBookBasicAI.hpp"

namespace core {
BookManager::~BookManager()
{
	// Join the worker before _books is destroyed.
	shutdown();
}

bool BookManager::new_order(const ob::request::NewOrder& order)
{
	return instance().enqueue(order);
}

bool BookManager::cancel_order(const ob::request::CancelOrder& order)
{
	return instance().enqueue(order);
}

bool BookManager::flush()
{
	return instance().enqueue(ob::request::Flush{});
}

void BookManager::handle(const book_command_t& cmd)
{
	std::visit([this](const auto& c) { handle(c); }, cmd);
}

void BookManager::handle(const ob::request::NewOrder& order)
{
	if (!_books.contains(order.symbol)) {
		_books[order.symbol] = std::make_unique<OrderBookBasicAI>();
	}
	_books[order.symbol]->new_order(order);
}

void BookManager::handle(const ob::request::CancelOrder& order)
{
	// A cancel carries no symbol; each book resolves it via its own index.
	for (auto& entry : _books) {
		if (entry.second->cancel_order(order)) {
			return;
		}
	}
}

void BookManager::handle(const ob::request::Flush& /*flush*/)
{
	for (auto& entry : _books) {
		entry.second->flush();
	}
	_books.clear();
}

} // namespace core
