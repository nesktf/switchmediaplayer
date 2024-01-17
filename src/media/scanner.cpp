#include "media/scanner.hpp"

#include "core/database.hpp"
#include "media/tag_reader.hpp"

#include <filesystem>

namespace media {
void registerPath(const std::string& path) {
  auto& db = core::MediaDB::instance();
  db.insertMediaPath(path);
}

void scan(void) {
  auto& db = core::MediaDB::instance();

  for (auto media_source_path : db.getMediaPaths()) {
    brls::Logger::debug("Path: {}", media_source_path);
    for (auto const& dir_entry : std::filesystem::recursive_directory_iterator{media_source_path}) {
      if (dir_entry.is_directory())
        continue;
      std::string ext = dir_entry.path().extension();

      if (ext.find("ogg") != std::string::npos) {
        brls::Logger::debug("Found ogg file");
        auto music = media::parseOggTags(dir_entry.path(), media_source_path);
        if (!db.isAlbumRegistered(music.album_name)) {
          core::AlbumData album;
          album.title = music.album_name;
          db.insertAlbum(album);
        }
        db.insertMusic(music);
      }

      if (ext.find("opus") != std::string::npos) {
        brls::Logger::debug("Found opus file");
        auto music = media::parseOpusTags(dir_entry.path(), media_source_path);
        if (!db.isAlbumRegistered(music.album_name)) {
          core::AlbumData album;
          album.title = music.album_name;
          db.insertAlbum(album);
        }
        db.insertMusic(music);
      }
      // if (std::find(supp_ext.begin(), supp_ext.end(), ext) != supp_ext.end()) {
      //   brls::Logger::debug("FILE: {}", dir_entry.path().string());
      // }
    }
  }
}
}
