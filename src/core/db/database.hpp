#pragma once

#include <borealis.hpp>
#include <borealis/core/singleton.hpp>

#include "util/sqlite.hpp"
#include "core/types.hpp"

namespace core {
class Database : public brls::Singleton<Database> {
public:
  enum class SortBy {
    Title,
    Album,
    Genre,
    Artist
  };
  enum class SortOrder {
    Asc,
    Desc,
    Random
  };
public:
  Database();
  ~Database();

  bool insertSource(const std::string& path);
  bool insertMusic(const mediadata::Music& data);

  std::vector<mediadata::Source> getSources(void);
  mediadata::Source getSourceByID(const int source_id);

  std::vector<mediadata::Album> getAlbums(SortOrder order, int limit = 10);
  mediadata::Album getAlbumByID(const int album_id);
  std::vector<mediadata::Music> getAlbumMusic(const int album_id, SortBy sort, SortOrder order);

  std::vector<mediadata::Genre> getGenres(SortOrder order, int limit = 10);
  mediadata::Genre getGenreByID(const int genre_id);
  std::vector<mediadata::Music> getGenreMusic(const int genre_id, SortBy sort, SortOrder order);

  std::vector<mediadata::Artist> getArtists(SortOrder order, int limit = 10);
  mediadata::Artist getArtistByID(const int artist_id);
  std::vector<mediadata::Music> getArtistMusic(const int artist_id, SortBy sort, SortOrder order);

  std::vector<mediadata::Music> getAllMusic(SortBy sort, SortOrder order);

  bool validateMusic(const mediadata::Music& data);

  bool updateMusic(const mediadata::Music& data);
  bool updateSource(const mediadata::Source& data);

  bool removeSource(const int source_id);
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
  bool getMusicID(const std::string& title, int& out);
private:
  util::SQLite* db;
};
}
