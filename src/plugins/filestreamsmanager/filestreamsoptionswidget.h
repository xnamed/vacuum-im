#ifndef FILESTREAMSOPTIONS_H
#define FILESTREAMSOPTIONS_H

#include <QWidget>
#include <QCheckBox>
#include <interfaces/ifilestreamsmanager.h>
#include <interfaces/idatastreamsmanager.h>
#include <interfaces/ioptionsmanager.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_filestreamsoptionswidget2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_filestreamsoptionswidget.h"
#endif

class FileStreamsOptionsWidget :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	FileStreamsOptionsWidget(IFileStreamsManager *AFileManager, QWidget *AParent);
	virtual QWidget* instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
protected slots:
	void onDirectoryButtonClicked();
#ifdef EYECU_MOBILE
    void onFileSelected(QString ANameDir){FSelDirName=ANameDir;}	 // *** <<< eyeCU >>> ***
#endif
private:
	Ui::FileStreamsOptionsWidgetClass ui;
private:
	IFileStreamsManager *FFileManager;
    QString FSelDirName;	/*** <<< eyeCU >>> ***/
};

#endif // FILESTREAMSOPTIONS_H
