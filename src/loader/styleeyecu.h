#ifndef STYLEEYECU_H
#define STYLEEYECU_H

#include <QObject>
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QScreen>
#include <QFont>

#include <utils/iconstorage.h>

class StyleEyecu :public QObject
{
	Q_OBJECT
public:
	StyleEyecu(QApplication *APpl);
	void init();
	QString saveStyle();
protected:
	QString spinBox(int ASize);
	QString checkBox(int ASize);
	QString radioBot(int ASize);
	QString slider(int ASize);

private:
	qreal FScale;
};

#endif // STYLEEYECU_H
