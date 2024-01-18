#include "core/path.hpp"

namespace core {
namespace path {
#if defined(__SWITCH__)
const std::string db_file = "/switch/switchmp/switchmp.db";
const std::string romfs_db_dummy = "romfs:/misc/template.db";
#else
const std::string db_file = "./switchmp.db";
const std::string romfs_db_dummy = "./romfs/misc/template.db";
#endif
}
}
