#include "util/sqlite.hpp"

#include <borealis.hpp>

void debugLog(const std::string& msg) {
  brls::Logger::debug("[SQLite] {}", msg);
}
void errorLog(const std::string& msg) {
  brls::Logger::error("[SQLite] {}", msg);
}

// Thanks to tallbl0nde !

namespace util {
SQLite::ScopedRW::ScopedRW(SQLite* db) {
  this->db = db;
  this->restore_con = db->connectionType();
  
  if (restore_con != Connection::None) {
    db->close();
  }
  db->open(Connection::ReadWrite);
}

SQLite::ScopedRW::~ScopedRW() {
  db->close();
  db->open(restore_con);
}

SQLite::SQLite(const std::string& path) : path(path) {
  this->connection_type = Connection::None;
  this->db = nullptr;
  this->query = nullptr;
  this->query_status = QueryStatus::None;
  this->err_msg = "";
  this->transaction_active = false;
  this->bind_counter = 0;
}

SQLite::~SQLite() { close(); }

void SQLite::setErrMsg(const std::string& str) {
  if (str.length() > 0) {
    err_msg = str;
  } else if (db == nullptr) {
    err_msg = "Database not initialized";
  } else {
    int err_code = sqlite3_errcode(db);
    const char* err_str = sqlite3_errmsg(db);
    err_msg = fmt::format("{} ({})", err_str, err_code);
  }
  errorLog(err_msg);
}

bool SQLite::open(Connection type) {
  if (connection_type == Connection::None || type == Connection::None)
    return true;

#if defined(__SWITCH__)
  // HOS can only open a non empty database with unix-none vfs
  // https://gbatemp.net/threads/help-compiling-sqlite3-for-switch-and-getting-it-working.558876/
  const char* vfs_mode = "unix-none";
#else
  const char* vfs_mode = nullptr;
#endif

  connection_type = type;
  int rc;
  if (type == Connection::ReadOnly) {
    rc = sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READONLY, vfs_mode);
    if (rc != SQLITE_OK) {
      setErrMsg();
      connection_type = Connection::None;
      return false;
    }
    debugLog("Opened read-only connection");
  } else if (type == Connection::ReadWrite) {
    rc = sqlite3_open_v2(path.c_str(), &db, SQLITE_OPEN_READWRITE, vfs_mode);
    if (rc != SQLITE_OK) {
      setErrMsg();
      connection_type = Connection::None;
      return false;
    }
    debugLog("Opened read-write connection");
  }

  // General db setup
  sqlite3_extended_result_codes(db, 1);
  bool ret;
  ret = prepare("PRAGMA journal_mode=MEMORY;"); // Another HOS quirk
  if (ret) {
    ret = exec();
  } else {
    setErrMsg("Failed to set journal mode to MEMORY");
    return false;
  }

  ret = prepare("PRAGMA foreign_keys=ON;");
  if (ret) {
    ret = exec();
  } else {
    setErrMsg("Failed to enable foreign keys");
    return false;
  }

  return ret;
}

void SQLite::close(void) {
  // Finalize existing queries
  finalize();

  if (transaction_active) {
    rollback();
  }

  if (connection_type != Connection::None) {
    sqlite3_close(db);
    db = nullptr;
    debugLog("Closed connection");
  }
  connection_type = Connection::None;
}

bool SQLite::prepare(const std::string& query) {
  if (connection_type == Connection::None) {
    setErrMsg("Database not initialized");
    return false;
  }

  // Finalize previous query
  finalize();

  int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &this->query, nullptr);
  if (rc == SQLITE_OK || this->query == nullptr) {
    setErrMsg();
    return false;
  }
  query_status = QueryStatus::Ready;
  return true;
}

bool SQLite::exec(void) {
  if (query_status != QueryStatus::Ready) {
    setErrMsg("Query not prepared");
    return false;
  }

  int rc = sqlite3_step(query);
  if (rc == SQLITE_DONE) {
    query_status = QueryStatus::Finished;
  } else if (rc == SQLITE_ROW) {
    query_status = QueryStatus::Results;
  } else {
    query_status = QueryStatus::Finished;
    setErrMsg();
    return false;
  }

  return true;
}

bool SQLite::prepareExec(const std::string& query) {
  bool ret = prepare(query);
  if (ret)
    ret = exec();
  return ret;
}

void SQLite::finalize(void) {
  if (query_status != QueryStatus::None || query == nullptr) {
    sqlite3_finalize(query);
  }
  query = nullptr;
  query_status = QueryStatus::None;
}

bool SQLite::nextRow(void) {
  if (query_status != QueryStatus::Results) {
    setErrMsg("No rows available");
    return false;
  }

  int rc = sqlite3_step(query);
  if (rc == SQLITE_ROW) {
    return true;
  } else {
    query_status = QueryStatus::Finished;
  }
  
  return false;
}

bool SQLite::transaction(void) {
  if (transaction_active) {
    setErrMsg("Previous transaction is still active");
    return false;
  }

  bool ret = prepareExec("BEGIN TRANSACTION;");
  if (ret) {
    transaction_active = true;
    debugLog("Transaction started");
  } else {
    transaction_active = false;
    setErrMsg("Failed to start transaction");
  }

  return ret;
}

bool SQLite::commit(void) {
  if (!transaction_active) {
    setErrMsg("Transaction not active");
    return false;
  }

  bool ret = prepareExec("COMMIT;");
  if (ret) {
    debugLog("Transaction commited successfully");
  } else {
    setErrMsg("Falied to commit transaction, rolling back");
    rollback();
  }

  transaction_active = false;
  return ret;
}

bool SQLite::rollback(void) {
  if (!transaction_active) {
    setErrMsg("Transaction not active");
    return false;
  }

  bool ret = prepareExec("ROLLBACK;");
  if (ret) {
    debugLog("Transaction rolled back");
  } else {
    setErrMsg("Failed to rollback transaction");
  }

  transaction_active = false;
  return ret;
}

template<typename... Args>
bool SQLite::bind(int num, Args... data) {
  if (query_status != QueryStatus::Ready) {
    setErrMsg("Can't bind an unprepared query");
    return false;
  }

  int ret;
  if (num < 0) {
    sqlite3_bind_null(query, ++bind_counter);
  } else {
    ret = sqlite3_bind_int(query, ++bind_counter, num);
  }
  if (ret != SQLITE_OK) {
    setErrMsg();
    bind_counter = 0;
    return false;
  }

  return bind(data...);
}

template<typename... Args>
bool SQLite::bind(const char* str, Args... data) {
  if (query_status != QueryStatus::Ready) {
    setErrMsg("Can't bind an unprepared query");
    return false;
  }

  int ret = sqlite3_bind_text(query, ++bind_counter, str, strlen(str), SQLITE_TRANSIENT);
  if (ret != SQLITE_OK) {
    setErrMsg();
    bind_counter = 0;
    return false;
  }

  return bind(data...);
}

template<typename... Args>
bool SQLite::bind(Nullable<int> num, Args... data) {
  if (query_status != QueryStatus::Ready) {
    setErrMsg("Can't bind an unprepared query");
    return false;
  }
  
  int ret;
  if (num.null_cond) {
    ret = sqlite3_bind_null(query, ++bind_counter);
  } else {
    ret = sqlite3_bind_int(query, ++bind_counter, num.data);
  }
  if (ret != SQLITE_OK) {
    setErrMsg();
    bind_counter = 0;
    return false;
  }

  return bind(data...);
}

template<typename... Args>
bool SQLite::bind(Nullable<const char*> str, Args... data) {
  if (query_status != QueryStatus::Ready) {
    setErrMsg("Can't bind an unprepared query");
    return false;
  }
  
  int ret;
  if (str.null_cond) {
    ret = sqlite3_bind_null(query, ++bind_counter);
  } else {
    ret = sqlite3_bind_text(query, ++bind_counter, str.data, strlen(str.data), SQLITE_TRANSIENT);
  }
  if (ret != SQLITE_OK) {
    setErrMsg();
    bind_counter = 0;
    return false;
  }

  return bind(data...);
}


// Last binding, should always return true
bool SQLite::bind(void) { bind_counter = 0; return true; }

bool SQLite::getString(unsigned int i, std::string& out) {
  if (query_status != QueryStatus::Results) {
    setErrMsg("Can't get string, no rows available");
    return false;
  }

  out = reinterpret_cast<const char*>(sqlite3_column_text(query, i));
  return true;
}

bool SQLite::getInt(unsigned int i, int& out) {
  if (query_status != QueryStatus::Results) {
    setErrMsg("Can't get int, no rows available");
    return false;
  }

  out = sqlite3_column_int(query, i);
  return true;
}

bool SQLite::getBool(unsigned int i, bool& out) {
  if (query_status != QueryStatus::Results) {
    setErrMsg("Can't get bool, no rows available");
    return false;
  }

  int tmp;
  bool ret = getInt(i, tmp);
  out = (tmp == 1);
  return ret;
}

SQLite::Connection SQLite::connectionType(void) { return connection_type; }

bool SQLite::hasRow(void) { return (query_status == QueryStatus::Results); }
}


