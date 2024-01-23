#include <borealis.hpp>
#include "ui/extern/svg_image.hpp"
#include "ui/extern/auto_tab_frame.hpp"
#include "ui/extern/recycling_grid.hpp"
#include "ui/extern/h_recycling.hpp"

#include "ui/view/object/svg_button.hpp"

#include "ui/tab/home_tab.hpp"
#include "ui/tab/music_tab.hpp"
#include "ui/tab/video_tab.hpp"
#include "ui/tab/settings_tab.hpp"
#include "ui/tab/browser_tab.hpp"

#include "ui/activity/main_activity.hpp"

int main(int argc, char* argv[]) {
  // Init borealis
  brls::Logger::setLogLevel(brls::LogLevel::LOG_DEBUG);
  // brls::Logger::setLogLevel(brls::LogLevel::LOG_VERBOSE);
  if (!brls::Application::init()) {
    brls::Logger::error("Unable to init application");
    return EXIT_FAILURE;
  }
  brls::Application::createWindow("switchmp");
  brls::Logger::info("createWindow done");

  // Views
  brls::Application::registerXMLView("SVGImage", view::SVGImage::create);
  brls::Application::registerXMLView("AutoTabFrame", view::AutoTabFrame::create);
  brls::Application::registerXMLView("RecyclerGrid", view::RecyclingGrid::create);
  brls::Application::registerXMLView("HRecyclerFrame", view::HRecyclerFrame::create);

  // Objects
  brls::Application::registerXMLView("SVGButton", view::SVGButton::create);

  // Tabs
  brls::Application::registerXMLView("HomeTab", tab::HomeTab::create);
  brls::Application::registerXMLView("MusicTab", tab::MusicTab::create);
  brls::Application::registerXMLView("VideoTab", tab::VideoTab::create);
  brls::Application::registerXMLView("BrowserTab", tab::BrowserTab::create);
  brls::Application::registerXMLView("SettingsTab", tab::SettingsTab::create);

  // Theme things
  brls::Theme::getLightTheme().addColor("color/main", nvgRGB(2, 176, 183)); // #02B0B7
  brls::Theme::getDarkTheme().addColor("color/main", nvgRGB(51, 186, 227)); // #33BAE3
  brls::Theme::getLightTheme().addColor("color/grey1", nvgRGB(245, 246, 247));// #F5F6F7
  brls::Theme::getDarkTheme().addColor("color/grey1", nvgRGB(51, 52, 53)); // #333435
  brls::Theme::getLightTheme().addColor("color/grey2", nvgRGBA(200, 200, 200, 16)); // #C8C8C8 10
  brls::Theme::getDarkTheme().addColor("color/grey2", nvgRGBA(160, 160, 160, 160)); // #A0A0A0 A0

  // Start Borealis
  brls::Application::pushActivity(new activity::MainActivity());
  while(brls::Application::mainLoop())
    ;

  return EXIT_SUCCESS;
}
