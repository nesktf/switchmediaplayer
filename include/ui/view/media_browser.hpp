#pragma once

#include "ui/extern/recycling_grid.hpp"

#include <filesystem>

namespace view {
//
// class BrowserActivity;
//
// enum class BrowserCellType {
//   AUDIO,
//   VIDEO,
//   IMAGE,
//   FOLDER,
//   PLAYLIST,
//   FILE
// };
//
// struct BrowserCellData {
//   std::string title;
//   std::string path; 
//   BrowserCellType type;
//   BrowserActivity* activity;
// };
//
// class BrowserSource : public view::RecyclingGridDataSource {
// public:
//   BrowserSource() {};
//   size_t getItemCount() override { return data.size(); };
//   view::RecyclingGridItem* cellForRow(view::RecyclingView* recycler, size_t index) override;
//   void onItemSelected(brls::View* recycler, size_t index) override;
//   void clearData() override;
//   std::vector<BrowserCellData> data;
// };
//
// class BrowserActivity : public brls::Box {
// public: BrowserActivity();
//   void init();
//   void modifyContents(const std::string& path);
//   BRLS_BIND(view::RecyclingGrid, content_frame, "media_browser/content"); protected:
//   BrowserActivity(const std::string& path);
//   virtual BrowserSource* getSource() = 0;
//   std::filesystem::path curr_path;
// protected:
//   BRLS_BIND(brls::Label, path_label, "media_browser/path_label");
// };
//
// class FileBrowser : public BrowserActivity {
// public:
//   FileBrowser(const std::string& path);
// protected:
//   BrowserSource* getSource() override;
// };

}
