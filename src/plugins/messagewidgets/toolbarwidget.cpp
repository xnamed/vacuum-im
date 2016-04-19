#include "toolbarwidget.h"

ToolBarWidget::ToolBarWidget(IMessageWindow *AWindow, QWidget *AParent) : QToolBar(AParent)
{
	FWindow = AWindow;
	FToolBarChanger = new ToolBarChanger(this);
#ifdef EYECU_MOBILE
    setIconSize(QSize(16*IconStorage::scale(),16*IconStorage::scale()));
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
