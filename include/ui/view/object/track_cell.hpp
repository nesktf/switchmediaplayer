#pragma once

#include "ui/extern/recycling_grid.hpp"

#include "core/types.hpp"

namespace view {
class TrackCell : public RecyclingGridItem {
public:
  TrackCell();
  static TrackCell* create() { return new TrackCell(); }
  BRLS_BIND(brls::Label, index, "track_cell/index_label");
  BRLS_BIND(brls::Label, title, "track_cell/title_label");
  BRLS_BIND(brls::Label, subtitle, "track_cell/subtitle_label");
  BRLS_BIND(brls::Label, length, "track_cell/length_label");
};

class TrackDataSource : public RecyclingGridDataSource {
public:
  size_t getItemCount() override { return data.size(); }
  RecyclingGridItem* cellForRow(RecyclingView* recycler, size_t index) override;
  void onItemSelected(brls::View* recycler, size_t index) override;
  void clearData() override { data.clear(); };

  std::vector<core::mediadata::Music> data;
};

}
