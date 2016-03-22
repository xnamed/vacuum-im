#include "aboutbox.h"

#include <QDesktopServices>
#include <utils/logger.h>
#include <utils/iconstorage.h>
#include <definitions/resources.h>
#include <definitions/menuicons.h>

AboutBox::AboutBox(IPluginManager *APluginManager, QWidget *AParent) : QDialog(AParent)
{
	REPORT_VIEW;
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose,true);
#ifdef EYECU_MOBILE
    showMaximized();
#endif
	ui.lblName->setText(CLIENT_NAME);
    ui.svgLogo->load(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_EYECU_LOGO));
	ui.lblVersion->setText(tr("Version: %1.%2 %3").arg(APluginManager->version()).arg(APluginManager->revisionDate().date().toString("yyyyMMdd")).arg(CLIENT_VERSION_SUFFIX));
	ui.lblRevision->setText(tr("Revision: %1").arg(APluginManager->revision()));

	connect(ui.lblHomePage,SIGNAL(linkActivated(const QString &)),SLOT(onLabelLinkActivated(const QString &)));
}

AboutBox::~AboutBox()
{

}

void AboutBox::onLabelLinkActivated(const QString &ALink)
{
	QDesktopServices::openUrl(ALink);
}
