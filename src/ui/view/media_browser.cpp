#include "ui/view/media_browser.hpp"

#include "ui/view/video_player.hpp"
#include "ui/view/object/thumb_cell.hpp"

#include "core/db/database.hpp"

namespace view {

// bool fileIsHidden(const std::filesystem::path path) {
//   std::filesystem::path::string_type p {path.filename()};
//   return p[0] == '.';
// }
//
// view::RecyclingGridItem* BrowserSource::cellForRow(view::RecyclingView* recycler, size_t index) {
//   view::MediaCell* cell = dynamic_cast<view::MediaCell*>(recycler->dequeueReusableCell("cell")); auto& item {data[index]};
//   cell->title->setText(item.title);
//   if (item.type == BrowserCellType::FOLDER)
//     cell->subtitle->setText("(folder)");
//   else
//     cell->subtitle->setText("");
//   return cell;
// }
//
// void BrowserSource::onItemSelected(brls::View* recycler, size_t index) {
//   auto& item = data[index];
//   brls::Logger::debug("File: {}, index: {}", item.path, std::to_string(index));
//   if (item.type == BrowserCellType::FOLDER) {
//     item.activity->modifyContents(item.path);
//   } else if (item.type == BrowserCellType::FILE || item.type == BrowserCellType::AUDIO) {
//     view::VideoPlayer* player = new view::VideoPlayer(item.path);
//     brls::sync([player]() { brls::Application::giveFocus(player); });
//   }
// }
//
// void BrowserSource::clearData() {
//   data.clear();
// }
//
// BrowserActivity::BrowserActivity() {
//   inflateFromXMLRes("xml/view/media_browser.xml");
//   this->content_frame->registerCell("cell", view::MediaCell::create);
// }
//
// void BrowserActivity::init() {
//   this->content_frame->setDataSource(getSource());
//   this->path_label->setText(curr_path);
// }
//
// void BrowserActivity::modifyContents(const std::string& path) {
//   curr_path = std::filesystem::path { path };
//   this->content_frame->setDataSource(getSource());
//   this->path_label->setText(curr_path);
//   // Focus first cell (maybe find a better way?)
//   brls::sync([this]() {brls::Application::giveFocus(content_frame);});
// }
//
// BrowserActivity::BrowserActivity(const std::string& path) : BrowserActivity() {
//   curr_path = std::filesystem::path { path };
// }
//
// FileBrowser::FileBrowser(const std::string& path) : BrowserActivity(path) {
//   this->init();
//   this->registerAction("Root Folder", brls::BUTTON_X, [this](...) {
//     if (curr_path.string() != "/")
//       modifyContents(curr_path.parent_path());
//     return true;
//   });
// }
//
// BrowserSource* FileBrowser::getSource() {
//   BrowserSource* source = new BrowserSource();
//   if (curr_path.string() != "/")
//     source->data = {
//       {"..", curr_path.parent_path(), BrowserCellType::FOLDER, this}
//     };
//
//   std::set<std::filesystem::path> content_dirs;
//   std::set<std::filesystem::path> content_files;
//
//   for (auto const& dir_entry : std::filesystem::directory_iterator{curr_path}) {
//     if (fileIsHidden(dir_entry.path()))
//       continue;
//
//     if (dir_entry.is_directory())
//       content_dirs.insert(dir_entry);
//     else
//       content_files.insert(dir_entry);
//   }
//
//   for (auto& file : content_dirs) {
//     source->data.push_back({
//       .title = file.filename(),
//       .path = file.string(),
//       .type = BrowserCellType::FOLDER,
//       .activity = this
//     });
//   }
//
//   for (auto& file : content_files) {
//     source->data.push_back({
//       .title = file.filename(),
//       .path = file.string(),
//       .type = BrowserCellType::FILE,
//       .activity = this
//     });
//   }
//
//   return source;
// }
//
// AlbumBrowser::AlbumBrowser(const int album_id) : BrowserActivity("Music") {
//   init();
//   this->album_id = album_id;
// }
//
// BrowserSource* AlbumBrowser::getSource() {
//   BrowserSource* source = new BrowserSource();
//   auto& db = core::Database::instance();
//
//   for (auto item : db.getMusic(album_id, core::Database::SortOrder::Desc)) {
//     source->data.push_back((BrowserCellData) {
//       .title = item.title,
//       .path = item.path,
//       .type = BrowserCellType::AUDIO,
//       .activity = this
//     });
//   }
//   return source;
// }

}
