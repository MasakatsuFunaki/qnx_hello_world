#include "config.h"

namespace config {

void Config::set(const std::string& key, const std::string& value) {
    log_.info("Config set: " + key + " = " + value);
    values_[key] = value;
}

std::string Config::get(const std::string& key, const std::string& default_val) const {
    auto it = values_.find(key);
    if (it != values_.end()) return it->second;
    return default_val;
}

bool Config::has(const std::string& key) const {
    return values_.count(key) > 0;
}

}  // namespace config
