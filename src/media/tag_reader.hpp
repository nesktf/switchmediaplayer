#pragma once

#include <borealis.hpp>
#include <filesystem>

#include "core/database.hpp"

namespace media {
core::MusicData parseOpusTags(const std::filesystem::path& path, const std::string& media_source);
core::MusicData parseOggTags(const std::filesystem::path& path, const std::string& media_source);
}
