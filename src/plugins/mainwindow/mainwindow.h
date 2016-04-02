#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QSplitter>
#include <QGestureEvent>				// <<< eyeCU <<<
#include <QTapAndHoldGesture>			// <<< eyeCU <<<
#include <interfaces/imainwindow.h>
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
	virtual Menu *mainMenuRight() const;		// <<< eyeCU <<<
	virtual MenuBarChanger *mainMenuBar() const;
	// Widgets Management
	virtual BoxWidget *mainLeftWidget() const;
#ifndef EYECU_MOBILE
	virtual IMainTabWidget *mainTabWidget() const;
	virtual bool isCentralWidgetVisible() const;
#endif
	virtual IMainCentralWidget *mainCentralWidget() const;
	// Tool Bars Management
	virtual ToolBarChanger *topToolBarChanger() const;
	virtual ToolBarChanger *bottomToolBarChanger() const;
	virtual QList<ToolBarChanger *> toolBarChangers() const;
	virtual int toolBarChangerOrder(ToolBarChanger *AChanger) const;
	virtual ToolBarChanger *toolBarChangerByOrder(int AOrderId) const;
	virtual void insertToolBarChanger(int AOrderId, ToolBarChanger *AChanger);
	virtual void removeToolBarChanger(ToolBarChanger *AChanger);
	virtual bool event(QEvent *AEvent);
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
#ifndef EYECU_MOBILE
	bool eventFilter(QObject *AObject, QEvent *AEvent);
#endif
// *** <<< eyeCU <<< ***
	// Gesture Handler
	void closeEvent(QCloseEvent *AEvent);
//	void keyPressEvent(QKeyEvent *AEvent) Q_DECL_OVERRIDE;
//	void keyReleaseEvent(QKeyEvent * AEvent);
	bool gestureEvent(QGestureEvent *AEvent);				// <<< eyeCU <<<
	void tapAndHoldGesture(QTapAndHoldGesture *AGesture);
// *** >>> eyeCU >>> ***
protected slots:
#ifndef EYECU_MOBILE
	void onUpdateCentralWidgetVisible();
	void onSplitterMoved(int APos, int AIndex);
#endif
	void onCurrentCentralPageChanged();
	void onCentralPageAddedOrRemoved(IMainCentralPage *APage);
private:
#ifndef EYECU_MOBILE
	IMainTabWidget *FTabWidget;
#endif
	IMainCentralWidget *FCentralWidget;
private:
	Menu *FMainMenu;
	Menu *FMainMenuRight;		// *** <<< eyeCU <<< ***
#ifndef EYECU_MOBILE
	QSplitter *FSplitter;
#endif
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
