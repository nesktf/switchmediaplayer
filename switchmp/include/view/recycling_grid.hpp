//
// Created by fang on 2022/6/15.
//

#pragma once

#include <borealis.hpp>

class RecyclingView;

class RecyclingGridItem : public brls::Box {
public:
    RecyclingGridItem();
    ~RecyclingGridItem() override;

    /*
     * Cell's position inside recycler frame
     */
    size_t getIndex() const;

    /*
     * DO NOT USE! FOR INTERNAL USAGE ONLY!
     */
    void setIndex(size_t value);

    /*
     * A string used to identify a cell that is reusable.
     */
    std::string reuseIdentifier;

    /*
     * Prepares a reusable cell for reuse by the recycler frame's data source.
     */
    virtual void prepareForReuse() {}

    /*
     * 表单项回收
     */
    virtual void cacheForReuse() {}

private:
    size_t index;
};

class RecyclingGridDataSource {
public:
    virtual ~RecyclingGridDataSource() = default;

    /*
     * Tells the data source to return the number of items in a recycler frame.
     */
    virtual size_t getItemCount() { return 0; }

    /*
     * Asks the data source for a cell to insert in a particular location of the recycler frame.
     */
    virtual RecyclingGridItem* cellForRow(RecyclingView* recycler, size_t index) { return nullptr; }

    /*
     * Asks the data source for the height to use for a row in a specified location.
     * Return -1 to use autoscaling.
     */
    virtual float heightForRow(brls::View* recycler, size_t index) { return -1; }

    /*
     * Tells the data source a row is selected.
     */
    virtual void onItemSelected(brls::View* recycler, size_t index) {}

    virtual void clearData() = 0;
};

class RecyclingView {
public:
    virtual ~RecyclingView() = default;

    void registerCell(std::string identifier, std::function<RecyclingGridItem*()> allocation);

    RecyclingGridItem* dequeueReusableCell(std::string identifier);

    RecyclingGridDataSource* getDataSource() const;

protected:
    // 回收列表项
    void queueReusableCell(RecyclingGridItem* cell);

    RecyclingGridDataSource* dataSource = nullptr;

    std::map<std::string, std::vector<RecyclingGridItem*>*> queueMap;
    std::map<std::string, std::function<RecyclingGridItem*(void)>> allocationMap;
};

class RecyclingGridContentBox;

class RecyclingGrid : public brls::ScrollingFrame, public RecyclingView {
public:
    RecyclingGrid();

    void draw(NVGcontext* vg, float x, float y, float width, float height, brls::Style style,
        brls::FrameContext* ctx) override;

    void setDefaultCellFocus(size_t index);

    size_t getDefaultCellFocus() const;

    void setDataSource(RecyclingGridDataSource* source);

    void showSkeleton(unsigned int num = 12);

    // 重新加载数据
    void reloadData();

    void notifyDataChanged();

    /// 获取当前指定索引数据所在的item指针
    /// 注意，因为是循环使用列表项的，所以此指针只能在获取时刻在主线程内使用
    RecyclingGridItem* getGridItemByIndex(size_t index);

    void clearData();

    void setEmpty(std::string msg = "");

    void setError(std::string error = "");

    void selectRowAt(size_t index, bool animated);

    // 计算从start元素的顶点到index (不包含index) 元素顶点的距离
    float getHeightByCellIndex(size_t index, size_t start = 0);

    View* getNextCellFocus(brls::FocusDirection direction, View* currentView);

    void forceRequestNextPage();

    void onLayout() override;

    /// 当前数据总数量
    size_t getItemCount();

    /// 当前数据总行数
    size_t getRowCount();

    /// 导航到页面尾部时触发回调函数
    void onNextPage(const std::function<void()>& callback = nullptr);

    void setPadding(float padding) override;
    void setPadding(float top, float right, float bottom, float left) override;
    void setPaddingTop(float top) override;
    void setPaddingRight(float right) override;
    void setPaddingBottom(float bottom) override;
    void setPaddingLeft(float left) override;

    ~RecyclingGrid() override;

    static View* create();

    /// 元素间距
    float estimatedRowSpace = 20;

    /// 默认行高(元素实际高度 = 默认行高 - 元素间隔)
    float estimatedRowHeight = 240;

    /// 列数
    int spanCount = 4;

    /// 预取的行数
    int preFetchLine = 1;

    /// 瀑布流模式，每一项高度不固定（仅在spanCount为1时可用）
    bool isFlowMode = false;

private:
    bool layouted = false;
    float oldWidth = -1;

    bool requestNextPage = false;
    // true表示正在请求下一页，此时不会再次触发下一页请求
    // 数据为空时不请求下一页，因为有些时候首页和下一页请求的内容或方式不同
    // 当列表元素有变动时（添加或修改数据源，会重置为false，这是将允许请求下一页）

    uint32_t visibleMin, visibleMax;
    size_t defaultCellFocus = 0;

    float paddingTop = 0;
    float paddingRight = 0;
    float paddingBottom = 0;
    float paddingLeft = 0;

    std::function<void()> nextPageCallback = nullptr;

    RecyclingGridContentBox* contentBox = nullptr;
    brls::Image* hintImage;
    brls::Label* hintLabel;
    brls::Rect renderedFrame;
    std::vector<float> cellHeightCache;

    //检查宽度是否有变化
    bool checkWidth();

    void itemsRecyclingLoop();

    void addCellAt(size_t index, int downSide);
};

class RecyclingGridContentBox : public brls::Box {
public:
    RecyclingGridContentBox(RecyclingGrid* recycler);
    brls::View* getNextFocus(brls::FocusDirection direction, brls::View* currentView) override;

private:
    RecyclingGrid* recycler;
};

class SkeletonCell : public RecyclingGridItem {
public:
    SkeletonCell();

    static RecyclingGridItem* create();

    void draw(NVGcontext* vg, float x, float y, float width, float height, brls::Style style,
        brls::FrameContext* ctx) override;

private:
    NVGcolor background = brls::Application::getTheme()["color/grey_3"];
};