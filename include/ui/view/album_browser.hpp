#pragma once

#include "ui/view/media_browser.hpp"
#include "core/db/database.hpp"

namespace view {

class AlbumBrowser : public MediaBrowser<int, core::Database::SortOrder> {
public:
  AlbumBrowser(core::Database::SortOrder order = core::Database::SortOrder::Asc);

  ThumbCellSource<int>* getContents(const core::Database::SortOrder& order) override;
};
}
