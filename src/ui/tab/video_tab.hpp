#pragma once

#include "ui/view/auto_tab_frame.hpp"
#include "ui/view/h_recycling.hpp"

namespace tab {
class Video : public view::AttachedView {
public:
    Video();
    static brls::View* create();
private:
    BRLS_BIND(view::HRecyclerFrame, content_frame, "tab/video/content_frame");
    BRLS_BIND(view::HRecyclerFrame, recent_frame, "tab/video/recent_frame");
};
}
