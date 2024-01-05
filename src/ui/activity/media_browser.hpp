#pragma once

#include <borealis.hpp>
#include "ui/view/recycling_grid.hpp"
#include "ui/view/media_source.hpp"

namespace activity {
class MediaBrowser : public brls::Activity {
public:
  CONTENT_FROM_XML_RES("activity/media_browser.xml");
  // MediaBrowser();
  void onContentAvailable() override;
private:
  BRLS_BIND(view::RecyclingGrid, content_frame, "frame/content");
};
}
