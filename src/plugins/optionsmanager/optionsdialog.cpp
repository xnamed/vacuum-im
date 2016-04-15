#include "optionsdialog.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTextDocument>
#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <definitions/optionvalues.h>
#include <utils/widgetmanager.h>
#include <utils/iconstorage.h>
#include <utils/options.h>
#include <utils/logger.h>
#include "optionsdialogheader.h"

#define IDR_ORDER  Qt::UserRole + 1

// *** <<< eyeCU <<< ***
//	#define NODE_ITEM_ICONSIZE   QSize(16,16)
//	#define NODE_ITEM_SIZEHINT   QSize(24,24)
// *** >>> eyeCU >>> ***

static const QString NodeDelimiter = ".";

bool SortFilterProxyModel::lessThan(const QModelIndex &ALeft, const QModelIndex &ARight) const
{
	if (ALeft.data(IDR_ORDER).toInt() != ARight.data(IDR_ORDER).toInt())
		return ALeft.data(IDR_ORDER).toInt() < ARight.data(IDR_ORDER).toInt();
	return QSortFilterProxyModel::lessThan(ALeft,ARight);
}

OptionsDialog::OptionsDialog(IOptionsManager *AOptionsManager, const QString &ARootId, QWidget *AParent) : QDialog(AParent)
{
	REPORT_VIEW;
	ui.setupUi(this);
	setWindowTitle(tr("Options"));
	setWindowModality(Qt::WindowModal);
	setAttribute(Qt::WA_DeleteOnClose,true);
	IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->insertAutoIcon(this,MNI_OPTIONS_DIALOG,0,0,"windowIcon");

#ifdef EYECU_MOBILE
    FNodeItemIconSize.setWidth(16*IconStorage::scale());
    FNodeItemIconSize.setHeight(16*IconStorage::scale());
    FNodeItemSizeHint.setWidth(1.5*16*IconStorage::scale());
    FNodeItemSizeHint.setHeight(1.5*16*IconStorage::scale());
    //! Set styles for OptionsDialog
	FStyleOn=true;
	FTrvNodesStyle=QString("background-color:#F4F0F0;");// color:white; #0ac525 #4cb9f2
	FScaStyle     =QString("background-color:#F4F0F0;");// color:white; #0ac525 #4cb9f2
	FHeaderStyle  =QString("background-color:#039702; color:white;");//#069105 4061d1
//    if(FStyleOn)  setStyleSheet(FTrvNodesStyle);
	showMaximized();
#else
    FNodeItemIconSize.setWidth(16);
    FNodeItemIconSize.setHeight(16);
    FNodeItemSizeHint.setWidth(24);
    FNodeItemSizeHint.setHeight(24);
#endif
	FRootNodeId = ARootId;
    delete ui.scaScroll->takeWidget();

	FOptionsManager = AOptionsManager;
	connect(FOptionsManager->instance(),SIGNAL(optionsDialogNodeInserted(const IOptionsDialogNode &)),SLOT(onOptionsDialogNodeInserted(const IOptionsDialogNode &)));
	connect(FOptionsManager->instance(),SIGNAL(optionsDialogNodeRemoved(const IOptionsDialogNode &)),SLOT(onOptionsDialogNodeRemoved(const IOptionsDialogNode &)));

	FItemsModel = new QStandardItemModel(ui.trvNodes);
	FItemsModel->setColumnCount(1);

	FProxyModel = new SortFilterProxyModel(FItemsModel);
	FProxyModel->setSourceModel(FItemsModel);
	FProxyModel->setSortLocaleAware(true);
	FProxyModel->setDynamicSortFilter(true);
	FProxyModel->setSortCaseSensitivity(Qt::CaseInsensitive);

	ui.trvNodes->setModel(FProxyModel);
//	ui.trvNodes->setIconSize(NODE_ITEM_ICONSIZE);
    ui.trvNodes->setIconSize(FNodeItemIconSize);    // *** <<< eyeCU <<< ***
	ui.trvNodes->setRootIsDecorated(false);
	ui.trvNodes->setUniformRowHeights(false);
	ui.trvNodes->sortByColumn(0,Qt::AscendingOrder);
#ifdef EYECU_MOBILE         // *** <<< eyeCU <<< ***
//    ui.trvNodes->setAlternatingRowColors(true);   //--all grey--???
    connect(ui.trvNodes,SIGNAL(clicked(QModelIndex)),SLOT(onClicked(QModelIndex)));
#else				// *** >>> eyeCU >>> ***
	connect(ui.trvNodes->selectionModel(),SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),SLOT(onCurrentItemChanged(const QModelIndex &, const QModelIndex &)));
#endif
	ui.dbbButtons->button(QDialogButtonBox::Apply)->setEnabled(false);
	ui.dbbButtons->button(QDialogButtonBox::Reset)->setEnabled(false);
	connect(ui.dbbButtons,SIGNAL(clicked(QAbstractButton *)),SLOT(onDialogButtonClicked(QAbstractButton *)));

	foreach (const IOptionsDialogNode &node, FOptionsManager->optionsDialogNodes())
		onOptionsDialogNodeInserted(node);
	ui.trvNodes->setVisible(FItemsModel->rowCount() > 0);
#ifndef EYECU_MOBILE        // *** <<< eyeCU <<< ***
    if (!restoreGeometry(Options::fileValue("optionsmanager.optionsdialog.geometry",FRootNodeId).toByteArray()))
        setGeometry(WidgetManager::alignGeometry(FItemsModel->rowCount()>0 ? QSize(750,560) : QSize(570,560),this));
	if (!ui.sprSplitter->restoreState(Options::fileValue("optionsmanager.optionsdialog.splitter.state",FRootNodeId).toByteArray()))
		ui.sprSplitter->setSizes(QList<int>() << 180 << 620);
#endif
}

OptionsDialog::~OptionsDialog()
{
#ifndef EYECU_MOBILE        // *** <<< eyeCU <<< ***
	Options::setFileValue(saveGeometry(),"optionsmanager.optionsdialog.geometry",FRootNodeId);
	Options::setFileValue(ui.sprSplitter->saveState(),"optionsmanager.optionsdialog.splitter.state",FRootNodeId);
#endif
	FCleanupHandler.clear();
}

void OptionsDialog::showNode(const QString &ANodeId)
{
    QStandardItem *item = FNodeItems.value(ANodeId, NULL);
	if (item)
		ui.trvNodes->setCurrentIndex(FProxyModel->mapFromSource(FItemsModel->indexFromItem(item)));
}

QWidget *OptionsDialog::createNodeWidget(const QString &ANodeId)
{
	LOG_DEBUG(QString("Creating options dialog widgets for node=%1").arg(ANodeId));

    QWidget *nodeWidget = new QWidget(ui.scaScroll);
	QVBoxLayout *nodeLayout = new QVBoxLayout(nodeWidget);
	nodeLayout->setMargin(1);

	QMultiMap<int, IOptionsDialogWidget *> orderedWidgets;
	foreach(IOptionsDialogHolder *optionsHolder, FOptionsManager->optionsDialogHolders())
		orderedWidgets += optionsHolder->optionsDialogWidgets(ANodeId,nodeWidget);

	if (!orderedWidgets.isEmpty())
	{
		QVBoxLayout *headerLayout = NULL;
        IOptionsDialogWidget *headerWidget = NULL;
		foreach(IOptionsDialogWidget *widget, orderedWidgets)
        {
			bool isHeader = qobject_cast<OptionsDialogHeader *>(widget->instance()) != NULL;
			if (!isHeader)
			{
				if (headerLayout == NULL)
				{
					headerLayout = new QVBoxLayout;
#ifndef EYECU_MOBILE	// *** <<< eyeCU <<< ---
					headerLayout->setContentsMargins(15,0,0,0);
#endif
					nodeLayout->addLayout(headerLayout);
				}
                headerLayout->addWidget(widget->instance());
#ifdef EYECU_MOBILE	// *** <<< eyeCU <<< ---
//				widget->instance()->installEventFilter(this);//for mouse
				LineOnWidget *line=new LineOnWidget;
				headerLayout->addWidget(line);
#endif				// *** <<< eyeCU <<< ---
			}
			else
			{
#ifdef EYECU_MOBILE	// *** <<< eyeCU <<< ---
				if(FStyleOn)
				{
					widget->instance()->setStyleSheet(FHeaderStyle);
					widget->instance()->setFixedHeight(16*(IconStorage::scale()+1));
				}
#endif				// *** <<< eyeCU <<< ---
				if (headerLayout != NULL)
				{
#ifdef EYECU_MOBILE	// *** <<< eyeCU <<< ---
					nodeLayout->addSpacing(0);
#else
					nodeLayout->addSpacing(10);
#endif				// *** <<< eyeCU <<< ---
					headerLayout = NULL;
				}
				else if (headerWidget != NULL)
				{
					delete headerWidget->instance();
				}
                nodeLayout->addWidget(widget->instance());
#ifdef EYECU_MOBILE	// *** <<< eyeCU <<< ---
				nodeLayout->addSpacing(10);		//! Spaser after header
#endif				// *** <<< eyeCU <<< ---
				headerWidget = widget;
			}

			connect(this,SIGNAL(applied()),widget->instance(),SLOT(apply()));
			connect(this,SIGNAL(reseted()),widget->instance(),SLOT(reset()));
			connect(widget->instance(),SIGNAL(modified()),SLOT(onOptionsWidgetModified()));
		}

		if (headerWidget!=NULL && headerLayout==NULL)
			delete headerWidget->instance();

		if (!canExpandVertically(nodeWidget))
			nodeLayout->addStretch();
	}
	else
	{
		QLabel *label = new QLabel(tr("Options are absent"),nodeWidget);
		label->setAlignment(Qt::AlignCenter);
		label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
		nodeLayout->addWidget(label);
	}

	FCleanupHandler.add(nodeWidget);
	return nodeWidget;
}

QStandardItem *OptionsDialog::getNodeModelItem(const QString &ANodeId)
{
	QStandardItem *item = FNodeItems.value(ANodeId);
	if (item == NULL)
	{
		item = new QStandardItem(ANodeId);
		FItemsModel->appendRow(item);
		FNodeItems.insert(ANodeId,item);
	}
	return item;
}

bool OptionsDialog::canExpandVertically(const QWidget *AWidget) const
{
	bool expanding = AWidget->sizePolicy().verticalPolicy() == QSizePolicy::Expanding;
	if (!expanding)
	{
		QObjectList childs = AWidget->children();
		for (int i=0; !expanding && i<childs.count(); i++)
			if (childs.at(i)->isWidgetType())
				expanding = canExpandVertically(qobject_cast<QWidget *>(childs.at(i)));
	}
	return expanding;
}

void OptionsDialog::onOptionsWidgetModified()
{
	ui.dbbButtons->button(QDialogButtonBox::Apply)->setEnabled(true);
	ui.dbbButtons->button(QDialogButtonBox::Reset)->setEnabled(true);
}

void OptionsDialog::onOptionsDialogNodeInserted(const IOptionsDialogNode &ANode)
{
	QString prefix = FRootNodeId + NodeDelimiter;
	if (!ANode.nodeId.isEmpty() && !ANode.caption.isEmpty() && !FRootNodeId.isEmpty() ? ANode.nodeId.startsWith(prefix) : true)
	{
		// Do not show child nodes
		if (ANode.nodeId.indexOf(NodeDelimiter,!FRootNodeId.isEmpty() ? prefix.size()+1 : 0) < 0)
		{
			QStandardItem *item = getNodeModelItem(ANode.nodeId);
			item->setText(ANode.caption);
			item->setData(ANode.order,IDR_ORDER);
//			item->setData(NODE_ITEM_SIZEHINT,Qt::SizeHintRole);
            item->setData(FNodeItemSizeHint,Qt::SizeHintRole);  // *** <<< eyeCU <<< ***
			item->setIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(ANode.iconkey));

			ui.trvNodes->setVisible(FItemsModel->rowCount() > 0);
		}
	}
}

void OptionsDialog::onOptionsDialogNodeRemoved(const IOptionsDialogNode &ANode)
{
	if (FNodeItems.contains(ANode.nodeId))
	{
		QStandardItem *item = FNodeItems.take(ANode.nodeId);
		qDeleteAll(FItemsModel->takeRow(item->row()));
		delete FItemWidgets.take(item);

		ui.trvNodes->setVisible(FItemsModel->rowCount() > 0);
	}
	else if (ANode.nodeId == FRootNodeId)
	{
		reject();
	}
}

// *** <<< eyeCU <<< ***
void OptionsDialog::onClicked(const QModelIndex &ACurrent)
{
	onCurrentItemChanged(ACurrent, ACurrent);
}

// *** >>> eyeCU >>> ***

void OptionsDialog::onCurrentItemChanged(const QModelIndex &ACurrent, const QModelIndex &APrevious)
{
	Q_UNUSED(APrevious);
    ui.scaScroll->takeWidget();

	QStandardItem *curItem = FItemsModel->itemFromIndex(FProxyModel->mapToSource(ACurrent));
	QString nodeId = FNodeItems.key(curItem);
	LOG_DEBUG(QString("Changing current options dialog node to %1").arg(nodeId));

	if (curItem && !FItemWidgets.contains(curItem))
		FItemWidgets.insert(curItem,createNodeWidget(nodeId));

    QWidget *curWidget = FItemWidgets.value(curItem);
	if (curWidget)
	{
		ui.scaScroll->setWidget(curWidget);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
		curWidget->layout()->setSpacing(0);
		ui.scaScroll->showMaximized();
		ui.scaScroll->setVisible(true);
		if(FStyleOn)
			ui.scaScroll->setStyleSheet(FScaStyle);
#endif
// *** >>> eyeCU >>> ***
    }
	Options::setFileValue(nodeId,"options.dialog.last-node",FRootNodeId);
}

void OptionsDialog::onDialogButtonClicked(QAbstractButton *AButton)
{
	switch (ui.dbbButtons->buttonRole(AButton))
	{
	case QDialogButtonBox::AcceptRole:
		emit applied();
		accept();
		break;
	case QDialogButtonBox::ApplyRole:
		emit applied();
		ui.dbbButtons->button(QDialogButtonBox::Apply)->setEnabled(false);
		ui.dbbButtons->button(QDialogButtonBox::Reset)->setEnabled(false);
		break;
	case QDialogButtonBox::ResetRole:
		emit reseted();
		ui.dbbButtons->button(QDialogButtonBox::Apply)->setEnabled(false);
		ui.dbbButtons->button(QDialogButtonBox::Reset)->setEnabled(false);
		break;
	case QDialogButtonBox::RejectRole:
		reject();
		break;
	default:
		break;
	}
}
