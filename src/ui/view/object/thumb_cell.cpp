#include "ui/view/object/thumb_cell.hpp"

namespace view {
const std::string thumbcell_xml_content = R"xml(
<brls:Box
  highlightCornerRadius="8"
  axis="column">

  <brls:Image
    id="thumb_cell/image"
    backgroundColor="@theme/color/grey1"
    scalingType="fill"
    width="100%"
    height="80%"
    cornerRadius="0"/>

  <brls:Box
    id="thumb_cell/shadow"
    positionBottom="0"
    positionType="absolute"
    background="vertical_linear"
    paddingLeft="10"
    paddingRight="10"
    width="100%"
    height="100%"
    alignItems="center"/>

  <brls:Box
    axis="column"
    height="20%"
    alignItems="center"
    backgroundColor="#353535">

    <brls:Label
      id="thumb_cell/title"
      wireframe="false"
      fontSize="15"
      marginTop="5"
      text="title"/>
    <brls:Label
      id="thumb_cell/subtitle"
      wireframe="false"
      fontSize="12"
      marginTop="5"
      text="subtitle"/>

  </brls:Box>

</brls:Box>
)xml";

ThumbCell::ThumbCell() {
  this->inflateFromXMLString(thumbcell_xml_content);
}

ThumbCell* ThumbCell::create() { return new ThumbCell(); }

view::RecyclingGridItem* ThumbCellSource::cellForRow(view::RecyclingView* recycler, size_t index) {
  ThumbCell* cell = dynamic_cast<ThumbCell*>(recycler->dequeueReusableCell("cell"));
  auto& item = data[index];
  cell->title->setText(item.title);
  cell->subtitle->setText(item.subtitle);
  if (!item.image_path.empty())
    cell->image->setImageFromFile(item.image_path);
  return cell;
}

size_t ThumbCellSource::getItemCount() { return data.size(); }

void ThumbCellSource::clearData() { data.clear(); }
}
