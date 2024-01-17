#include "core/database.hpp"

namespace core {
#if defined(__SWITCH__)
const std::string MediaDB::db_path = "/switch/switchmp/switchmp.db";
#else
const std::string MediaDB::db_path = "./switchmp.db";
#endif

inline void nullable_bind(sqlite3_stmt* stmt, unsigned int i, const std::string& str) {
  if (!str.empty())
    sqlite3_bind_text(stmt, i, str.c_str(), str.size(), SQLITE_TRANSIENT);
  else
    sqlite3_bind_null(stmt, i);
}

inline void nullable_bind(sqlite3_stmt* stmt, unsigned int i, unsigned int val) {
  if (val != 0)
    sqlite3_bind_int(stmt, i, val);
  else
    sqlite3_bind_null(stmt, i);
}

class Query {
public:
  Query(sqlite3* db, const std::string& sql) {
    int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
    if (rc != SQLITE_OK)
      throw std::invalid_argument(":(");
  };
  void exec(const std::function<void(sqlite3_stmt*,unsigned int)> output_callback) {
    while (sqlite3_step(stmt) != SQLITE_DONE) {
      output_callback(stmt,sqlite3_column_count(stmt));
    }
    sqlite3_finalize(stmt);
  }
  void bind() {}
  template<typename... Args>
  void bind(int num, Args... data) { 
    sqlite3_bind_int(stmt, ++i, num);
    bind(data...);
  }
  template<typename... Args>
  void bind(std::string str, Args... data) {
    sqlite3_bind_text(stmt, ++i, str.c_str(), str.size(), SQLITE_TRANSIENT);
    bind(data...);
  }
public:
  static std::string getString(sqlite3_stmt* stmt, unsigned int i) {
    return reinterpret_cast<const char*>(sqlite3_column_text(stmt, i));
  }
  static int getInt(sqlite3_stmt* stmt, unsigned int i) {
    return sqlite3_column_int(stmt, i);
  }
  static int getDouble(sqlite3_stmt* stmt, unsigned int i) {
    return sqlite3_column_double(stmt, i);
  }
private:
  sqlite3_stmt* stmt;
  unsigned int i = 0;
};

MediaDB::MediaDB() {
  int rc;
  char* err_msg;
#if defined(__SWITCH__)
  rc = sqlite3_open_v2(db_path.c_str(), &db, SQLITE_OPEN_READWRITE, "unix-none");
#else
  rc = sqlite3_open_v2(db_path.c_str(), &db, SQLITE_OPEN_READWRITE, nullptr);
#endif
  if (rc != SQLITE_OK) {
    brls::Logger::debug("Error opening db: {}", rc);
    brls::fatal("");
  }

#if defined(__SWITCH__)
  rc = sqlite3_exec(db, "PRAGMA journal_mode=MEMORY;", nullptr, 0, &err_msg);
  if (rc != SQLITE_OK) {
    brls::Logger::debug(":( {}, {}", err_msg, rc);
    brls::fatal("");
  }

#endif

  std::string init_query = R"(
    CREATE TABLE IF NOT EXISTS [MediaSource] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Path] TEXT NOT NULL
    );
    CREATE TABLE IF NOT EXISTS [Albums] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Title] TEXT NOT NULL,
      [Artist] TEXT
    );
    CREATE TABLE IF NOT EXISTS [Music] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Path] TEXT NOT NULL,
      [Title] TEXT NOT NULL,
      [Artist] TEXT,
      [Date] INTEGER,
      [Genre] TEXT,
      [Track] INTEGER,
      [AlbumID] INTEGER,
      [MediaSourceID] INTEGER NOT NULL,
      FOREIGN KEY (AlbumID) REFERENCES Albums(ID),
      FOREIGN KEY (MediaSourceID) REFERENCES MediaSource(ID)
    );
    CREATE TABLE IF NOT EXISTS [Video] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Path] TEXT NOT NULL,
      [MediaSourceID] INTEGER NOT NULL,
      FOREIGN KEY (MediaSourceID) REFERENCES MediaSource(ID)
    );
    CREATE TABLE IF NOT EXISTS [History] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [Date] INTEGER NOT NULL
    );
    CREATE TABLE IF NOT EXISTS [MusicHistory] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [MusicID] INTEGER NOT NULL,
      [HistoryID] INTEGER NOT NULL,
      FOREIGN KEY (HistoryID) REFERENCES History(ID),
      FOREIGN KEY (MusicID) REFERENCES Music(ID)
    );
    CREATE TABLE IF NOT EXISTS [VideoHistory] (
      [ID] INTEGER NOT NULL PRIMARY KEY,
      [VideoID] INTEGER NOT NULL,
      [HistoryID] INTEGER NOT NULL,
      FOREIGN KEY (HistoryID) REFERENCES History(ID),
      FOREIGN KEY (VideoID) REFERENCES Video(ID)
    );
  )";

  if ((rc = sqlite3_exec(db, init_query.c_str(), nullptr, 0, &err_msg)) != SQLITE_OK) {
    brls::Logger::error("Failed to exec db init query: {}, {}", err_msg, rc);
    sqlite3_free(err_msg);
    brls::fatal("");
  } else {
    brls::Logger::debug("MediaDB inited successfully");
  }

}

MediaDB::~MediaDB() {
  sqlite3_close(db);
}

void MediaDB::insertAlbum(const AlbumData& data) {
  std::string sql = R"(INSERT INTO Albums (Title, Artist) VALUES(?, ?);)";

  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);

  sqlite3_bind_text(stmt, 1, data.title.c_str(), data.title.size(), SQLITE_TRANSIENT);
  nullable_bind(stmt, 2, data.artist);

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

bool MediaDB::isAlbumRegistered(const std::string& title) {
  bool out;
  Query query{db, R"(SELECT COUNT(1) FROM Albums WHERE Title=?;)"};
  query.bind(title);
  query.exec([&out](auto stmt, auto cols) {
    out = (Query::getInt(stmt, 0) > 0);
  });
  return out;
}

unsigned int MediaDB::getAlbumID(const std::string& title) {
  unsigned int out;
  Query query{db, R"(SELECT ID FROM Albums WHERE Title=?;)"};
  query.bind(title);
  query.exec([&out](auto stmt, auto cols) {
    out = Query::getInt(stmt, 0);
  });
  return out;
}

void MediaDB::insertMusic(const MusicData& data) {
  std::string sql = R"(INSERT INTO Music (Path, Title, Artist, Date, Genre, Track, AlbumID, MediaSourceID) VALUES (?,?,?,?,?,?,?,?);)";
  unsigned int album_id = 0;
  unsigned int media_source_id = getMediaSourceID(data.media_source_path);
  if (!data.album_name.empty())
    album_id = getAlbumID(data.album_name);

  sqlite3_stmt* stmt;
  int rc = sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);
  if (rc != SQLITE_OK)
    brls::Logger::debug("Error preparing thing");

  sqlite3_bind_text(stmt, 1, data.path.c_str(), data.path.size(), SQLITE_TRANSIENT);
  sqlite3_bind_text(stmt, 2, data.title.c_str(), data.title.size(), SQLITE_TRANSIENT);
  nullable_bind(stmt, 3, data.artist);
  nullable_bind(stmt, 4, data.date);
  nullable_bind(stmt, 5, data.genre);
  nullable_bind(stmt, 6, data.track);
  nullable_bind(stmt, 7, album_id);
  sqlite3_bind_int(stmt, 8, media_source_id);

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

bool MediaDB::isMusicRegistered(const std::string& path) {
  bool out;
  Query query{db, R"(SELECT COUNT(1) FROM Music WHERE Path=?;)"};
  query.bind(path);
  query.exec([&out](auto stmt, auto cols) {
    out = (Query::getInt(stmt, 0) > 0);
  });
  return out;
}

std::vector<MusicData> MediaDB::getAlbumContents(const std::string& title) {
  std::vector<MusicData> out;
  Query query{db, R"(SELECT M.Path, M.Title, M.Artist, M.Date, M.Genre, M.Track FROM Music AS M INNER JOIN Albums AS A ON A.ID = M.AlbumID WHERE A.Title=? ORDER BY M.Track DESC;)"};
  query.bind(title);
  query.exec([&out](auto stmt, auto cols) {
    out.push_back((MusicData) {
      .path = Query::getString(stmt, 0),
      .title = Query::getString(stmt, 1),
      .artist = Query::getString(stmt, 2)
    });
  });
  return out;
}

std::vector<std::string> MediaDB::getMediaPaths(void) {
  std::vector<std::string> out;
  Query query{db, R"(SELECT Path FROM MediaSource;)"};
  query.exec([&out](auto stmt, auto cols) {
    out.push_back(Query::getString(stmt, 0));
  });
  return out;
}

void MediaDB::insertMediaPath(const std::string& path) {
  std::string sql = R"(INSERT INTO MediaSource (Path) VALUES(?);)";

  sqlite3_stmt* stmt;
  sqlite3_prepare_v2(db, sql.c_str(), sql.size(), &stmt, nullptr);

  sqlite3_bind_text(stmt, 1, path.c_str(), path.size(), SQLITE_TRANSIENT);

  sqlite3_step(stmt);
  sqlite3_finalize(stmt);
}

unsigned int MediaDB::getMediaSourceID(const std::string& path) {
  unsigned int out;
  Query query{db, R"(SELECT ID FROM MediaSource WHERE Path=?;)"};
  query.bind(path);
  query.exec([&out](auto stmt, auto cols) {
    out = Query::getInt(stmt, 0);
  });
  return out;
}

std::vector<AlbumData> MediaDB::getTenAlbums(void) {
  std::vector<AlbumData> out;
  Query query{db, R"(SELECT Title FROM Albums ORDER BY ID DESC LIMIT 10;)"};
  query.exec([&out](auto stmt, auto cols) {
    out.push_back((AlbumData) {
      .title = Query::getString(stmt, 0)
    });
  });
  return out;
}
}
