#pragma once

#include "ui/extern/recycling_grid.hpp"
#include "ui/view/object/thumb_cell.hpp"

namespace view {
enum MediaCellType {
  Folder  = 0,
  File    = 1,
  Audio   = 2,
  Video   = 3,
  Image   = 4
};

template<typename T, typename K>
class MediaBrowser : public brls::Box {
public: 
  MediaBrowser() {
    this->inflateFromXMLRes("xml/view/media_browser.xml");
    this->content_frame->registerCell("cell", view::ThumbCell::create);
  }

  void setContents(const K& key, const std::string& title) {
    this->title->setText(title);
    this->content_frame->setDataSource(getContents(key));
    // Focus first cell (maybe find a better way?)
    brls::sync([this]() { brls::Application::giveFocus(content_frame); }); 
  }

  void init();

  virtual ThumbCellSource<T>* getContents(const K& key) = 0;
protected:
  BRLS_BIND(view::RecyclingGrid, content_frame, "media_browser/content");
  BRLS_BIND(brls::Label, title, "media_browser/title");
};
}
