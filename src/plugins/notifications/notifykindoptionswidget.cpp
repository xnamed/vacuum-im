#include <QDebug>

#include <QHeaderView>
#include <QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QLabel>
#include <QCheckBox>

#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <utils/iconstorage.h>
#include "utils/qt4qt5compat.h"
#include <utils/advanceditemdelegate.h>

#include "notifykindoptionswidget.h"

#define		CHBOXSTATE			Qt::ItemIsEnabled | Qt::ItemIsUserCheckable
//#define		CHBOXSIZEPOLICY		QSizePolicy::Maximum,QSizePolicy::Maximum
#define		CHBOXSIZEPOLICY		QSizePolicy::Preferred,QSizePolicy::Preferred
#define		LBLSIZEPOLICY		QSizePolicy::Expanding,QSizePolicy::Preferred


// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
NotifyKindOptionsWidget::NotifyKindOptionsWidget(INotifications *ANotifications, QWidget *AParent) : QWidget(AParent)
{
	FNotifications = ANotifications;
	int size=IconStorage::iconSize();
    registrOrderedTypes();

	QVBoxLayout *vblLayout = new QVBoxLayout(this);
    tlbNotifies=new QToolBox(this);
	tlbNotifies->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	connect(tlbNotifies,SIGNAL(currentChanged(int)),SLOT(onCurrentChanged(int)));

	QLabel *lblTitle = new QLabel(this);
	lblTitle->setText(tr("Events"));
	QFont font= lblTitle->font();
	font.setBold(true);
	lblTitle->setFont(font);
	lblTitle->setAlignment(Qt::AlignHCenter);
	vblLayout->addWidget(lblTitle);

	for (QMultiMap<int, NotificationType>::const_iterator it=FOrderedTypes.constBegin(); it!=FOrderedTypes.constEnd(); ++it)
	{
        QWidget *tabGeneral = new QWidget();
		tabGeneral->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

		QFormLayout *lotGeneral = new QFormLayout(tabGeneral);
		lotGeneral->setContentsMargins(size/2,0,0,0);
		lotGeneral->setRowWrapPolicy(QFormLayout::WrapLongRows);
		lotGeneral->setVerticalSpacing(4);
		//!------------------
		QCheckBox *chBoxSound = new QCheckBox;
		chBoxSound->setProperty("NTR_KIND",INotification::SoundPlay);
		chBoxSound->setSizePolicy(CHBOXSIZEPOLICY);
		QLabel *lblBoxSound = new QLabel(tr("Play sound at the notification"));
		lblBoxSound->setSizePolicy(LBLSIZEPOLICY);
		lblBoxSound->setWordWrap(true);
		if (it->kindMask & INotification::SoundPlay)
			chBoxSound->setEnabled(CHBOXSTATE);
		else
			chBoxSound->setEnabled(false);
		chBoxSound->setCheckState(Qt::Unchecked);
		lotGeneral->insertRow(NTC_SOUND,chBoxSound,lblBoxSound);
		connect(chBoxSound,SIGNAL(clicked(bool)),SIGNAL(modified()));

		QCheckBox *chBoxPopUp = new QCheckBox;
		chBoxPopUp->setSizePolicy(CHBOXSIZEPOLICY);
		chBoxPopUp->setProperty("NTR_KIND",INotification::PopupWindow);
		QLabel *lblBoxPopUp = new QLabel(tr("Display a notification in popup window"));
		lblBoxPopUp->setSizePolicy(LBLSIZEPOLICY);
		lblBoxPopUp->setWordWrap(true);
		if (it->kindMask & INotification::PopupWindow)
			chBoxPopUp->setEnabled(CHBOXSTATE);
		else
			chBoxPopUp->setEnabled(false);
		chBoxPopUp->setCheckState(Qt::Unchecked);
		lotGeneral->insertRow(NTC_POPUP,chBoxPopUp,lblBoxPopUp);
		connect(chBoxPopUp,SIGNAL(clicked(bool)),SIGNAL(modified()));

		QCheckBox *chBoxMinim = new QCheckBox;
		chBoxMinim->setSizePolicy(CHBOXSIZEPOLICY);
		chBoxMinim->setProperty("NTR_KIND",INotification::ShowMinimized);
		QLabel *lblBoxMinim = new QLabel(tr("Show the corresponding window minimized in the taskbar"));
		lblBoxMinim->setSizePolicy(LBLSIZEPOLICY);
		lblBoxMinim->setWordWrap(true);
		if (it->kindMask & INotification::ShowMinimized)
			chBoxMinim->setEnabled(CHBOXSTATE);
		else
			chBoxMinim->setEnabled(false);
		chBoxMinim->setCheckState(Qt::Unchecked);
		lotGeneral->insertRow(NTC_MINIMIZED,chBoxMinim,lblBoxMinim);
		connect(chBoxMinim,SIGNAL(clicked(bool)),SIGNAL(modified()));

		QCheckBox *chBoxVibration = new QCheckBox;
		chBoxVibration->setSizePolicy(CHBOXSIZEPOLICY);
		chBoxVibration->setProperty("NTR_KIND",INotification::Vibrate);
		QLabel *lblVibration = new QLabel(tr("Vibration"));
		lblVibration->setSizePolicy(LBLSIZEPOLICY);
		lblVibration->setWordWrap(true);
		if (it->kindMask & INotification::Vibrate)
			chBoxVibration->setEnabled(CHBOXSTATE);
        else
			chBoxVibration->setEnabled(false);
		chBoxVibration->setCheckState(Qt::Unchecked);
        lotGeneral->insertRow(NTC_VIBRATION,chBoxVibration,lblVibration);
		connect(chBoxVibration,SIGNAL(clicked(bool)),SIGNAL(modified()));

		QCheckBox *chBoxLights = new QCheckBox;
		chBoxLights->setSizePolicy(CHBOXSIZEPOLICY);
		chBoxLights->setProperty("NTR_KIND",INotification::Lights);
		QLabel *lblBoxLights = new QLabel(tr("Lights"));
		lblBoxLights->setSizePolicy(LBLSIZEPOLICY);
		lblBoxLights->setWordWrap(true);
		if (it->kindMask & INotification::Lights)
			chBoxLights->setEnabled(CHBOXSTATE);
        else
			chBoxLights->setEnabled(false);
		chBoxLights->setCheckState(Qt::Unchecked);
        lotGeneral->insertRow(NTC_LIGHTS,chBoxLights,lblBoxLights);
		connect(chBoxLights,SIGNAL(clicked(bool)),SIGNAL(modified()));
		//!----------
        tlbNotifies->addItem(tabGeneral,it->icon,it->title);
		plugTypeId << it->typeId;
	}

	tlbNotifies->setCurrentWidget(0);	// not working...
	vblLayout->addWidget(tlbNotifies);
	vblLayout->setMargin(0);

	reset();
}

void NotifyKindOptionsWidget::registrOrderedTypes()
{
	ushort visibleKinds = INotification::PopupWindow|INotification::Vibrate|INotification::Lights|INotification::SoundPlay|INotification::ShowMinimized;
	foreach(const QString &typeId, FNotifications->notificationTypes())
    {
        NotificationType notifyType = FNotifications->notificationType(typeId);
        if (!notifyType.title.isEmpty() && (notifyType.kindMask & visibleKinds)>0)
        {
            notifyType.typeId = typeId;
            FOrderedTypes.insertMulti(notifyType.order,notifyType);
        }
	}
}

void NotifyKindOptionsWidget::onCurrentChanged(int index)
{
	tlbNotifies->widget(index)->adjustSize();
	tlbNotifies->widget(index)->resize(tlbNotifies->widget(index)->width(),tlbNotifies->widget(index)->minimumSizeHint().height());
}

void NotifyKindOptionsWidget::apply()
{
	for (int cnt=0; cnt<tlbNotifies->count(); cnt++)
	{
		QString typeId = plugTypeId[cnt];
		NotificationType notifyType = FNotifications->notificationType(typeId);
		ushort typeKinds = notifyType.kindDefs & notifyType.kindMask;

		tlbNotifies->setCurrentIndex(cnt);
		QWidget *wd=tlbNotifies->currentWidget();
		QList<QCheckBox *> allChecBox = wd->findChildren<QCheckBox *>();
		if (allChecBox.count() > 0)
		{
			for (int i =0; i < allChecBox.count(); i++)
			{
				ushort kind = allChecBox.at(i)->property("NTR_KIND").toInt();
				if(allChecBox.at(i)->checkState() == Qt::Checked)
					typeKinds |= kind;
				else
					typeKinds &= ~kind;
			}
			FNotifications->setTypeNotificationKinds(typeId,typeKinds);
		}
	}
}

//! TO DO -pass a parameter 'typeId' through the object properties
void NotifyKindOptionsWidget::reset()
{
	for (int cnt=0; cnt<tlbNotifies->count(); cnt++)
	{
		QString typeId = plugTypeId[cnt];
		ushort typeKinds = FNotifications->typeNotificationKinds(typeId);

		tlbNotifies->setCurrentIndex(cnt);
		QWidget *wd=tlbNotifies->currentWidget();
		QList<QCheckBox *> allChecBox = wd->findChildren<QCheckBox *>();
		if (allChecBox.count() > 0)
		{
			for (int i =0; i < allChecBox.count(); i++)
			{
				ushort kind = allChecBox.at(i)->property("NTR_KIND").toInt();
				allChecBox.at(i)->setCheckState(typeKinds & kind ? Qt::Checked : Qt::Unchecked);
			}
		}
	}
}

void NotifyKindOptionsWidget::showEvent(QShowEvent *AEvent)
{
	QWidget::showEvent(AEvent);
}

#else
// *** >>> eyeCU >>> ***
NotifyKindOptionsWidget::NotifyKindOptionsWidget(INotifications *ANotifications, QWidget *AParent) : QWidget(AParent)
{
    FNotifications = ANotifications;

    tbwNotifies = new QTableWidget(this);
    tbwNotifies->setWordWrap(true);
    tbwNotifies->verticalHeader()->setVisible(false);
    tbwNotifies->horizontalHeader()->setHighlightSections(false);
    tbwNotifies->setSelectionMode(QTableWidget::NoSelection);
	//----
	int size=IconStorage::iconSize();
	tbwNotifies->setIconSize(QSize(size,size));
	tbwNotifies->setItemDelegate(new AdvancedItemDelegate(tbwNotifies));
	//----
    connect(tbwNotifies,SIGNAL(itemChanged(QTableWidgetItem *)),SIGNAL(modified()));

    tbwNotifies->setColumnCount(NTC__COUNT);
    tbwNotifies->setHorizontalHeaderLabels(QStringList() << tr("Event") << "" << "" << "" << "" );
    tbwNotifies->horizontalHeader()->SETRESIZEMODE(NTC_TYPE,QHeaderView::Stretch);

    tbwNotifies->horizontalHeader()->SETRESIZEMODE(NTC_SOUND,QHeaderView::ResizeToContents);
    tbwNotifies->horizontalHeaderItem(NTC_SOUND)->setToolTip(tr("Play sound at the notification"));
    tbwNotifies->horizontalHeaderItem(NTC_SOUND)->setIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_NOTIFICATIONS_SOUNDPLAY));

    tbwNotifies->horizontalHeader()->SETRESIZEMODE(NTC_POPUP,QHeaderView::ResizeToContents);
    tbwNotifies->horizontalHeaderItem(NTC_POPUP)->setToolTip(tr("Display a notification in popup window"));
    tbwNotifies->horizontalHeaderItem(NTC_POPUP)->setIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_NOTIFICATIONS_PUPUPWINDOW));

    tbwNotifies->horizontalHeader()->SETRESIZEMODE(NTC_MINIMIZED,QHeaderView::ResizeToContents);
    tbwNotifies->horizontalHeaderItem(NTC_MINIMIZED)->setToolTip(tr("Show the corresponding window minimized in the taskbar"));
    tbwNotifies->horizontalHeaderItem(NTC_MINIMIZED)->setIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_NOTIFICATIONS_SHOWMINIMIZED));

    tbwNotifies->horizontalHeader()->SETRESIZEMODE(NTC_TRAY,QHeaderView::ResizeToContents);
    tbwNotifies->horizontalHeaderItem(NTC_TRAY)->setToolTip(tr("Display a notification icon in the system tray"));
    tbwNotifies->horizontalHeaderItem(NTC_TRAY)->setIcon(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->getIcon(MNI_NOTIFICATIONS_TRAYICON));

    QVBoxLayout *vblLayout = new QVBoxLayout(this);
    vblLayout->addWidget(tbwNotifies);
    vblLayout->setMargin(0);

    QMultiMap<int, NotificationType> orderedTypes;
    ushort visibleKinds = INotification::PopupWindow|INotification::TrayNotify|INotification::SoundPlay|INotification::ShowMinimized;
    foreach(const QString &typeId, FNotifications->notificationTypes())
    {
        NotificationType notifyType = FNotifications->notificationType(typeId);
        if (!notifyType.title.isEmpty() && (notifyType.kindMask & visibleKinds)>0)
        {
            notifyType.typeId = typeId;
            orderedTypes.insertMulti(notifyType.order,notifyType);
        }
    }

    for (QMultiMap<int, NotificationType>::const_iterator it=orderedTypes.constBegin(); it!=orderedTypes.constEnd(); ++it)
    {
        int row = tbwNotifies->rowCount();
        tbwNotifies->setRowCount(row+1);

        QTableWidgetItem *type = new QTableWidgetItem(it->icon,it->title);
        type->setData(NTR_TYPE, it->typeId);
        type->setFlags(Qt::ItemIsEnabled);
        tbwNotifies->setItem(row,NTC_TYPE,type);

        QTableWidgetItem *sound = new QTableWidgetItem();
        sound->setData(NTR_KIND, INotification::SoundPlay);
        if (it->kindMask & INotification::SoundPlay)
            sound->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        else
            sound->setFlags(Qt::ItemIsUserCheckable);
        sound->setCheckState(Qt::Unchecked);
        tbwNotifies->setItem(row,NTC_SOUND,sound);

        QTableWidgetItem *popup = new QTableWidgetItem();
        popup->setData(NTR_KIND, INotification::PopupWindow);
        if (it->kindMask & INotification::PopupWindow)
            popup->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        else
            popup->setFlags(Qt::ItemIsUserCheckable);
        popup->setCheckState(Qt::Unchecked);
        tbwNotifies->setItem(row,NTC_POPUP,popup);

        QTableWidgetItem *minimized = new QTableWidgetItem();
        minimized->setData(NTR_KIND, INotification::ShowMinimized);
        if (it->kindMask & INotification::ShowMinimized)
            minimized->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        else
            minimized->setFlags(Qt::ItemIsUserCheckable);
        minimized->setCheckState(Qt::Unchecked);
        tbwNotifies->setItem(row,NTC_MINIMIZED,minimized);

        QTableWidgetItem *tray = new QTableWidgetItem();
        tray->setData(NTR_KIND, INotification::TrayNotify);
        if (it->kindMask & INotification::TrayNotify)
            tray->setFlags(Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
        else
            tray->setFlags(Qt::ItemIsUserCheckable);
        tray->setCheckState(Qt::Unchecked);
        tbwNotifies->setItem(row,NTC_TRAY,tray);

        tbwNotifies->verticalHeader()->SETRESIZEMODE(row,QHeaderView::ResizeToContents);
    }

    reset();
}

void NotifyKindOptionsWidget::apply()
{
    for (int row=0; row<tbwNotifies->rowCount(); row++)
    {
        QString typeId = tbwNotifies->item(row,NTC_TYPE)->data(NTR_TYPE).toString();
        NotificationType notifyType = FNotifications->notificationType(typeId);

        ushort typeKinds = notifyType.kindDefs & notifyType.kindMask;
        for (int col=1; col<tbwNotifies->columnCount(); col++)
        {
			ushort kind = tbwNotifies->item(row,col)->data(NTR_KIND).toInt();
            if (tbwNotifies->item(row,col)->checkState() == Qt::Checked)
                typeKinds |= kind;
            else
                typeKinds &= ~kind;
        }

        FNotifications->setTypeNotificationKinds(typeId,typeKinds);
    }
}

void NotifyKindOptionsWidget::reset()
{
    for (int row=0; row<tbwNotifies->rowCount(); row++)
    {
        QString typeId = tbwNotifies->item(row,NTC_TYPE)->data(NTR_TYPE).toString();
        ushort typeKinds = FNotifications->typeNotificationKinds(typeId);
        for (int col=1; col<tbwNotifies->columnCount(); col++)
        {
            ushort kind = tbwNotifies->item(row,col)->data(NTR_KIND).toInt();
            tbwNotifies->item(row,col)->setCheckState(typeKinds & kind ? Qt::Checked : Qt::Unchecked);

        }
    }
}

void NotifyKindOptionsWidget::showEvent(QShowEvent *AEvent)
{
    QWidget::showEvent(AEvent);

    int minHeight = tbwNotifies->horizontalHeader()->height() + tbwNotifies->frameWidth()*2;
    for (int row=0; row<tbwNotifies->rowCount(); row++)
        minHeight += tbwNotifies->verticalHeader()->sectionSize(row);
    tbwNotifies->setMinimumHeight(minHeight);
}
#endif
