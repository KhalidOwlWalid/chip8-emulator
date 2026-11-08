#pragma once

// Thin wrapper around spdlog.
//
// Usage:
//   logger::init();                       // once, at the start of main()
//   LOG_INFO("loaded ROM {} ({} bytes)", path, size);
//   LOG_TRACE("pc={:#06x} op={:#06x}", pc, op);
//
// Level at runtime: pass one to init(), or set the env var,
//   SPDLOG_LEVEL=trace ./main      (overrides the init() level)
// Compile-time floor: SPDLOG_ACTIVE_LEVEL (set in CMake). Calls below it are
// removed entirely, so trace logs in the hot loop cost nothing in release.

#include <memory>
#include <string>

#include <spdlog/spdlog.h>
#include <spdlog/cfg/env.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace logger {

// Logs go to a coloured console and to `log_file` (overwritten each run).
inline void init(spdlog::level::level_enum level = spdlog::level::info,
                 const std::string& log_file = "chip8.log")
{
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file = std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_file, true);

    auto log = std::make_shared<spdlog::logger>(
        "chip8", spdlog::sinks_init_list{console, file});

    // [time] [level] [file:line] message
    log->set_pattern("[%H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
    log->set_level(level);
    log->flush_on(spdlog::level::warn);

    spdlog::set_default_logger(log);
    spdlog::cfg::load_env_levels();  // SPDLOG_LEVEL env var wins if set
}

}  // namespace logger

// The SPDLOG_* macros capture file and line; plain spdlog::info() does not.
#define LOG_TRACE(...)    SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...)    SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)     SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
