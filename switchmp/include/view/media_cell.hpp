#pragma once

#include "view/recycling_grid.hpp"

namespace view {

class MediaCell : public RecyclingGridItem {
public:
    MediaCell();
    static MediaCell* create();
};
}
