#pragma once

#include "ui/extern/recycling_grid.hpp"

#include "core/types.hpp"

namespace view {
class ThumbCell : public view::RecyclingGridItem {
public:
  ThumbCell();
  static ThumbCell* create();

  BRLS_BIND(brls::Image, image, "thumb_cell/image");
  BRLS_BIND(brls::Label, title, "thumb_cell/title");
  BRLS_BIND(brls::Label, subtitle, "thumb_cell/subtitle");
};

template<typename T>
class ThumbCellSource : public view::RecyclingGridDataSource {
public:
  ThumbCellSource() {}

  view::RecyclingGridItem* cellForRow(view::RecyclingView* recycler, size_t index) override {
    ThumbCell* cell = dynamic_cast<ThumbCell*>(recycler->dequeueReusableCell("cell"));
    core::mediadata::ThumbCellData<T>& item = data[index];
    cell->title->setText(item.title);
    if (!item.subtitle.empty()) {
      cell->subtitle->setText(item.subtitle);
    } else {
      cell->subtitle->setVisibility(brls::Visibility::GONE);
    }
    if (!item.image_path.empty()) {
      cell->image->setImageFromFile(item.image_path);
    }
    return cell;
  }

  size_t getItemCount() override { return data.size(); }

  void clearData() override { data.clear(); }

  virtual void onItemSelected(brls::View* recycler, size_t index) override = 0;

  std::vector<core::mediadata::ThumbCellData<T>> data;
};
}
