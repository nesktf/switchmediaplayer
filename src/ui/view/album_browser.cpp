#include "ui/view/album_browser.hpp"

#include "core/db/database.hpp"
#include "ui/view/video_player.hpp"
#include "ui/view/svg_button.hpp"

#include "ui/view/music_player.hpp"

namespace view {
class TrackCell : public RecyclingGridItem {
public:
  TrackCell() {
    inflateFromXMLRes("xml/view/track_cell.xml");
  }
  static TrackCell* create() { return new TrackCell(); }
  BRLS_BIND(brls::Label, index, "index_label");
  BRLS_BIND(brls::Label, title, "title_label");
  BRLS_BIND(brls::Label, subtitle, "subtitle_label");
  BRLS_BIND(brls::Label, length, "length_label");
};

class TrackDataSource : public RecyclingGridDataSource {
public:
  size_t getItemCount() override { return data.size(); }
  RecyclingGridItem* cellForRow(RecyclingView* recycler, size_t index) override {
    TrackCell* cell = dynamic_cast<TrackCell*>(recycler->dequeueReusableCell("cell"));
    auto& item = data[index];
    cell->index->setText(std::to_string(item.track));
    cell->title->setText(item.title);
    cell->subtitle->setText(item.artist);
    cell->length->setText(std::to_string(item.length));
    cell->updateActionHint(brls::BUTTON_A, "Play");
    cell->registerAction("Play next", brls::BUTTON_X, [this, &item](...) {
      return true;
    });
    cell->registerAction("Play at end", brls::BUTTON_Y, [this, &item](...) {
      return true;
    });
    cell->registerAction("Add to playlist", brls::BUTTON_BACK, [this, &item](...) {
      brls::Logger::info("{}", item.title);
      return true;
    });
    return cell;
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    auto& item = data[index];
    auto& player = MusicPlayer::instance();
    player.setCurrent(item);
    recycler->present(new MusicPlayerWrapper());
  }
  void clearData() override { data.clear(); };
  std::vector<core::mediadata::Music> data;
};


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
