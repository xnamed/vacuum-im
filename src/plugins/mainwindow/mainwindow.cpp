#include "mainwindow.h"
#include <QDebug>
#include <QTimer>
#include <QResizeEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <QGestureEvent>
#include <QMessageBox>
#include <definitions/version.h>
#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <definitions/optionvalues.h>
#include <definitions/mainwindowwidgets.h>
#include <utils/widgetmanager.h>
#include <utils/options.h>
#include <definitions/toolbargroups.h>		// *** <<< eyeCU >>> ***

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

	FSplitter = new QSplitter(this);
#ifndef EYECU_MOBILE	// *** <<< TEST---
	FSplitter->installEventFilter(this);
	FSplitter->setOrientation(Qt::Horizontal);
	FSplitterHandleWidth = FSplitter->handleWidth();
	connect(FSplitter,SIGNAL(splitterMoved(int,int)),SLOT(onSplitterMoved(int,int)));
	setCentralWidget(FSplitter);
#endif	// *** >>> TEST---
	FLeftWidget = new BoxWidget(this);
	FLeftWidget->layout()->setSpacing(0);
#ifdef EYECU_MOBILE	// *** <<< TEST---
    setCentralWidget(FLeftWidget);		// *** <<< eyeCU <<< ***
#else
	FSplitter->addWidget(FLeftWidget);
	FSplitter->setCollapsible(0,false);
	FSplitter->setStretchFactor(0,1);
#endif	// *** >>> TEST---

	FCentralWidget = new MainCentralWidget(this,this);
	FCentralWidget->instance()->setFrameShape(QFrame::StyledPanel);
	connect(FCentralWidget->instance(),SIGNAL(currentCentralPageChanged(IMainCentralPage *)),SLOT(onCurrentCentralPageChanged()));
	connect(FCentralWidget->instance(),SIGNAL(centralPageAppended(IMainCentralPage *)),SLOT(onCentralPageAddedOrRemoved(IMainCentralPage *)));
	connect(FCentralWidget->instance(),SIGNAL(centralPageRemoved(IMainCentralPage *)),SLOT(onCentralPageAddedOrRemoved(IMainCentralPage *)));

#ifndef EYECU_MOBILE	// *** <<< TEST---
	FSplitter->addWidget(FCentralWidget->instance());
	FSplitter->setCollapsible(1,false);
	FSplitter->setStretchFactor(1,4);
#endif	// *** >>> TEST---
	FCentralVisible = false;
#ifndef EYECU_MOBILE	// *** <<< TEST---
	FSplitter->setHandleWidth(0);
#endif	// *** >>> TEST---
	FCentralWidget->instance()->setVisible(false);

	FTabWidget = new MainTabWidget(FLeftWidget);
	FTabWidget->instance()->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	FLeftWidget->insertWidget(MWW_TABPAGES_WIDGET,FTabWidget->instance(),100);

	QToolBar *topToolbar = new QToolBar(this);
    topToolbar->setFloatable(false);
	topToolbar->setMovable(false);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
	topToolbar->setStyleSheet(QString("border:0; background-color:red; color:white;"));
#endif
// *** >>> eyeCU >>> ***

	ToolBarChanger *topChanger = new ToolBarChanger(topToolbar);
	topChanger->setSeparatorsVisible(false);
	insertToolBarChanger(MWW_TOP_TOOLBAR,topChanger);

	QToolBar *bottomToolbar =  new QToolBar(this);
	bottomToolbar->setFloatable(false);
	bottomToolbar->setMovable(false);

	ToolBarChanger *bottomChanger = new ToolBarChanger(bottomToolbar);
	bottomChanger->setSeparatorsVisible(false);
	insertToolBarChanger(MWW_BOTTOM_TOOLBAR,bottomChanger);

    FMainMenu = new Menu(this);
	FMainMenu->setIcon(RSR_STORAGE_MENUICONS,MNI_MAINWINDOW_MENU);
    FMainMenu->setTitle(tr("Main menu"));                                         // *** <<< eyeCU >>> ***
	QToolButton *button =
		topToolBarChanger()->insertAction(FMainMenu->menuAction(),TBG_MWTTB_MAINMENU);	// *** <<< eyeCU >>> ***
    button->setPopupMode(QToolButton::InstantPopup);
 // *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
    FMainMenuRight = new Menu(this);
    FMainMenuRight->setIcon(RSR_STORAGE_MENUICONS,MNI_MAINWINDOW_MENU_RIGHT);
    FMainMenuRight->setTitle(tr("Right menu"));
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
//	grabKeyboard();
////	grabGesture(Qt::TapAndHoldGesture);
////    grabGesture(Qt::PinchGesture);
//	grabGesture(Qt::PanGesture);
//	grabGesture(Qt::SwipeGesture);
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
Menu *MainWindow::mainMenuRight() const
{
    return FMainMenuRight;
}
// *** >>> eyeCU >>>***
MenuBarChanger *MainWindow::mainMenuBar() const
{
	return FMainMenuBar;
}

BoxWidget *MainWindow::mainLeftWidget() const
{
	return FLeftWidget;
}

IMainTabWidget *MainWindow::mainTabWidget() const
{
	return FTabWidget;
}

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
	IMainCentralPage *page = isCentralWidgetVisible() ? mainCentralWidget()->currentCentralPage() : NULL;
	if (page && !page->centralPageCaption().isEmpty())
		setWindowTitle(QString(CLIENT_NAME" - %1").arg(page->centralPageCaption()));
	else
		setWindowTitle(CLIENT_NAME);
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

void MainWindow::showEvent(QShowEvent *AEvent)
{
	QMainWindow::showEvent(AEvent);
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
}

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

/*
void MainWindow::keyPressEvent(QKeyEvent *AEvent)
{
	if(AEvent->key()==Qt::Key_Back){
qDebug()<<"######################--MainWindow::keyPressEvent"<<AEvent->key();
		AEvent->accept();//AEvent->ignore();
		//QFrame::keyPressEvent(AEvent);
		return;
	}
	return;// QMainWindow::event(AEvent);
}

void MainWindow::keyReleaseEvent(QKeyEvent *AEvent)
{
	if(AEvent->key()==Qt::Key_Back){
qDebug()<<"######################--MainWindow::keyReleaseEvent()"<<AEvent->key();
		//AEvent->accept();//AEvent->ignore();
		return;
	}
	return;// QMainWindow::event(AEvent);
}
*/
/*
void MainWindow::closeEvent(QCloseEvent *AEvent)
{
	QKeyEvent *keyEvent = static_cast<QKeyEvent *>(AEvent);
QEvent *event=static_cast<QMainWindow::close() *>(AEvent);
//QMainWindow::event(AEvent);
	QMessageBox msgBox;
	msgBox.setText("The program will be closed!");
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	int ret = msgBox.exec();
qDebug()<<"MainWindow::closeEvent/ret="<<ret;
	if(ret){
		return;
	}
	else{
		hide();
		AEvent->ignore();
	}
}
*/

// *** <<< eyeCU <<< ***
bool MainWindow::event(QEvent *AEvent)
{
/*
	if (AEvent->type() == QEvent::KeyPress)
	{
//		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(AEvent);
//		int keyValue = keyEvent->key();
	}
	else if(AEvent->type() == QEvent::KeyRelease)
	{
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(AEvent);
		int keyValue = keyEvent->key();
		if(keyValue==Qt::Key_Back)	//16777313
		{
qDebug()<<"######################--MainWindow::event()/KeyRelease"<<keyValue;
			QMessageBox::StandardButton reply;
			reply=QMessageBox::question(this, QString::fromUtf8("Сообщение"),
								QString::fromUtf8("The program will be closed"),
								QMessageBox::Yes | QMessageBox::No );
			if (reply == QMessageBox::Yes)
			{
qDebug()<<"QMessageBox::Yes";
				AEvent->accept();
			}
			else{ //reply == QMessageBox::No
qDebug()<<"QMessageBox::No";
				this->hide();
				AEvent->ignore();
			}
		}
	}
*/
//!------------------------------
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

void MainWindow::onUpdateCentralWidgetVisible()
{
	setCentralWidgetVisible(!FCentralWidget->centralPages().isEmpty());
}

void MainWindow::onCurrentCentralPageChanged()
{
	updateWindow();
}

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
