#include "logger.h"

namespace logger {

const char* Logger::level_name(Level l) {
    switch (l) {
        case Level::DEBUG: return "DEBUG";
        case Level::INFO:  return "INFO";
        case Level::WARN:  return "WARN";
        case Level::ERROR: return "ERROR";
    }
    return "UNKNOWN";
}

void Logger::log(Level level, const std::string& msg) {
    entries_.push_back({level, msg});
}

}  // namespace logger
