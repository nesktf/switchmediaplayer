#pragma once

#include "ui/extern/auto_tab_frame.hpp"
#include "ui/extern/h_recycling.hpp"
#include "ui/view/music_player.hpp"

namespace tab {
class HomeTab : public view::AttachedView {
public:
  HomeTab();

  static brls::View* create();
  void onCreate() override;
  brls::Event<core::mediadata::Music>::Subscription player_change_sub;
  brls::Event<bool>::Subscription queue_empty_sub;
private:
  BRLS_BIND(view::HRecyclerFrame, playing_frame, "home_tab/playing_frame");
  BRLS_BIND(view::HRecyclerFrame, history_frame, "home_tab/history_frame");
  BRLS_BIND(brls::Box, playing_hint, "home_tab/playing_hint");
  BRLS_BIND(brls::Box, history_hint, "home_tab/history_hint");
};
}

