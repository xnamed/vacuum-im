#include "aboutbox.h"

#include <QDesktopServices>
#include <QScreen>
#include <utils/logger.h>
#include <utils/iconstorage.h>
#include <definitions/resources.h>
#include <definitions/menuicons.h>

AboutBox::AboutBox(IPluginManager *APluginManager, QWidget *AParent) : QDialog(AParent)
{
	REPORT_VIEW;
	ui.setupUi(this);
	setAttribute(Qt::WA_DeleteOnClose,true);

	ui.lblName->setText(CLIENT_NAME);
    ui.svgLogo->load(IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(MNI_EYECU_LOGO));
	ui.lblVersion->setText(tr("Version: %1.%2 %3").arg(APluginManager->version()).arg(APluginManager->revisionDate().date().toString("yyyyMMdd")).arg(CLIENT_VERSION_SUFFIX));
	ui.lblRevision->setText(tr("Revision: %1").arg(APluginManager->revision()));
// *** <<< eyeCU <<< ****
#ifdef EYECU_MOBILE
	QScreen *screen  = qApp->primaryScreen();
	QString aboutDevice=QString("<b>Device settings</b><br>"
				"LogicalDotsPerInch:%1<br>PhysicalDotsPerInch:%2<br>"
				"PhysicalSize:[%3x%4]cm<br>Depth:%5<br>"
				"devicePixelRatio:%6<br>Settings App PointSizeF:%7")
		.arg(screen->logicalDotsPerInch()).arg(screen->physicalDotsPerInch())
		.arg(screen->physicalSize().width()).arg(screen->physicalSize().height())
		.arg(screen->depth()).arg(screen->devicePixelRatio())
		.arg(qApp->font().pointSizeF());
	ui.lblDevice->setText(aboutDevice);
	showMaximized();
#else
	ui.lblDevice->setVisible(false);
#endif
// *** >>> eyeCU >>> ****
	connect(ui.lblHomePage,SIGNAL(linkActivated(const QString &)),SLOT(onLabelLinkActivated(const QString &)));
}

AboutBox::~AboutBox()
{

}

void AboutBox::onLabelLinkActivated(const QString &ALink)
{
	QDesktopServices::openUrl(ALink);
}
