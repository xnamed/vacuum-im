
#include "notifywidgetmobile.h"
#include "ui_notifywidgetmobile.h"

#define		CHBOXSTATE			Qt::ItemIsEnabled | Qt::ItemIsUserCheckable

NotifyWidgetMobile::NotifyWidgetMobile(ushort AKinds, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NotifyWidgetMobile),
    FKinds(AKinds)
{
    ui->setupUi(this);
    init();
    setValue();

    connect(ui->rButOff,SIGNAL(clicked(bool)),SLOT(onRButOff(bool)));
    connect(ui->rButStatusBar,SIGNAL(clicked(bool)),SLOT(onRButStatusBar(bool)));
    connect(ui->rButToast,SIGNAL(clicked(bool)),SLOT(onRButToast(bool)));

    connect(ui->rButOff,SIGNAL(clicked(bool)),SIGNAL(wdModify()));
    connect(ui->rButStatusBar,SIGNAL(clicked(bool)),SIGNAL(wdModify()));
    connect(ui->rButToast,SIGNAL(clicked(bool)),SIGNAL(wdModify()));
    connect(ui->chBoxSound,SIGNAL(clicked(bool)),SIGNAL(wdModify()));
    connect(ui->chBoxVibration,SIGNAL(clicked(bool)),SIGNAL(wdModify()));
    connect(ui->chBoxLights,SIGNAL(clicked(bool)),SIGNAL(wdModify()));

}

NotifyWidgetMobile::~NotifyWidgetMobile()
{
    delete ui;
}

void NotifyWidgetMobile::init()
{
    IconStorage	*AIconStorage=IconStorage::staticStorage(RSR_STORAGE_MENUICONS);
    QPixmap pixmapSound=AIconStorage->getStoragePixmap(AIconStorage->fileFullName(MNI_NOTIFICATIONS_SOUNDPLAY));
    QPixmap pixmapPopUp=AIconStorage->getStoragePixmap(AIconStorage->fileFullName(MNI_NOTIFICATIONS_PUPUPWINDOW));
    QPixmap pixmapMini=AIconStorage->getStoragePixmap(AIconStorage->fileFullName(MNI_NOTIFICATIONS_SHOWMINIMIZED));
    QPixmap pixmapSi=AIconStorage->getStoragePixmap(AIconStorage->fileFullName(MNI_NOTIFICATIONS_TRAYICON));
    QPixmap pixmapOff=AIconStorage->getStoragePixmap(AIconStorage->fileFullName(MNI_SCHANGER_CONNECTION_ERROR));

    ui->iconSound->setPixmap(pixmapSound);
    ui->iconToast->setPixmap(pixmapPopUp);
    ui->iconStatusBar->setPixmap(pixmapSi);
    ui->iconOff->setPixmap(pixmapOff);
    ui->iconVibro->setPixmap(pixmapMini);
    ui->iconLights->setPixmap(pixmapMini);
    ui->iconLTime->setPixmap(pixmapMini);
    ui->iconPlace->setPixmap(pixmapMini);

    //!-  for later ....
    bool visible=true;
    ui->iconSound->setVisible(visible);
    ui->iconToast->setVisible(visible);
    ui->iconStatusBar->setVisible(visible);
    ui->iconOff->setVisible(visible);
    ui->iconVibro->setVisible(visible);
    ui->iconLights->setVisible(visible);
    ui->iconLTime->setVisible(visible);
    ui->iconPlace->setVisible(visible);
    //!- ........
}

void NotifyWidgetMobile::setValue()
{
//    ui->chBoxPopUp->setProperty("NTR_KIND",INotification::PopupWindow);
//    ui->chBoxMinim->setProperty("NTR_KIND",INotification::ShowMinimized);

    ui->chBoxSound->setProperty("NTR_KIND",INotification::SoundPlay);
    if (FKinds & INotification::SoundPlay)
        ui->chBoxSound->setEnabled(CHBOXSTATE);
    else
        ui->chBoxSound->setEnabled(false);
    ui->chBoxSound->setCheckState(Qt::Unchecked);
//      lotGeneral->insertRow(NTC_SOUND,chBoxSound,lblBoxSound);

    ui->chBoxVibration->setProperty("NTR_KIND",INotification::Vibration);
    if (FKinds & INotification::Vibration)
        ui->chBoxVibration->setEnabled(CHBOXSTATE);
    else
        ui->chBoxVibration->setEnabled(false);
    ui->chBoxVibration->setCheckState(Qt::Unchecked);
//        lotGeneral->insertRow(NTC_VIBRATION,chBoxVibration,lblVibration);

    ui->chBoxLights->setProperty("NTR_KIND",INotification::Lights);
    if (FKinds & INotification::Lights)
        ui->chBoxLights->setEnabled(CHBOXSTATE);
    else
        ui->chBoxLights->setEnabled(false);
    ui->chBoxLights->setCheckState(Qt::Unchecked);
//        lotGeneral->insertRow(NTC_LIGHTS,chBoxLights,lblBoxLights);


/*        if (FKinds & INotification::PopupWindow)
            ui->chBoxPopUp->setEnabled(CHBOXSTATE);
        else
            ui->chBoxPopUp->setEnabled(false);
        ui->chBoxPopUp->setCheckState(Qt::Unchecked);
//        connect(ui->chBoxPopUp,SIGNAL(clicked(bool)),SIGNAL(modified()));

        if (FKinds & INotification::ShowMinimized)
            ui->chBoxMinim->setEnabled(CHBOXSTATE);
        else
            ui->chBoxMinim->setEnabled(false);
        ui->chBoxMinim->setCheckState(Qt::Unchecked);
//        lotGeneral->insertRow(NTC_MINIMIZED,chBoxMinim,lblBoxMinim);
//        connect(ui->chBoxMinim,SIGNAL(clicked(bool)),SIGNAL(modified()));
*/

}

void NotifyWidgetMobile::onRButOff(bool AStatus)
{
    setEnableStatusBar(!AStatus);
    setEnableToast(!AStatus);
}

void NotifyWidgetMobile::onRButStatusBar(bool AStatus)
{
    setEnableStatusBar(AStatus);
    setEnableToast(!AStatus);
}

void NotifyWidgetMobile::onRButToast(bool AStatus)
{
    setEnableToast(AStatus);
    setEnableStatusBar(!AStatus);
}

void NotifyWidgetMobile::setEnableStatusBar(bool AStatus)
{
    ui->iconStatusBar->setEnabled(AStatus);
    ui->lblOffStatusBar->setEnabled(AStatus);
    ui->chBoxSound->setEnabled(AStatus);
    ui->iconSound->setEnabled(AStatus);
    ui->lblSound->setEnabled(AStatus);
    ui->chBoxVibration->setEnabled(AStatus);
    ui->iconVibro->setEnabled(AStatus);
    ui->lblVibro->setEnabled(AStatus);
    ui->chBoxLights->setEnabled(AStatus);
    ui->iconLights->setEnabled(AStatus);
    ui->lblLights->setEnabled(AStatus);
}

void NotifyWidgetMobile::setEnableToast(bool AStatus)
{
    ui->iconToast->setEnabled(AStatus);
    ui->lblToast->setEnabled(AStatus);
    ui->chBoxLTime->setEnabled(AStatus);
    ui->iconLTime->setEnabled(AStatus);
    ui->lblLTime->setEnabled(AStatus);
    ui->chBoxPlace->setEnabled(AStatus);
    ui->iconPlace->setEnabled(AStatus);
    ui->lblPlace->setEnabled(AStatus);
}
