#include "util/fs.hpp"

#include <fstream>

namespace util {
namespace fs {
bool fileExists(const std::string& path) {
  return std::filesystem::exists(path);
}

void copyFile(const std::string& src, const std::string& dst) {
  std::ifstream src_s{src, std::ios::in | std::ios::binary};
  std::ofstream dst_s{dst, std::ios::out | std::ios::binary};
  dst_s << src_s.rdbuf();
  dst_s.flush();
}

bool isFileHidden(const std::filesystem::path& path) {
  std::filesystem::path::string_type p {path.filename()};
  return p[0] == '.';
}

bool isRootFolder(const std::filesystem::path& path) {
  return (path.string() == "/");
}
}
}
