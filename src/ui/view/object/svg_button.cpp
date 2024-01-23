#include "ui/view/object/svg_button.hpp"

namespace view {
const std::string svgbutton_xml_content = R"xml(
  <brls:Box
    justifyContent="center"
    alignItems="center"
    cornerRadius="10"
    highlightCornerRadius="10"
    padding="5px"
    axis="column"
    margin="10px"
    width="40"
    height="40"
    focusable="true">

    <SVGImage
      id="svg_button/icon"
      svg="@res/svg/9.svg"/>

</brls:Box>
)xml";

SVGButton::SVGButton() {
  this->inflateFromXMLString(svgbutton_xml_content);
  this->registerFilePathXMLAttribute("svg", [this](const std::string& value) { this->setImageFromSVGFile(value); });
  this->addGestureRecognizer(new brls::TapGestureRecognizer(this));
}

void SVGButton::setImageFromSVGFile(const std::string& file) {
  this->icon->setImageFromSVGFile(file);
}

void SVGButton::setImageFromSVGRes(const std::string& res) {
  this->icon->setImageFromSVGRes(res);
}
}
