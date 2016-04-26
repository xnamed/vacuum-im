#ifndef EDITPROXYDIALOG_H
#define EDITPROXYDIALOG_H

#include <QDialog>
#include <interfaces/iconnectionmanager.h>
// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_editproxydialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_editproxydialog.h"
#endif

class EditProxyDialog :
	public QDialog
{
	Q_OBJECT
public:
	EditProxyDialog(IConnectionManager *AManager, QWidget *AParent = NULL);
	~EditProxyDialog();
protected:
	QListWidgetItem *createProxyItem(const QUuid &AId, const IConnectionProxy &AProxy) const;
	void updateProxyItem(QListWidgetItem *AItem);
	void updateProxyWidgets(QListWidgetItem *AItem);
protected slots:
	void onAddButtonClicked(bool);
	void onDeleteButtonClicked(bool);
	void onCurrentProxyItemChanged(QListWidgetItem *ACurrent, QListWidgetItem *APrevious);
	void onDialogButtonBoxAccepted();
private:
	Ui::EditProxyDialogClass ui;
private:
	IConnectionManager *FManager;
};

#endif // EDITPROXYDIALOG_H
