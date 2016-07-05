#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#ifdef EYECU_MOBILE
#include <QGestureEvent>				// <<< eyeCU <<<
#include <QTapAndHoldGesture>			// <<< eyeCU <<<
#else
#include <QSplitter>
#endif
#include <interfaces/imainwindow.h>
#include <interfaces/irostersview.h>
#include "maintabwidget.h"
#include "maincentralwidget.h"

class MainWindow :
	public QMainWindow,
	public IMainWindow
{
	Q_OBJECT;
	Q_INTERFACES(IMainWindow);
public:
	MainWindow(QWidget *AParent = NULL, Qt::WindowFlags AFlags = 0);
	~MainWindow();
	//IMainWindow
	virtual QMainWindow *instance() { return this; }
	virtual bool isActive() const;
	virtual void showWindow(bool AMinimized = false);
	virtual void closeWindow();
	// Menu Management
	virtual Menu *mainMenu() const;
// *** >>> eyeCU >>> ***
#ifdef EYECU_MOBILE
	virtual Menu *mainMenuRight() const;
#endif
// *** <<< eyeCU <<< ***
	virtual MenuBarChanger *mainMenuBar() const;
	// Widgets Management
	virtual BoxWidget *mainLeftWidget() const;
#ifndef EYECU_MOBILE
	virtual IMainTabWidget *mainTabWidget() const;
#endif
	virtual bool isCentralWidgetVisible() const;
	virtual IMainCentralWidget *mainCentralWidget() const;
	// Tool Bars Management
	virtual ToolBarChanger *topToolBarChanger() const;
	virtual ToolBarChanger *bottomToolBarChanger() const;
	virtual QList<ToolBarChanger *> toolBarChangers() const;
	virtual int toolBarChangerOrder(ToolBarChanger *AChanger) const;
	virtual ToolBarChanger *toolBarChangerByOrder(int AOrderId) const;
	virtual void insertToolBarChanger(int AOrderId, ToolBarChanger *AChanger);
	virtual void removeToolBarChanger(ToolBarChanger *AChanger);
#ifdef EYECU_MOBILE
	virtual bool event(QEvent *AEvent);
#endif
signals:
	void toolBarChangerInserted(int AOrderId, ToolBarChanger *AChanger);
	void toolBarChangerRemoved(ToolBarChanger *AChanger);
#ifndef EYECU_MOBILE
	void centralWidgetVisibleChanged(bool AVisible);
#endif
public:
	void saveWindowGeometryAndState();
	void loadWindowGeometryAndState();
protected:
	void updateWindow();
	QMenu *createPopupMenu();
	void correctWindowPosition();
	void restoreAcceptDrops(QWidget *AParent);
#ifndef EYECU_MOBILE
	void setCentralWidgetVisible(bool AVisible);
#endif
protected:
	void showEvent(QShowEvent *AEvent);
// *** <<< eyeCU <<< ***
#ifndef EYECU_MOBILE
	bool eventFilter(QObject *AObject, QEvent *AEvent);
#else
	// Gesture Handler
	void closeEvent(QCloseEvent *AEvent);
	bool gestureEvent(QGestureEvent *AEvent);
	void tapAndHoldGesture(QTapAndHoldGesture *AGesture);
// *** >>> eyeCU >>> ***
#endif
protected slots:
#ifndef EYECU_MOBILE
	void onUpdateCentralWidgetVisible();
	void onSplitterMoved(int APos, int AIndex);
	void onCentralPageAddedOrRemoved(IMainCentralPage *APage);
#endif
	void onCurrentCentralPageChanged(IMainCentralPage *APage);
private:
#ifdef EYECU_MOBILE
	QList<IMainCentralPage *> FCentralPageOpenStack;
#else
	IMainTabWidget *FTabWidget;
#endif
	IMainCentralWidget *FCentralWidget;
private:
	Menu *FMainMenu;
// *** <<< eyeCU <<< ***
#ifndef EYECU_MOBILE
	QSplitter *FSplitter;
#else
	Menu *FMainMenuRight;
#endif
// *** >>> eyeCU >>> ***
	BoxWidget *FLeftWidget;
	MenuBarChanger *FMainMenuBar;
private:
	bool FAligned;
	bool FCentralVisible;
	int FLeftWidgetWidth;
	int FSplitterHandleWidth;
	QMap<int, ToolBarChanger *> FToolBarOrders;
};

#endif // MAINWINDOW_H
