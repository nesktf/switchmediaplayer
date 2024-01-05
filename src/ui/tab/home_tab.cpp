#include "ui/tab/home_tab.hpp"

namespace tab {

view::MediaCellSource* getNowPlaying(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {
      .title = "ふわふわどれみー", 
      .subtitle = "nachi",
      .image_path = "img/cover_test0.jpg",
      .type = view::MediaCellType::AUDIO_FILE_CELL
    }
  };
  return source;
}

view::MediaCellSource* getHistory(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  source->data = {
    {
      .title = "ふわふわどれみー", 
      .subtitle = "nachi",
      .image_path = "img/cover_test0.jpg",
      .type = view::MediaCellType::AUDIO_FILE_CELL
    },
    { 
      .title = "vp9_opus.webm",
      .type = view::MediaCellType::VIDEO_FILE_CELL
    }
  };
  return source;
}


Home::Home() {
  this->inflateFromXMLRes("xml/tab/home.xml");

  this->playing_frame->registerCell("cell", view::MediaCell::create);
  this->playing_frame->setDataSource(getNowPlaying());
  this->playing_frame->setVisibility(brls::Visibility::GONE);
  brls::Logger::debug("playing_frame setup done");

  this->history_frame->registerCell("cell", view::MediaCell::create);
  this->history_frame->setDataSource(getHistory());
  this->history_frame->setVisibility(brls::Visibility::GONE);
  brls::Logger::debug("history_frame setup done");
}

brls::View* Home::create() { return new Home(); }

void Home::onCreate() {
    this->registerAction("dou", brls::BUTTON_X, [this](...) {
        brls::Logger::debug("Pressed the dou button");
        this->playing_frame->setVisibility(brls::Visibility::VISIBLE);
        this->history_frame->setVisibility(brls::Visibility::VISIBLE);
        this->playing_hint->setVisibility(brls::Visibility::GONE);
        this->history_hint->setVisibility(brls::Visibility::GONE);
        return true;
    });
}

}
