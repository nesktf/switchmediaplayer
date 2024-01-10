#pragma once

#include "ui/view/h_recycling.hpp"
#include "ui/view/auto_tab_frame.hpp"

namespace tab {
class Audio : public view::AttachedView {
public:
    Audio();
    static brls::View* create();
private:
    BRLS_BIND(view::HRecyclerFrame, content_frame, "tab/audio/content_frame");
    BRLS_BIND(view::HRecyclerFrame, categories_frame, "tab/audio/categories_frame");
    BRLS_BIND(view::HRecyclerFrame, recent_frame, "tab/audio/recent_frame");
    BRLS_BIND(view::HRecyclerFrame, random_albums_frame, "tab/audio/random_albums_frame");
    BRLS_BIND(view::HRecyclerFrame, random_artists_frame, "tab/audio/random_artists_frame");
};
}
