#include "toolbarwidget.h"

ToolBarWidget::ToolBarWidget(IMessageWindow *AWindow, QWidget *AParent) : QToolBar(AParent)
{
	FWindow = AWindow;
	FToolBarChanger = new ToolBarChanger(this);
#ifdef Q_OS_ANDROID
	setIconSize(QSize(32,32));
#else
	setIconSize(QSize(16,16));
#endif
}

ToolBarWidget::~ToolBarWidget()
{

}

bool ToolBarWidget::isVisibleOnWindow() const
{
	return isVisibleTo(FWindow->instance());
}

IMessageWindow *ToolBarWidget::messageWindow() const
{
	return FWindow;
}

ToolBarChanger *ToolBarWidget::toolBarChanger() const
{
	return FToolBarChanger;
}
