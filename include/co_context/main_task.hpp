#pragma once

#include <coroutine>
#include <exception>
#include <atomic>
#include "task_info.hpp"
#include "co_context/log/log.hpp"

namespace co_context {

class [[nodiscard("Did you forget to co_spawn?")]] main_task {
  public:
    struct promise_type {
        detail::task_info io_info{detail::task_info::task_type::co_spawn};

        inline constexpr std::suspend_always initial_suspend() const noexcept {
            return {};
        }

        inline constexpr std::suspend_never final_suspend() const noexcept {
            return {};
        }

        inline main_task get_return_object() noexcept {
            // printf("main_task started\n");
            auto handle =
                std::coroutine_handle<promise_type>::from_promise(*this);
            io_info.handle = handle;
            log::v("main_task generated\n");
            return main_task{handle};
        }

        inline void unhandled_exception() const {
            std::rethrow_exception(std::current_exception());
        }
    };

    main_task(std::coroutine_handle<promise_type> handle) noexcept
        : handle(handle) {
    }

    main_task(const main_task &) noexcept = default;

    inline detail::task_info *get_io_info_ptr() const noexcept {
        return &handle.promise().io_info;
    }

  private:
    std::coroutine_handle<promise_type> handle;
};

} // namespace co_context
