#pragma once

#include <borealis.hpp>

#include "core/mpv.hpp"

namespace view {
class VideoPlayer : public brls::Box {
public:
  VideoPlayer(const std::string& file_path);
  ~VideoPlayer();
  void draw(NVGcontext* vg, float x, float y, float w, float h, brls::Style style, brls::FrameContext* ctx) override;
  brls::View* getNextFocus(brls::FocusDirection direction, brls::View* currentView) override { return this; }
  void registerMPVEvent();
  brls::Event<MpvEvent>::Subscription event_sub_id;
};
}
