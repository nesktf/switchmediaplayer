#pragma once

#include "ui/view/recycling_grid.hpp"

namespace view {

enum class MediaCellType {
  NONE,
  AUDIO_FILE_CELL,
  VIDEO_FILE_CELL,
  ALBUM_CELL,
  BROWSER_CELL
};

struct MediaCellData {
  std::string title;
  std::string subtitle;
  std::string image_path; 
  std::string path;
  MediaCellType type;
};

class MediaCellSource : public RecyclingGridDataSource {
public:
  MediaCellSource() {};
  size_t getItemCount() override { return data.size(); };
  RecyclingGridItem* cellForRow(RecyclingView* recycler, size_t index) override;
  void onItemSelected(brls::View* recycler, size_t index) override;
  void clearData() override;
  std::vector<MediaCellData> data;
};

}
