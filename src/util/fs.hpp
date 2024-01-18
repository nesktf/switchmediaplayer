#pragma once

#include <string>

namespace util {
namespace fs {
bool fileExists(const std::string& path);
void copyFile(const std::string& src, const std::string& dst);
}
}
