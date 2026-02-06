// Bottom-level library: logger (no dependencies)
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

namespace logger {

enum class Level { DEBUG, INFO, WARN, ERROR };

struct LogEntry {
    Level level;
    std::string message;
};

class Logger {
public:
    void log(Level level, const std::string& msg);
    void debug(const std::string& msg) { log(Level::DEBUG, msg); }
    void info(const std::string& msg)  { log(Level::INFO, msg); }
    void warn(const std::string& msg)  { log(Level::WARN, msg); }
    void error(const std::string& msg) { log(Level::ERROR, msg); }

    const std::vector<LogEntry>& entries() const { return entries_; }
    std::size_t count() const { return entries_.size(); }
    void clear() { entries_.clear(); }

    static const char* level_name(Level l);
private:
    std::vector<LogEntry> entries_;
};

}  // namespace logger

#endif
