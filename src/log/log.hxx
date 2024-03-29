#ifndef JARALYN_LOG_HXX
#define JARALYN_LOG_HXX

#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "memory_sink.hxx"

enum class LogLevel {
    Debug = spdlog::level::debug,
    Info = spdlog::level::info,
    Warn = spdlog::level::warn,
    Error = spdlog::level::err,
};

struct LogEntry {
    LogLevel level;
    spdlog::log_clock::time_point time_point;
    std::string time_point_formatted;
    std::string message;
};

class Log {
    friend class LogXray;
    friend class MemorySink;
    friend class Game;
    using LogPtr = std::unique_ptr<spdlog::logger>;
    using LogStore = std::deque<LogEntry>;
public:
    template<typename... Args>
    static inline void debug(Args&&... args) {
        log_->debug(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void info(Args&&... args) {
        log_->info(std::forward<Args>(args)...);
    }
    template<typename... Args>
    static inline void warn(Args&&... args) {
        log_->warn(std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void error(Args&&... args) {
        log_->error(std::forward<Args>(args)...);
    }

    /**
     * @brief Sets the current log level. Existing logs will be unaffected.
     */
    static void set_level(LogLevel level);
private:
    static void init();

    /**
     * @brief Library logger this class wraps.
     */
    static inline LogPtr log_;

    /**
     * @brief Stores logs in a FIFO-like container with random access.
     *
     * The store is currently implemented as a deque.
     */
    static inline LogStore logs_;

    /**
     * @brief Maximum amount of log entries before evicting the next front entry.
     */
    static inline u16 max_entries_ = 1000U;

    /**
     * @brief The current log level.
     */
    static inline LogLevel level_ = LogLevel::Debug;

    /**
     * @brief Clears and resizes the log store to the given size.
     *
     * Accessed from the LogXray class.
     */
    static void set_capacity(u16 new_capacity);
};

#endif
