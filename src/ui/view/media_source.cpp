#include "ui/view/media_source.hpp"

#include "ui/view/media_cell.hpp"
#include "ui/view/audio_player.hpp"
#include "ui/view/media_browser.hpp"
#include "ui/view/album_browser.hpp"
#include "ui/view/video_player.hpp"

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
    AudioPlayer* player = new AudioPlayer();
    recycler->present(player);
  }
  else if (item.type == MediaCellType::BROWSER_CELL) {
    if (item.title == "Songs") {
      MusicBrowser* browser = new MusicBrowser();
      recycler->present(browser);
    }
    else {
      FileBrowser* browser = new FileBrowser("/");
      recycler->present(browser);
    }
  }
  else if (item.type == MediaCellType::ALBUM_CELL) {
    AlbumBrowser* browser = new AlbumBrowser();
    recycler->present(browser);
  }
  else if (item.type == MediaCellType::VIDEO_FILE_CELL) {
    VideoPlayer* player = new VideoPlayer("/sdr/Files/Media/Media - Sync/Music/@Videogames/@Arrangements/ShibayanRecords/TOHO BOSSA NOVA 8 [例大祭16]/09. [坂上なち] ふわふわどれみー.opus");
    brls::sync([player]() { brls::Application::giveFocus(player); });
  }
}

void MediaCellSource::clearData() {}

}
