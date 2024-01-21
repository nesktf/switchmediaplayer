#include "ui/tab/settings_tab.hpp"

#include "core/scanner.hpp"

#include "core/db/database.hpp"

namespace tab {
Settings::Settings() {
  this->inflateFromXMLRes("xml/tab/settings.xml");
  path_input->init("Media Path", "", [](auto){}, "", "", 255);
  path_button->registerClickAction([this](...) {
    std::string input = path_input->getValue();
    auto& db = core::Database::instance();
    db.insertSource(input);
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
