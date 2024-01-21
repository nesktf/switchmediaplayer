#pragma once

#include "ui/view/recycling_grid.hpp"

#include "core/types.hpp"

namespace view {
class MediaListSource : public RecyclingGridDataSource {
public:
  MediaListSource() {};
  size_t getItemCount() override;
  RecyclingGridItem* cellForRow(RecyclingView* recycler, size_t index) override;
  virtual void onItemSelected(brls::View* recycler, size_t index) override = 0;
  void clearData() override;
  std::vector<core::mediadata::CellData> data;
};

}
