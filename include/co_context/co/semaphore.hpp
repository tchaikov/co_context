#pragma once

#include <type_traits>
#include "co_context/task_info.hpp"
#include "co_context/utility/as_atomic.hpp"

namespace co_context {

class counting_semaphore final {
  private:
    using task_info = detail::task_info;
    using T = config::semaphore_counting_type;
    static_assert(std::is_integral_v<T>);

    class [[nodiscard("Did you forget to co_await?")]] acquire_awaiter final {
      public:
        explicit acquire_awaiter(counting_semaphore & sem) noexcept : sem(sem) {}

        bool await_ready() noexcept {
            T old_counter =
                sem.counter.fetch_sub(1, std::memory_order_acquire); // seq_cst?
            return old_counter > 0;
        }

        void await_suspend(std::coroutine_handle<> current) noexcept;
        void await_resume() const noexcept {}

      private:
        counting_semaphore &sem;
        acquire_awaiter *next;
        std::coroutine_handle<> handle;
        friend class counting_semaphore;
        friend class io_context;
    };

  public:
    constexpr explicit counting_semaphore(T desired) noexcept
        : counter(desired)
        , awaiting(nullptr)
        , to_resume(nullptr)
        , awaken_task(task_info::task_type::semaphore_release) {
        awaken_task.sem = this;
        as_atomic(awaken_task.update).store(0, std::memory_order_relaxed);
    }

    counting_semaphore(const counting_semaphore &) = delete;

    ~counting_semaphore() noexcept;

    bool try_acquire() noexcept;

    acquire_awaiter acquire() noexcept {
        log::d("semaphore %lx acquiring\n", this);
        return acquire_awaiter{*this};
    }

    void release(T update = 1) noexcept;

  private:
    friend class io_context;
    std::coroutine_handle<> try_release() noexcept;

  private:
    std::atomic<acquire_awaiter *> awaiting;
    acquire_awaiter *to_resume;
    std::atomic<T> counter;

    task_info awaken_task;
};

} // namespace co_context