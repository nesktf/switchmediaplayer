#include "ui/view/album_browser.hpp"

#include "ui/view/album_view.hpp"

namespace view {
class AlbumBrowserSource : public ThumbCellSource<int> {
public:
  AlbumBrowserSource(const core::Database::SortOrder order) {
    data = core::Database::instance().getAlbumCells(order);
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    recycler->present(new AlbumView(data[index].data));
  }
};

AlbumBrowser::AlbumBrowser(core::Database::SortOrder order) {
  this->setContents(order, "Albums");
}

ThumbCellSource<int>* AlbumBrowser::getContents(const core::Database::SortOrder& order) {
  return new AlbumBrowserSource(order);
}
}
