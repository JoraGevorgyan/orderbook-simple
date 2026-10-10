#include "../BookManager.hpp"

#include <iostream>
#include <memory>
#include <mutex>

#include "core/OrderBookBasicAI.hpp"

namespace core {
namespace {
// Guards start()/shutdown() against concurrent lifecycle changes.
std::mutex lifecycle_mutex;
} // namespace

BookManager& BookManager::instance()
{
	static BookManager inst; // thread-safe since C++11
	return inst;
}

BookManager::~BookManager()
{
	shutdown();
}

void BookManager::start()
{
	instance().launch();
}

void BookManager::stop()
{
	instance().shutdown();
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

bool BookManager::enqueue(const command_t& cmd)
{
	return _queue.push(cmd);
}

std::unique_ptr<OrderBookBase> BookManager::make_book()
{
	return std::make_unique<OrderBookBasicAI>();
}

void BookManager::launch()
{
	const std::lock_guard lock(lifecycle_mutex);
	if (_worker.joinable()) {
		return;
	}
	_worker = std::thread([this] { run_worker(); });
	std::cout << "book manager started" << std::endl;
}

void BookManager::shutdown()
{
	const std::lock_guard lock(lifecycle_mutex);
	_queue.close();
	if (_worker.joinable()) {
		_worker.join();
	}
}

void BookManager::run_worker()
{
	while (auto cmd = _queue.pop()) {
		std::visit([this](const auto& c) { handle(c); }, *cmd);
	}
}

void BookManager::handle(const ob::request::NewOrder& order)
{
	const std::string symbol(order.symbol.data(), order.symbol.size());
	auto& book = _books[symbol];
	if (!book) {
		book = make_book();
	}
	book->new_order(order);
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
