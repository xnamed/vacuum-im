#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QObjectCleanupHandler>
#include <interfaces/ioptionsmanager.h>
#include <interfaces/imainwindow.h>

#ifdef EYECU_MOBILE
	#include "optionsdialogmobile.h"
#else
	#include "ui_optionsdialog.h"
#endif

class SortFilterProxyModel :
	public QSortFilterProxyModel
{
	Q_OBJECT
public:
	SortFilterProxyModel(QObject *AParent) : QSortFilterProxyModel(AParent) {}
protected:
	virtual bool lessThan(const QModelIndex &ALeft, const QModelIndex &ARight) const;
};

class OptionsDialog :
	public QDialog
#ifdef EYECU_MOBILE
    ,public IMainCentralPage
#endif
{
	Q_OBJECT
#ifdef EYECU_MOBILE
    Q_INTERFACES(IMainCentralPage)
#endif
public:
	OptionsDialog(IOptionsManager *AOptionsManager, const QString &ARootId = QString::null, QWidget *AParent = NULL);
	~OptionsDialog();
public:
    virtual OptionsDialog *instance() { return this; }
	void showNode(const QString &ANodeId);
#ifdef EYECU_MOBILE
    // IMainCentralPage
    void showCentralPage(bool AMinimized = false);
    QIcon centralPageIcon() const;
    QString centralPageCaption() const {return QString("Options");}
    QString centralPageName() const {return QString("options");}
#endif
signals:
	void applied();
	void reseted();
#ifdef EYECU_MOBILE
    // IMainCentralPage
    void centralPageShow(bool AMinimized);
    void centralPageChanged();
    void centralPageDestroyed();
#endif
protected:
	QWidget *createNodeWidget(const QString &ANodeId);
	QStandardItem *getNodeModelItem(const QString &ANodeId);
	bool canExpandVertically(const QWidget *AWidget) const;
// *** <<< eyeCU <<< ***
protected:
    bool eventFilter(QObject *AObj, QEvent *AEvent);
// *** >>> eyeCU >>> ***
protected slots:
	void onOptionsWidgetModified();
	void onOptionsDialogNodeInserted(const IOptionsDialogNode &ANode);
	void onOptionsDialogNodeRemoved(const IOptionsDialogNode &ANode);
	void onCurrentItemChanged(const QModelIndex &ACurrent, const QModelIndex &APrevious);
	void onDialogButtonClicked(QAbstractButton *AButton);
	void onClicked(const QModelIndex &ACurrent);	// *** <<< eyeCU <<< ***
private:
    Ui::OptionsDialogClass ui;

private:
	IOptionsManager *FOptionsManager;
private:
	QStandardItemModel *FItemsModel;
	SortFilterProxyModel *FProxyModel;
private:
	QString FRootNodeId;
	QObjectCleanupHandler FCleanupHandler;
	QMap<QString, QStandardItem *> FNodeItems;
	QMap<QStandardItem *, QWidget *> FItemWidgets;
    QString FTrvNodesStyle;     // *** <<< eyeCU <<< ***
    QString FScaStyle;          // *** <<< eyeCU <<< ***
	QSize	FNodeItemIconSize;  // *** <<< eyeCU <<< ***
	QSize	FNodeItemSizeHint;  // *** <<< eyeCU <<< ***
	bool	FStyleOn;			// *** <<< eyeCU <<< ***
};

#endif // OPTIONSDIALOG_H
