#pragma once

#include "ui/view/auto_tab_frame.hpp"

namespace tab {
class Settings : public view::AttachedView {
public:
  Settings();
  static brls::View* create();
private:
  BRLS_BIND(brls::BooleanCell, boolean_cell, "setting/bool");
  BRLS_BIND(brls::SelectorCell, selector_cell, "setting/selector");
  BRLS_BIND(brls::InputCell, path_input, "setting/path_input");
  BRLS_BIND(brls::DetailCell, path_button, "setting/path_button");
  BRLS_BIND(brls::RadioCell, scan_button, "setting/scan_button");
};
}
