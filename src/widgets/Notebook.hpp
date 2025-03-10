#pragma once

#include "pajlada/signals/signal.hpp"
#include "widgets/BaseWidget.hpp"

#include <QList>
#include <QMenu>
#include <QMessageBox>
#include <QWidget>
#include <pajlada/signals/signalholder.hpp>

namespace chatterino {

class Window;
class UpdateDialog;
class NotebookButton;
class NotebookTab;
class SplitContainer;

enum NotebookTabLocation { Top = 0, Left = 1, Right = 2, Bottom = 3 };

class Notebook : public BaseWidget
{
    Q_OBJECT

public:
    explicit Notebook(QWidget *parent);
    ~Notebook() override = default;

    NotebookTab *addPage(QWidget *page, QString title = QString(),
                         bool select = false);
    void removePage(QWidget *page);
    void removeCurrentPage();

    int indexOf(QWidget *page) const;
    virtual void select(QWidget *page, bool focusPage = true);
    void selectIndex(int index, bool focusPage = true);
    void selectNextTab(bool focusPage = true);
    void selectPreviousTab(bool focusPage = true);
    void selectLastTab(bool focusPage = true);

    int getPageCount() const;
    QWidget *getPageAt(int index) const;
    int getSelectedIndex() const;
    QWidget *getSelectedPage() const;

    QWidget *tabAt(QPoint point, int &index, int maxWidth = 2000000000);
    void rearrangePage(QWidget *page, int index);

    bool getAllowUserTabManagement() const;
    void setAllowUserTabManagement(bool value);

    bool getShowTabs() const;
    void setShowTabs(bool value);

    bool getShowAddButton() const;
    void setShowAddButton(bool value);

    void performLayout(bool animate = false);

    void setTabLocation(NotebookTabLocation location);

    bool isNotebookLayoutLocked() const;
    void setLockNotebookLayout(bool value);

    void addNotebookActionsToMenu(QMenu *menu);

protected:
    virtual void scaleChangedEvent(float scale_) override;
    virtual void resizeEvent(QResizeEvent *) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *) override;

    NotebookButton *getAddButton();
    NotebookButton *addCustomButton();

    struct Item {
        NotebookTab *tab{};
        QWidget *page{};
        QWidget *selectedWidget{};
    };

    const QList<Item> items()
    {
        return items_;
    }

private:
    void updateTabVisibilityMenuAction();
    void resizeAddButton();

    bool containsPage(QWidget *page);
    Item &findItem(QWidget *page);

    static bool containsChild(const QObject *obj, const QObject *child);
    NotebookTab *getTabFromPage(QWidget *page);

    // Returns the number of buttons in `customButtons_` that are visible
    size_t visibleButtonCount() const;

    QList<Item> items_;
    QMenu menu_;
    QWidget *selectedPage_ = nullptr;

    NotebookButton *addButton_;
    std::vector<NotebookButton *> customButtons_;

    bool allowUserTabManagement_ = false;
    bool showTabs_ = true;
    bool showAddButton_ = false;
    int lineOffset_ = 20;
    bool lockNotebookLayout_ = false;
    NotebookTabLocation tabLocation_ = NotebookTabLocation::Top;
    QAction *lockNotebookLayoutAction_;
    QAction *showTabsAction_;
};

class SplitNotebook : public Notebook
{
public:
    SplitNotebook(Window *parent);

    SplitContainer *addPage(bool select = false);
    SplitContainer *getOrAddSelectedPage();
    void select(QWidget *page, bool focusPage = true) override;

protected:
    void showEvent(QShowEvent *event) override;

private:
    void addCustomButtons();

    pajlada::Signals::SignalHolder signalHolder_;
};

}  // namespace chatterino
