#include "ui/tab/settings_tab.hpp"

#include "core/media_scan.hpp"
#include "core/db/database.hpp"

namespace tab {
SettingsTab::SettingsTab() {
  this->inflateFromXMLRes("xml/tab/settings_tab.xml");
  path_input->init("Media Path", "", [](auto){}, "", "", 255);
  path_button->registerClickAction([this](...) {
    std::string input = path_input->getValue();
    auto& db = core::Database::instance();
    db.insertSource(input);
    brls::Logger::debug("Added media path: {}", input);
    return true;
  });
  scan_button->registerClickAction([this](...) {
    core::media::scan();
    return true;
  });
}

brls::View* SettingsTab::create() { return new SettingsTab(); }
}
