#include <QDebug>		//! ----TEMP---
#include <QScreen>
#include <QLibrary>
#include <QApplication>
#include <QStyleFactory>		// *** <<< eyeCU <<< ***
#include <utils/iconstorage.h>
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
    //int dipNorm = 96; // desktop norm
#ifdef Q_OS_IOS
	int dipNorm = 160; // TODO test different sizes
#endif

qDebug()<<"*****************screen/physicalDotsPerInch="<< qApp->primaryScreen()->physicalDotsPerInch();
qDebug()<<"*****************screen/logicalDotsPerInch="<< qApp->primaryScreen()->logicalDotsPerInch();
qDebug()<<"*****************qApp->font/pointSizeF="<<qApp->font().pointSizeF()<<qApp->font().pixelSize();

#ifdef EYECU_MOBILE
//	if (QStyleFactory::keys().contains("fusion"))
//		app.setStyle(QStyleFactory::create("Fusion"));
    int dipNorm = 160;
    QScreen *screen = qApp->primaryScreen();
    qreal	dipScaleFactor = screen->physicalDotsPerInch() / screen->devicePixelRatio() / dipNorm;
	if(screen->logicalDotsPerInch()>96)
	{
		QFont font = app.font();
		int curFontPointSize=font.pointSize();
		if(curFontPointSize<0)
			curFontPointSize=8;
		float newPointSize =  curFontPointSize*dipScaleFactor;
newPointSize =  18.0;//!--TEMP--
		font.setPointSizeF(newPointSize);
		app.setFont(font);
		IconStorage::setScale(3);
	}
#endif
qDebug()<<"*****************New-font.PointSize="<< app.font().pointSize();
qDebug()<<"*****************New-App Style="<< app.style();
qDebug()<<"*****************Screen/physicalSize="<< app.primaryScreen()->physicalSize();
//! *** >>> eyeCU >>> ********************


	QLibrary utils(app.applicationDirPath()+"/utils",&app);
	utils.load();
	PluginManager pm(&app);
	pm.restart();
	return app.exec();
}
