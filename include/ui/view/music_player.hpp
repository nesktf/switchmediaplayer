#pragma once

#include <borealis.hpp>
#include <borealis/core/singleton.hpp>
#include "ui/extern/recycling_grid.hpp"

#include "core/types.hpp"

struct MusicQueue {
  std::unordered_map<int,core::mediadata::Music> contents;
  std::vector<int> order;
  int curr = 0;
};

struct MusicPlayerSource;

namespace view {
class MusicPlayer : public brls::Box, public brls::Singleton<MusicPlayer> {
public:
  MusicPlayer();
  ~MusicPlayer();

  void registerMPVEvent();
  void addItemAndPlay(const core::mediadata::Music& item);
  void setPlayIndex(int index);
  void clear(void);

  MusicQueue* queue;
  MusicPlayerSource* source;
  brls::Event<core::mediadata::Music> curr_change_event;
  brls::Event<bool> queue_empty_event;

private:
  void setContents(const core::mediadata::Music& item);

private:
  BRLS_BIND(brls::Label, artist, "music_player/artist");
  BRLS_BIND(brls::Label, title, "music_player/title");
  BRLS_BIND(brls::Image, cover, "music_player/cover");
  BRLS_BIND(brls::Box, controls_container, "music_player/controls_container");
  BRLS_BIND(RecyclingGrid, queue_grid, "music_player/queue");

};

class MusicPlayerView : public brls::Box {
public:
  MusicPlayerView();
  ~MusicPlayerView();
private:
  brls::Box* content;
};
}
