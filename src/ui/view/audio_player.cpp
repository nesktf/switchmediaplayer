#include "ui/view/audio_player.hpp"

namespace view {
AudioPlayer::AudioPlayer() {
  inflateFromXMLRes("xml/view/audio_player.xml");
}
}
