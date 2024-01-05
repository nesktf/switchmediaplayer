#pragma once

#include <borealis.hpp>

namespace activity {
class AlbumBrowser : public brls::Activity {
public:
  CONTENT_FROM_XML_RES("activity/album_browser.xml");
};
}
