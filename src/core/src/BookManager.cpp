#include "../BookManager.hpp"

#include <iostream>
#include <mutex>

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

// TODO: forward to the order book once it exists.
void BookManager::handle(const ob::request::NewOrder& order)
{
	std::cout << "NEW order u=" << order.u_id << " o=" << order.o_id << '\n';
}

void BookManager::handle(const ob::request::CancelOrder& order)
{
	std::cout << "CANCEL order u=" << order.u_id << " o=" << order.o_id << '\n';
}

void BookManager::handle(const ob::request::Flush& /*flush*/)
{
	std::cout << "FLUSH\n";
}

} // namespace core
