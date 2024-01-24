#include "ui/view/file_browser.hpp"

#include "ui/view/video_player.hpp"

#include "util/fs.hpp"

namespace view {
class FileBrowserSource : public ThumbCellSource<FileData> {
public:
  FileBrowserSource(const std::string& path, FileBrowser* view) {
    this->curr_path = std::filesystem::path{path};
    this->is_root = true;

    if (!util::fs::isRootFolder(curr_path)) {
      this->is_root = false;
      data.push_back((core::mediadata::ThumbCellData<FileData>) {
        .data = (FileData) {
          .path = curr_path.parent_path(),
          .type = MediaCellType::Folder,
          .view = view
        },
        .title = "..",
        .subtitle = "(folder)"
      });
    }

    std::set<std::filesystem::path> dirs;
    std::set<std::filesystem::path> files;

    for (auto const& entry : std::filesystem::directory_iterator{curr_path}) {
      if (util::fs::isFileHidden(entry.path())) {
        continue;;
      }
      if (entry.is_directory()) {
        dirs.insert(entry);
      } else {
        files.insert(entry);
      }
    }

    for (auto const& dir : dirs) {
      data.push_back((core::mediadata::ThumbCellData<FileData>) {
        .data = (FileData) {
          .path = dir.string(),
          .type = MediaCellType::Folder,
          .view = view
        },
        .title = dir.filename(),
        .subtitle = "(folder)"
      });
    }

    for (auto const& file : files) {
      data.push_back((core::mediadata::ThumbCellData<FileData>) {
        .data = (FileData) {
          .path = file.string(),
          .type = MediaCellType::File,
          .view = view
        },
        .title = file.filename(),
        .subtitle = ""
      });
    }
  }
  void onItemSelected(brls::View* recycler, size_t index) override {
    auto& item = data[index];
    if (item.data.type == MediaCellType::Folder) {
      item.data.view->setContents(item.data.path, item.data.path);
    } else {
      VideoPlayer* player = new VideoPlayer();
      player->openFile(item.data.path);
      // brls::sync([&player]() { brls::Application::giveFocus(player); });
    }
  }
  bool is_root;
private:
  std::filesystem::path curr_path;
};

FileBrowser::FileBrowser(const std::string& path) {
  this->setContents(path, path);
}

ThumbCellSource<FileData>* FileBrowser::getContents(const std::string& path) { 
  auto source = new FileBrowserSource(path, this);
  this->is_root = source->is_root;
  return source;
}
}
