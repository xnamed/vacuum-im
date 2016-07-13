#ifndef STYLEEYECU_H
#define STYLEEYECU_H

#include <QObject>
#include <QApplication>
#include <QStyleFactory>
#include <QStyle>
#include <QScreen>
#include <QFont>

#include <utils/iconstorage.h>
//#include "dialog.h"

class StyleEyecu :public QObject
{
	Q_OBJECT
public:
	StyleEyecu(QApplication *APpl);
	void init();
	QString saveStyle();
protected:
    QString spinBox();
	QString doubleSpinBox();
    QString checkBox();
	QString comboBox();
    QString radioBottom();
    QString slider();
	QString treeWidget();
	QString progressBar();
	QString scrollBar();
	QString treeView();
private:
    QApplication *FAPpl;
    qreal   FScale;
    int     FSize;
    float   FPointSizeF;
};

#endif // STYLEEYECU_H
