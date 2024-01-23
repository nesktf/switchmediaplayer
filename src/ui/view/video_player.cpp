#include "ui/view/video_player.hpp"

using namespace brls::literals;

namespace view {
VideoPlayer::VideoPlayer() {
  this->inflateFromXMLRes("xml/view/video_player.xml");
  this->setHideHighlightBorder(true);
  this->setHideHighlightBackground(true);
  this->setHideClickAnimation(true);

  float w = brls::Application::contentWidth;
  float h = brls::Application::contentHeight;
  this->container = new brls::Box();
  this->container->setDimensions(w, h);
  this->setDimensions(w, h);
  this->setWidthPercentage(100);
  this->setHeightPercentage(100);
  this->setId("video");
  this->container->addView(this);

  this->registerAction(
    "hints/back"_i18n, brls::BUTTON_B,
    [this](brls::View* view) {
      return brls::Application::popActivity(brls::TransitionAnimation::NONE, []() {
        auto mpvce = core::MPV::instance().getCustomEvent();
        mpvce->fire("VIDEO_CLOSE", nullptr);
      });
    },
    true);

  this->registerMPVEvent();
}

void VideoPlayer::openFile(const std::string& file_path) {
  auto& mpv = core::MPV::instance();
  mpv.stop();
  mpv.setUrl(file_path);
  brls::Application::pushActivity(new brls::Activity(container), brls::TransitionAnimation::NONE);
}

void VideoPlayer::draw(NVGcontext* vg, float x, float y, float w, float h, brls::Style style, brls::FrameContext* ctx) {
  auto& mpv = core::MPV::instance();
  if (!mpv.isValid()) return;
  mpv.draw(getFrame(), getAlpha());
}

void VideoPlayer::registerMPVEvent() {
  auto& mpv = core::MPV::instance();
  event_sub_id = mpv.getEvent()->subscribe([this](core::MpvEvent event) {
    // auto& mpv = core::MPV::instance();
    // brls::Logger::debug("MPV event!!");
  });
}

VideoPlayer::~VideoPlayer() {
  auto& mpv = core::MPV::instance();
  mpv.getEvent()->unsubscribe(event_sub_id);
  mpv.stop();
}
}
