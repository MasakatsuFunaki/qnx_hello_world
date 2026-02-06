#include "app.h"
#include <iostream>

namespace app {

Application::Application(const std::string& name)
    : name_(name), config_(logger_) {
    logger_.info("Application '" + name_ + "' created");
}

void Application::configure(const std::string& key, const std::string& value) {
    config_.set(key, value);
}

std::string Application::setting(const std::string& key) const {
    return config_.get(key, "<unset>");
}

void Application::run() {
    logger_.info("Application '" + name_ + "' running");
    std::cout << "Application '" << name_ << "' is running\n";
}

}  // namespace app
