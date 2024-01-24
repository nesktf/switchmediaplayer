#include "ui/tab/music_tab.hpp"

#include "ui/view/album_view.hpp"
#include "ui/view/object/thumb_cell.hpp"
#include "ui/view/file_browser.hpp"
#include "ui/view/album_browser.hpp"

#include "core/db/database.hpp"

namespace tab {
enum class AudioCategories {
  Albums,
  Genres,
  Artists,
  Songs
};

class AudioCategorySource : public view::ThumbCellSource<AudioCategories> {
public:
  AudioCategorySource() {
    this->data = {
      {.data = AudioCategories::Albums, .title = "Albums",},
      {.data = AudioCategories::Genres, .title = "Genres"}, 
      {.data = AudioCategories::Artists, .title = "Artists"},
      {.data = AudioCategories::Songs, .title = "Songs"}
    };
  };
  void onItemSelected(brls::View* recycler, size_t index) override {
    auto& item = data[index];
    switch (item.data) {
      case AudioCategories::Albums: {
        recycler->present(new view::AlbumBrowser());
        break;
      }
      default: {

      }
    }
  }
};

class AlbumListSource : public view::ThumbCellSource<int> {
public:
  AlbumListSource() {
    auto& db = core::Database::instance();
    data = db.getAlbumCells(core::Database::SortOrder::Asc, 5);
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    recycler->present(new view::AlbumView(data[index].data));
  }
};

class AudioContentSource : public view::ThumbCellSource<int> {
public:
  AudioContentSource() {
    auto& db = core::Database::instance();
    for (auto& source : db.getSources()) {
      data.push_back({.title = source.path});
    }
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    auto& item = data[index];
    recycler->present(new view::FileBrowser(item.title));
  }
};

MusicTab::MusicTab() {
  this->inflateFromXMLRes("xml/tab/music_tab.xml");
  this->content_frame->registerCell("cell", view::ThumbCell::create);
  this->content_frame->setDataSource(new AudioContentSource());

  this->categories_frame->registerCell("cell", view::ThumbCell::create);
  this->categories_frame->setDataSource(new AudioCategorySource());

  this->recent_frame->registerCell("cell", view::ThumbCell::create);
  // this->recent_frame->setDataSource(getRecentAlbums());

  this->random_albums_frame->registerCell("cell", view::ThumbCell::create);
  // this->random_albums_frame->setDataSource(getRandomAlbums());

  this->random_artists_frame->registerCell("cell", view::ThumbCell::create);
  // this->random_artists_frame->setDataSource(getRandomArtists());

  // brls::async([this]() {
  AlbumListSource* source = new AlbumListSource();
  this->recent_frame->setDataSource(source);
  // });
}

brls::View* MusicTab::create() { return new MusicTab(); }
}
