#pragma once

#include <string>

namespace core {
namespace mediadata {
struct Source {
  std::string path;
  int music_count;

  int id;
};

struct Album {
  std::string title;
  std::string cover_path;
  std::string artist_name;
  int track_count;
  int music_count;

  int id;
};

struct Genre {
  std::string title;
  int music_count;

  int id;
};

struct Artist {
  std::string title;
  int music_count;
  int album_count;

  int id;
};

struct Music {
  std::string path;
  std::string title;
  std::string album;
  std::string genre;
  std::string artist;
  std::string cover_path;
  int track;
  int date;
  int length;

  int id;
  int album_id;
  int genre_id;
  int artist_id;
  int source_id;
};

struct Video {
  std::string path;
  std::string title;
};
}
}
