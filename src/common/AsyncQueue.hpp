#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <utility>

namespace ob {

// Unbounded multi-producer / single-consumer blocking queue.
// After close(), push() is rejected and pop() drains the remaining items,
// then returns std::nullopt.
template <typename T>
class AsyncQueue {
public:
	AsyncQueue() = default;
	AsyncQueue(const AsyncQueue&) = delete;
	AsyncQueue& operator=(const AsyncQueue&) = delete;

	bool push(T item)
	{
		{
			std::lock_guard lock(_mutex);
			if (_closed) {
				return false;
			}
			_items.push_back(std::move(item));
		}
		_cv.notify_one();
		return true;
	}

	// Blocks until an item is available or the queue is closed and drained.
	std::optional<T> pop()
	{
		std::unique_lock lock(_mutex);
		_cv.wait(lock, [this] { return _closed || !_items.empty(); });
		if (_items.empty()) {
			return std::nullopt;
		}
		T item = std::move(_items.front());
		_items.pop_front();
		return item;
	}

	void close()
	{
		{
			std::lock_guard lock(_mutex);
			_closed = true;
		}
		_cv.notify_all();
	}

private:
	std::mutex _mutex;
	std::condition_variable _cv;
	std::deque<T> _items;
	bool _closed = false;
};

} // namespace ob
