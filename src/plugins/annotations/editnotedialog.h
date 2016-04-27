#ifndef EDITNOTEDIALOG_H
#define EDITNOTEDIALOG_H

#include <QDialog>
#include <interfaces/iannotations.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_editnotedialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_editnotedialog.h"
#endif

class EditNoteDialog :
	public QDialog
{
	Q_OBJECT;
public:
	EditNoteDialog(IAnnotations *AAnnotations, const Jid &AStreamJid, const Jid &AContactJid, QWidget *AParent = NULL);
	~EditNoteDialog();
	Jid streamJid() const;
	Jid contactJid() const;
signals:
	void dialogDestroyed();
protected slots:
	void onDialogAccepted();
private:
	Ui::EditNoteDialogClass ui;
private:
	IAnnotations *FAnnotations;
private:
	Jid FStreamJid;
	Jid FContactJid;
};

#endif // EDITNOTEDIALOG_H
