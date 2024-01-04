#pragma once

#include <borealis.hpp>

namespace view {
class ItemBox : public brls::Box {
public:
    ItemBox();
    static brls::View* create();
};
}
