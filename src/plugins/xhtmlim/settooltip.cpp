#include <definitions/resources.h>
#include <definitions/xhtmlicons.h>
#include <utils/iconstorage.h>

#include "settooltip.h"
#include "ui_settooltip.h"

SetToolTip::SetToolTip(int AType, const QString &ATitleText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetToolTip)
{
	setWindowIcon(IconStorage::staticStorage(RSR_STORAGE_HTML)->getIcon(XHI_SET_TOOLTIP));
    ui->setupUi(this);
    ui->ledToolTipText->setText(ATitleText);
	ui->ledToolTipText->selectAll();
	ui->ledToolTipText->setFocus();
    ui->cmbType->setCurrentIndex(AType);
}

SetToolTip::~SetToolTip()
{
    delete ui;
}

QString SetToolTip::toolTipText() const
{
    return ui->ledToolTipText->text();
}

int SetToolTip::type() const
{
    return ui->cmbType->currentIndex();
}
