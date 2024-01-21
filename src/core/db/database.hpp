#pragma once

#include <borealis.hpp>
#include <borealis/core/singleton.hpp>

#include "util/sqlite.hpp"
#include "core/types.hpp"

namespace core {
class Database : public brls::Singleton<Database> {
public:
  enum class SortOrder {
    Asc,
    Desc,
    Random
  };
public:
  Database();
  ~Database();

  std::vector<mediadata::CellData> getAlbumCells(SortOrder order, unsigned int limit = 0);
  mediadata::Album getAlbumData(unsigned int album_id);

  std::vector<mediadata::CellData> getMusicCells(unsigned int album_id, SortOrder order, unsigned int limit = 0);
  std::vector<mediadata::Music> getMusicData(unsigned int album_id, SortOrder order, unsigned int limit = 0);

  // Sources
  std::vector<mediadata::Source> getSources(void);
  // mediadata::Source getSourceByID(const int source_id);
  bool insertSource(const std::string& path);
  // bool deleteSource(const int source_id);

  // Music
  // mediadata::Music getMusicByID(const int music_id);
  bool insertMusic(const mediadata::Music& data);
  // bool updateMusic(const mediadata::Music& data, const int music_id);

private:
  bool migrate(void);
  int getVersion(void);
 
  bool insertAlbum(const std::string& title, const std::string& cover_path);
  bool insertGenre(const std::string& title);
  bool insertArtist(const std::string& artist);

  int insertGetAlbum(const std::string& title, const std::string& cover_path);
  int insertGetGenre(const std::string& name);
  int insertGetArtist(const std::string& name);

  bool getSourceID(const std::string& path, int& out);
  bool getAlbumID(const std::string& title, int& out);
  bool getGenreID(const std::string& name, int& out);
  bool getArtistID(const std::string& name, int& out);
  bool getMusicID(const std::string& title, const std::string& album, int& out);
private:
  util::SQLite* db;
};
}
