#include "ui/tab/audio_tab.hpp"

namespace tab {

Audio::Audio() {
    this->inflateFromXMLRes("xml/tab/audio.xml");
    this->content_frame->registerCell("Cell", view::MediaCell::create);
    this->content_frame->setDataSource(new view::MediaSource(10));
    this->categories_frame->registerCell("Cell", view::MediaCell::create);
    this->categories_frame->setDataSource(new view::MediaSource(6));
    this->recent_frame->registerCell("Cell", view::MediaCell::create);
    this->recent_frame->setDataSource(new view::MediaSource(12));
    this->random_albums_frame->registerCell("Cell", view::MediaCell::create);
    this->random_albums_frame->setDataSource(new view::MediaSource(8));
    this->random_artists_frame->registerCell("Cell", view::MediaCell::create);
    this->random_artists_frame->setDataSource(new view::MediaSource(9));
}

brls::View* Audio::create() { return new Audio(); }
}
