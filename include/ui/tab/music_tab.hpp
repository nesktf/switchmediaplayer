#pragma once

#include "ui/extern/h_recycling.hpp"
#include "ui/extern/auto_tab_frame.hpp"

namespace tab {
class MusicTab : public view::AttachedView {
public:
  MusicTab();

  static brls::View* create();
private:
  BRLS_BIND(view::HRecyclerFrame, content_frame, "music_tab/content_frame");
  BRLS_BIND(view::HRecyclerFrame, categories_frame, "music_tab/categories_frame");
  BRLS_BIND(view::HRecyclerFrame, recent_frame, "music_tab/recent_frame");
  BRLS_BIND(view::HRecyclerFrame, random_albums_frame, "music_tab/random_albums_frame");
  BRLS_BIND(view::HRecyclerFrame, random_artists_frame, "music_tab/random_artists_frame");
};
}
