#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include <interfaces/ijabbersearch.h>
#include <interfaces/idataforms.h>
#include <interfaces/iservicediscovery.h>
#include <interfaces/irosterchanger.h>
#include <interfaces/ivcardmanager.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_searchdialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_searchdialog.h"
#endif

class SearchDialog :
	public QDialog
{
	Q_OBJECT;
public:
	SearchDialog(IJabberSearch *ASearch, const Jid &AStreamJid, const Jid &AServiceJid, QWidget *AParent = NULL);
	~SearchDialog();
public:
	virtual Jid streamJid() const;
	virtual Jid serviceJid() const;
	virtual ISearchItem currentItem() const;
protected:
	void resetDialog();
	void requestFields();
	void requestResult();
	bool setDataForm(const IDataForm &AForm);
	void createToolBarActions();
protected slots:
	void onSearchFields(const QString &AId, const ISearchFields &AFields);
	void onSearchResult(const QString &AId, const ISearchResult &AResult);
	void onSearchError(const QString &AId, const XmppError &AError);
	void onToolBarActionTriggered(bool);
	void onDialogButtonClicked(QAbstractButton *AButton);
private:
	Ui::SearchDialogClass ui;
private:
	IJabberSearch *FSearch;
	IDataForms *FDataForms;
	IServiceDiscovery *FDiscovery;
	IVCardManager *FVCardManager;
	IRosterChanger *FRosterChanger;
private:
	Action *FDiscoInfo;
	Action *FAddContact;
	Action *FShowVCard;
	ToolBarChanger *FToolBarChanger;
private:
	Jid FStreamJid;
	Jid FServiceJid;
	QString FFieldsRequestId;
	QString FRequestId;
	IDataFormWidget *FCurrentForm;
};

#endif // SEARCHDIALOG_H
