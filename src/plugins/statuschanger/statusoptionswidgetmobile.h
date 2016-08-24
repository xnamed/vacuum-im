#ifndef STATUSOPTIONSWIDGETMOBILE_H
#define STATUSOPTIONSWIDGETMOBILE_H

#include <QObject>
#include <QPushButton>
#include <QTableWidget>
#include <QStyledItemDelegate>
#include <QToolBox>

#include <QWidgetItem>
#include <QSpinBox>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets/QFormLayout>
#include <QHeaderView>
#include <QMessageBox>

#include <QInputDialog>
#include <QTextDocument>
#include <QTextEdit>
#include <QPlainTextEdit>


#include <interfaces/ipresencemanager.h>
#include <interfaces/istatuschanger.h>
#include <interfaces/ioptionsmanager.h>

struct RowData2 {
    int show;
    QString name;
    QString fullFileName;
    QString messageText;
    int priority;
};

class StatusOptionsWidgetMobile :
    public QWidget,
    public IOptionsDialogWidget
{
    Q_OBJECT
    Q_INTERFACES(IOptionsDialogWidget)
public:
    StatusOptionsWidgetMobile(IStatusChanger *AStatusChanger, QWidget *AParent);
    virtual QWidget* instance() { return this; }

public slots:
    virtual void apply();
    virtual void reset();
signals:
    void modified();
    void childApply();
    void childReset();
protected slots:
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void onCurrentChanged(int AIndex);
protected:
	QComboBox *getComboBox();
    QSpinBox *getSpinBox();
    int getCurrentStatus(int AIndex);
private:
    IStatusChanger *FStatusChanger;
private:
    QPushButton *pbtAdd;
    QPushButton *pbtDelete;
    QToolBox *tlbStatus;
private:
    QList<int> FDeletedStatuses;
    QMap<int, RowData2> FStatusItems;
};

#endif // STATUSOPTIONSWIDGETMOBILE_H
