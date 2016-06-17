#ifndef EDITUSERSLISTDIALOG_H
#define EDITUSERSLISTDIALOG_H

#include <QDialog>
#include <interfaces/imultiuserchat.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_edituserslistdialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_edituserslistdialog.h"
#endif

class EditUsersListDialog :
	public QDialog
{
	Q_OBJECT;
public:
	EditUsersListDialog(const QString &AAffiliation, const QList<IMultiUserListItem> &AList, QWidget *AParent = NULL);
	~EditUsersListDialog();
	QString affiliation() const;
	QList<IMultiUserListItem> deltaList() const;
	void setTitle(const QString &ATitle);
protected slots:
	void onAddClicked();
	void onDeleteClicked();
private:
	Ui::EditUsersListDialogClass ui;
private:
	QString FAffiliation;
	QList<Jid> FDeletedItems;
	QMap<Jid,QTableWidgetItem *> FAddedItems;
	QMap<Jid,QTableWidgetItem *> FCurrentItems;
};

#endif // EDITUSERSLISTDIALOG_H
