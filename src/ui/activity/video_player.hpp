#pragma once

#include <borealis.hpp>

namespace activity {
class VideoPlayer : public brls::Activity {
public:
  CONTENT_FROM_XML_RES("activity/video_player.xml");
};
}
