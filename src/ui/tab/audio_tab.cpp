#include "ui/tab/audio_tab.hpp"

#include "ui/activity/audio_player.hpp"

namespace tab {

view::MediaCellSource* getAudioContent(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {.title="/mnt/marisa",.path="/mnt/marisa"},
    {.title="/mnt/patchouli",.path="/mnt/patchouli"}
  };
  for (auto& item : source->data)
    item.type = view::MediaCellType::BROWSER_CELL;
  return source;
}

view::MediaCellSource* getAudioCategories(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {"Albums"},
    {"Artists"},
    {"Genres"},
    {"Songs"}
  };
  for (auto& item : source->data)
    item.type = view::MediaCellType::BROWSER_CELL;
  return source;
}

view::MediaCellSource* getRecentAlbums(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {
      .title = "TOHO BOSSA NOVA 8",
      .subtitle = "ShibayanRecords",
      .image_path = "img/cover_test0.jpg"
    }
  };
  for (auto& item : source->data)
    item.type = view::MediaCellType::ALBUM_CELL;
  return source;
}

view::MediaCellSource* getRandomArtists(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {"ShibayanRecords"}
  };
  for (auto& item : source->data)
    item.type = view::MediaCellType::BROWSER_CELL;
  return source;
}

view::MediaCellSource* getRandomAlbums(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {
      .title = "TOHO BOSSA NOVA 8",
      .subtitle = "ShibayanRecords",
      .image_path = "img/cover_test0.jpg"
    }
  };
  for (auto& item : source->data)
    item.type = view::MediaCellType::ALBUM_CELL;
  return source;
}

Audio::Audio() {
  this->inflateFromXMLRes("xml/tab/audio.xml");
  this->content_frame->registerCell("cell", view::MediaCell::create);
  this->content_frame->setDataSource(getAudioContent());

  this->categories_frame->registerCell("cell", view::MediaCell::create);
  this->categories_frame->setDataSource(getAudioCategories());

  this->recent_frame->registerCell("cell", view::MediaCell::create);
  this->recent_frame->setDataSource(getRecentAlbums());

  this->random_albums_frame->registerCell("cell", view::MediaCell::create);
  this->random_albums_frame->setDataSource(getRandomAlbums());

  this->random_artists_frame->registerCell("cell", view::MediaCell::create);
  this->random_artists_frame->setDataSource(getRandomArtists());
}

brls::View* Audio::create() { return new Audio(); }
}
