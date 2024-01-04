#pragma once

#include "view/auto_tab_frame.hpp"

namespace tab {
class Settings : public AttachedView {
public:
    Settings();
    static brls::View* create();
private:
    BRLS_BIND(brls::BooleanCell, boolean_cell, "setting/bool");
    BRLS_BIND(brls::SelectorCell, selector_cell, "setting/selector");
    BRLS_BIND(brls::RadioCell, radio_cell, "setting/radio");
};
}
