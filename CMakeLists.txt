cmake_minimum_required(VERSION 3.13)

# Borealis things
set(PROJECT_RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/romfs)
set(BOREALIS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/borealis)
set(BOREALIS_LIBRARY ${BOREALIS_DIR}/library)
include(${BOREALIS_LIBRARY}/cmake/commonOption.cmake)
include(${BOREALIS_LIBRARY}/cmake/toolchain.cmake)

# Project things
project(switchmp)
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_REVISION "1")
set(PROJECT_AUTHOR "nesktf")
set(PROJECT_ICON ${PROJECT_RESOURCES}/img/icon.jpg)
set(APP_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_REVISION}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Add src files for switch build
file(GLOB_RECURSE MAIN_SRC "src/*.cpp" )
if (PLATFORM_SWITCH)
  list(APPEND MAIN_SRC ${BOREALIS_LIBRARY}/lib/platforms/switch/switch_wrapper.c)
  list(APPEND APP_PLATFORM_OPTION -DDEBUG -D_DEBUG -D_GLIBCXX_ASSERTIONS)
endif()

# Compile libs
add_subdirectory(lib)

# MPV stuff
if (PLATFORM_DESKTOP)
  find_package(MPV REQUIRED)
  list(APPEND APP_PLATFORM_INCLUDE ${MPV_INCLUDE_DIR})
  list(APPEND APP_PLATFORM_LIB ${MPV_LIBRARY})
else()
  find_package(PkgConfig REQUIRED)
  pkg_search_module(MPV REQUIRED mpv)
  list(APPEND APP_PLATFORM_INCLUDE ${MPV_INCLUDE_DIR})
  list(APPEND APP_PLATFORM_LIB ${MPV_STATIC_LIBRARIES})
  list(APPEND APP_PLATFORM_LINK_OPTION -L${MPV_LIBRARY_DIRS})
endif()

# Main Target
program_target(${PROJECT_NAME} "${MAIN_SRC}")
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

# Configure nro target
set(BUILD_FONT_DIR ${CMAKE_BINARY_DIR}/romfs/font)
add_custom_target(${PROJECT_NAME}.nro
  DEPENDS ${PROJECT_NAME}
  COMMAND ${NX_NACPTOOL_EXE} --create ${PROJECT_NAME} ${PROJECT_AUTHOR} ${APP_VERSION} ${PROJECT_NAME}.nacp
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_RESOURCES} ${CMAKE_BINARY_DIR}/romfs
  COMMAND ${CMAKE_COMMAND} -E remove -f ${BUILD_FONT_DIR}/switch_font.ttf
  COMMAND ${NX_ELF2NRO_EXE} ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${PROJECT_ICON} --nacp=${PROJECT_NAME}.nacp --romfsdir=${CMAKE_BINARY_DIR}/romfs
  ALL
)

# Include things
target_include_directories(${PROJECT_NAME} PRIVATE src ${APP_PLATFORM_INCLUDE})
target_compile_options(${PROJECT_NAME} PRIVATE -ffunction-sections -fdata-sections -Wunused-variable ${APP_PLATFORM_OPTION})
target_link_libraries(${PROJECT_NAME} PRIVATE borealis lunasvg ${APP_PLATFORM_LIB})
target_link_options(${PROJECT_NAME} PRIVATE ${APP_PLATFORM_LINK_OPTION})
