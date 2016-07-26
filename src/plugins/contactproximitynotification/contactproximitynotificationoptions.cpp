#include <definitions/optionvalues.h>
#include "contactproximitynotificationoptions.h"
#include <utils/iconstorage.h>

#ifdef EYECU_MOBILE
#include "ui_contactproximitynotificationoptions2.h"
#else
#include "ui_contactproximitynotificationoptions.h"
#endif

#define SPACEFACTOR		4

ContactProximityNotificationOptions::ContactProximityNotificationOptions(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactProximityNotificationOptions)
{
    ui->setupUi(this);
#ifdef EYECU_MOBILE		// *** <<< eyeCU <<< ***
	ui->gridLayout->setSpacing(IconStorage::iconSize()/SPACEFACTOR);
	ui->verticalLayout->setSpacing(IconStorage::iconSize()/SPACEFACTOR);
#endif
    reset();
    updateUnits(ui->spbDistance);
    updateUnits(ui->spbTreshold);
}

ContactProximityNotificationOptions::~ContactProximityNotificationOptions()
{
    delete ui;
}

void ContactProximityNotificationOptions::apply()
{
    Options::node(OPV_CONTACTPROXIMITYNOTIFICATIONS_DISTANCE).setValue(ui->spbDistance->value());
    Options::node(OPV_CONTACTPROXIMITYNOTIFICATIONS_TRESHOLD).setValue(ui->spbTreshold->value());
    Options::node(OPV_CONTACTPROXIMITYNOTIFICATIONS_IGNOREOWN).setValue(ui->chkIgnoreOwnResources->isChecked());
    emit childApply();
}

void ContactProximityNotificationOptions::reset()
{
    ui->spbDistance->setValue(Options::node(OPV_CONTACTPROXIMITYNOTIFICATIONS_DISTANCE).value().toInt());
    ui->spbTreshold->setValue(Options::node(OPV_CONTACTPROXIMITYNOTIFICATIONS_TRESHOLD).value().toInt());
    ui->chkIgnoreOwnResources->setChecked(Options::node(OPV_CONTACTPROXIMITYNOTIFICATIONS_IGNOREOWN).value().toBool());
    emit childReset();
}

void ContactProximityNotificationOptions::updateUnits(QSpinBox *ASpinBox)
{
    ASpinBox->setSuffix(" "+tr("meter(s)", "distance units", ASpinBox->value()));
}

void ContactProximityNotificationOptions::onValueChanged(int AValue)
{
	Q_UNUSED(AValue)

    updateUnits(qobject_cast<QSpinBox *>(sender()));
    emit modified();
}

void ContactProximityNotificationOptions::onCheckStateChanged(int AChecked)
{
	Q_UNUSED(AChecked)

    emit modified();
}
