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
	app.setApplicationName("eyeCU");

//! *** <<< eyeCU <<< *********************
#ifdef EYECU_MOBILE

qDebug()<<"*****Main/devicePixelRatio="		<<qApp->primaryScreen()->devicePixelRatio();
qDebug()<<"*****Main/logicalDotsPerInch="	<<qApp->primaryScreen()->logicalDotsPerInch();
qDebug()<<"*****Main/physicalDotsPerInch="	<<qApp->primaryScreen()->physicalDotsPerInch();
qDebug()<<"*****Main/physicalSize="			<<qApp->primaryScreen()->physicalSize();
qDebug()<<"*****Main/screen/depth="			<<qApp->primaryScreen()->depth();

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
1. xlarge screens are at least [960dp x 720dp]
2. large  screens are at least [640dp x 480dp]
3. normal screens are at least [470dp x 320dp]
4. small  screens are at least [426dp x 320dp]
--------------------------------------------------------------------
*/
    QScreen *screen = qApp->primaryScreen();
	qreal logicalDotsPerInch= screen->logicalDotsPerInch();
	qreal physicalDotsPerInch= screen->physicalDotsPerInch();
	qreal midleDotsPerInch=(logicalDotsPerInch+physicalDotsPerInch)/2;

	qreal scale=1.0;
	float newPointSizeF=8;
	if(midleDotsPerInch>110 && midleDotsPerInch<=160)		{scale=1.5; newPointSizeF=12;}
	else if(midleDotsPerInch>160 && midleDotsPerInch<=240)	{scale=2.0; newPointSizeF=16;}
	else if(midleDotsPerInch>240 && midleDotsPerInch<=320)	{scale=2.5; newPointSizeF=20;}
	else if(midleDotsPerInch>320 && midleDotsPerInch<=480)	{scale=3.0; newPointSizeF=24;}
	else if(midleDotsPerInch>480 && midleDotsPerInch<=640)	{scale=3.5; newPointSizeF=28;}
	else if(midleDotsPerInch>640)							{scale=4.5;	newPointSizeF=36;}

#ifdef Q_OS_WIN		//! *** To DEBUG ****
	scale=2.0;
	newPointSizeF=16;
#endif

	QFont font = app.font();
	font.setPointSizeF(newPointSizeF);
	app.setFont(font);
	IconStorage::setScale(scale);
#else
	IconStorage::setScale(1.0);		//!---For Q_OS_WIN---
#endif
//! *** >>> eyeCU >>> ********************

	QLibrary utils(app.applicationDirPath()+"/utils",&app);
	utils.load();
	PluginManager pm(&app);
	pm.restart();
	return app.exec();
}
