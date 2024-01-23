#pragma once

#include <borealis.hpp>

#include "core/mpv.hpp"

namespace view {
class VideoPlayer : public brls::Box {
public:
  VideoPlayer();
  ~VideoPlayer();

  void openFile(const std::string& file_path);

  void draw(NVGcontext* vg, float x, float y, float w, float h, brls::Style style, brls::FrameContext* ctx) override;
  void registerMPVEvent();

  brls::View* getNextFocus(brls::FocusDirection direction, brls::View* currentView) override { return this; }

  brls::Event<core::MpvEvent>::Subscription event_sub_id;
private:
  brls::Box* container;
};
}
