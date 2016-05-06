#include <QDebug>		//! ----TEMP---
#include <QScreen>
#include <QLibrary>
#include <QApplication>
#include <QStyleFactory>		// *** <<< eyeCU <<< ***
#include <utils/iconstorage.h>	// *** <<< eyeCU <<< ***
#include "pluginmanager.h"

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACX
	if (QSysInfo::MacintoshVersion == QSysInfo::MV_YOSEMITE )
	{
		// https://bugreports.qt-project.org/browse/QTBUG-40833
		QFont::insertSubstitution(".Helvetica Neue DeskInterface", "Helvetica Neue");
	}
#endif

	QApplication app(argc, argv);
	app.setQuitOnLastWindowClosed(false);
	app.addLibraryPath(app.applicationDirPath());
	app.setApplicationName(CLIENT_NAME);
// *** <<< eyeCU <<< ***
#ifdef Q_OS_MACX
	//Icons on the menu are not used in Mac
	app.setAttribute(Qt::AA_DontShowIconsInMenus);
#endif
// *** >>> eyeCU >>> ***

//! *** <<< eyeCU <<< *********************
/*!
densities:--dpi--icon--koef--font-comments--------------------------
desktop  : [96]  [16]  1.0   8	  (base)					~96dpi
ldpi	 : [120] [24]  1,5   12   (low)						~120dpi
mdpi	 : [160] [32]  2.0   16   (medium)(standard size)	~160dpi
hdpi	 : [240] [40]  2.5   20   (high)					~240dpi
xhdpi	 : [320] [48]  3.0   24   (extra-high)				~320dpi
xxhdpi	 : [480] [56]  3.5   28   (extra-extra-high)		~480dpi
xxxhdpi	 : [640] [72]  4.5   36   (extra-extra-extra-high)	~640dpi
--------------------------------------------------------------------
							   [height x width]
1. xlarge screens are at least [960dp  x 720dp]
2. large  screens are at least [640dp  x 480dp]
3. normal screens are at least [470dp  x 320dp]
4. small  screens are at least [426dp  x 320dp]
--------------------------------------------------------------------
*/
#ifdef EYECU_MOBILE
    QScreen *screen = qApp->primaryScreen();
	qreal logicalDotsPerInch= screen->logicalDotsPerInch();
	qreal physicalDotsPerInch= screen->physicalDotsPerInch();
	qreal midleDotsPerInch=(logicalDotsPerInch+physicalDotsPerInch)/2;
	qreal scale;					//! PointSizeF=96
    float newPointSizeF;
		 if(midleDotsPerInch<=110)                          {scale=1.0; newPointSizeF= 8.0;}
	else if(midleDotsPerInch>110 && midleDotsPerInch<=160)	{scale=1.5; newPointSizeF=12.0;}
	else if(midleDotsPerInch>160 && midleDotsPerInch<=240)	{scale=2.0; newPointSizeF=18.0;}
	else if(midleDotsPerInch>240 && midleDotsPerInch<=320)	{scale=2.5; newPointSizeF=20.0;}
	else if(midleDotsPerInch>320 && midleDotsPerInch<=480)	{scale=3.0; newPointSizeF=24.0;}
	else if(midleDotsPerInch>480 && midleDotsPerInch<=640)	{scale=3.5; newPointSizeF=28.0;}
	else if(midleDotsPerInch>640)							{scale=4.5;	newPointSizeF=36.0;}

//!---- delete Later-------
#ifdef Q_OS_WIN		//! *** To DEBUG ****
	scale=2.0;			//2.0
	newPointSizeF=16;	//16
#endif
//!---- delete Later-------

	// -- select style for APP ---
	QStringList styles=QStyleFactory::keys();	// "Android","Windows", "Fusion"
	if(styles.contains("Fusion"))//Qt::CaseInsensitive
		app.setStyle(QStyleFactory::create("Fusion"));
	//! Style modify
	int newSize=16*scale;
    QString chBoxStyle=QString("QCheckBox::indicator {width: %1px; height: %2px; spacing: %3px;}")
            .arg(newSize).arg(newSize).arg(4);
    QString radBotSyle=QString("QRadioButton::indicator {width: %1px; height: %2px; spacing: %3px;}")
            .arg(newSize).arg(newSize).arg(4);
    app.setStyleSheet(QString("%1 %2").arg(chBoxStyle).arg(radBotSyle));
    app.style()->setObjectName("Fusion");
	//!----------------
	QFont font = app.font();
	font.setPointSizeF(newPointSizeF);
	app.setFont(font);
	//!----------------
	IconStorage::setScale(scale);
	IconStorage::setFontPointSize(newPointSizeF);
#else
    IconStorage::setScale(1.0);         //!---For Q_OS_WIN OR DECKTOP---
    IconStorage::setFontPointSize(8.0); //!---For Q_OS_WIN OR DECKTOP---
#endif
//! *** >>> eyeCU >>> ********************

	QLibrary utils(app.applicationDirPath()+"/utils",&app);
	utils.load();
	PluginManager pm(&app);
	pm.restart();
	return app.exec();
}
