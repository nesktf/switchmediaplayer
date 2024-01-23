#pragma once

#include <borealis.hpp>
#include <borealis/core/singleton.hpp>

#include "core/types.hpp"

namespace view {
class MusicPlayer : public brls::Box, public brls::Singleton<MusicPlayer> {
public:
  MusicPlayer();

  void registerMPVEvent();
  void setCurrent(const core::mediadata::Music& music);
private:
  BRLS_BIND(brls::Label, artist, "music_player/artist");
  BRLS_BIND(brls::Label, title, "music_player/title");
  BRLS_BIND(brls::Image, cover, "music_player/cover");
  BRLS_BIND(brls::Box, controls_container, "music_player/controls_container");

  std::vector<core::mediadata::Music> playlist;
};

class MusicPlayerView : public brls::Box {
public:
  MusicPlayerView();
  ~MusicPlayerView();
private:
  brls::Box* content;
};
}
