#include "tab/home.hpp"

namespace tab {
Home::Home() {
    this->inflateFromXMLRes("xml/tab/home.xml");

    this->playing_frame->registerCell("Cell", view::MediaCell::create);
    this->playing_frame->setDataSource(new view::MediaSource(1));
    brls::Logger::debug("playing_frame setup done");
    this->playing_frame->setVisibility(brls::Visibility::GONE);

    this->history_frame->registerCell("Cell", view::MediaCell::create);
    this->history_frame->setDataSource(new view::MediaSource(8));
    brls::Logger::debug("history_frame setup done");
    this->history_frame->setVisibility(brls::Visibility::GONE);
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
