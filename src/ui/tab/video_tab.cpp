#include "ui/tab/video_tab.hpp"

namespace tab {

Video::Video() {
    this->inflateFromXMLRes("xml/tab/video.xml");
    this->content_frame->registerCell("Cell", view::MediaCell::create);
    this->content_frame->setDataSource(new view::MediaSource(5));
    this->recent_frame->registerCell("Cell", view::MediaCell::create);
    this->recent_frame->setDataSource(new view::MediaSource(20));
    // this->content_frame->showSkeleton(20);
}

brls::View* Video::create() { return new Video(); }
}
