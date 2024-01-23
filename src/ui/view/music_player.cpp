#include "ui/view/music_player.hpp"

#include "ui/view/object/svg_button.hpp"

#include "core/mpv.hpp"

using namespace brls::literals;

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
}

void MusicPlayer::setCurrent(const core::mediadata::Music& music) {
  title->setText(music.title);
  artist->setText(music.artist);
  brls::Logger::info(music.cover_path);
  cover->setImageFromFile(music.cover_path);

  auto& mpv = core::MPV::instance();
  mpv.setUrl(music.path);
}

MusicPlayerView::MusicPlayerView() {
  this->content = new brls::Box();
  auto& view = MusicPlayer::instance();
  content->addView(&view);
  this->addView(content);

  brls::Logger::debug("Created Music Player wrapper");
}

MusicPlayerView::~MusicPlayerView() {
  auto& mpv = core::MPV::instance();
  mpv.stop();
  MusicPlayer::instance().setParent(nullptr);
  content->clearViews(false);
}

}
