#include "utils/iconstorage.h"
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
#ifdef EYECU_MOBILE
	int size=IconStorage::iconSize();
	ui->lblIcon->setPixmap(AIcon.pixmap(size,size));
	ui->lblTitle->setText(tr("New center"));
#else
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