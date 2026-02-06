// Shared library: a string utilities library
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

namespace stringutils {

std::string to_upper(const std::string& s);
std::string to_lower(const std::string& s);
std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);
std::string join(const std::vector<std::string>& parts, const std::string& sep);
bool contains(const std::string& haystack, const std::string& needle);
std::string replace_all(const std::string& s, const std::string& from, const std::string& to);

}  // namespace stringutils

#endif  // STRINGUTILS_H
