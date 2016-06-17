#ifndef EDITBOOKMARKDIALOG_H
#define EDITBOOKMARKDIALOG_H

#include <QDialog>
#include <interfaces/ibookmarks.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_editbookmarkdialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_editbookmarkdialog.h"
#endif

class EditBookmarkDialog :
	public QDialog
{
	Q_OBJECT;
public:
	EditBookmarkDialog(IBookmark *ABookmark, QWidget *AParent = NULL);
	~EditBookmarkDialog();
protected slots:
	void onGroupBoxClicked(bool AChecked);
	void onDialogAccepted();
private:
	Ui::EditBookmarkDialogClass ui;
private:
	IBookmark *FBookmark;
};

#endif // EDITBOOKMARKDIALOG_H
