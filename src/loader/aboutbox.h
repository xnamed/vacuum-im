#ifndef ABOUTBOX_H
#define ABOUTBOX_H

#include <QDialog>
#include <definitions/version.h>
#include <interfaces/ipluginmanager.h>

#ifdef EYECU_MOBILE
#include "ui_aboutbox2.h"
#else
#include "ui_aboutbox.h"
#endif

class AboutBox :
	public QDialog
{
	Q_OBJECT;
public:
	AboutBox(IPluginManager *APluginManager, QWidget *AParent = NULL);
	~AboutBox();
protected slots:
	void onLabelLinkActivated(const QString &ALink);
#ifdef EYECU_MOBILE
    void onLinkActivated();
#endif
private:
	Ui::AboutBoxClass ui;
};

#endif // ABOUTBOX_H
