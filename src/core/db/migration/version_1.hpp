#pragma once

#include "util/sqlite.hpp"

namespace core {
namespace db {
bool migrationV1(util::SQLite* db);
}
}
