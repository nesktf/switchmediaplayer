#pragma once

#include "ui/view/media_browser.hpp"

namespace view {
class FileBrowser;

struct FileData {
  std::string path;
  MediaCellType type;
  FileBrowser* view;
};

class FileBrowser : public MediaBrowser<FileData> {
public:
  FileBrowser(const std::string& path);

  ThumbCellSource<FileData>* getContents(const std::string& path) override;
private:
  bool is_root;
};
}
