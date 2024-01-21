#include "ui/tab/audio_tab.hpp"

#include "ui/view/media_source.hpp"
#include "ui/view/media_cell.hpp"
#include "ui/view/album_browser.hpp"

#include "core/db/database.hpp"

namespace tab {
class AudioCategorySource : public view::MediaListSource {
public:
  AudioCategorySource() {
    this->data = {
      {.data_id = 0, .title = "Albums",},
      {.data_id = 1, .title = "Genres"}, 
      {.data_id = 2, .title = "Artists"},
      {.data_id = 3, .title = "Songs"}
    };
  };
  void onItemSelected(brls::View* recycler, size_t index) override {
  }
};

class AlbumListSource : public view::MediaListSource {
public:
  AlbumListSource() {
    auto& db = core::Database::instance();
    data = db.getAlbumCells(core::Database::SortOrder::Desc, 10);
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    view::AlbumBrowser* browser = new view::AlbumBrowser(data[index].data_id);
    recycler->present(browser);
  }
};

Audio::Audio() {
  this->inflateFromXMLRes("xml/tab/audio.xml");
  this->content_frame->registerCell("cell", view::MediaCell::create);
  // this->content_frame->setDataSource(getAlbumCells());

  this->categories_frame->registerCell("cell", view::MediaCell::create);
  this->categories_frame->setDataSource(new AudioCategorySource());

  this->recent_frame->registerCell("cell", view::MediaCell::create);
  // this->recent_frame->setDataSource(getRecentAlbums());

  this->random_albums_frame->registerCell("cell", view::MediaCell::create);
  // this->random_albums_frame->setDataSource(getRandomAlbums());

  this->random_artists_frame->registerCell("cell", view::MediaCell::create);
  // this->random_artists_frame->setDataSource(getRandomArtists());

  // brls::async([this]() {
  AlbumListSource* source = new AlbumListSource();
  this->content_frame->setDataSource(source);
  // });
}

brls::View* Audio::create() { return new Audio(); }
}
