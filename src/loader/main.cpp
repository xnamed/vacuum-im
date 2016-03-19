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
	QScreen *screen = qApp->primaryScreen();
	QFont font = app.font();
	int curFontPointSize=font.pointSize();
	if(curFontPointSize<0)
		curFontPointSize=8;
	qreal dipScaleFactor;

#ifdef EYECU_MOBILE
	int dipNorm = 160;
	dipScaleFactor = screen->physicalDotsPerInch() / screen->devicePixelRatio() / dipNorm;
	float newPointSize = 17.0; // curFontPointSize*dipScaleFactor;
	font.setPointSizeF(newPointSize);
	app.setFont(font);
	//!---------
	IconStorage::setScale(3);
#else
	int dipNorm = 96; // desktop norm
	dipScaleFactor = 1;
	IconStorage::setScale(1);		//!---For Q_OS_WIN---
#endif
	//qreal dipScaleFactor = screen->physicalDotsPerInch() / screen->devicePixelRatio() / dipNorm;

//! *** >>> eyeCU >>> ********************

	QLibrary utils(app.applicationDirPath()+"/utils",&app);
	utils.load();
	PluginManager pm(&app);
	pm.restart();
	return app.exec();
}
