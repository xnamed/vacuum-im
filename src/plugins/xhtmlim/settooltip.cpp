#include <definitions/resources.h>
#include <definitions/xhtmlicons.h>
#include <utils/iconstorage.h>

#include "settooltip.h"

#ifdef EYECU_MOBILE
#include "ui_settooltip2.h"
#else
#include "ui_settooltip.h"
#endif

SetToolTip::SetToolTip(int AType, const QString &ATitleText, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetToolTip)
{
    ui->setupUi(this);

#ifdef EYECU_MOBILE
	int size=IconStorage::iconSize();
    ui->verLayout->setMargin(size);
    ui->lblIcon->setPixmap(IconStorage::staticStorage(RSR_STORAGE_HTML)->getIcon(XHI_SET_TOOLTIP).pixmap(QSize(size,size)));
#else
    setWindowIcon(IconStorage::staticStorage(RSR_STORAGE_HTML)->getIcon(XHI_SET_TOOLTIP));
#endif

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
