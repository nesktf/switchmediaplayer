#pragma once

#include "ui/view/recycling_grid.hpp"

namespace view {

class MediaCell : public RecyclingGridItem {
public:
    MediaCell();
    static MediaCell* create();
};
}
