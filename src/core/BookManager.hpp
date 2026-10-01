#pragma once

#include <thread>
#include <variant>

#include "common/AsyncQueue.hpp"
#include "common/request.hpp"

namespace core {

// Singleton that owns an async command queue and a worker thread.
// Producers (e.g. listeners) enqueue commands through the static API and never
// need the instance; the worker thread consumes and applies them in order.
class BookManager {
public:
	using command_t = std::variant<
		ob::request::NewOrder,
		ob::request::CancelOrder,
		ob::request::Flush>;

	// Creates the instance (if needed) and starts the worker. Idempotent.
	static void start();
	// Stops accepting commands, drains the queue and joins the worker.
	static void stop();

	// Thread-safe, non-blocking. Return false if the manager has been stopped.
	static bool new_order(const ob::request::NewOrder& order);
	static bool cancel_order(const ob::request::CancelOrder& order);
	static bool flush();

	BookManager(const BookManager&) = delete;
	BookManager& operator=(const BookManager&) = delete;
	~BookManager();

private:
	BookManager() = default;

	static BookManager& instance();

	bool enqueue(const command_t& cmd);
	void run_worker();
	void launch();
	void shutdown();

	// Command handlers, executed on the worker thread only.
	void handle(const ob::request::NewOrder& order);
	void handle(const ob::request::CancelOrder& order);
	void handle(const ob::request::Flush& flush);

	ob::AsyncQueue<command_t> _queue;
	std::thread _worker;
};

} // namespace core
