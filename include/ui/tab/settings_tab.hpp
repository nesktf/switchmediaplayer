#pragma once

#include "ui/extern/auto_tab_frame.hpp"

namespace tab {
class SettingsTab : public view::AttachedView {
public:
  SettingsTab();

  static brls::View* create();
private:
  BRLS_BIND(brls::BooleanCell, boolean_cell, "settings_tab/bool");
  BRLS_BIND(brls::SelectorCell, selector_cell, "settings_tab/selector");
  BRLS_BIND(brls::InputCell, path_input, "settings_tab/path_input");
  BRLS_BIND(brls::DetailCell, path_button, "settings_tab/path_button");
  BRLS_BIND(brls::RadioCell, scan_button, "settings_tab/scan_button");
};
}
