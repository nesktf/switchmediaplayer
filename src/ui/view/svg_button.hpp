#pragma once

#include <borealis.hpp>
#include "ui/view/svg_image.hpp"

namespace view {
class SVGButton : public brls::Box {
public:
  SVGButton(const std::string& svg_res = "svg/9.svg");
private:
  BRLS_BIND(SVGImage, icon, "icon");
};
}
