#ifndef INPUTTEXTDIALOG_H
#define INPUTTEXTDIALOG_H

#include <QDialog>
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_inputtextdialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_inputtextdialog.h"
#endif

class InputTextDialog :
	public QDialog
{
	Q_OBJECT;
public:
	InputTextDialog(QWidget *AParent, const QString &ATitle, const QString &ALabel, QString &AText);
	~InputTextDialog();
private:
	Ui::InputTextDialogClass ui;
protected slots:
	void onDialogButtonClicked(QAbstractButton *AButton);
private:
	QString &FText;
};

#endif // INPUTTEXTDIALOG_H
