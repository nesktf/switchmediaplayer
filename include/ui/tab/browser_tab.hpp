#pragma once

#include "ui/extern/auto_tab_frame.hpp"

namespace tab {
class BrowserTab : public view::AttachedView {
public:
  BrowserTab();

  static brls::View* create();
};
}
