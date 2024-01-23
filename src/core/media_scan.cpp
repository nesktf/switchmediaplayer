#include "core/media_scan.hpp"

#include "core/db/database.hpp"

#include <filesystem>

#include <fileref.h>
#include <opusfile.h>
#include <oggfile.h>


core::mediadata::Music parseOpusTags(const std::filesystem::path& path, const int source_id) {
  using namespace core;
  TagLib::FileRef f {path.c_str()};
  auto file = dynamic_cast<TagLib::Ogg::Opus::File*>(f.file());
  auto tag = file->tag();
  auto prop = file->audioProperties();

  mediadata::Music music;
  music.path = path.string();
  music.title = tag->title().to8Bit(true);
  music.album = tag->album().to8Bit(true);
  music.genre = tag->genre().to8Bit(true);
  music.artist = tag->artist().to8Bit(true);
  music.track = tag->track();
  music.date = tag->year();
  music.length = prop->lengthInSeconds();
  music.source_id = source_id;

  auto dir_path = path.parent_path();
  for (auto const& dir_entry : std::filesystem::directory_iterator{dir_path}) {
    if (dir_entry.is_directory())
      continue;

    std::string name = dir_entry.path().filename().string();
    if (name.find("cover.") != std::string::npos) {
      music.cover_path = dir_entry.path();
    }
  }

  return music;
}

namespace core {
namespace media {
const std::vector<std::string> supp_ext = {
  ".opus", ".mp3", ".ogg", ".flac", ".wav",
  ".mp4", ".mkv", ".webm"
};

void scan(void) {
  brls::Logger::info("Scanning");
  auto& db = core::Database::instance();

  for (auto& source : db.getSources()) {
    brls::Logger::debug("Path: {}", source.path);
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{source.path}) {
      if (dir_entry.is_directory())
        continue;

      std::string ext = dir_entry.path().extension();
      if (ext.find("opus") != std::string::npos) {
        brls::Logger::debug("Found opus file");
        auto music = parseOpusTags(dir_entry.path(), source.id);
        db.insertMusic(music);
      }
    }
  }
  brls::Logger::info("Scan ended");
}
}
}
