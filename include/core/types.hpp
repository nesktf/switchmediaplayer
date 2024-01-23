#pragma once

#include <string>
#include <vector>

namespace core {
namespace mediadata {
enum class Format {
  Flac, Opus, Wav, Ogg, Mp3,
  Mp4, Mkv, Webm
};

enum class Type {
  Music,
  Video,
  Image
};
// General 
struct ThumbCellData {
  int data_id;
  std::string title;
  std::string subtitle;
  std::string image_path;
};

struct Source {
  std::string path;
  int id;
};

struct Music {
  // Not Null: path, title, length, source_id
  std::string path; // relative to source path
  std::string title;
  std::string album;
  std::string genre;
  std::string artist;
  std::string cover_path;
  int track;
  int date;
  int length;
  int source_id;

  int id;
  int album_id;
  int genre_id;
  int artist_id;
};

struct Album {
  std::string title;
  std::string cover_path;
  std::string artist;
  std::vector<int> music;
  unsigned int length;
  int id;
};
}
}
