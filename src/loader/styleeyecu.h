#ifndef STYLEEYECU_H
#define STYLEEYECU_H

#include <QObject>
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QScreen>
#include <QFont>
#include <QDir>

#include <definitions/resources.h>
#include <utils/iconstorage.h>

class StyleEyecu :public QObject
{
	Q_OBJECT
public:
	StyleEyecu(QApplication *APpl);
	void init();
	QString saveStyle();
protected:
    QString spinBox();
    QString checkBox();
    QString radioBot();
    QString slider();

private:
    qreal   FScale;
    int     FSize;
};

#endif // STYLEEYECU_H
