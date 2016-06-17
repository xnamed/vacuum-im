#ifndef COMMANDDIALOG_H
#define COMMANDDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <interfaces/icommands.h>
#include <interfaces/idataforms.h>

// *** <<< eyeCU <<< ***
#ifdef EYECU_MOBILE
#include "ui_commanddialog2.h"
#else
// *** >>> eyeCU >>> ***
#include "ui_commanddialog.h"
#endif

class CommandDialog :
	public QDialog,
	public ICommandClient
{
	Q_OBJECT;
	Q_INTERFACES(ICommandClient);
public:
	CommandDialog(ICommands *ACommands, IDataForms *ADataForms, const Jid &AStreamJid, const Jid &ACommandJid, const QString &ANode, QWidget *AParent = NULL);
	~CommandDialog();
	//ICommandClient
	virtual Jid streamJid() const;
	virtual Jid commandJid() const;
	virtual QString node() const;
	virtual QString sessionId() const;
	virtual bool receiveCommandResult(const ICommandResult &AResult);
	virtual bool receiveCommandError(const ICommandError &AError);
	//CommandDialog
	void executeCommand();
protected:
	void resetDialog();
	QString sendRequest(const QString &AAction);
	void executeAction(const QString &AAction);
protected slots:
	void onDialogButtonClicked(QAbstractButton *AButton);
private:
	Ui::CommandDialogClass ui;
private:
	ICommands *FCommands;
	IDataForms *FDataForms;
private:
	QPushButton *FPrevButton;
	QPushButton *FNextButton;
	QPushButton *FCompleteButton;
private:
	Jid FStreamJid;
	Jid FCommandJid;
	QString FNode;
	QString FRequestId;
	QString FSessionId;
	bool FCanceledByUser;
	IDataFormWidget *FCurrentForm;
};

#endif // COMMANDDIALOG_H
