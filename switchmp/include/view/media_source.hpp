#pragma once

#include "view/media_cell.hpp"

namespace view {

class MediaSource : public RecyclingGridDataSource {
public:
    MediaSource(size_t item_count);
    size_t getItemCount() override { return item_count; };
    RecyclingGridItem* cellForRow(RecyclingView* recycler, size_t index) override;
    void onItemSelected(brls::View* recycler, size_t index) override;
    void clearData() override;
protected:
    size_t item_count;
};
}
