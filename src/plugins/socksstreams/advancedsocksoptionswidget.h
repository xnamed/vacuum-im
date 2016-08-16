#ifndef ADVANCEDSOCKSOPTIONSWIDGET_H
#define ADVANCEDSOCKSOPTIONSWIDGET_H

#include <QWidget>
#include <interfaces/isocksstreams.h>
#include <interfaces/iconnectionmanager.h>
#include <interfaces/ioptionsmanager.h>
#include <utils/options.h>

#ifdef EYECU_MOBILE     // *** <<< eyeCU <<< ***
#include <utils/mgroupbox.h>
#include "ui_advancedsocksoptionswidget2.h"
#else               // *** >>> eyeCU >>> ***
#include "ui_advancedsocksoptionswidget.h"
#endif

class AdvancedSocksOptionsWidget :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	AdvancedSocksOptionsWidget(ISocksStreams *ASocksStreams, IConnectionManager *AConnectionManager, const OptionsNode &ANode, QWidget *AParent = NULL);
	~AdvancedSocksOptionsWidget();
	virtual QWidget *instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();

protected:
    virtual void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
    virtual void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
    virtual void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}

protected slots:
	void onAddStreamProxyClicked(bool);
	void onStreamProxyUpClicked(bool);
	void onStreamProxyDownClicked(bool);
	void onDeleteStreamProxyClicked(bool);
	void onTimeoutValueChanged(int ATimeout);
private:
	Ui::AdvancedSocksOptionsWidgetClass ui;
private:
	ISocksStreams *FSocksStreams;
	IConnectionManager *FConnectionManager;
	OptionsNode FOptionsNode;
	IOptionsDialogWidget *FProxySettings;
};

#endif // ADVANCEDSOCKSOPTIONSWIDGET_H
