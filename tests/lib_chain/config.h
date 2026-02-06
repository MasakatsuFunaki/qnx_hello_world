// Mid-level library: config (depends on logger)
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <map>
#include "logger.h"

namespace config {

class Config {
public:
    explicit Config(logger::Logger& log) : log_(log) {}

    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key, const std::string& default_val = "") const;
    bool has(const std::string& key) const;
    std::size_t size() const { return values_.size(); }

private:
    std::map<std::string, std::string> values_;
    logger::Logger& log_;
};

}  // namespace config

#endif
