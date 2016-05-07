#include "aboutbox.h"
#include <QDebug>
#include <QDesktopServices>
#include <QScreen>
#include <QStyleFactory>
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
    QString aboutDevice=QString("<b>Device Settings</b><br>"
								"Logical  Dots Per Inch: %1 ;[%2x%3]<br>"
								"Physical Dots Per Inch: %4 [%5x%6]<br>"
								"Screen's physical size: [%7x%8]mm<br>"
								"Screen's resolution: [%9x%10]pixel<br>"
								"Pixel in mm: x=%11,&nbsp;y=%12<br>"
								"Pixel in mm2: %13<br>"
								"Application font point size: %14")
        .arg(screen->logicalDotsPerInch())
            .arg(screen->logicalDotsPerInchX()).arg(screen->logicalDotsPerInchY())
        .arg(screen->physicalDotsPerInch())
            .arg(screen->physicalDotsPerInchX()).arg(screen->physicalDotsPerInchY())
        .arg(screen->physicalSize().width()).arg(screen->physicalSize().height())
        .arg(screen->size().width()).arg(screen->size().height())
        .arg(screen->size().width()/screen->physicalSize().width())
        .arg(screen->size().height()/screen->physicalSize().height())
        .arg(screen->size().width()/screen->physicalSize().width()*screen->size().height()/screen->physicalSize().height())
        .arg(qApp->font().pointSizeF());
    ui.lblDevice->setText(aboutDevice);

	QString allStyles;
	for (int i = 0; i < QStyleFactory::keys().size(); ++i)
		allStyles += QStyleFactory::keys()[i]+", ";
	QString aboutStyles=QString("Current style: <b>%1</b> of %2")
			.arg(qApp->style()->objectName()).arg(allStyles);
	ui.lblStyles->setText(aboutStyles);
//!----------
    connect(ui.lblPosv,SIGNAL(linkActivated(const QString &)),SLOT(onLinkActivated()));
//!----------
	showMaximized();
#else
    ui.lblDevice->setVisible(false);
	ui.lblStyles->setVisible(false);
    ui.lblPosv->setVisible(false);
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

#ifdef EYECU_MOBILE
void AboutBox::onLinkActivated()
{
/*
Памяти моего безвременно ушедшего сына Вячеслава Вячеславовича Целых (14.11.1992-21.08.2015) посвящается.
*/
	QWidget *dedicated=new QWidget;
	dedicated->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
	dedicated->setStyleSheet(QString("background-color:red;"));

	QVBoxLayout *lay= new QVBoxLayout(dedicated);
	lay->setContentsMargins(2,2,2,2);
	lay->setAlignment(Qt::AlignVCenter);

    QLabel *lblIcon= new QLabel;
    lblIcon->setAlignment(Qt::AlignHCenter);
    QString fileName= IconStorage::staticStorage(RSR_STORAGE_MENUICONS)->fileFullName(SLAVIK);
    lblIcon->setPixmap(QPixmap::fromImage(QImageReader(fileName).read()));

    QLabel *lbl= new QLabel;
	lbl->setStyleSheet(QString("border:0;color:white;"));//background-color:red;
    lbl->setWordWrap(true);
	lbl->setAlignment(Qt::AlignHCenter);

//	QFont font=lbl->font();
//	font.setPointSizeF(lbl->font().pointSizeF()*IconStorage::scale()*.7);
//	lbl->setFont(font);
	lbl->setText(tr("Dedicated\nto my prematurely\ndeceased son\n Vyacheslav V. Tselykh\n14.11.1992 - 21.08.2015 "));
    lay->addWidget(lblIcon);
    lay->addWidget(lbl);

	dedicated->show();
}
#endif
