#include "ui/view/album_browser.hpp"

#include "core/db/database.hpp"
#include "ui/view/video_player.hpp"

namespace view {
AlbumBrowser::AlbumBrowser(const int album_id) {
  inflateFromXMLRes("xml/view/album_browser.xml");

  auto& db = core::Database::instance();
  this->album_data = db.getAlbumData(album_id);
  this->music_data = db.getMusicData(album_id, core::Database::SortOrder::Desc);

  this->title->setText(album_data.title);
  if (!album_data.artist.empty()) {
    this->artists->setText(album_data.artist);
  } else {
    this->artists->setText("Unknown Artist");
  }
  this->length->setText("Total Length: "+std::to_string(album_data.length));
  this->music_count->setText("Music count: "+std::to_string(album_data.music.size()));
  if (!album_data.cover_path.empty()) {
    this->cover->setImageFromFile(album_data.cover_path);
  }

  for (auto& song : music_data) {
    // TODO: Cell focus, it sometimes segfaults
    brls::DetailCell* cell = new brls::DetailCell();
    cell->title->setText(fmt::format("{} ({})", song.title, song.artist));
    cell->setFocusable(true);
    cell->registerClickAction([&cell, &song](...) {
      VideoPlayer* player = new VideoPlayer(song.path);
      cell->present(player);
      return true;
    });
    content_frame->addView(cell);
  }

}
}
