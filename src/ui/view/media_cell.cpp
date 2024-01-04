#include "ui/view/media_cell.hpp"

namespace view {
MediaCell::MediaCell() {
    this->inflateFromXMLRes("xml/view/media_cell.xml");
}

MediaCell * MediaCell::create() { return new MediaCell(); }

}
