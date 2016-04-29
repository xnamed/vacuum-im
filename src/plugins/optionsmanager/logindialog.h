#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <interfaces/ioptionsmanager.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_logindialog2.h"
#else
#include "ui_logindialog.h"
#endif
// *** >>> eyeCU >>> ***

class LoginDialog :
	public QDialog
{
	Q_OBJECT;
public:
	LoginDialog(IOptionsManager *AOptionsManager, QWidget *AParent = NULL);
	~LoginDialog();
protected slots:
	void onProfileAdded(const QString &AProfile);
	void onProfileRenamed(const QString &AProfile,const QString &ANewName);
	void onProfileRemoved(const QString &AProfile);
	void onEditProfilesClicked(bool);
	void onDialogAccepted();
	void onDialogRejected();
private:
	Ui::LoginDialogClass ui;
private:
	IOptionsManager *FManager;
};

#endif // LOGINDIALOG_H
