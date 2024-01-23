#include "ui/view/album_browser.hpp"

#include "ui/view/video_player.hpp"
#include "ui/view/music_player.hpp"
#include "ui/view/object/svg_button.hpp"
#include "ui/view/object/track_cell.hpp"

#include "core/db/database.hpp"

namespace view {

AlbumBrowser::AlbumBrowser(const int album_id) {
  inflateFromXMLRes("xml/view/album_browser.xml");

  this->content->registerCell("cell", TrackCell::create);

  auto* play_album_but = new SVGButton();
  auto* add_album_but = new SVGButton();
  auto* add_album_next_but = new SVGButton();
  this->button_container->addView(play_album_but);
  this->button_container->addView(add_album_but);
  this->button_container->addView(add_album_next_but);

  auto& db = core::Database::instance();
  this->album_data = db.getAlbumData(album_id);
  TrackDataSource* source = new TrackDataSource();
  source->data = db.getMusicData(album_id, core::Database::SortOrder::Desc);
  content->setDataSource(source);

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


}
}
