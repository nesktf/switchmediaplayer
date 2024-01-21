#include "ui/tab/video_tab.hpp"

#include "ui/view/media_source.hpp"
#include "ui/view/media_cell.hpp"

namespace tab {

// view::MediaCellSource* getVideoContent(void) {
//   view::MediaCellSource* source = new view::MediaCellSource();
//   source->data = {
//     {.title="/mnt/marisa",.path="/mnt/marisa"}
//   };
//   for(auto& item : source->data)
//     item.type = view::MediaCellType::BROWSER_CELL;
//   return source;
// }
//
// view::MediaCellSource* getRecentVideos(void) {
//   view::MediaCellSource* source = new view::MediaCellSource();
//   source->data = {
//     {"vp9_opus.webm"},
//     {"h264_aac.mp4"}
//   };
//   for (auto& item : source->data)
//     item.type = view::MediaCellType::VIDEO_FILE_CELL;
//   return source;
// }

Video::Video() {
  this->inflateFromXMLRes("xml/tab/video.xml");

  this->content_frame->registerCell("cell", view::MediaCell::create);
  // this->content_frame->setDataSource(getVideoContent());

  this->recent_frame->registerCell("cell", view::MediaCell::create);
  // this->recent_frame->setDataSource(getRecentVideos());
    // this->content_frame->showSkeleton(20);
}

brls::View* Video::create() { return new Video(); }
}
