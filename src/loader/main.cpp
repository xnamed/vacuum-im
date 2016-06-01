//#include <QLibrary>
#include <QApplication>

#include "styleeyecu.h"		// *** <<< eyeCU <<< ***
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
	StyleEyecu curStyle(&app);
	curStyle.init();
// *** >>> eyeCU >>> ***

	QLibrary utils(app.applicationDirPath()+"/utils",&app);
	utils.load();

	PluginManager pm(&app);
	pm.restart();
	return app.exec();
}
