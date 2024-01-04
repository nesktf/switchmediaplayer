#pragma once

#include "view/auto_tab_frame.hpp"
#include "view/h_recycling.hpp"
#include "view/media_source.hpp"

namespace tab {
class Home : public AttachedView {
public:
    Home();
    static brls::View* create();
    void onCreate() override;
private:
    BRLS_BIND(HRecyclerFrame, playing_frame, "tab/home/playing_frame");
    BRLS_BIND(HRecyclerFrame, history_frame, "tab/home/history_frame");
    BRLS_BIND(brls::Box, playing_hint, "tab/home/playing_hint");
    BRLS_BIND(brls::Box, history_hint, "tab/home/history_hint");
};
}

