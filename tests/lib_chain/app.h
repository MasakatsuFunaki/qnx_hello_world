// Top-level library: app (depends on config, which depends on logger)
#ifndef APP_H
#define APP_H

#include <string>
#include "config.h"
#include "logger.h"

namespace app {

class Application {
public:
    Application(const std::string& name);

    void configure(const std::string& key, const std::string& value);
    std::string setting(const std::string& key) const;
    void run();

    logger::Logger& get_logger() { return logger_; }

private:
    std::string name_;
    logger::Logger logger_;
    config::Config config_;
};

}  // namespace app

#endif
