#pragma once

#include <filesystem>

#include <borealis.hpp>

#include "ui/view/recycling_grid.hpp"

namespace view {

class BrowserActivity;

enum class BrowserCellType {
  AUDIO,
  VIDEO,
  IMAGE,
  FOLDER,
  PLAYLIST,
  FILE
};

struct BrowserCellData {
  std::string title;
  std::string path; 
  BrowserCellType type;
  BrowserActivity* activity;
};

class BrowserSource : public view::RecyclingGridDataSource {
public:
  BrowserSource() {};
  size_t getItemCount() override { return data.size(); };
  view::RecyclingGridItem* cellForRow(view::RecyclingView* recycler, size_t index) override;
  void onItemSelected(brls::View* recycler, size_t index) override;
  void clearData() override;
  std::vector<BrowserCellData> data;
};

class BrowserActivity : public brls::Box {
public:
  BrowserActivity();
  void init();
  void modifyPath(const std::string& path);
  BRLS_BIND(view::RecyclingGrid, content_frame, "frame/content"); protected:
  BrowserActivity(const std::string& path);
  virtual BrowserSource* parseFolder() = 0;
  std::filesystem::path curr_path;
protected:
  BRLS_BIND(brls::Label, path_label, "view/browser/path_label");
};

class FileBrowser : public BrowserActivity {
public:
  FileBrowser(const std::string& path);
protected:
  BrowserSource* parseFolder() override;
};

}
