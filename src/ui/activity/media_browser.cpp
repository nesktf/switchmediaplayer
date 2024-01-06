#include "ui/activity/media_browser.hpp"

namespace activity {

bool fileIsHidden(const std::filesystem::path path) {
  std::filesystem::path::string_type p {path.filename()};
  return p[0] == '.';
}

view::RecyclingGridItem* BrowserSource::cellForRow(view::RecyclingView* recycler, size_t index) {
  view::MediaCell* cell = dynamic_cast<view::MediaCell*>(recycler->dequeueReusableCell("cell"));
  auto& item {data[index]};
  cell->title->setText(item.title);
  if (item.type == BrowserCellType::FOLDER)
    cell->subtitle->setText("(folder)");
  else
    cell->subtitle->setText("");
  return cell;
}

void BrowserSource::onItemSelected(brls::View* recycler, size_t index) {
  auto& item = data[index];
  brls::Logger::debug(std::to_string(index));
  if (item.type == BrowserCellType::FOLDER)
    item.activity->modifyPath(item.path);
}

void BrowserSource::clearData() {
  data.clear();
}

void BrowserActivity::onContentAvailable() {
  this->content_frame->registerCell("cell", view::MediaCell::create);
  this->content_frame->setDataSource(parseFolder());
  this->path_label->setText(curr_path);

  this->registerAction("Root Folder", brls::BUTTON_X, [this](...) {
    if (curr_path.string() != "/")
      modifyPath(curr_path.parent_path());
    return true;
  });
}

void BrowserActivity::modifyPath(const std::string& path) {
  curr_path = std::filesystem::path { path };
  this->content_frame->setDataSource(parseFolder());
  this->path_label->setText(curr_path);
  // Focus first cell (maybe find a better way?)
  brls::sync([this]() {brls::Application::giveFocus(content_frame);});
}

BrowserActivity::BrowserActivity(const std::string& path) {
  curr_path = std::filesystem::path { path };
}

FileBrowser::FileBrowser(const std::string& path) : BrowserActivity(path) {}

BrowserSource* FileBrowser::parseFolder() {
  BrowserSource* source = new BrowserSource();
  if (curr_path.string() != "/")
    source->data = {
      {"..", curr_path.parent_path(), BrowserCellType::FOLDER, this}
    };

  std::set<std::filesystem::path> content_dirs;
  std::set<std::filesystem::path> content_files;

  for (auto const& dir_entry : std::filesystem::directory_iterator{curr_path}) {
    if (fileIsHidden(dir_entry.path()))
      continue;

    if (dir_entry.is_directory())
      content_dirs.insert(dir_entry);
    else
      content_files.insert(dir_entry);
  }

  for (auto& file : content_dirs) {
    source->data.push_back({
      .title = file.filename(),
      .path = file.string(),
      .type = BrowserCellType::FOLDER,
      .activity = this
    });
  }

  for (auto& file : content_files) {
    source->data.push_back({
      .title = file.filename(),
      .path = file.string(),
      .type = BrowserCellType::FILE,
      .activity = this
    });
  }

  return source;
}

}
