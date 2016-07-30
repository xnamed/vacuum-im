#ifndef NOTIFYKINDOPTIONSWIDGETMOBILE_H
#define NOTIFYKINDOPTIONSWIDGETMOBILE_H

#include <QWidget>
#include <QTableWidget>
#include <QToolBox>
#include <interfaces/inotifications.h>
#include <interfaces/ioptionsmanager.h>

enum NotifiesTableRow {
    NTC_TYPE,
    NTC_SOUND,
    NTC_POPUP,
    NTC_MINIMIZED,
    NTC_VIBRATION,
    NTC_LIGHTS
};

enum NotifiesTableRoles {
    NTR_TYPE = Qt::UserRole,
    NTR_KIND
};

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
// *** <<< eyeCU <<< ***
protected slots:
	void onCurrentChanged(int index);
protected:
	void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
	void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
	void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}
// *** >>> eyeCU >>> ***
private:
	virtual void showEvent(QShowEvent *AEvent);
private:
	INotifications *FNotifications;
private:
    QToolBox        *tlbNotifies;
    QMultiMap<int, NotificationType> FOrderedTypes;
    QStringList     plugTypeId;
};

#endif // NOTIFYKINDOPTIONSWIDGETMOBILE_H
