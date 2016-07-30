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

#define		CHBOXSTATE			Qt::ItemIsEnabled | Qt::ItemIsUserCheckable
//#define		CHBOXSIZEPOLICY		QSizePolicy::Maximum,QSizePolicy::Maximum
#define		CHBOXSIZEPOLICY		QSizePolicy::Preferred,QSizePolicy::Preferred
#define		LBLSIZEPOLICY		QSizePolicy::Expanding,QSizePolicy::Preferred



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
        QWidget *tabGeneral = new QWidget();
		QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		sizePolicy.setHorizontalStretch(0);
		sizePolicy.setVerticalStretch(0);
		sizePolicy.setHeightForWidth(tabGeneral->sizePolicy().hasHeightForWidth());
		tabGeneral->setSizePolicy(sizePolicy);

		QVBoxLayout *verticalLayout= new QVBoxLayout(tabGeneral);
		verticalLayout->setContentsMargins(2, 1, 2, 1);

		QGroupBox *grBoxNotify = new QGroupBox(tr("Notify On/Off"),tabGeneral);
		sizePolicy.setHeightForWidth(grBoxNotify->sizePolicy().hasHeightForWidth());
		grBoxNotify->setSizePolicy(sizePolicy);
		grBoxNotify->setCheckable(true);
		//connect(grBoxNotify,SIGNAL(clicked(bool)),thih,SLOT()));

		QVBoxLayout *vLayoutGrBox = new QVBoxLayout(grBoxNotify);
		vLayoutGrBox->setSpacing(4);
		vLayoutGrBox->setContentsMargins(2, 1, 2, 1);

		QHBoxLayout *horizontalLayout = new QHBoxLayout();
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

		QLabel *lblPlace = new QLabel(tr("Show Notification at"),grBoxNotify);
		horizontalLayout->addWidget(lblPlace);

		QRadioButton *rButTop = new QRadioButton(tr("Top"),grBoxNotify);
		QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
		sizePolicy1.setHorizontalStretch(0);
		sizePolicy1.setVerticalStretch(0);
		sizePolicy1.setHeightForWidth(rButTop->sizePolicy().hasHeightForWidth());
		rButTop->setSizePolicy(sizePolicy1);
		horizontalLayout->addWidget(rButTop);

		QLabel *lblOnly = new QLabel(tr("only"),grBoxNotify);
		lblOnly->setVisible(false);
		horizontalLayout->addWidget(lblOnly);

		QRadioButton *rButCentral = new QRadioButton(tr("Central"),grBoxNotify);
		sizePolicy1.setHeightForWidth(rButCentral->sizePolicy().hasHeightForWidth());
		rButCentral->setSizePolicy(sizePolicy1);
		horizontalLayout->addWidget(rButCentral);
		vLayoutGrBox->addLayout(horizontalLayout);

		QFrame *line = new QFrame(grBoxNotify);
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		vLayoutGrBox->addWidget(line);

		QFormLayout *formLayout = new QFormLayout();
		formLayout->setContentsMargins(size/2,0,0,0);
		formLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);
		formLayout->setVerticalSpacing(size/4);

		QCheckBox *chBoxSound = new QCheckBox(grBoxNotify);
		chBoxSound->setProperty("NTR_KIND",INotification::SoundPlay);
		QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Preferred);
		sizePolicy2.setHorizontalStretch(0);
		sizePolicy2.setVerticalStretch(0);
		sizePolicy2.setHeightForWidth(chBoxSound->sizePolicy().hasHeightForWidth());
		chBoxSound->setSizePolicy(sizePolicy2);
		if (it->kindMask & INotification::SoundPlay)
			chBoxSound->setEnabled(CHBOXSTATE);
		else
			chBoxSound->setEnabled(false);
		chBoxSound->setCheckState(Qt::Unchecked);
		QLabel *lblBoxSound = new QLabel(tr("Play sound at the notification"),grBoxNotify);
		QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Preferred);
		sizePolicy3.setHorizontalStretch(0);
		sizePolicy3.setVerticalStretch(0);
		sizePolicy3.setHeightForWidth(lblBoxSound->sizePolicy().hasHeightForWidth());
		lblBoxSound->setSizePolicy(sizePolicy3);
		lblBoxSound->setWordWrap(true);
		formLayout->insertRow(NTC_SOUND,chBoxSound,lblBoxSound);
		connect(chBoxSound,SIGNAL(clicked(bool)),SIGNAL(modified()));
//!---
		QCheckBox *chBoxPopUp = new QCheckBox(grBoxNotify);
		chBoxPopUp->setProperty("NTR_KIND",INotification::PopupWindow);
		QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Maximum);
		sizePolicy4.setHorizontalStretch(0);
		sizePolicy4.setVerticalStretch(0);
		sizePolicy4.setHeightForWidth(chBoxPopUp->sizePolicy().hasHeightForWidth());
		chBoxPopUp->setSizePolicy(sizePolicy4);
		if (it->kindMask & INotification::PopupWindow)
			chBoxPopUp->setEnabled(CHBOXSTATE);
		else
			chBoxPopUp->setEnabled(false);
		chBoxPopUp->setCheckState(Qt::Unchecked);
		QLabel *lblBoxPopUp = new QLabel(tr("Display a notification in popup window"),grBoxNotify);
		sizePolicy3.setHeightForWidth(lblBoxPopUp->sizePolicy().hasHeightForWidth());
		lblBoxPopUp->setSizePolicy(sizePolicy3);
		lblBoxPopUp->setWordWrap(true);
		formLayout->insertRow(NTC_POPUP,chBoxPopUp,lblBoxPopUp);
		connect(chBoxPopUp,SIGNAL(clicked(bool)),SIGNAL(modified()));
//!---
		QCheckBox *chBoxMinim = new QCheckBox(grBoxNotify);
		chBoxMinim->setProperty("NTR_KIND",INotification::ShowMinimized);
		sizePolicy2.setHeightForWidth(chBoxMinim->sizePolicy().hasHeightForWidth());
		chBoxMinim->setSizePolicy(sizePolicy2);
		QLabel *lblBoxMinim = new QLabel(tr("Show the corresponding window minimized in the taskbar"),grBoxNotify);
		sizePolicy3.setHeightForWidth(lblBoxMinim->sizePolicy().hasHeightForWidth());
		lblBoxMinim->setSizePolicy(sizePolicy3);
		lblBoxMinim->setWordWrap(true);
		if (it->kindMask & INotification::ShowMinimized)
			chBoxMinim->setEnabled(CHBOXSTATE);
		else
			chBoxMinim->setEnabled(false);
		chBoxMinim->setCheckState(Qt::Unchecked);
		formLayout->insertRow(NTC_MINIMIZED,chBoxMinim,lblBoxMinim);
		connect(chBoxMinim,SIGNAL(clicked(bool)),SIGNAL(modified()));
//!---
		QCheckBox *chBoxVibration = new QCheckBox(grBoxNotify);
		chBoxVibration->setProperty("NTR_KIND",INotification::AndroidVibrate);
		sizePolicy2.setHeightForWidth(chBoxVibration->sizePolicy().hasHeightForWidth());
		chBoxVibration->setSizePolicy(sizePolicy2);
		QLabel *lblVibration = new QLabel(tr("Vibration"),grBoxNotify);
		sizePolicy3.setHeightForWidth(lblVibration->sizePolicy().hasHeightForWidth());
		lblVibration->setSizePolicy(sizePolicy3);
		lblVibration->setWordWrap(true);
		if (it->kindMask & INotification::AndroidVibrate)
			chBoxVibration->setEnabled(CHBOXSTATE);
		else
			chBoxVibration->setEnabled(false);
		chBoxVibration->setCheckState(Qt::Unchecked);
		formLayout->insertRow(NTC_VIBRATION,chBoxVibration,lblVibration);
		connect(chBoxVibration,SIGNAL(clicked(bool)),SIGNAL(modified()));
//!---
		QCheckBox *chBoxLights = new QCheckBox(grBoxNotify);
		chBoxLights->setProperty("NTR_KIND",INotification::AndroidLights);
		sizePolicy2.setHeightForWidth(chBoxLights->sizePolicy().hasHeightForWidth());
		chBoxLights->setSizePolicy(sizePolicy2);
		QLabel *lblBoxLights = new QLabel(tr("Lights"),grBoxNotify);
		sizePolicy3.setHeightForWidth(lblBoxLights->sizePolicy().hasHeightForWidth());
		lblBoxLights->setSizePolicy(sizePolicy3);
		lblBoxLights->setWordWrap(true);
		if (it->kindMask & INotification::AndroidLights)
			chBoxLights->setEnabled(CHBOXSTATE);
		else
			chBoxLights->setEnabled(false);
		chBoxLights->setCheckState(Qt::Unchecked);
		formLayout->insertRow(NTC_LIGHTS,chBoxLights,lblBoxLights);
		connect(chBoxLights,SIGNAL(clicked(bool)),SIGNAL(modified()));

		//!-------
		vLayoutGrBox->addLayout(formLayout);
		verticalLayout->addWidget(grBoxNotify);

		//!----------
        tlbNotifies->addItem(tabGeneral,it->icon,it->title);
		plugTypeId << it->typeId;
	}

	tlbNotifies->setCurrentIndex(0);
	//tlbNotifies->setCurrentWidget(0);	// not working...
	vblLayout->addWidget(tlbNotifies);
	vblLayout->setMargin(0);

	reset();
}

void NotifyKindOptionsWidgetMobile::registrOrderedTypes()
{
	ushort visibleKinds = INotification::PopupWindow|INotification::AndroidVibrate|INotification::AndroidLights|INotification::SoundPlay|INotification::ShowMinimized;
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
	}
	tlbNotifies->setCurrentIndex(0);
}

//! TO DO -pass a parameter 'typeId' through the object properties
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
	}
}

void NotifyKindOptionsWidgetMobile::showEvent(QShowEvent *AEvent)
{
	QWidget::showEvent(AEvent);
}
