#pragma once

#include <string>
#include <filesystem>

namespace util {
namespace fs {
bool fileExists(const std::string& path);
void copyFile(const std::string& src, const std::string& dst);
bool isFileHidden(const std::filesystem::path& path);
bool isRootFolder(const std::filesystem::path& path);
}
}
