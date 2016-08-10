#ifndef NOTIFYKINDOPTIONSWIDGETMOBILE_H
#define NOTIFYKINDOPTIONSWIDGETMOBILE_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QToolBox>
#include <interfaces/inotifications.h>
#include <interfaces/ioptionsmanager.h>

struct NotificationType :	public INotificationType
{
	NotificationType(const INotificationType &AType) : INotificationType(AType) {}
	QString typeId;
};


class NotifyKindOptionsWidgetMobile :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT
	Q_INTERFACES(IOptionsDialogWidget)
public:
	NotifyKindOptionsWidgetMobile(INotifications *ANotifications, QWidget *AParent);
	virtual QWidget *instance() { return this; }
public slots:
	virtual void apply();
	virtual void reset();
signals:
	void modified();
	void childApply();
	void childReset();
protected:
	void registrOrderedTypes();
protected slots:
	void onCurrentChanged(int index);
private:
	virtual void showEvent(QShowEvent *AEvent);
private:
	INotifications *FNotifications;
	QToolBox        *tlbNotifies;
	QMultiMap<int, NotificationType> FOrderedTypes;
	QStringList     plugTypeId;
};

#endif // NOTIFYKINDOPTIONSWIDGETMOBILE_H
