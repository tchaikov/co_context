#pragma once

// #include <new>
#include <cstddef>
#include <cstdint>
#include "uring/io_uring.h"

namespace co_context {

namespace config {

    inline constexpr bool using_hyper_threading = true;
// #define USE_CPU_AFFINITY
#ifdef USE_CPU_AFFINITY
    inline constexpr bool use_CPU_affinity = true;
#else
    inline constexpr bool use_CPU_affinity = false;
#endif

#if __cpp_lib_hardware_interference_size >= 201603
    inline constexpr size_t cache_line_size =
        std::hardware_destructive_interference_size;
#else
    inline constexpr size_t cache_line_size = 64;
    // inline constexpr size_t cache_line_size = 128;
#endif

    // About io_context
    // inline constexpr unsigned io_uring_flags = 0;

    inline constexpr unsigned io_uring_flags = IORING_SETUP_SQPOLL;

    inline constexpr bool is_SQPOLL = io_uring_flags & IORING_SETUP_SQPOLL;

    using threads_number_width_t = uint16_t;
    using tid_t = threads_number_width_t;

    // inline constexpr unsigned total_threads_number = 11;

    // inline constexpr unsigned total_threads_number = 6;

    // inline constexpr unsigned total_threads_number = 5;

    // inline constexpr unsigned total_threads_number = 4;

    // inline constexpr unsigned total_threads_number = 3;

    inline constexpr unsigned total_threads_number = 3;

    inline constexpr unsigned worker_threads_number =
        total_threads_number - 1 - is_SQPOLL;

    inline constexpr bool low_latency_mode = true;

    // swap zone
    using swap_capacity_width_t = uint16_t;

    // swap_capacity should be (N * 8)

    // inline constexpr uint16_t swap_capacity = 32768;

    // inline constexpr uint16_t swap_capacity = 16384;

    // inline constexpr uint16_t swap_capacity = 8192;

    // inline constexpr uint16_t swap_capacity = 4096;

    // inline constexpr uint16_t swap_capacity = 2048;

    // inline constexpr uint16_t swap_capacity = 512;

    // inline constexpr uint16_t swap_capacity = 256;

    // inline constexpr uint16_t swap_capacity = 128;

    // inline constexpr uint16_t swap_capacity = 64;

    // inline constexpr uint16_t swap_capacity = 32;

    // inline constexpr uint16_t swap_capacity = 16;

    inline constexpr uint16_t swap_capacity = 8;

    inline constexpr uint8_t submit_poll_rounds = 1;

    inline constexpr uint8_t reap_poll_rounds = 1;

    // net configuration
    inline constexpr bool loopback_only = true;

    // co configuration
    using semaphore_counting_t = std::ptrdiff_t;
    using condition_variable_counting_t = std::uintptr_t;

    // eager_io configuration
    using eager_io_state_t = uint8_t;

} // namespace config

// logging config
namespace config {
    enum class level : uint8_t {
        v, // verbose
        d, // debug
        i, // info
        w, // warning
        e, // error
        no_log
    };

    // inline constexpr level log_level = level::v;
    // inline constexpr level log_level = level::d;
    // inline constexpr level log_level = level::i;
    // inline constexpr level log_level = level::w;
    // inline constexpr level log_level = level::e;
    inline constexpr level log_level = level::no_log;

} // namespace config

} // namespace co_context