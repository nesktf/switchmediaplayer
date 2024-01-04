#pragma once

#include "view/auto_tab_frame.hpp"
#include "view/h_recycling.hpp"
#include "view/media_source.hpp"

namespace tab {
class Audio : public AttachedView {
public:
    Audio();
    static brls::View* create();
private:
    BRLS_BIND(HRecyclerFrame, content_frame, "tab/audio/content_frame");
    BRLS_BIND(HRecyclerFrame, categories_frame, "tab/audio/categories_frame");
    BRLS_BIND(HRecyclerFrame, recent_frame, "tab/audio/recent_frame");
    BRLS_BIND(HRecyclerFrame, random_albums_frame, "tab/audio/random_albums_frame");
    BRLS_BIND(HRecyclerFrame, random_artists_frame, "tab/audio/random_artists_frame");
};
}
