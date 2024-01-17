#pragma once

#include <borealis.hpp>
#include <borealis/core/singleton.hpp>

#include <sqlite3.h>

namespace core {
struct AlbumData {
  std::string title;
  std::string artist;
};

struct MusicData {
  std::string path;
  std::string title;
  std::string artist;
  unsigned int date;
  std::string genre;
  unsigned int track;
  std::string album_name;
  std::string media_source_path;
};

class MediaDB : public brls::Singleton<MediaDB> {
public:
  MediaDB();
  ~MediaDB();
  void insertAlbum(const AlbumData& data);
  void insertMusic(const MusicData& data);
  bool isAlbumRegistered(const std::string& title);
  bool isMusicRegistered(const std::string& path);
  unsigned int getAlbumID(const std::string& title);
  unsigned int getMediaSourceID(const std::string& path);
  std::vector<MusicData> getAlbumContents(const std::string& title);
  std::vector<std::string> getMediaPaths(void);
  void insertMediaPath(const std::string& path);
  std::vector<AlbumData> getTenAlbums(void);
private:
  sqlite3* db;
  static const std::string db_path;
  void dbSelect(const std::string& sql);
};
}
