#include "maincentralwidget.h"

MainCentralWidget::MainCentralWidget(IMainWindow *AMainWindow, QWidget *AParent) : QStackedWidget(AParent)
{
	FMainWindow = AMainWindow;
	connect(this,SIGNAL(currentChanged(int)),SLOT(onCurrentIndexChanged(int)));
}

MainCentralWidget::~MainCentralWidget()
{
	while (currentCentralPage() != NULL)
		removeCentralPage(currentCentralPage());
}

QList<IMainCentralPage *> MainCentralWidget::centralPages() const
{
	return FCentralPages;
}

IMainCentralPage *MainCentralWidget::currentCentralPage() const
{
	return qobject_cast<IMainCentralPage *>(currentWidget());
}

void MainCentralWidget::setCurrentCentralPage(IMainCentralPage *APage)
{
	if (centralPages().contains(APage))
		setCurrentWidget(APage->instance());
}

void MainCentralWidget::appendCentralPage(IMainCentralPage *APage)
{
	if (!FCentralPages.contains(APage))
	{
		FCentralPages.append(APage);
		connect(APage->instance(),SIGNAL(centralPageShow(bool)),SLOT(onCentralPageShow(bool)));
		connect(APage->instance(),SIGNAL(centralPageChanged()),SLOT(onCentralPageChanged()));
		connect(APage->instance(),SIGNAL(centralPageDestroyed()),SLOT(onCentralPageDestroyed()));
		addWidget(APage->instance());
		emit centralPageAppended(APage);
	}
}

// *** <<< eyeCU >>> ***
void MainCentralWidget::insertCentralPage(int AIndex, IMainCentralPage *APage)
{
	if(AIndex>=0)
	{
		FCentralPages.insert(AIndex,APage);
		connect(APage->instance(),SIGNAL(centralPageShow(bool)),SLOT(onCentralPageShow(bool)));
		connect(APage->instance(),SIGNAL(centralPageChanged()),SLOT(onCentralPageChanged()));
		connect(APage->instance(),SIGNAL(centralPageDestroyed()),SLOT(onCentralPageDestroyed()));
		insertWidget(AIndex,APage->instance());
		emit centralPageInserted(APage);
	}
}
// *** <<< eyeCU >>> ***

void MainCentralWidget::removeCentralPage(IMainCentralPage *APage)
{
	if (FCentralPages.contains(APage))
	{
		FCentralPages.removeAll(APage);
		disconnect(APage->instance());
		removeWidget(APage->instance());
		APage->instance()->setParent(NULL);
		emit centralPageRemoved(APage);
	}
}

void MainCentralWidget::onCurrentIndexChanged(int AIIndex)
{
	IMainCentralPage *page = qobject_cast<IMainCentralPage *>(widget(AIIndex));
	emit currentCentralPageChanged(page);
}

void MainCentralWidget::onCentralPageShow(bool AMinimized)
{
	IMainCentralPage *page = qobject_cast<IMainCentralPage *>(sender());
	if (page)
	{
		if (!AMinimized)
			setCurrentCentralPage(page);
		FMainWindow->showWindow(AMinimized);
	}
}

void MainCentralWidget::onCentralPageChanged()
{
	IMainCentralPage *page = qobject_cast<IMainCentralPage *>(sender());
	if (page && page==currentCentralPage())
		emit currentCentralPageChanged(page);
}

void MainCentralWidget::onCentralPageDestroyed()
{
	IMainCentralPage *page = qobject_cast<IMainCentralPage *>(sender());
	removeCentralPage(page);
}
