#pragma once

#include <borealis.hpp>

#include "core/types.hpp"

#include "ui/view/recycling_grid.hpp"

namespace view {
class AlbumBrowser : public brls::Box {
public:
  AlbumBrowser(const int album_id);
private:
  BRLS_BIND(brls::Image, cover, "album_browser/cover");
  BRLS_BIND(brls::Label, title, "album_browser/title");
  BRLS_BIND(brls::Label, artists, "album_browser/artists");
  BRLS_BIND(brls::Label, length, "album_browser/length");
  BRLS_BIND(brls::Label, music_count, "album_browser/music_count");
  BRLS_BIND(RecyclingGrid, content, "album_browser/content");

  core::mediadata::Album album_data;
};
}
