#pragma once

#include <sqlite3.h>

#include <string>

// Thanks to tallbl0nde !

namespace util {
class SQLite {
public:
  enum class Connection {
    None,
    ReadOnly,
    ReadWrite
  };
  template<typename T>
  struct Nullable {
    T data;
    bool null_cond;
  };
  class ScopedRW {
  public:
    ScopedRW(SQLite* db);
    ~ScopedRW();
  private:
    SQLite* db;
    Connection restore_con;
  };
public:
  SQLite(const std::string& path);
  ~SQLite();
  bool open(Connection type);
  void close(void);

  bool prepare(const std::string& query);
  bool exec(void);
  bool prepareExec(const std::string& query);

  Connection connectionType(void);
  bool hasRow(void);
  bool nextRow(void);

  bool transaction(void);
  bool commit(void);
  bool rollback(void);

  // Bind starts index at 1
  template<typename... Args>
  bool bind(int num, Args... data);
  template<typename... Args>
  bool bind(const char* str, Args... data);
  template<typename... Args>
  bool bind(Nullable<int> num, Args... data);
  template<typename... Args>
  bool bind(Nullable<const char*> str, Args... data);

  // Get starts index at 0
  bool getString(unsigned int i, std::string& out);
  bool getInt(unsigned int i, int& out);
  bool getBool(unsigned int i, bool& out);
private:
  enum class QueryStatus {
    None,
    Ready,
    Results,
    Finished
  };
private:
  void setErrMsg(const std::string& str = "");
  void finalize(void);
  bool bind(void);
private:
  sqlite3* db;
  sqlite3_stmt* query;

  Connection connection_type;
  QueryStatus query_status;

  std::string err_msg;
  std::string path;

  bool transaction_active;
  unsigned int bind_counter;
};
}
