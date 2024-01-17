#include "ui/tab/settings_tab.hpp"

#include "media/scanner.hpp"

#include "core/database.hpp"

namespace tab {
Settings::Settings() {
  this->inflateFromXMLRes("xml/tab/settings.xml");
  path_input->init("Media Path", "", [](auto){}, "", "", 255);
  path_button->registerClickAction([this](...) {
    std::string input = path_input->getValue();
    auto& db = core::MediaDB::instance();
    db.insertMediaPath(input);
    brls::Logger::debug("Added media path: {}", input);
    return true;
  });
  scan_button->registerClickAction([this](...) {
    media::scan(); 
    return true;
  });
}

brls::View* Settings::create() { return new Settings(); }
}
