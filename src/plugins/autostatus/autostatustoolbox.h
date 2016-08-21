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


class AutoStatusToolBox : public QWidget
{
	Q_OBJECT
public:
	explicit AutoStatusToolBox(IAutoStatus *AAutoStatus, IStatusChanger *AStatusChanger, QWidget *AParent = NULL);
	~AutoStatusToolBox();
	void setupUi();
protected:
	QComboBox *getComboBox();
	QSpinBox *getSpinBox();
	QTimeEdit *getTimeEdit();
	int getCurrentStatus(int AIndex);
protected:
	void appendTableRow(const QUuid &ARuleId, const IAutoStatusRule &ARule);
protected slots:
	void onRuledItemSelectionChanged();
	void onDialogButtonBoxClicked(QAbstractButton *AButton);
private:
	IAutoStatus *FAutoStatus;
	IStatusChanger *FStatusChanger;
private:
	QToolBox *toolBox;
	QPushButton *pbtAdd;
	QPushButton *pbtDelete;
	QDialogButtonBox *dbbButtonBox;

};

#endif // AUTOSTATUSTOOLBOX_H
