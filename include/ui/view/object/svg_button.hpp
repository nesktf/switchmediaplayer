#pragma once

#include <borealis.hpp>

#include "ui/extern/svg_image.hpp"

namespace view {
class SVGButton : public brls::Box {
public:
  SVGButton();
  void setImageFromSVGFile(const std::string& file);
  void setImageFromSVGRes(const std::string& res);
private:
  BRLS_BIND(SVGImage, icon, "svg_button/icon");
};
}
