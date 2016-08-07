#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include "activityselect.h"
#include <QDebug>
ActivitySelect::ActivitySelect(Activity *AActivity, const QHash<QString, QStringList> &AActivityList, const QHash<QString, QStringList> &AActivityTexts, const ActivityData &AActivityData, QWidget *parent) :
    QDialog(parent),
    FActivity(AActivity),
    FActivityData(AActivityData),
    FActivityNames(AActivityList),
    FActivityTexts(AActivityTexts),
	FCurScale(4),
	ui(new Ui::ActivitySelect)
{
    ui->setupUi(this);
    ui->lstActivity->sortItems(0,Qt::AscendingOrder);

    FCurScale=IconStorage::scale();
    QFont fnt=ui->lstActivity->font();
#ifdef EYECU_MOBILE
	fnt.setPointSizeF(IconStorage::fontPointSize());
	ui->lstActivity->setIconSize(QSize(IconStorage::iconSize(),IconStorage::iconSize()));
    ui->lblIcon->setScaledContents(true);
	showMaximized();
#else
	ui->lstActivity->setIconSize(QSize(16,16));
#endif
	ui->lstActivity->setFont(fnt);
	ui->cmbText->setFont(fnt);
	setWindowIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(QString(MNI_ACTIVITY)));
    fillActivityTree();    
    fillTextList(AActivityData.iconFileName());

    connect(ui->lstActivity,SIGNAL(itemActivated(QTreeWidgetItem*,int)),SLOT(accept()));
    connect(ui->lstActivity,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),SLOT(onCurItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
    connect(ui->cmbText,SIGNAL(editTextChanged(QString)),SLOT(onEditTextChanged(QString)));

	for (QTreeWidgetItemIterator it(ui->lstActivity, QTreeWidgetItemIterator::All) ; *it; ++it)
		if ((*it)->data(0,Qt::UserRole).toString()==AActivityData.nameDetailed && (*it)->text(1)==AActivityData.nameBasic)
		{
			ui->lstActivity->setCurrentItem(*it);
			(*it)->setSelected(true);
			break;
		}

	ui->cmbText->setEditText(AActivityData.text);
}

ActivitySelect::~ActivitySelect()
{
    delete ui;
}

void ActivitySelect::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void ActivitySelect::onCurItemChanged(QTreeWidgetItem *AItemNew, QTreeWidgetItem *AItemOld)
{
    Q_UNUSED(AItemOld);    
	ui->lblIcon->setPixmap(AItemNew->icon(0).pixmap(IconStorage::iconSize()));
    FActivityData.nameBasic = AItemNew->text(1);
    FActivityData.nameDetailed = AItemNew->data(0, Qt::UserRole).toString();
    fillTextList(FActivityData.iconFileName());
}

void ActivitySelect::onEditTextChanged(const QString &ANewText)
{
    if (!FActivityData.isEmpty())
        FActivityData.text = ANewText;
}

void ActivitySelect::fillTextList(const QString &AActivity)
{
    ui->cmbText->clear();
    ui->cmbText->addItems(FActivityTexts.value(AActivity));
    ui->cmbText->setEditable(true);
}

void ActivitySelect::fillActivityTree()
{
    QTreeWidgetItem* pItem;
    QTreeWidgetItem* pItemDir;
	ui->lstActivity->sortItems(0, Qt::AscendingOrder);
    for (QHash<QString, QStringList>::const_iterator it=FActivityNames.constBegin(); it!=FActivityNames.constEnd(); it++)
    {
        ActivityData data(it.key());
        pItem = new QTreeWidgetItem(ui->lstActivity);
		pItem->setText(1, data.nameBasic);
		pItem->setText(0, (data.isEmpty()?QString("<%1>").arg(FActivity->getActivityText("no_activity")):FActivity->getActivityText(data.nameBasic)));
        pItem->setIcon(0, FActivity->getIcon(data));
        for (QStringList::const_iterator itn=(*it).constBegin(); itn!=(*it).constEnd(); itn++)
        {
            data.nameDetailed = *itn;
            pItemDir = new QTreeWidgetItem(pItem);
            pItemDir->setText(1, data.nameBasic);                       // dir-name
            pItemDir->setText(0, FActivity->getActivityText(data.nameDetailed)); // transl-name
            pItemDir->setData(0, Qt::UserRole, data.nameDetailed);            // engl-name
            pItemDir->setIcon(0, FActivity->getIcon(data));
        }
        pItem->setExpanded(false);
    }
}


