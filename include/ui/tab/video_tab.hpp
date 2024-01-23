#pragma once

#include "ui/extern/auto_tab_frame.hpp"
#include "ui/extern/h_recycling.hpp"

namespace tab {
class VideoTab : public view::AttachedView {
public:
  VideoTab();

  static brls::View* create();
private:
  BRLS_BIND(view::HRecyclerFrame, content_frame, "video_tab/content_frame");
  BRLS_BIND(view::HRecyclerFrame, recent_frame, "video_tab/recent_frame");
};
}
