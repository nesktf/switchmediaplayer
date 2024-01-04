#pragma once

#include "view/auto_tab_frame.hpp"
#include "view/h_recycling.hpp"
#include "view/media_source.hpp"

namespace tab {
class Video : public AttachedView {
public:
    Video();
    static brls::View* create();
private:
    BRLS_BIND(HRecyclerFrame, content_frame, "tab/video/content_frame");
    BRLS_BIND(HRecyclerFrame, recent_frame, "tab/video/recent_frame");
};
}
