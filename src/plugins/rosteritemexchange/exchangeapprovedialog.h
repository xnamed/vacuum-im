#ifndef EXCHANGEAPPROVEDIALOG_H
#define EXCHANGEAPPROVEDIALOG_H

#include <QDialog>
#include <interfaces/irostermanager.h>
#include <interfaces/irosteritemexchange.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_exchangeapprovedialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_exchangeapprovedialog.h"
#endif

class ExchangeApproveDialog : 
	public QDialog
{
	Q_OBJECT;
public:
	ExchangeApproveDialog(IRoster *ARoster, const IRosterExchangeRequest &ARequest, QWidget *AParent = NULL);
	~ExchangeApproveDialog();
	bool subscribeNewContacts() const;
	IRosterExchangeRequest receivedRequest() const;
	IRosterExchangeRequest approvedRequest() const;
signals:
	void dialogDestroyed();
protected:
	QString groupSetToString(const QSet<QString> &AGroups) const;
	QString contactName(const Jid &AContactJid, bool AWithJid = true) const;
	void appendRequestItems(const QList<IRosterExchangeItem> &AItems);
private:
	Ui::ExchangeApproveDialogClass ui;
private:
	IRoster *FRoster;
	IRosterExchangeRequest FRequest;
	QMap<QTableWidgetItem *, IRosterExchangeItem> FItems;
};

#endif // EXCHANGEAPPROVEDIALOG_H
