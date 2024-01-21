#include "core/db/database.hpp"

#include "core/path.hpp"
#include "core/types.hpp"

#include "util/fs.hpp"

#include "core/db/migration/migration.hpp"

using namespace util;

#define DB_VERSION 1

inline void leechFalse(bool& a, const bool b) { a = (a&&b); }

inline std::string orderStr(const std::string& name, core::Database::SortOrder order, unsigned int limit) { 
  std::string end = "ORDER BY ";
  if (order == core::Database::SortOrder::Random) {
    end += "RANDOM() ";
  } else if (order == core::Database::SortOrder::Asc) {
    end += fmt::format("{} ASC ", name);
  } else if (order == core::Database::SortOrder::Desc) {
    end += fmt::format("{} DESC ", name);
  }
  end += fmt::format("LIMIT {}", limit);

  return end;
}

namespace core {
Database::Database() {
  // HOS can only open a non empty database with unix-none vfs
  // https://gbatemp.net/threads/help-compiling-sqlite3-for-switch-and-getting-it-working.558876/
  if (!fs::fileExists(path::db_file)) {
    fs::copyFile(path::romfs_db_dummy, path::db_file);
  }

  this->db = new SQLite(path::db_file);
  {
    SQLite::ScopedRW s {db};
    this->migrate();
  }
  db->open(SQLite::Connection::ReadOnly);
}

Database::~Database() {
  db->close();
  delete this->db;
}

bool Database::migrate(void) {
  brls::Logger::info("[Database] Attemping to migrate database");

  int ver = getVersion();
  if (ver == DB_VERSION) {
    brls::Logger::info("[Database] Database already migrated");
    db->close();
    return true;
  } else if (ver > DB_VERSION) {
    brls::Logger::error("[Database] Database version mismatch");
    db->close();
    return false;
  }

  bool ret;
  // Template database is version 0
  if (db->transaction()) {
    switch(ver) {
      case 0:
        ret = db::migrationV1(db);
        if (!ret) {
          brls::Logger::error("[Database] Error migrating to version 1");
          break;
        }
        brls::Logger::info("[Database] Migrated database to version 1");
    }
  } else {
    brls::Logger::error("[Database] Unable to init migration transaction");
    return false;
  }

  if (!ret) {
    brls::Logger::error("[Database] Rolling back migration changes");
    db->rollback();
    return false;
  }

  db->commit();
  db->close();
  brls::Logger::info("[Database] Successfully migrated to version {}", DB_VERSION);

  return true;
}

bool Database::insertSource(const std::string& path) {
  bool ret = true;
  if (path.empty()) {
    brls::Logger::error("[Database] Can't insert source with empty path");
    return false;
  }
  {
    SQLite::ScopedRW s{db};
    db->transaction();
    leechFalse(ret, db->prepare("INSERT INTO Sources (Path) VALUES (?)"));
    leechFalse(ret, db->bind(1, path));
    leechFalse(ret, db->exec());

    if (!ret) {
      brls::Logger::error("[Database] Error inserting source path: {}, rolling back changes", path);
      db->rollback();
    } else {
      db->commit();
    }
  }
  return ret;
}

bool Database::insertMusic(const mediadata::Music& data) {
  bool ret = true;
  {
    SQLite::ScopedRW s{db};
    db->transaction();

    int artist_id, genre_id, album_id;
    if (!data.artist.empty()) {
      artist_id = insertGetArtist(data.artist);
    } else {
      artist_id = 0;
    }
    
    if (!data.album.empty()) {
      album_id = insertGetAlbum(data.album, data.cover_path);
    } else {
      album_id = 0;
    }

    if (!data.genre.empty()) {
      genre_id = insertGetGenre(data.genre);
    } else {
      genre_id = 0;
    }

    SQLite::Nullable<int> date {data.date, data.date == 0};
    SQLite::Nullable<int> artist {artist_id, artist_id == 0};
    SQLite::Nullable<int> genre {genre_id, genre_id == 0};
    SQLite::Nullable<int> album {album_id, album_id == 0};

    leechFalse(ret, db->prepare("INSERT INTO Music (Path, Title, Length, Track, Date, ArtistID, GenreID, AlbumID, SourceID) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);"));
    leechFalse(ret, db->bind(1, data.path));
    leechFalse(ret, db->bind(2, data.title));
    leechFalse(ret, db->bind(3, data.length));
    leechFalse(ret, db->bind(4, data.track));
    leechFalse(ret, db->bind(5, date));
    leechFalse(ret, db->bind(6, artist));
    leechFalse(ret, db->bind(7, genre));
    leechFalse(ret, db->bind(8, album));
    leechFalse(ret, db->bind(9, data.source_id));
    leechFalse(ret, db->exec());

    if (!ret) {
      brls::Logger::error("[Database] Error inserting music: {}, rolling back changes", data.title);
      db->rollback();
    } else {
      db->commit();
    }
  }
  return ret;
}

bool Database::insertAlbum(const std::string& title, const std::string& cover_path) {
  bool ret = true;

  leechFalse(ret, db->prepare("INSERT INTO Albums (Title, Cover) VALUES (?, ?);"));
  leechFalse(ret, db->bind(1, title));
  leechFalse(ret, db->bind(2, cover_path));
  leechFalse(ret, db->exec());

  if (!ret) {
    brls::Logger::error("[Database] Error inserting album: {}", title);
  }
  return ret;
}

bool Database::insertGenre(const std::string& name) {
  bool ret = true;
  leechFalse(ret, db->prepare("INSERT INTO Genres (Name) VALUES (?);"));
  leechFalse(ret, db->bind(1, name));
  leechFalse(ret, db->exec());

  if (!ret) {
    brls::Logger::error("[Database] Error inserting genre: {}", name);
  }
  return ret;
}

bool Database::insertArtist(const std::string& name) {
  bool ret = true;
  leechFalse(ret, db->prepare("INSERT INTO Artists (Name) VALUES (?);"));
  leechFalse(ret, db->bind(1, name));
  leechFalse(ret, db->exec());

  if (!ret) {
    brls::Logger::error("[Database] Error inserting artist: {}", name);
  }
  return ret;
}

int Database::getVersion(void) {
  int out;
  db->prepareExec("SELECT Value FROM Vars WHERE Name='db_version';");
  db->getInt(0, out);
  return out;
}

bool Database::getAlbumID(const std::string& title, int& out) {
  bool ret = true;
  leechFalse(ret, db->prepare("SELECT ID FROM Albums WHERE Title=?"));
  leechFalse(ret, db->bind(1, title));
  leechFalse(ret, db->exec());

  if (!db->hasRow()) {
    brls::Logger::debug("[Database] No results for album: {}", title);
    return false;
  }

  leechFalse(ret, db->getInt(0, out));

  if (!ret) {
    brls::Logger::error("[Database] Error getting id for album: {}", title);
  }
  return ret;
}

bool Database::getGenreID(const std::string& name, int& out) {
  bool ret = true;
  leechFalse(ret, db->prepare("SELECT ID FROM Genres WHERE Name=?"));
  leechFalse(ret, db->bind(1, name));
  leechFalse(ret, db->exec());

  if (!db->hasRow()) {
    brls::Logger::debug("[Database] No results for genre: {}", name);
    return false;
  }

  leechFalse(ret, db->getInt(0, out));

  if (!ret) {
    brls::Logger::error("[Database] Error getting id for genre: {}", name);
  }
  return ret;
}

bool Database::getArtistID(const std::string& name, int& out) {
  bool ret = true;
  leechFalse(ret, db->prepare("SELECT ID FROM Artists WHERE Name=?"));
  leechFalse(ret, db->bind(1, name));
  leechFalse(ret, db->exec());

  if (!db->hasRow()) {
    brls::Logger::debug("[Database] No results for artist: {}", name);
    return false;
  }

  leechFalse(ret, db->getInt(0, out));

  if (!ret) {
    brls::Logger::error("[Database] Error getting id for artist: {}", name);
  }
  return ret;
}

bool Database::getMusicID(const std::string& title, const std::string& album, int& out) {
  bool ret = true;
  leechFalse(ret, db->prepare("SELECT ID FROM Music WHERE Title=?"));
  leechFalse(ret, db->bind(1, title));
  leechFalse(ret, db->exec());
  
  if (!db->hasRow()) {
    brls::Logger::debug("[Database] No results for music: {}", title);
    return false;
  }

  leechFalse(ret, db->getInt(0, out));

  if (!ret) {
    brls::Logger::error("[Database] Error getting id for music: {}", title);
  }
  return ret;
}

bool Database::getSourceID(const std::string& path, int& out) {
  bool ret = true;
  leechFalse(ret, db->prepare("SELECT ID FROM Sources WHERE Path=?"));
  leechFalse(ret, db->bind(1, path));
  leechFalse(ret, db->exec());

  if (!db->hasRow()) {
    brls::Logger::debug("[Database] No results for path: {}", path);
    return false;
  }

  leechFalse(ret, db->getInt(0, out));

  if (!ret) {
    brls::Logger::error("[Database] Error getting id for source: {}", path);
  }
  return ret;
}

int Database::insertGetAlbum(const std::string& title, const std::string& cover_path) {
  int id = 0;
  if (!getAlbumID(title, id)) {
    insertAlbum(title, cover_path);
    getAlbumID(title, id);
  }
  return id;
}

int Database::insertGetGenre(const std::string& name) {
  int id = 0;
  if (!getGenreID(name, id)) {
    insertGenre(name);
    getGenreID(name, id);
  }
  return id;
}

int Database::insertGetArtist(const std::string& name) {
  int id = 0;
  if (!getArtistID(name, id)) {
    insertArtist(name);
    getArtistID(name, id);
  }
  return id;
}

std::vector<mediadata::CellData> Database::getAlbumCells(SortOrder order, unsigned int limit) {
  std::vector<mediadata::CellData> out;

  db->prepareExec("SELECT ID, Title, Artists, Cover FROM Albums;");
  while (db->hasRow()) {
    mediadata::CellData data;
    db->getInt(0, data.data_id);
    db->getString(1, data.title);
    db->getString(2, data.subtitle);
    db->getString(3, data.image_path);
    out.push_back(data);
    db->nextRow();
  }

  return out;
}

mediadata::Album Database::getAlbumData(unsigned int album_id) {
  mediadata::Album out;
  
  db->prepare("SELECT ID, Title, Artists, Cover FROM Albums WHERE ID=?");
  db->bind(1, album_id);
  db->exec();

  if (db->hasRow()) {
    db->getInt(0, out.id);
    db->getString(1, out.title);
    db->getString(2, out.artist);
    db->getString(3, out.cover_path);
    db->nextRow();
  }

  db->prepare("SELECT ID, Length From Music WHERE AlbumID=? ORDER BY Track ASC");
  db->bind(1, album_id);
  db->exec();

  out.length = 0;
  while (db->hasRow()) {
    int id, len;
    db->getInt(0, id);
    db->getInt(1, len);
    out.music.push_back(id);
    out.length += len;
    db->nextRow();
  }

  return out;
}

std::vector<mediadata::CellData> Database::getMusicCells(unsigned int album_id, SortOrder order, unsigned int limit) {
  std::vector<mediadata::CellData> out;

  db->prepare(R"(
    SELECT Mu.ID, Mu.Title, Ar.Name, Al.Cover
    FROM Music AS Mu
    INNER JOIN Albums AS Al
      ON Al.ID = Mu.AlbumID
    INNER JOIN Artists AS Ar
      ON Ar.ID = Mu.ArtistID
    WHERE Al.ID = ?
    ORDER BY Track ASC;
  )");
  db->bind(1, album_id);
  db->exec();

  while (db->hasRow()) {
    mediadata::CellData data;
    db->getInt(0, data.data_id);
    db->getString(1, data.title);
    db->getString(2, data.subtitle);
    db->getString(3, data.image_path);
    out.push_back(data);
    db->nextRow();
  }

  return out;
}

std::vector<mediadata::Source> Database::getSources(void) {
  std::vector<mediadata::Source> out;

  db->prepareExec("SELECT ID, Path FROM Sources;");
  
  while (db->hasRow()) {
    mediadata::Source data;
    db->getInt(0, data.id);
    db->getString(1, data.path);
    out.push_back(data);
    db->nextRow();
  }

  return out;
}

std::vector<mediadata::Music> Database::getMusicData(unsigned int album_id, SortOrder order, unsigned int limit) {
  std::vector<mediadata::Music> out;
  db->prepare(R"(
    SELECT Mu.ID, Mu.Path, Mu.Title, Mu.Length, Mu.Track, Mu.Date, Ar.Name, Ge.Name, Al.Title
    FROM Music AS Mu
    INNER JOIN Artists AS Ar
      ON Ar.ID = Mu.ArtistID
    INNER JOIN Genres AS Ge
      ON Ge.ID = Mu.GenreID
    INNER JOIN Albums AS Al
      ON Al.ID = Mu.AlbumID
    WHERE Al.ID = ?
    ORDER BY Track ASC;
  )");
  db->bind(1, album_id);
  db->exec();

  brls::Logger::info("Doing thing");
  while (db->hasRow()) {
    mediadata::Music data;
    db->getInt(0, data.id);
    db->getString(1, data.path);
    db->getString(2, data.title);
    db->getInt(3, data.length);
    db->getInt(4, data.track);
    db->getInt(5, data.date);
    db->getString(6, data.artist);
    db->getString(7, data.genre);
    db->getString(8, data.album);
    out.push_back(data);
    db->nextRow();
  }

  brls::Logger::info("Thing done");
  return out;
}

}
