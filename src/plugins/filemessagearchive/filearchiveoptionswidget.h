#ifndef FILEARCHIVEOPTIONS_H
#define FILEARCHIVEOPTIONS_H

#include <interfaces/ipluginmanager.h>
#include <interfaces/ioptionsmanager.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_filearchiveoptionswidget2.h"
#else
#include "ui_filearchiveoptionswidget.h"
#endif
// *** >>> eyeCU >>> ***
class FileArchiveOptionsWidget : 
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	FileArchiveOptionsWidget(IPluginManager *APluginManager, QWidget *AParent = NULL);
	~FileArchiveOptionsWidget();
	virtual QWidget *instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
protected slots:
	void onSelectLocationFolder();
private:
	Ui::FileArchiveOptionsClass ui;
private:
	IPluginManager *FPluginManager;
};

#endif // FILEARCHIVEOPTIONS_H
