#include "media/tag_reader.hpp"

#include <fileref.h>
#include <opusfile.h>
#include <oggfile.h>

#include "core/database.hpp"

namespace media {
core::MusicData parseOpusTags(const std::filesystem::path& path, const std::string& media_source) {
  TagLib::FileRef f {path.c_str()};
  auto file = dynamic_cast<TagLib::Ogg::Opus::File*>(f.file());
  auto tag = file->tag();

  core::MusicData music;
  music.path = path.string();
  music.title = tag->title().to8Bit(true);
  music.artist = tag->artist().to8Bit(true);
  music.date = tag->year();
  music.genre = tag->genre().to8Bit(true);
  music.track = tag->track();
  music.album_name = tag->album().to8Bit(true);
  music.media_source_path = media_source;

  return music;
}

core::MusicData parseOggTags(const std::filesystem::path& path, const std::string& media_source) {
  TagLib::FileRef f{path.c_str()};
  auto file = dynamic_cast<TagLib::Ogg::File*>(f.file());
  auto tag = file->tag();

  core::MusicData music;
  music.path = path.string();
  music.title = tag->title().to8Bit(true);
  music.artist = tag->artist().to8Bit(true);
  music.date = tag->year();
  music.genre = tag->genre().to8Bit(true);
  music.track = tag->track();
  music.album_name = tag->album().to8Bit(true);
  music.media_source_path = media_source;

  return music;
}
}
