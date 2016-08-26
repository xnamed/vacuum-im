#ifndef AUTOSTATUSTOOLBOX_H
#define AUTOSTATUSTOOLBOX_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QTimeEdit>
#include <QToolBox>
#include <QVBoxLayout>

#include <interfaces/ipresencemanager.h>
#include <interfaces/iautostatus.h>
#include <interfaces/istatuschanger.h>
#include <utils/options.h>
#include <utils/widgetmanager.h>
#include "utils/qt4qt5compat.h"


struct AutoStatusRuleExt
{
	IAutoStatusRule rule;
	QUuid uidId;
	bool enable;
};

class AutoStatusToolBox : public QWidget
{
	Q_OBJECT
public:
	explicit AutoStatusToolBox(IAutoStatus *AAutoStatus, IStatusChanger *AStatusChanger, QWidget *AParent = NULL);
	~AutoStatusToolBox();
signals:
	void m_accepted();
protected:
	QComboBox *getComboBox(QWidget *page=0);
	QSpinBox *getSpinBox(QWidget *page=0);
	QSpinBox *getSpinBoxTime(QWidget *page=0);
	int getCurrentStatus(int AIndex);
protected:
	void getStatusTable(const AutoStatusRuleExt ARuleExt, int ARow);
	QList<AutoStatusRuleExt> setAutoRules();
protected slots:
    void onRuledItemSelectionChanged(int AIndex);
	void onDialogButtonBoxClicked(QAbstractButton *AButton);
private:
    IAutoStatus     *FAutoStatus;
    IStatusChanger  *FStatusChanger;
private:
    QToolBox        *toolBox;
//    QPushButton     *pbtHelp;
	QDialogButtonBox *dbbButtonBox;
};

#endif // AUTOSTATUSTOOLBOX_H
