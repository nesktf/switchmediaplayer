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

template<typename T>
class MediaBrowser : public brls::Box {
public: 
  MediaBrowser() {
    this->inflateFromXMLRes("xml/view/media_browser.xml");
    this->content_frame->registerCell("cell", view::ThumbCell::create);
  }

  void setPath(const std::string& path) {
    this->curr_path = path;
    this->path_label->setText(path);
    this->content_frame->setDataSource(getContents(path));
    // Focus first cell (maybe find a better way?)
    brls::sync([this]() { brls::Application::giveFocus(content_frame); }); 
  }

  void init();

  virtual ThumbCellSource<T>* getContents(const std::string& path) = 0;
protected:
  BRLS_BIND(view::RecyclingGrid, content_frame, "media_browser/content");
  BRLS_BIND(brls::Label, path_label, "media_browser/path_label");

  std::string curr_path;
};
}
