#include "ui/view/music_player.hpp"

#include "ui/view/object/svg_button.hpp"
#include "ui/view/object/track_cell.hpp"

#include "core/mpv.hpp"

using namespace brls::literals;

struct MusicPlayerSource : public view::RecyclingGridDataSource {
  MusicPlayerSource(MusicQueue* queue) { this->queue = queue; }
  size_t getItemCount() override { 
    return queue->contents.size();
  }
  view::RecyclingGridItem* cellForRow(view::RecyclingView* recycler, size_t index) override {
    view::TrackCell* cell = dynamic_cast<view::TrackCell*>(recycler->dequeueReusableCell("cell"));

    int id = queue->order[index];
    auto& item = queue->contents[id];

    cell->title->setText(item.title);
    cell->subtitle->setText(item.artist);
    cell->length->setText(std::to_string(item.length));
    return cell;
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    auto& player = view::MusicPlayer::instance();
    player.setPlayIndex(index);
  }

  void clearData() override {
    auto& player = view::MusicPlayer::instance();
    player.clear();
  };

  MusicQueue* queue;
};

namespace view {
MusicPlayer::MusicPlayer() {
  inflateFromXMLRes("xml/view/music_player.xml");

  auto& mpv = core::MPV::instance();
  auto play_but = new SVGButton();
  play_but->registerClickAction([&mpv](...) {
    mpv.togglePlay();
    return true;
  });

  auto prev_but = new SVGButton();
  prev_but->registerClickAction([&mpv](...) {
    mpv.seek(-5, "");
    return true;
  });

  auto next_but = new SVGButton();
  next_but->registerClickAction([&mpv](...) {
    mpv.seek(5, "");
    return true;
  });


  this->controls_container->addView(prev_but);
  this->controls_container->addView(play_but);
  this->controls_container->addView(next_but);

  this->queue = new MusicQueue();
  this->source = new MusicPlayerSource(this->queue);
  this->queue_grid->registerCell("cell", TrackCell::create);
  this->queue_grid->setDataSource(this->source);
}

MusicPlayer::~MusicPlayer() {
  delete this->queue;
}

void MusicPlayer::setPlayIndex(int index) {
  if (queue->order.size() <= index)
    return;

  auto& mpv = core::MPV::instance();
  auto& item = queue->contents[queue->order[index]];
  setContents(item);
  mpv.setUrl(item.path);
}

void MusicPlayer::addItemAndPlay(const core::mediadata::Music& item) { 
  auto& mpv = core::MPV::instance();
  this->queue->contents[item.id] = item;
  this->queue->order.push_back(item.id);
  this->queue->curr = item.id;
  this->queue_grid->notifyDataChanged();
  setContents(item);
  mpv.setUrl(item.path);
}

void MusicPlayer::clear(void) {
  auto& mpv = core::MPV::instance();
  this->queue->curr = 0;
  this->queue->contents.clear();;
  this->queue->order.clear();
  this->queue_grid->notifyDataChanged();
  mpv.stop();
  queue_empty_event.fire(true);
}

void MusicPlayer::setContents(const core::mediadata::Music& item) {
  curr_change_event.fire(item);
  title->setText(item.title);
  artist->setText(item.artist);
  if (!item.cover_path.empty()) {
    cover->setImageFromFile(item.cover_path);
  } else {
    // The strongest album
    cover->setImageFromRes("img/icon.jpg");
  }
}

MusicPlayerView::MusicPlayerView() {
  this->content = new brls::Box();
  auto& view = MusicPlayer::instance();
  content->addView(&view);
  this->addView(content);

  this->registerAction("Clear queue", brls::BUTTON_X, [this](...) {
    view::MusicPlayer::instance().clear();
    return true;
  });


  brls::Logger::debug("Created Music Player wrapper");
}

MusicPlayerView::~MusicPlayerView() {
  MusicPlayer::instance().setParent(nullptr);
  content->clearViews(false);
}

}
