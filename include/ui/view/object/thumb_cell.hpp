#pragma once

#include "ui/extern/recycling_grid.hpp"

#include "core/types.hpp"

namespace view {
class ThumbCellSource : public view::RecyclingGridDataSource {
public:
  ThumbCellSource() {};
  size_t getItemCount() override;
  void clearData() override;

  view::RecyclingGridItem* cellForRow(view::RecyclingView* recycler, size_t index) override;
  virtual void onItemSelected(brls::View* recycler, size_t index) override = 0;

  std::vector<core::mediadata::ThumbCellData> data;
};

class ThumbCell : public view::RecyclingGridItem {
public:
  ThumbCell();
  static ThumbCell* create();

  BRLS_BIND(brls::Image, image, "thumb_cell/image");
  BRLS_BIND(brls::Label, title, "thumb_cell/title");
  BRLS_BIND(brls::Label, subtitle, "thumb_cell/subtitle");
};
}
