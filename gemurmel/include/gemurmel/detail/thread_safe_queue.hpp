//
// Created by Jan Henke on 07.05.26.
//

#ifndef GEMURMEL_THREAD_SAFE_QUEUE_HPP
#define GEMURMEL_THREAD_SAFE_QUEUE_HPP

#include <mutex>
#include <optional>
#include <queue>

namespace gemurmel::detail {

/**
 * A wrapper around std::queue that provides thread-safe access.
 * @tparam T The type of elements stored in the queue
 */
template <typename T>
class ThreadSafeQueue {
	std::queue<T> queue_;
	mutable std::mutex mutex_;

   public:
	/**
	 * Pushes a value into the queue.
	 * @param value The value to push into the queue
	 */
	auto push(T value) -> void {
		std::lock_guard lock(mutex_);
		queue_.push(std::move(value));
	}

	/**
	 * Remove the current front element from the queue.
	 * @return The current front element.
	 */
	auto try_pop() -> std::optional<T> {
		std::lock_guard lock(mutex_);
		if (queue_.empty()) {
			return std::nullopt;
		}
		T element = std::move(queue_.front());
		queue_.pop();
		return element;
	}

	/**
	 * Check if the queue is empty.
	 * @return true if the queue is empty, false otherwise
	 */
	auto empty() const -> bool {
		std::lock_guard lock(mutex_);
		return queue_.empty();
	}
};

}  // namespace gemurmel::detail

#endif  //GEMURMEL_THREAD_SAFE_QUEUE_HPP
