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
};

#endif // STYLEEYECU_H
