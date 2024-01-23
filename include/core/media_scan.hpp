#pragma once

#include <borealis.hpp>

namespace core {
namespace media {
extern const std::vector<std::string> supp_ext;

void registerPath(const std::string& path);
void scan(void);
}
}
