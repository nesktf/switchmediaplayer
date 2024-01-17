#include "ui/view/video_player.hpp"

using namespace brls::literals;

namespace view {
VideoPlayer::VideoPlayer(const std::string& file_path) {
  inflateFromXMLRes("xml/view/video_player.xml");
  setHideHighlightBorder(true);
  setHideHighlightBackground(true);
  setHideClickAnimation(true);

  auto& mpv = core::MPV::instance();
  // mpv.stop();

  float w = brls::Application::contentWidth;
  float h = brls::Application::contentHeight;
  brls::Box* container = new brls::Box();
  container->setDimensions(w, h);
  setDimensions(w, h);
  setWidthPercentage(100);
  setHeightPercentage(100);
  setId("video");
  container->addView(this);
  brls::Application::pushActivity(new brls::Activity(container), brls::TransitionAnimation::NONE);

  registerAction(
    "hints/back"_i18n, brls::BUTTON_B,
    [this](brls::View* view) {
      return brls::Application::popActivity(brls::TransitionAnimation::NONE, []() {
        auto mpvce = core::MPV::instance().getCustomEvent();
        mpvce->fire("VIDEO_CLOSE", nullptr);
      });
    },
    true);

  registerMPVEvent();

  mpv.setUrl(file_path);
}

void VideoPlayer::draw(NVGcontext* vg, float x, float y, float w, float h, brls::Style style, brls::FrameContext* ctx) {
  auto& mpv = core::MPV::instance();
  if (!mpv.isValid()) return;
  mpv.draw(getFrame(), getAlpha());
}

void VideoPlayer::registerMPVEvent() {
  auto& mpv = core::MPV::instance();
  event_sub_id = mpv.getEvent()->subscribe([this](core::MpvEvent event) {
    auto& mpv = core::MPV::instance();
    brls::Logger::debug("MPV event!!");

  });
}

VideoPlayer::~VideoPlayer() {
  auto& mpv = core::MPV::instance();
  mpv.getEvent()->unsubscribe(event_sub_id);
  mpv.stop();
}
}
