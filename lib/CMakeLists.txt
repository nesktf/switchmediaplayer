cmake_minimum_required(VERSION 3.13)

# lunasvg
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
add_subdirectory(lunasvg EXCLUDE_FROM_ALL)

# taglib
add_subdirectory(taglib)
set(TAGLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib)
list(APPEND APP_PLATFORM_INCLUDE ${CMAKE_BINARY_DIR}/lib/taglib)
execute_process(COMMAND find "${TAGLIB_DIR}" -type d -printf "%p;"
  OUTPUT_VARIABLE TAGLIB_INCLUDES
)
list(APPEND APP_PLATFORM_INCLUDE ${TAGLIB_INCLUDES})
set(APP_PLATFORM_INCLUDE ${APP_PLATFORM_INCLUDE} PARENT_SCOPE)

# Borealis
add_subdirectory(borealis/library)
