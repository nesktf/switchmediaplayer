#include "ui/tab/browser_tab.hpp"

namespace tab {
BrowserTab::BrowserTab() {
  this->inflateFromXMLRes("xml/tab/browser_tab.xml");
}

brls::View* BrowserTab::create() { return new BrowserTab(); }

}
