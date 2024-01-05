#include "ui/view/media_source.hpp"

#include "ui/activity/media_browser.hpp"
#include "ui/activity/album_browser.hpp"
#include "ui/activity/video_player.hpp"

namespace view {
RecyclingGridItem* MediaCellSource::cellForRow(RecyclingView* recycler, size_t index) {
  MediaCell* cell = dynamic_cast<MediaCell*>(recycler->dequeueReusableCell("cell"));
  auto& item = data[index];
  cell->title->setText(item.title);
  cell->subtitle->setText(item.subtitle);
  if (!item.image_path.empty())
    cell->image->setImageFromRes(item.image_path);
  return cell;
}

void MediaCellSource::onItemSelected(brls::View* recycler, size_t index) {
  auto& item = data[index];
  if (item.type == MediaCellType::AUDIO_FILE_CELL) {
    brls::Application::pushActivity(new activity::AudioPlayer(), brls::TransitionAnimation::NONE);
  }
  else if (item.type == MediaCellType::BROWSER_CELL) {
    brls::Application::pushActivity(new activity::MediaBrowser(), brls::TransitionAnimation::NONE);
  }
  else if (item.type == MediaCellType::ALBUM_CELL) {
    brls::Application::pushActivity(new activity::AlbumBrowser(), brls::TransitionAnimation::NONE);
  }
  else if (item.type == MediaCellType::VIDEO_FILE_CELL) {
    brls::Application::pushActivity(new activity::VideoPlayer(), brls::TransitionAnimation::NONE);
  }
}

void MediaCellSource::clearData() {}

}
