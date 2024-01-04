#include <borealis.hpp>
#include <cstdlib>

#include "activity/main_activity.hpp"
#include "view/auto_tab_frame.hpp"
#include "view/svg_image.hpp"
#include "view/recycling_grid.hpp"
#include "view/h_recycling.hpp"
#include "view/item_box.hpp"
#include "tab/settings.hpp"
#include "tab/home.hpp"
#include "tab/video.hpp"
#include "tab/audio.hpp"

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
  brls::Application::registerXMLView("SVGImage", SVGImage::create);
  brls::Application::registerXMLView("AutoTabFrame", AutoTabFrame::create);
  brls::Application::registerXMLView("RecyclerFrame", RecyclingGrid::create);
  brls::Application::registerXMLView("HRecyclerFrame", HRecyclerFrame::create);
  brls::Application::registerXMLView("ItemBox", view::ItemBox::create);

  // Tabs
  brls::Application::registerXMLView("HomeTab", tab::Home::create);
  brls::Application::registerXMLView("VideoTab", tab::Video::create);
  brls::Application::registerXMLView("SettingsTab", tab::Settings::create);
  brls::Application::registerXMLView("AudioTab", tab::Audio::create);

  // Theme things
  brls::Theme::getLightTheme().addColor("color/grey_1", nvgRGB(245, 246, 247));
  brls::Theme::getDarkTheme().addColor("color/grey_1", nvgRGB(51, 52, 53));
  brls::Theme::getLightTheme().addColor("color/switchmp", nvgRGB(2, 176, 183));
  brls::Theme::getDarkTheme().addColor("color/switchmp", nvgRGB(51, 186, 227));
  brls::Theme::getLightTheme().addColor("color/grey_3", nvgRGBA(200, 200, 200, 16));
  brls::Theme::getDarkTheme().addColor("color/grey_3", nvgRGBA(160, 160, 160, 160));
  brls::Theme::getDarkTheme().addColor("color/grayed_out", nvgRGB(104, 104, 104));
  brls::Theme::getLightTheme().addColor("color/grayed_out", nvgRGB(104, 104, 104));

  // Start Borealis
  brls::Application::pushActivity(new MainActivity());
  while(brls::Application::mainLoop())
    ;

  return EXIT_SUCCESS;
}
