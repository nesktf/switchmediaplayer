#include "ui/tab/settings_tab.hpp"

namespace tab {
Settings::Settings() {
    this->inflateFromXMLRes("xml/tab/settings.xml");
}

brls::View* Settings::create() { return new Settings(); }
}
