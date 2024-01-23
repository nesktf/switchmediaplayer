#include "ui/tab/video_tab.hpp"

#include "ui/view/object/thumb_cell.hpp"

namespace tab {

// view::ThumbCellSource* getVideoContent(void) {
//   view::ThumbCellSource* source = new view::ThumbCellSource();
//   source->data = {
//     {.title="/mnt/marisa",.path="/mnt/marisa"}
//   };
//   for(auto& item : source->data)
//     item.type = view::ThumbCellType::BROWSER_CELL;
//   return source;
// }
//
// view::ThumbCellSource* getRecentVideos(void) {
//   view::ThumbCellSource* source = new view::ThumbCellSource();
//   source->data = {
//     {"vp9_opus.webm"},
//     {"h264_aac.mp4"}
//   };
//   for (auto& item : source->data)
//     item.type = view::ThumbCellType::VIDEO_FILE_CELL;
//   return source;
// }

VideoTab::VideoTab() {
  this->inflateFromXMLRes("xml/tab/video_tab.xml");

  this->content_frame->registerCell("cell", view::ThumbCell::create);
  // this->content_frame->setDataSource(getVideoContent());

  this->recent_frame->registerCell("cell", view::ThumbCell::create);
  // this->recent_frame->setDataSource(getRecentVideos());
    // this->content_frame->showSkeleton(20);
}

brls::View* VideoTab::create() { return new VideoTab(); }
}
