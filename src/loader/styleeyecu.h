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
    QString spinBox();
    QString checkBox();
    QString radioBottom();
    QString slider();
    QString treeView();

private:
    QApplication *FAPpl;
    qreal   FScale;
    int     FSize;
    float   FPointSizeF;
};

#endif // STYLEEYECU_H
