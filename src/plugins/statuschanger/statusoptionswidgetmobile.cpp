
#include <utils/qt4qt5compat.h>
#include <utils/iconstorage.h>

#include "statusoptionswidgetmobile.h"

enum StatusTableColumns {
	STC_STATUS,
	STC_NAME,
	STC_MESSAGE,
	STC_PRIORITY,
	STC__COUNT
};
enum StatusTableRows {
	TB_STATUS,
    TB_NAME,
    TB_MESSAGE,
    TB_PRIORITY,
    TB__COUNT
};
enum StatusTableRoles {
    STR_STATUSID = Qt::UserRole,
    STR_COLUMN,
    STR_VALUE
};

#define		WDTSIZEPOLICY		QSizePolicy::Preferred,QSizePolicy::Preferred
#define		LBLSIZEPOLICY		QSizePolicy::Expanding,QSizePolicy::Preferred
#define		WIDGETSTATE			Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable
#define		WIDGETSTATEOFF		Qt::ItemIsSelectable | Qt::ItemIsEnabled
#define     STR_STATUSID        "STR_STATUSID"
#define     STR_PROPERTY        "STR_PROPERTY"
///! ---------------------------------------------------------
//! \brief StatusOptionsWidgetMobile::StatusOptionsWidgetMobile
//! \param AStatusChanger
//! \param AParent
//!
StatusOptionsWidgetMobile::StatusOptionsWidgetMobile(IStatusChanger *AStatusChanger, QWidget *AParent) :
    QWidget(AParent)
{
    int size=IconStorage::iconSize();
    FStatusChanger = AStatusChanger;
	this->setAttribute(Qt::WA_DeleteOnClose,true);

    pbtAdd = new QPushButton(this);
    pbtAdd->setText(tr("Add"));
    pbtAdd->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(pbtAdd,SIGNAL(clicked(bool)),SLOT(onAddButtonClicked()));

    pbtDelete = new QPushButton(this);
    pbtDelete->setText(tr("Delete"));
    pbtDelete->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    connect(pbtDelete,SIGNAL(clicked(bool)),SLOT(onDeleteButtonClicked()));
    //!----------
    tlbStatus = new QToolBox(this);
    tlbStatus->setFrameStyle(QFrame::Panel | QFrame::Raised);
    tlbStatus->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	connect(tlbStatus,SIGNAL(currentChanged(int)),SLOT(onCurrentChanged(int)));
    //!----------
    QHBoxLayout *hltlayout = new QHBoxLayout;
    hltlayout->setMargin(0);
    hltlayout->addWidget(pbtAdd);
    hltlayout->addStretch();
    hltlayout->addWidget(pbtDelete);

	QVBoxLayout *vltLayout = new QVBoxLayout(this);
    vltLayout->setMargin(1);
    vltLayout->setSpacing(size/2);
    vltLayout->addWidget(tlbStatus);
    vltLayout->addLayout(hltlayout);
    //!----------
	reset();
}

void StatusOptionsWidgetMobile::apply()
{
    foreach (int statusId, FDeletedStatuses)
        FStatusChanger->removeStatusItem(statusId);
	FDeletedStatuses.clear();

	for (int row=0; row<tlbStatus->count(); row++)
	{
		tlbStatus->setCurrentIndex(row);
		QWidget *wd=tlbStatus->currentWidget();
        int statusId =wd->property(STR_STATUSID).toInt();

        int show;
        QList<QComboBox *> allComboBox = wd->findChildren<QComboBox *>();
		if(allComboBox.count() >0)
            show=getCurrentStatus(allComboBox.at(0)->currentIndex());

		QString name;
        QString text;
		QList<QLineEdit *> allLineEdit = wd->findChildren<QLineEdit *>();
		if(allLineEdit.count() >0){
			name=allLineEdit.at(0)->text();
			text=allLineEdit.at(1)->text();
		}

		int priority;
		QList<QSpinBox *> allSpinBox = wd->findChildren<QSpinBox *>();
		if(allSpinBox.count() >0)
			priority=allSpinBox.at(0)->value();

		RowData2 status = FStatusItems.value(statusId);
		if (statusId <= STATUS_NULL_ID)
		{
			int name_i = 1;
			while (name.isEmpty() || FStatusChanger->statusByName(name)!=STATUS_NULL_ID)
			{
				name += QString("_%1").arg(name_i++);
                allLineEdit.at(0)->setProperty("VALUE",name);
                allLineEdit.at(0)->setText(name);
			}

			status.show = show;
			status.name = name;
            status.messageText = text;
			status.priority = priority;
			statusId = FStatusChanger->addStatusItem(name,show,text,priority);
            allComboBox.at(0)->setCurrentText(QString().setNum(statusId));
			FStatusItems.insert(statusId,status);
			wd->setProperty(STR_STATUSID,statusId);
		}
        else if (status.name!=name || status.show!=show || status.messageText!=text || status.priority!=priority)
		{
			FStatusChanger->updateStatusItem(statusId,name,show,text,priority);
			if(status.name!=name)
				tlbStatus->setItemText(row,name);
			if(status.show!=show)
				tlbStatus->setItemIcon(row,FStatusChanger->iconByShow(show));
		}
	}

	tlbStatus->setCurrentIndex(0);
	emit childApply();
}

void StatusOptionsWidgetMobile::reset()
{
    int size=IconStorage::iconSize();
    FDeletedStatuses.clear();
    FStatusItems.clear();

    foreach(int statusId, FStatusChanger->statusItems())
    {
        if (statusId > STATUS_NULL_ID)
        {
			QWidget *tabGeneral = new QWidget;
			tabGeneral->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
            tabGeneral->setProperty(STR_STATUSID,statusId);

            RowData2 status;
            status.show         = FStatusChanger->statusItemShow(statusId);
            status.name         = FStatusChanger->statusItemName(statusId);
            status.messageText  = FStatusChanger->statusItemText(statusId);
            status.priority     = FStatusChanger->statusItemPriority(statusId);

            FStatusItems.insert(statusId,status);

            QIcon icon=FStatusChanger->iconByShow(status.show);
			QString itemText=status.name;

			QFormLayout *lotGeneral = new QFormLayout(tabGeneral);
            lotGeneral->setRowWrapPolicy(QFormLayout::WrapLongRows);
            lotGeneral->setVerticalSpacing(size/4);

			QLabel *lblStatus=new QLabel(tr("Status"));
			QComboBox *show = getComboBox();
			show->setProperty(STR_PROPERTY,STC_STATUS);
			show->setCurrentText(FStatusChanger->nameByShow(status.show));
			lotGeneral->insertRow(TB_STATUS,lblStatus,show);

			QLabel *lblName=new QLabel(tr("Name"));
            QLineEdit *name= new QLineEdit;
			name->setProperty(STR_PROPERTY,STC_NAME);
            name->setText(status.name);
            name->setSizePolicy(WDTSIZEPOLICY);
			lotGeneral->insertRow(TB_NAME,lblName,name);

            QLabel *lblMessage=new QLabel(tr("Message"));
			QLineEdit *txtMessage= new QLineEdit;
			txtMessage->setProperty(STR_PROPERTY,STC_MESSAGE);
            txtMessage->setSizePolicy(WDTSIZEPOLICY);
            txtMessage->setText(status.messageText);
			connect(txtMessage,SIGNAL(textChanged(QString)),SIGNAL(modified()));
            lotGeneral->insertRow(TB_MESSAGE,lblMessage,txtMessage);

            QLabel *lblSpinBox=new QLabel(tr("Priority"));
			QSpinBox *spinBox=getSpinBox();
			spinBox->setProperty(STR_PROPERTY,STC_PRIORITY);
			spinBox->setValue(status.priority);
			connect(spinBox,SIGNAL(valueChanged(int)),SIGNAL(modified()));
            lotGeneral->insertRow(TB_PRIORITY,lblSpinBox,spinBox);

			if (statusId > STATUS_MAX_STANDART_ID)
			{
				show->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable));
				show->setVisible(true);
				lblStatus->setVisible(true);
                connect(show,SIGNAL(currentIndexChanged(QString)),SIGNAL(modified()));
				name->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable));
				name->setVisible(true);
				lblName->setVisible(true);
                connect(name,SIGNAL(textChanged(QString)),SIGNAL(modified()));
				txtMessage->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable));
				spinBox->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable));
			}
			else
			{
				show->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
				show->setVisible(false);
				lblStatus->setVisible(false);
                disconnect(show,0,0,0);
				name->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled));
				name->setVisible(false);
				lblName->setVisible(false);
                disconnect(name,0,0,0);
				txtMessage->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable));
				spinBox->setProperty("FLAG",QString().setNum(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable));
			}

            //!----------need scale  icon !!!
			tlbStatus->addItem(tabGeneral,icon,itemText);
        }
    }

	tlbStatus->setCurrentIndex(0);
	onCurrentChanged(0);
    emit childReset();
}

void StatusOptionsWidgetMobile::onAddButtonClicked()
{
	int row = tlbStatus->count();
	tlbStatus->setCurrentIndex(row+1);

	QWidget *tabGeneral = new QWidget;
	tabGeneral->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    tabGeneral->setProperty(STR_STATUSID,STATUS_NULL_ID);

	QFormLayout *lotGeneral = new QFormLayout(tabGeneral);
	lotGeneral->setRowWrapPolicy(QFormLayout::WrapLongRows);

	QLabel *lblStatus=new QLabel(tr("Status"));
	QComboBox *show = getComboBox();
	show->setProperty(STR_PROPERTY,STC_STATUS);
	show->setCurrentIndex(0);
	lotGeneral->insertRow(TB_STATUS,lblStatus,show);

    QLabel *lblName=new QLabel(tr("Name"));
	QLineEdit *name= new QLineEdit;
	name->setProperty(STR_PROPERTY,STC_NAME);
    name->setText(tr("New Name"));
    lotGeneral->insertRow(TB_NAME,lblName,name);

    QLabel *lblMessage=new QLabel(tr("Message"));
    QLineEdit *txtMessage= new QLineEdit;
	txtMessage->setProperty(STR_PROPERTY,STC_MESSAGE);
    txtMessage->setText(tr("New Message"));
    lotGeneral->insertRow(TB_MESSAGE,lblMessage,txtMessage);

    QLabel *lblSpinBox=new QLabel(tr("Priority"));
    QSpinBox *spinBox=getSpinBox();
	spinBox->setProperty(STR_PROPERTY,STC_PRIORITY);
    spinBox->setValue(30);
    lotGeneral->insertRow(TB_PRIORITY,lblSpinBox,spinBox);

	tlbStatus->addItem(tabGeneral,tr("New Status"));
    emit modified();
}

///!
//! \brief StatusOptionsWidgetMobile::onDeleteButtonClicked
//! Delete current row in ToolboBox
void StatusOptionsWidgetMobile::onDeleteButtonClicked()
{
    //for (int row=0; row<tlbStatus->count(); row++)
    {
        int row=tlbStatus->currentIndex();
//        tlbStatus->setCurrentIndex(row);
        QWidget *wd=tlbStatus->currentWidget();
        if (wd->property(STR_STATUSID).isValid())
        {
            int statusId=wd->property(STR_STATUSID).toInt();
            if (statusId == STATUS_NULL_ID)
            {
                tlbStatus->removeItem(row);
            }
            else if (statusId <= STATUS_MAX_STANDART_ID)
            {
                QMessageBox::information(this,tr("Can't delete status"),tr("You can not delete standard statuses."));
            }
            else if (FStatusChanger->activeStatusItems().contains(statusId))
            {
                QMessageBox::information(this,tr("Can't delete status"),tr("You can not delete active statuses."));
            }
            else if (FStatusItems.contains(statusId))
            {
                FDeletedStatuses.append(statusId);
                tlbStatus->removeItem(row);
            }
            emit modified();
            //break;
        }
    }
}

void StatusOptionsWidgetMobile::onCurrentChanged(int)
{
	QWidget *wd=tlbStatus->currentWidget();
	int statusId =wd->property(STR_STATUSID).toInt();
	if(statusId>STATUS_MAX_STANDART_ID)
		pbtDelete->setEnabled(true);
	else
		pbtDelete->setEnabled(false);
}

QComboBox *StatusOptionsWidgetMobile::getComboBox()
{
	QComboBox *comboBox = new QComboBox;
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Online),FStatusChanger->nameByShow(IPresence::Online),IPresence::Online);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Chat),FStatusChanger->nameByShow(IPresence::Chat),IPresence::Chat);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Away),FStatusChanger->nameByShow(IPresence::Away),IPresence::Away);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::DoNotDisturb),FStatusChanger->nameByShow(IPresence::DoNotDisturb),IPresence::DoNotDisturb);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::ExtendedAway),FStatusChanger->nameByShow(IPresence::ExtendedAway),IPresence::ExtendedAway);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Invisible),FStatusChanger->nameByShow(IPresence::Invisible),IPresence::Invisible);
	comboBox->addItem(FStatusChanger->iconByShow(IPresence::Offline),FStatusChanger->nameByShow(IPresence::Offline),IPresence::Offline);
	comboBox->setEditable(false);
	return comboBox;
}

QSpinBox *StatusOptionsWidgetMobile::getSpinBox()
{
	QSpinBox *spinBox = new QSpinBox;
	spinBox->setAlignment(Qt::AlignHCenter);
	spinBox->setMinimum(-128);
	spinBox->setMaximum(128);
    spinBox->setSizePolicy(WDTSIZEPOLICY);
	return spinBox;
}

int StatusOptionsWidgetMobile::getCurrentStatus(int AIndex)
{
    switch (AIndex)
    {
        case 0:  return IPresence::Online;
        case 1:  return IPresence::Chat;
        case 2:  return IPresence::Away;
        case 3:  return IPresence::DoNotDisturb;
        case 4:  return IPresence::ExtendedAway;
        case 5:  return IPresence::Invisible;
        case 6:  return IPresence::Offline;
        default: return IPresence::Online;
    }
	return 0;
}
