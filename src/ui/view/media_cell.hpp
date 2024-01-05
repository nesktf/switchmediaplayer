#pragma once

#include "ui/view/recycling_grid.hpp"

namespace view {
class MediaCell : public RecyclingGridItem {
public:
  MediaCell();
  static MediaCell* create();
  BRLS_BIND(brls::Box, root, "view/media_cell/root");
  BRLS_BIND(brls::Image, image, "view/media_cell/image");
  BRLS_BIND(brls::Label, title, "view/media_cell/title");
  BRLS_BIND(brls::Label, subtitle, "view/media_cell/subtitle");
};
}
