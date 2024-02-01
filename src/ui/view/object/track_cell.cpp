#include "ui/view/object/track_cell.hpp"

#include "ui/view/music_player.hpp"

namespace view {
const std::string trackcell_xml_content = R"xml(
<brls:Box
  highlightCornerRadius="8"
  alignItems="center"
  lineBottom="1"
  lineColor="@theme/brls/header/border">

  <brls:Label
    id="track_cell/index_label"
    fontSize="@style/brls/header/font_size"
    width="30" />

  <brls:Box
    grow="1.0">
    <brls:Label
      id="track_cell/title_label"
      fontSize="@style/brls/header/font_size" />
    <brls:Padding />

    <brls:Label
      id="track_cell/subtitle_label"
      fontSize="15"
      marginTop="5"
      textColor="@theme/brls/header/subtitle" />

  </brls:Box>

  <brls:Box
    axis="column"
    marginLeft="20">

    <brls:Label
      id="track_cell/length_label"
      singleLine="true"
      fontSize="18" />

  </brls:Box>

</brls:Box>
)xml";

TrackCell::TrackCell() {
  this->inflateFromXMLString(trackcell_xml_content);
}

RecyclingGridItem* TrackDataSource::cellForRow(RecyclingView* recycler, size_t index) {
  TrackCell* cell = dynamic_cast<TrackCell*>(recycler->dequeueReusableCell("cell"));
  auto& item = data[index];
  cell->index->setText(std::to_string(item.track));
  cell->title->setText(item.title);
  cell->subtitle->setText(item.artist);
  cell->length->setText(std::to_string(item.length));
  cell->updateActionHint(brls::BUTTON_A, "Play");
  cell->registerAction("Play next", brls::BUTTON_X, [this, &item](...) {
    return true;
  });
  cell->registerAction("Play at end", brls::BUTTON_Y, [this, &item](...) {
    return true;
  });
  cell->registerAction("Add to playlist", brls::BUTTON_BACK, [this, &item](...) {
    brls::Logger::info("{}", item.title);
    return true;
  });
  return cell;
}

void TrackDataSource::onItemSelected(brls::View* recycler, size_t index) {
  auto& item = data[index];
  auto& player = MusicPlayer::instance();
  player.addItemAndPlay(item);
  recycler->present(new MusicPlayerView());
}
}
