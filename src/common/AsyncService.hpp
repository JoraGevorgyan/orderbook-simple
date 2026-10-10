#pragma once

#include <mutex>
#include <thread>

#include "common/AsyncQueue.hpp"

namespace ob {

// Reusable CRTP base for singleton services that consume commands on a
// dedicated worker thread.
//
// It owns the singleton instance, the command queue and the worker lifecycle.
// `Derived` must:
//   - have a private default constructor and befriend AsyncService<Derived,
//   Command>,
//   - provide `void handle(const Command&)` (called on the worker thread only),
//   - call `shutdown()` from its destructor, so the worker is joined before the
//     derived members it uses are destroyed.
// Producers enqueue through static methods of `Derived` built on `instance()`.
template <typename Derived, typename Command>
class AsyncService {
public:
	AsyncService(const AsyncService&) = delete;
	AsyncService& operator=(const AsyncService&) = delete;

	// Creates the instance (if needed) and starts the worker. Idempotent.
	static void start() { instance().launch(); }

	// Stops accepting commands, drains the queue and joins the worker.
	static void stop() { instance().shutdown(); }

protected:
	AsyncService() = default;
	~AsyncService() = default;

	static Derived& instance()
	{
		static Derived inst; // thread-safe since C++11
		return inst;
	}

	// Thread-safe, non-blocking. Returns false if the service has been stopped.
	bool enqueue(const Command& cmd) { return _queue.push(cmd); }

	void shutdown()
	{
		const std::lock_guard lock(_lifecycle_mutex);
		_queue.close();
		if (_worker.joinable()) {
			_worker.join();
		}
	}

private:
	void launch()
	{
		const std::lock_guard lock(_lifecycle_mutex);
		if (_worker.joinable()) {
			return;
		}
		_worker = std::thread([this] { run_worker(); });
	}

	void run_worker()
	{
		while (auto cmd = _queue.pop()) {
			static_cast<Derived*>(this)->handle(*cmd);
		}
	}

	AsyncQueue<Command> _queue;
	std::thread _worker;
	std::mutex _lifecycle_mutex;
};

} // namespace ob
