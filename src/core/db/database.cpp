#include "core/db/database.hpp"

#include "core/path.hpp"
#include "core/types.hpp"

#include "util/fs.hpp"

#include "core/db/migration/migration.hpp"

using namespace util;

#define DB_VERSION 1

inline void leechFalse(bool& a, const bool b) { a = !(a&&b); }

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
  {
    SQLite::ScopedRW s{db};
    db->transaction();
    leechFalse(ret, db->prepare("INSERT INTO Sources (Path) VALUES (?)"));
    leechFalse(ret, db->bind(path.c_str()));
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

bool Database::insertMusic(const mediadata::Music& data, const int source_id) {
  bool ret = true;
  {
    SQLite::ScopedRW s{db};
    db->transaction();
    leechFalse(ret, db->prepare("INSERT INTO Music (Path, Title, Date, Track, ArtistID, GenreID, AlbumID, SourceID) VALUES (?, ?, ?, ?, ?, ?, ?, ?);"));
    SQLite::Nullable<int> date {data.date, data.date > 0};
    SQLite::Nullable<int> track {data.track, data.track > 0};
    int artist_id = insertGetArtist(data.artist);
    int genre_id = insertGetGenre(data.genre);
    int album_id = insertGetAlbum(data.album);
    leechFalse(ret, db->bind(data.path.c_str(), data.title.c_str(), date, track, artist_id, genre_id, album_id, source_id));
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

bool Database::insertAlbum(const std::string& title) {
  bool ret = true;
  leechFalse(ret, db->prepare("INSERT INTO Albums (Title) VALUES (?);"));
  leechFalse(ret, db->bind(title.c_str()));
  leechFalse(ret, db->exec());

  if (!ret) {
    brls::Logger::error("[Database] Error inserting album: {}", title);
  }
  return ret;
}

bool Database::insertGenre(const std::string& name) {
  bool ret = true;
  leechFalse(ret, db->prepare("INSERT INTO Genres (Name) VALUES (?);"));
  leechFalse(ret, db->bind(name.c_str()));
  leechFalse(ret, db->exec());

  if (!ret) {
    brls::Logger::error("[Database] Error inserting genre: {}", name);
  }
  return ret;
}

bool Database::insertArtist(const std::string& name) {
  bool ret = true;
  leechFalse(ret, db->prepare("INSERT INTO Artists (Name) VALUES (?);"));
  leechFalse(ret, db->bind(name.c_str()));
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
  leechFalse(ret, db->bind(title.c_str()));
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
  leechFalse(ret, db->bind(name.c_str()));
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
  leechFalse(ret, db->bind(name.c_str()));
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

bool Database::getMusicID(const std::string& title, int& out) {
  bool ret = true;
  leechFalse(ret, db->prepare("SELECT ID FROM Music WHERE Title=?"));
  leechFalse(ret, db->bind(title.c_str()));
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
  leechFalse(ret, db->bind(path.c_str()));
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

int Database::insertGetAlbum(const std::string& title) {
  int id = 0;
  if (!getAlbumID(title, id)) {
    insertAlbum(title);
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


//
//   if ((rc = sqlite3_exec(db, init_query.c_str(), nullptr, 0, &err_msg)) != SQLITE_OK) {
//     brls::Logger::error("Failed to exec db init query: {}, {}", err_msg, rc);
//     sqlite3_free(err_msg);
//     brls::fatal("");
//   } else {
//     brls::Logger::debug("MediaDB inited successfully");
//   }
//
// }
//
// MediaDB::~MediaDB() {
//   sqlite3_close(db);
// }
//
// void MediaDB::insertAlbum(const AlbumData& data) {
//   std::string sql = R"(INSERT INTO Albums (Title, Artist) VALUES(?, ?);)";
//
//   sqlite3_stmt* stmt;
//   sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
//
//   sqlite3_bind_text(stmt, 1, data.title.c_str(), data.title.size(), SQLITE_TRANSIENT);
//   nullable_bind(stmt, 2, data.artist);
//
//   sqlite3_step(stmt);
//   sqlite3_finalize(stmt);
// }
//
// bool MediaDB::isAlbumRegistered(const std::string& title) {
//   bool out;
//   Query query{db, R"(SELECT COUNT(1) FROM Albums WHERE Title=?;)"};
//   query.bind(title);
//   query.exec([&out](auto stmt, auto cols) {
//     out = (Query::getInt(stmt, 0) > 0);
//   });
//   return out;
// }
//
// unsigned int MediaDB::getAlbumID(const std::string& title) {
//   unsigned int out;
//   Query query{db, R"(SELECT ID FROM Albums WHERE Title=?;)"};
//   query.bind(title);
//   query.exec([&out](auto stmt, auto cols) {
//     out = Query::getInt(stmt, 0);
//   });
//   return out;
// }
//
// void MediaDB::insertMusic(const MusicData& data) {
//   std::string sql = R"(INSERT INTO Music (Path, Title, Artist, Date, Genre, Track, AlbumID, MediaSourceID) VALUES (?,?,?,?,?,?,?,?);)";
//   unsigned int album_id = 0;
//   unsigned int media_source_id = getMediaSourceID(data.media_source_path);
//   if (!data.album_name.empty())
//     album_id = getAlbumID(data.album_name);
//
//   sqlite3_stmt* stmt;
//   int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
//   if (rc != SQLITE_OK)
//     brls::Logger::debug("Error preparing thing");
//
//   sqlite3_bind_text(stmt, 1, data.path.c_str(), data.path.size(), SQLITE_TRANSIENT);
//   sqlite3_bind_text(stmt, 2, data.title.c_str(), data.title.size(), SQLITE_TRANSIENT);
//   nullable_bind(stmt, 3, data.artist);
//   nullable_bind(stmt, 4, data.date);
//   nullable_bind(stmt, 5, data.genre);
//   nullable_bind(stmt, 6, data.track);
//   nullable_bind(stmt, 7, album_id);
//   sqlite3_bind_int(stmt, 8, media_source_id);
//
//   sqlite3_step(stmt);
//   sqlite3_finalize(stmt);
// }
//
// bool MediaDB::isMusicRegistered(const std::string& path) {
//   bool out;
//   Query query{db, R"(SELECT COUNT(1) FROM Music WHERE Path=?;)"};
//   query.bind(path);
//   query.exec([&out](auto stmt, auto cols) {
//     out = (Query::getInt(stmt, 0) > 0);
//   });
//   return out;
// }
//
// std::vector<MusicData> MediaDB::getAlbumContents(const std::string& title) {
//   std::vector<MusicData> out;
//   Query query{db, R"(SELECT M.Path, M.Title, M.Artist, M.Date, M.Genre, M.Track FROM Music AS M INNER JOIN Albums AS A ON A.ID = M.AlbumID WHERE A.Title=? ORDER BY M.Track DESC;)"};
//   query.bind(title);
//   query.exec([&out](auto stmt, auto cols) {
//     out.push_back((MusicData) {
//       .path = Query::getString(stmt, 0),
//       .title = Query::getString(stmt, 1),
//       .artist = Query::getString(stmt, 2)
//     });
//   });
//   return out;
// }
//
// std::vector<std::string> MediaDB::getMediaPaths(void) {
//   std::vector<std::string> out;
//   Query query{db, R"(SELECT Path FROM MediaSource;)"};
//   query.exec([&out](auto stmt, auto cols) {
//     out.push_back(Query::getString(stmt, 0));
//   });
//   return out;
// }
//
// void MediaDB::insertMediaPath(const std::string& path) {
//   std::string sql = R"(INSERT INTO MediaSource (Path) VALUES(?);)";
//
//   sqlite3_stmt* stmt;
//   sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
//
//   sqlite3_bind_text(stmt, 1, path.c_str(), path.size(), SQLITE_TRANSIENT);
//
//   sqlite3_step(stmt);
//   sqlite3_finalize(stmt);
// }
//
// unsigned int MediaDB::getMediaSourceID(const std::string& path) {
//   unsigned int out;
//   Query query{db, R"(SELECT ID FROM MediaSource WHERE Path=?;)"};
//   query.bind(path);
//   query.exec([&out](auto stmt, auto cols) {
//     out = Query::getInt(stmt, 0);
//   });
//   return out;
// }
//
// std::vector<AlbumData> MediaDB::getTenAlbums(void) {
//   std::vector<AlbumData> out;
//   Query query{db, R"(SELECT Title FROM Albums ORDER BY ID DESC LIMIT 10;)"};
//   query.exec([&out](auto stmt, auto cols) {
//     out.push_back((AlbumData) {
//       .title = Query::getString(stmt, 0)
//     });
//   });
//   return out;
// }
}
