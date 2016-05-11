#ifndef SOCKSOPTIONSWIDGET_H
#define SOCKSOPTIONSWIDGET_H

#include <QWidget>
#include <interfaces/isocksstreams.h>
#include <interfaces/iconnectionmanager.h>
#include <interfaces/ioptionsmanager.h>
#include <utils/options.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_socksoptionswidget2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_socksoptionswidget.h"
#endif

class SocksOptionsWidget :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	SocksOptionsWidget(ISocksStreams *ASocksStreams, IConnectionManager *AConnectionManager, const OptionsNode &ANode, QWidget *AParent = NULL);
	virtual QWidget *instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
private:
	Ui::SocksOptionsWidgetClass ui;
private:
	OptionsNode FOptionsNode;
	ISocksStreams *FSocksStreams;
	IOptionsDialogWidget *FProxySettings;
	IConnectionManager *FConnectionManager;
};

#endif // SOCKSOPTIONSWIDGET_H
