#include <utils/iconstorage.h>		// *** <<< eyeCU <<< ***
#include "setlocation.h"

#ifdef EYECU_MOBILE
#include "ui_setlocation2.h"
#else
#include "ui_setlocation.h"
#endif

SetLocation::SetLocation(const MercatorCoordinates &ACoordinates, const QIcon &AIcon, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetLocation)
{
    ui->setupUi(this);
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
	//QPixmap pixmap=IconStorage::getStoragePixmap(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_CLIENTINFO));
	QSize size(16*IconStorage::scale(),16*IconStorage::scale());
	ui->lblIcon->setPixmap(AIcon.pixmap(size));
	ui->verticalLayout->setMargin(16*IconStorage::scale());
#else	// *** >>> eyeCU >>> ***
    setWindowIcon(AIcon);
#endif
    ui->dspbLatitude->setValue(ACoordinates.latitude());
    ui->dspbLongitude->setValue(ACoordinates.longitude());
}

SetLocation::~SetLocation()
{
    delete ui;
}

MercatorCoordinates SetLocation::coordinates() const
{
    return MercatorCoordinates(ui->dspbLatitude->value(), ui->dspbLongitude->value());
}

void SetLocation::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
	}
}

void SetLocation::discard()
{
	done(Discarded);
}
