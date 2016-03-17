#include "accountitemwidget.h"

#include <QTextDocument>
#include <definitions/menuicons.h>
#include <definitions/resources.h>
#include <utils/iconstorage.h>
#include <utils/qt4qt5compat.h>

#ifdef EYECU_MOBILE
	#define SIZEPIXMAP 32
#else
	#define SIZEPIXMAP 16
#endif

AccountItemWidget::AccountItemWidget(const QUuid &AAccountId, QWidget *AParent) : QWidget(AParent)
{
	ui.setupUi(this);
//ui.setupUi
	FAccountId = AAccountId;

	ui.lblMove->setVisible(false);
	IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->insertAutoIcon(ui.lblMove,MNI_ACCOUNT_MOVE,0,0,"pixmap");

	connect(ui.chbActive,SIGNAL(clicked(bool)),SIGNAL(modified()));

	ui.lblSettings->setText(QString("<a href='settings'>%1</a>").arg(tr("Settings...")));
	connect(ui.lblSettings,SIGNAL(linkActivated(const QString &)),SLOT(onSettingsLinkActivated()));

	connect(ui.tlbRemove,SIGNAL(clicked()),SLOT(onRemoveButtonClicked()));
}

AccountItemWidget::~AccountItemWidget()
{

}

QUuid AccountItemWidget::accountId() const
{
	return FAccountId;
}

bool AccountItemWidget::isActive() const
{
	return ui.chbActive->isChecked();
}

void AccountItemWidget::setActive(bool AActive)
{
	ui.chbActive->setChecked(AActive);
}

void AccountItemWidget::setIcon(const QIcon &AIcon)
{
	if (!AIcon.isNull())
		ui.lblIcon->setPixmap(AIcon.pixmap(QSize(SIZEPIXMAP,SIZEPIXMAP)));
	else
		ui.lblIcon->setVisible(false);
}

QString AccountItemWidget::name() const
{
	return FName;
}

void AccountItemWidget::setName(const QString &AName)
{
	FName = AName;
	ui.lblName->setText(QString("<b>%1<b>").arg(HTML_ESCAPE(AName)));
}

Jid AccountItemWidget::accountJid() const
{
	return FAccountJid;
}

void AccountItemWidget::setAccountJid(const Jid &AAccountJid)
{
	FAccountJid = AAccountJid;
	ui.lblJid->setText(QString("<%1>").arg(FAccountJid.uBare()));
}

void AccountItemWidget::enterEvent(QEvent *AEvent)
{
	Q_UNUSED(AEvent);
	ui.lblMove->setVisible(false);
}

void AccountItemWidget::leaveEvent(QEvent *AEvent)
{
	Q_UNUSED(AEvent);
	ui.lblMove->setVisible(false);
}

void AccountItemWidget::onRemoveButtonClicked()
{
	emit removeClicked(FAccountId);
}

void AccountItemWidget::onSettingsLinkActivated()
{
	emit settingsClicked(FAccountId);
}
