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
	int dipNorm = 96; // desktop norm
#ifdef Q_OS_IOS
	dipNorm = 160; // TODO test different sizes
#endif

#ifdef EYECU_MOBILE
	dipNorm = 160;
    QScreen *screen = qApp->primaryScreen();
    qreal	dipScaleFactor = screen->physicalDotsPerInch() / screen->devicePixelRatio() / dipNorm;
	if(screen->logicalDotsPerInch()>96)
	{
		QFont font = app.font();
		int curFontPointSize=font.pointSize();
		if(curFontPointSize<0)
			curFontPointSize=8;
		float newPointSize =  curFontPointSize*dipScaleFactor;
newPointSize =  17.0;//!--TEMP--
		font.setPointSizeF(newPointSize);
		app.setFont(font);
		int toSize=ceil(screen->devicePixelRatio()/96.0);
		IconStorage::setScale(3);
qDebug()<<"************************-main/newPointSize="<<newPointSize<<toSize;
	}
#else
	IconStorage::setScale(1);		//!---For Q_OS_WIN---
#endif
//! *** >>> eyeCU >>> ********************

	QLibrary utils(app.applicationDirPath()+"/utils",&app);
	utils.load();
	PluginManager pm(&app);
	pm.restart();
	return app.exec();
}
