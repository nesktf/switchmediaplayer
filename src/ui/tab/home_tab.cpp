#include "ui/tab/home_tab.hpp"

#include "ui/view/object/thumb_cell.hpp"

namespace tab {

class PlayingSource : public view::ThumbCellSource<int> {
public:
  PlayingSource(const core::mediadata::Music& item) {
    this->data.push_back((core::mediadata::ThumbCellData<int>) {
      .data = item.id,
      .title = item.title,
      .subtitle = item.artist,
      .image_path = item.cover_path
    });
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    auto player_view = new view::MusicPlayerView();
    recycler->present(player_view);
  }
};

HomeTab::HomeTab() {
  this->inflateFromXMLRes("xml/tab/home_tab.xml");

  this->playing_frame->registerCell("cell", view::ThumbCell::create);
  this->playing_frame->setVisibility(brls::Visibility::GONE);

  this->history_frame->registerCell("cell", view::ThumbCell::create);
  this->history_frame->setVisibility(brls::Visibility::GONE);

  auto& player = view::MusicPlayer::instance();
  this->player_change_sub = player.curr_change_event.subscribe([this](const core::mediadata::Music& item) {
    this->playing_frame->setVisibility(brls::Visibility::VISIBLE);
    this->playing_hint->setVisibility(brls::Visibility::GONE);
    this->playing_frame->setDataSource(new PlayingSource(item));
  });
  this->queue_empty_sub = player.queue_empty_event.subscribe([this](...) {
    this->playing_frame->setVisibility(brls::Visibility::GONE);
    this->playing_hint->setVisibility(brls::Visibility::VISIBLE);
  });
}

brls::View* HomeTab::create() { return new HomeTab(); }

void HomeTab::onCreate() {
    // this->registerAction("dou", brls::BUTTON_X, [this](...) {
    //     brls::Logger::debug("Pressed the dou button");
    //     this->playing_frame->setVisibility(brls::Visibility::VISIBLE);
    //     this->history_frame->setVisibility(brls::Visibility::VISIBLE);
    //     this->playing_hint->setVisibility(brls::Visibility::GONE);
    //     this->history_hint->setVisibility(brls::Visibility::GONE);
    //     return true;
    // });
}

}
