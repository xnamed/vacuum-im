#include <QDebug>

#include <QHeaderView>
#include <QVBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QLabel>
#include <QCheckBox>
#include <QGroupBox>
#include <QRadioButton>

#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <utils/iconstorage.h>
#include "utils/qt4qt5compat.h"
#include <utils/advanceditemdelegate.h>

#include "notifykindoptionswidgetm.h"
#include "notifywidgetmobile.h"

//! for MAC OS: change widget <-> Label
NotifyKindOptionsWidgetMobile::NotifyKindOptionsWidgetMobile(INotifications *ANotifications, QWidget *AParent) : QWidget(AParent)
{
	FNotifications = ANotifications;
	int size=IconStorage::iconSize();
	registrOrderedTypes();

	QVBoxLayout *vblLayout = new QVBoxLayout(this);
	vblLayout->setSpacing(size/8);
    tlbNotifies=new QToolBox(this);
    tlbNotifies->setFrameStyle(QFrame::Panel | QFrame::Raised);
	tlbNotifies->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	connect(tlbNotifies,SIGNAL(currentChanged(int)),SLOT(onCurrentChanged(int)));

	QLabel *lblTitle = new QLabel(tr("Events"),this);
	QFont font= lblTitle->font();
	font.setBold(true);
	lblTitle->setFont(font);
	lblTitle->setAlignment(Qt::AlignHCenter);
	vblLayout->addWidget(lblTitle);

	for (QMultiMap<int, NotificationType>::const_iterator it=FOrderedTypes.constBegin(); it!=FOrderedTypes.constEnd(); ++it)
	{
        NotifyWidgetMobile *tabGeneral = new NotifyWidgetMobile(it->kindMask);
        connect(tabGeneral,SIGNAL(wdModify()),SIGNAL(modified()));
        connect(tlbNotifies,SIGNAL(currentChanged(int)),tabGeneral,SLOT(stress()));
        tlbNotifies->addItem(tabGeneral,it->icon,it->title);
        plugTypeId << it->typeId;
     }

     vblLayout->addWidget(tlbNotifies);
     vblLayout->setMargin(0);
     reset();

 }

void NotifyKindOptionsWidgetMobile::registrOrderedTypes()
{
#ifdef EYECU_MOBILE
     ushort visibleKinds = INotification::NotifyOff|
             INotification::StatusBar|INotification::SoundPlay|INotification::Vibration|INotification::Lights|
             INotification::PopupWindow|INotification::LongTime|INotification::PlaceView;
#else		// Only assembly
     ushort visibleKinds = INotification::PopupWindow|INotification::TrayNotify|INotification::TrayAction|
             INotification::SoundPlay|INotification::ShowMinimized;
#endif		// Only assembly
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

void NotifyKindOptionsWidgetMobile::onCurrentChanged(int index)
{
    tlbNotifies->widget(index)->adjustSize();
    tlbNotifies->widget(index)->resize(tlbNotifies->widget(index)->width(),tlbNotifies->widget(index)->minimumSizeHint().height());
}

void NotifyKindOptionsWidgetMobile::apply()
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
         QList<QRadioButton *> allRadioButtons = wd->findChildren<QRadioButton *>();
         if (allRadioButtons.count() > 0)
         {
             for (int i =0; i < allRadioButtons.count(); i++)
             {
                ushort kind = allRadioButtons.at(i)->property("NTR_KIND").toInt();
				if(allRadioButtons.at(i)->isChecked())
                    typeKinds |= kind;
                else
                    typeKinds &= ~kind;
             }
			 FNotifications->setTypeNotificationKinds(typeId,typeKinds);
         }
    }
    tlbNotifies->setCurrentIndex(0);
}

//! 01.06.16-TO DO -pass a parameter 'typeId' through the object properties
void NotifyKindOptionsWidgetMobile::reset()
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

        QList<QRadioButton *> allRadioButtons = wd->findChildren<QRadioButton *>();
        if (allRadioButtons.count() > 0)
        {
            for (int i =0; i < allRadioButtons.count(); i++)
            {
                ushort kind = allRadioButtons.at(i)->property("NTR_KIND").toInt();
                allRadioButtons.at(i)->setChecked(typeKinds & kind ? Qt::Checked : Qt::Unchecked);
            }
        }
    }
    tlbNotifies->setCurrentIndex(0);
}

void NotifyKindOptionsWidgetMobile::showEvent(QShowEvent *AEvent)
{
    QWidget::showEvent(AEvent);
}
