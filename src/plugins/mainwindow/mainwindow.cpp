#include "mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QGestureEvent>
#include <QMessageBox>
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include <QLabel>
#endif
// *** >>> eyeCU >>> ***
#include <definitions/version.h>
#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <definitions/optionvalues.h>
#include <definitions/mainwindowwidgets.h>
// *** <<< eyeCU <<< ***
#include <definitions/toolbargroups.h>
#include <interfaces/irostersview.h>
// *** >>> eyeCU >>> ***
#include <utils/widgetmanager.h>
#include <utils/options.h>

#define ONE_WINDOW_MODE_OPTIONS_NS "one-window-mode"

MainWindow::MainWindow(QWidget *AParent, Qt::WindowFlags AFlags) : QMainWindow(AParent,AFlags)
{
    setWindowFlags(Qt::WindowCloseButtonHint); // *** <<< eyeCU >>> ***
	setWindowRole("MainWindow");
	setAttribute(Qt::WA_DeleteOnClose,false);
    IconStorage *iconStorage = IconStorage::staticStorage(RSR_STORAGE_MENUICONS);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    int size=16*(iconStorage->scale()+1);
    setIconSize(QSize(size,size));
//	QString mainWindowStyle=QString("background-color:#F4F0F0;");
	QString topToolbarStyle=QString("border:0; background-color:#08AC07; color:white;");
	QString bottomToolbarStyle=QString("border:0; background-color:#08AC07; color:white;");
	QString menuMainRightStyle=QString("background-color:#A4F9A3; color:white;");//C6FAC6

//	setStyleSheet(mainWindowStyle);
#else
// *** >>> eyeCU >>> ***
	setIconSize(QSize(16,16));
#endif

	FAligned = false;
	FLeftWidgetWidth = 0;

	QIcon icon;
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO16), QSize(16,16));
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO24), QSize(24,24));
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO32), QSize(32,32));
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO48), QSize(48,48));
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO64), QSize(64,64));
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO96), QSize(96,96));
	icon.addFile(iconStorage->fileFullName(MNI_MAINWINDOW_LOGO128), QSize(128,128));
	setWindowIcon(icon);

	FLeftWidget = new BoxWidget(this);
	FLeftWidget->layout()->setSpacing(0);
#ifndef EYECU_MOBILE	// *** <<< TEST---
	FSplitter = new QSplitter(this);
	FSplitter->installEventFilter(this);
	FSplitter->setOrientation(Qt::Horizontal);
	FSplitterHandleWidth = FSplitter->handleWidth();
	connect(FSplitter,SIGNAL(splitterMoved(int,int)),SLOT(onSplitterMoved(int,int)));
	setCentralWidget(FSplitter);
	FSplitter->addWidget(FLeftWidget);
	FSplitter->setCollapsible(0,false);
	FSplitter->setStretchFactor(0,1);
#else
	setCentralWidget(FLeftWidget);
#endif	// *** >>> TEST---

	FCentralWidget = new MainCentralWidget(this,this);
	FCentralWidget->instance()->setFrameShape(QFrame::StyledPanel);
	connect(FCentralWidget->instance(),SIGNAL(currentCentralPageChanged(IMainCentralPage *)),SLOT(onCurrentCentralPageChanged(IMainCentralPage *)));
#ifndef EYECU_MOBILE	// *** <<< TEST---
	connect(FCentralWidget->instance(),SIGNAL(centralPageAppended(IMainCentralPage *)),SLOT(onCentralPageAddedOrRemoved(IMainCentralPage *)));
	connect(FCentralWidget->instance(),SIGNAL(centralPageRemoved(IMainCentralPage *)),SLOT(onCentralPageAddedOrRemoved(IMainCentralPage *)));

	FSplitter->addWidget(FCentralWidget->instance());
	FSplitter->setCollapsible(1,false);
	FSplitter->setStretchFactor(1,4);
	FCentralVisible = false;
	FSplitter->setHandleWidth(0);
	FTabWidget = new MainTabWidget(FLeftWidget);
	FTabWidget->instance()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	FLeftWidget->insertWidget(MWW_TABPAGES_WIDGET,FTabWidget->instance(),100);
	FCentralWidget->instance()->setVisible(false);
#else
	FCentralVisible = true;
	FLeftWidget->insertWidget(MWW_TABPAGES_WIDGET,FCentralWidget->instance(),100);
	FCentralWidget->instance()->setVisible(true);
#endif	// *** >>> TEST---	

	QToolBar *topToolbar = new QToolBar(this);
	topToolbar->setIconSize(QSize(48,48));
    topToolbar->setFloatable(false);
	topToolbar->setMovable(false);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    topToolbar->setStyleSheet(topToolbarStyle);
#endif
// *** >>> eyeCU >>> ***
	ToolBarChanger *topChanger = new ToolBarChanger(topToolbar);
	topChanger->setSeparatorsVisible(false);
	insertToolBarChanger(MWW_TOP_TOOLBAR,topChanger);

	QToolBar *bottomToolbar =  new QToolBar(this);
	//bottomToolbar->setIconSize();
	bottomToolbar->setFloatable(false);
	bottomToolbar->setMovable(false);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    bottomToolbar->setStyleSheet(bottomToolbarStyle);
#endif
// *** >>> eyeCU >>> ***
	ToolBarChanger *bottomChanger = new ToolBarChanger(bottomToolbar);
	bottomChanger->setSeparatorsVisible(false);
	insertToolBarChanger(MWW_BOTTOM_TOOLBAR,bottomChanger);

    FMainMenu = new Menu(this);
	FMainMenu->setIcon(RSR_STORAGE_MENUICONS,MNI_MAINWINDOW_MENU);
// *** <<< eyeCU <<< ***
	FMainMenu->setTitle(tr("Main menu"));
//	FMainMenu->setStyleSheet(menuMainRightStyle);
	QToolButton *button = topToolBarChanger()->insertAction(FMainMenu->menuAction(),TBG_MWTTB_MAINMENU);
// *** >>> eyeCU >>> ***
    button->setPopupMode(QToolButton::InstantPopup);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
	FMainMenuRight = new Menu(this);
    FMainMenuRight->setIcon(RSR_STORAGE_MENUICONS,MNI_MAINWINDOW_MENU_RIGHT);
    FMainMenuRight->setTitle(tr("Right menu"));
	FMainMenuRight->setStyleSheet(menuMainRightStyle);
	button = topToolBarChanger()->insertAction(FMainMenuRight->menuAction(),TBG_MWTTB_RIGHTMENU);
    button->setPopupMode(QToolButton::InstantPopup);

	QLabel *title = new QLabel(CLIENT_NAME);
	topToolBarChanger()->insertWidget(title, TBG_MWTTB_TITLE);
#endif
// *** >>> eyeCU >>>***

	FMainMenuBar = new MenuBarChanger(new QMenuBar());
	setMenuBar(FMainMenuBar->menuBar());
	updateWindow();

// *** <<< eyeCU <<< ***
    grabGesture(Qt::TapAndHoldGesture);
////    grabGesture(Qt::PinchGesture);
// *** >>> eyeCU >>>***
}

MainWindow::~MainWindow()
{
	delete FMainMenuBar->menuBar();
}

bool MainWindow::isActive() const
{
	return WidgetManager::isActiveWindow(this);
}

void MainWindow::showWindow(bool AMinimized)
{
	if (!Options::isNull())
	{
		if (!AMinimized)
			WidgetManager::showActivateRaiseWindow(this);
		else if (!isVisible())
			showMinimized();

		if (!FAligned)
		{
			FAligned = true;
			QString ns = isCentralWidgetVisible() ? ONE_WINDOW_MODE_OPTIONS_NS : "";
			WidgetManager::alignWindow(this,(Qt::Alignment)Options::fileValue("mainwindow.align",ns).toInt());
		}

		correctWindowPosition();
	}
}

void MainWindow::closeWindow()
{
    close();
}

Menu *MainWindow::mainMenu() const
{
	return FMainMenu;
}

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
Menu *MainWindow::mainMenuRight() const
{
    return FMainMenuRight;
}
#endif
// *** >>> eyeCU >>>***
MenuBarChanger *MainWindow::mainMenuBar() const
{
	return FMainMenuBar;
}

BoxWidget *MainWindow::mainLeftWidget() const
{
	return FLeftWidget;
}
#ifndef EYECU_MOBILE
IMainTabWidget *MainWindow::mainTabWidget() const
{
	return FTabWidget;
}
#endif
bool MainWindow::isCentralWidgetVisible() const
{
	return FCentralVisible;
}

IMainCentralWidget * MainWindow::mainCentralWidget() const
{
	return FCentralWidget;
}

ToolBarChanger *MainWindow::topToolBarChanger() const
{
	return toolBarChangerByOrder(MWW_TOP_TOOLBAR);
}

ToolBarChanger *MainWindow::bottomToolBarChanger() const
{
	return toolBarChangerByOrder(MWW_BOTTOM_TOOLBAR);
}

QList<ToolBarChanger *> MainWindow::toolBarChangers() const
{
	return FToolBarOrders.values();
}

int MainWindow::toolBarChangerOrder(ToolBarChanger *AChanger) const
{
	return FToolBarOrders.key(AChanger);
}

ToolBarChanger *MainWindow::toolBarChangerByOrder(int AOrderId) const
{
	return FToolBarOrders.value(AOrderId);
}

void MainWindow::insertToolBarChanger(int AOrderId, ToolBarChanger *AChanger)
{
	if (FLeftWidget->widgetByOrder(AOrderId) == NULL)
	{
		AChanger->toolBar()->setIconSize(iconSize());
		FLeftWidget->insertWidget(AOrderId,AChanger->toolBar());
		FToolBarOrders.insert(AOrderId,AChanger);
		emit toolBarChangerInserted(AOrderId,AChanger);
	}
}

void MainWindow::removeToolBarChanger(ToolBarChanger *AChanger)
{
	if (toolBarChangers().contains(AChanger))
	{
		FLeftWidget->removeWidget(AChanger->toolBar());
		FToolBarOrders.remove(toolBarChangerOrder(AChanger));
		emit toolBarChangerRemoved(AChanger);
	}
}

void MainWindow::saveWindowGeometryAndState()
{
	QString ns = isCentralWidgetVisible() ? ONE_WINDOW_MODE_OPTIONS_NS : "";
	if (isCentralWidgetVisible() && FLeftWidgetWidth>0)
		Options::setFileValue(FLeftWidgetWidth,"mainwindow.left-frame-width",ns);
	Options::setFileValue(saveGeometry(),"mainwindow.geometry",ns);
	Options::setFileValue((int)WidgetManager::windowAlignment(this),"mainwindow.align",ns);
}

void MainWindow::loadWindowGeometryAndState()
{
	FAligned = false;
	QString ns = isCentralWidgetVisible() ? ONE_WINDOW_MODE_OPTIONS_NS : "";
	if (!restoreGeometry(Options::fileValue("mainwindow.geometry",ns).toByteArray()))
	{
		if (isCentralWidgetVisible())
		{
			FLeftWidgetWidth = 200;
			Options::setFileValue(0,"mainwindow.align",ns);
			setGeometry(WidgetManager::alignGeometry(QSize(800,600),this,Qt::AlignCenter));
		}
		else
		{
			Options::setFileValue((int)(Qt::AlignRight|Qt::AlignBottom),"mainwindow.align",ns);
			setGeometry(WidgetManager::alignGeometry(QSize(200,600),this,Qt::AlignRight|Qt::AlignBottom));
		}
	}
	else if (isCentralWidgetVisible())
	{
		FLeftWidgetWidth = Options::fileValue("mainwindow.left-frame-width",ns).toInt();
	}
}

void MainWindow::updateWindow()
{
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
	QLabel *title;
	QList<QAction *> actions = topToolBarChanger()->groupItems(TBG_MWTTB_TITLE);
	if (!actions.isEmpty())
		title = qobject_cast<QLabel *>(topToolBarChanger()->handleWidget(actions.first()));
#define SET_TITLE(T) title->setText(T)
#else
#define SET_TITLE(T) setWindowTitle(T)
#endif
// *** >>> eyeCU >>> ***
	IMainCentralPage *page = isCentralWidgetVisible() ? mainCentralWidget()->currentCentralPage() : NULL;
	if (page && !page->centralPageCaption().isEmpty())
// *** <<< eyeCU <<< ***
		SET_TITLE(QString(CLIENT_NAME" - %1").arg(page->centralPageCaption()));
	else
		SET_TITLE(CLIENT_NAME);
// *** >>> eyeCU >>> ***
}

QMenu *MainWindow::createPopupMenu()
{
	return NULL;
}

void MainWindow::correctWindowPosition()
{
	QRect windowRect = geometry();
	QRect screenRect = qApp->desktop()->availableGeometry(this);
	if (!screenRect.isEmpty() && !windowRect.isEmpty())
	{
		Qt::Alignment align = 0;
		if (windowRect.right() <= screenRect.left())
			align |= Qt::AlignLeft;
		else if (windowRect.left() >= screenRect.right())
			align |= Qt::AlignRight;
		if (windowRect.top() >= screenRect.bottom())
			align |= Qt::AlignBottom;
		else if (windowRect.bottom() <= screenRect.top())
			align |= Qt::AlignTop;
		WidgetManager::alignWindow(this,align);
	}
}

void MainWindow::restoreAcceptDrops(QWidget *AParent)
{
#ifndef EYECU_MOBILE //  Q_OS_WIN
	foreach(QObject *object, AParent->children())
	{
		if (object->isWidgetType())
		{
			QWidget *childWidget = qobject_cast<QWidget *>(object);
			if (childWidget->acceptDrops())
			{
				childWidget->setAcceptDrops(false);
				childWidget->setAcceptDrops(true);
			}
			restoreAcceptDrops(childWidget);
		}
	}
#else
	Q_UNUSED(AParent);
#endif
}
#ifndef EYECU_MOBILE
void MainWindow::setCentralWidgetVisible(bool AVisible)
{
	if (AVisible != FCentralVisible)
	{
		bool windowVisible = isVisible();
		saveWindowGeometryAndState();
		closeWindow();
		
		FCentralVisible = AVisible;
		if (AVisible)
		{
			FSplitter->setHandleWidth(FSplitterHandleWidth);
			FLeftWidget->setFrameShape(QFrame::StyledPanel);
			FCentralWidget->instance()->setVisible(true);
			setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint); // *** <<< eyeCU >>> ***
		}
		else
		{
			FSplitter->setHandleWidth(0);
			FLeftWidget->setFrameShape(QFrame::NoFrame);
			FCentralWidget->instance()->setVisible(false);
			setWindowFlags(Qt::WindowCloseButtonHint); // *** <<< eyeCU >>> ***
		}

		updateWindow();
		setMinimumSize(0,0);
		restoreAcceptDrops(this);
		loadWindowGeometryAndState();

		if (windowVisible)
			showWindow();

		emit centralWidgetVisibleChanged(AVisible);
	}
}
#endif

void MainWindow::showEvent(QShowEvent *AEvent)
{
	QMainWindow::showEvent(AEvent);
#ifndef EYECU_MOBILE // *** <<< eyeCU >>> ***
	if (isCentralWidgetVisible())
	{
		QList<int> splitterSizes = FSplitter->sizes();
		int leftIndex = FSplitter->indexOf(FLeftWidget);
		int rightIndex = FSplitter->indexOf(FCentralWidget->instance());
		if (FLeftWidgetWidth>0 && leftIndex>=0 && rightIndex>=0 && splitterSizes.value(leftIndex)!=FLeftWidgetWidth)
		{
			splitterSizes[rightIndex] += splitterSizes.value(leftIndex) - FLeftWidgetWidth;
			splitterSizes[leftIndex] = FLeftWidgetWidth;
			FSplitter->setSizes(splitterSizes);
		}
	}
#endif // *** <<< eyeCU >>> ***
}
#ifndef EYECU_MOBILE // *** <<< eyeCU >>> ***
bool MainWindow::eventFilter(QObject *AObject, QEvent *AEvent)
{
	if (AObject == FSplitter)
	{
		if (isCentralWidgetVisible() && AEvent->type()==QEvent::Resize)
		{
			int leftIndex = FSplitter->indexOf(FLeftWidget);
			int rightIndex = FSplitter->indexOf(FCentralWidget->instance());
			QResizeEvent *resizeEvent = static_cast<QResizeEvent *>(AEvent);
			if (resizeEvent && FLeftWidgetWidth>0 && leftIndex>=0 && rightIndex>=0 && resizeEvent->oldSize().width()>0)
			{
				double k = (double)resizeEvent->size().width() / resizeEvent->oldSize().width();
				QList<int> splitterSizes = FSplitter->sizes();
				for (int i=0; i<splitterSizes.count(); i++)
					splitterSizes[i] = qRound(splitterSizes[i]*k);
				if (splitterSizes.value(leftIndex) != FLeftWidgetWidth)
				{
					splitterSizes[rightIndex] += splitterSizes.value(leftIndex) - FLeftWidgetWidth;
					splitterSizes[leftIndex] = FLeftWidgetWidth;
					FSplitter->setSizes(splitterSizes);
				}
			}
		}
	}
	return QMainWindow::eventFilter(AObject,AEvent);
}

void MainWindow::onUpdateCentralWidgetVisible()
{
	setCentralWidgetVisible(!FCentralWidget->centralPages().isEmpty());
}
// *** <<< eyeCU <<< ***
#else
void MainWindow::closeEvent(QCloseEvent *AEvent)
{
	AEvent->ignore();

	if (FCentralPageOpenStack.size()>1)
	{
		FCentralPageOpenStack.removeLast();
		FCentralWidget->setCurrentCentralPage(FCentralPageOpenStack.last());
	}
	else
		showMinimized();
}
bool MainWindow::event(QEvent *AEvent)
{
	if (AEvent->type() == QEvent::Gesture)
		return gestureEvent(static_cast<QGestureEvent*>(AEvent));
	return QMainWindow::event(AEvent);
}

bool MainWindow::gestureEvent(QGestureEvent *AEvent)
{
/*! Reserve for other gestures
	if (QGesture *pinch = AEvent->gesture(Qt::PinchGesture))
		pinchTriggered(static_cast<QPinchGesture *>(pinch));
*/
	if(QGesture *gesture = AEvent->gesture(Qt::TapAndHoldGesture))
		tapAndHoldGesture(static_cast<QTapAndHoldGesture *>(gesture));
	AEvent->accept();
	return true;
}

void MainWindow::tapAndHoldGesture(QTapAndHoldGesture *AGesture)
{
	if (AGesture)
	{
		if (AGesture->state()==Qt::GestureFinished)
		{
			QPoint hotSpot(AGesture->hotSpot().toPoint());
			QWidget *child = childAt(mapFromGlobal(hotSpot));
			if (child)
				QCoreApplication::postEvent(child,
				   new QContextMenuEvent(QContextMenuEvent::Other,child->mapFromGlobal(hotSpot),hotSpot,Qt::NoModifier));
		}
	}
}
// *** >>> eyeCU >>> ***

#endif
void MainWindow::onCurrentCentralPageChanged(IMainCentralPage *APage)
{
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
	if (FCentralPageOpenStack.contains(APage))
		FCentralPageOpenStack.removeAll(APage);
	FCentralPageOpenStack.append(APage);
#endif
// *** >>> eyeCU >>> ***
	updateWindow();
}
#ifndef EYECU_MOBILE
void MainWindow::onCentralPageAddedOrRemoved(IMainCentralPage *APage)
{
	Q_UNUSED(APage);

	QTimer::singleShot(0,this,SLOT(onUpdateCentralWidgetVisible()));
}

void MainWindow::onSplitterMoved(int APos, int AIndex)
{
	Q_UNUSED(APos); Q_UNUSED(AIndex);
	FLeftWidgetWidth = FSplitter->sizes().value(FSplitter->indexOf(FLeftWidget));
}
#endif
