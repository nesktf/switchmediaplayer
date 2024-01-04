#include "ui/view/media_source.hpp"

namespace view {

MediaSource::MediaSource(size_t item_count) {
    this->item_count = item_count;
}

RecyclingGridItem* MediaSource::cellForRow(RecyclingView* recycler, size_t index) {
    MediaCell* cell = dynamic_cast<MediaCell*>(recycler->dequeueReusableCell("Cell"));
    return cell;
}

void MediaSource::onItemSelected(brls::View* recycler, size_t index) {
    brls::Logger::debug("Selected item");
}


void MediaSource::clearData() { }
}
