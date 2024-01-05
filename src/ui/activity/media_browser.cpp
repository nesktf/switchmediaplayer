#include "ui/activity/media_browser.hpp"

namespace activity {

view::MediaCellSource* getBrowserItems(void) {
  view::MediaCellSource* source = new view::MediaCellSource();
  for (size_t i = 0; i < 20; ++i) {
    source->data.push_back({"test"+std::to_string(i)});
  }
  return source;
}

void MediaBrowser::onContentAvailable() {
  this->content_frame->registerCell("cell", view::MediaCell::create);
  this->content_frame->setDataSource(getBrowserItems());
}
}
