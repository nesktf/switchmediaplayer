#include "ui/view/svg_button.hpp"

namespace view {
SVGButton::SVGButton(const std::string& svg_res) {
  inflateFromXMLRes("xml/view/svg_button.xml");
  icon->setImageFromSVGRes(svg_res);
  addGestureRecognizer(new brls::TapGestureRecognizer(this));
}
}
