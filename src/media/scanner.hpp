#pragma once

#include <borealis.hpp>

namespace media {
static const std::vector<std::string> supp_ext = {
  ".opus", ".mp3", ".ogg", ".flac", ".wav",
  ".mp4", ".mkv", ".webm"
};

void registerPath(const std::string& path);
void scan(void);
}
