#pragma once

#include <borealis.hpp>

namespace activity {
class AudioPlayer : public brls::Activity {
public:
  CONTENT_FROM_XML_RES("activity/audio_player.xml");
};
}
