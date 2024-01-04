#include "view/item_box.hpp"

namespace view {
ItemBox::ItemBox() {
    this->inflateFromXMLRes("xml/view/item_box.xml");
}
brls::View * ItemBox::create()
{
    return new ItemBox();
}

}
