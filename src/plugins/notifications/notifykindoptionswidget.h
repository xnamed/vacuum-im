#ifndef NOTIFYKINDOPTIONSWIDGET_H
#define NOTIFYKINDOPTIONSWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QToolBox>
#include <interfaces/inotifications.h>
#include <interfaces/ioptionsmanager.h>

#ifdef EYECU_MOBILE     // *** <<< eyeCU <<< ***
enum NotifiesTableRow {
    NTC_TYPE,
    NTC_SOUND,
    NTC_POPUP,
    NTC_MINIMIZED,
    NTC_VIBRATION,
    NTC_LIGHTS
};
#else      // *** >>> eyeCU >>> ***
enum NotifiesTableColumn {
    NTC_TYPE,
    NTC_SOUND,
    NTC_POPUP,
    NTC_MINIMIZED,
    NTC_TRAY,
    NTC__COUNT,
};
#endif

enum NotifiesTableRoles {
    NTR_TYPE = Qt::UserRole,
    NTR_KIND
};

struct NotificationType :	public INotificationType
{
    NotificationType(const INotificationType &AType) : INotificationType(AType) {}
    QString typeId;
};


class NotifyKindOptionsWidget :
	public QWidget,
	public IOptionsDialogWidget
{
	Q_OBJECT;
	Q_INTERFACES(IOptionsDialogWidget);
public:
	NotifyKindOptionsWidget(INotifications *ANotifications, QWidget *AParent);
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
#ifdef EYECU_MOBILE		// *** <<< eyeCU <<< ***
protected slots:
	void onCurrentChanged(int index);
protected:
	void mousePressEvent(QMouseEvent *AEvent){QWidget::mousePressEvent(AEvent);}
	void mouseMoveEvent(QMouseEvent *AEvent){QWidget::mouseMoveEvent(AEvent);}
	void mouseReleaseEvent(QMouseEvent *AEvent){QWidget::mouseReleaseEvent(AEvent);}
#endif			// *** >>> eyeCU >>> ***
private:
	virtual void showEvent(QShowEvent *AEvent);
private:
	INotifications *FNotifications;
private:
    QTableWidget    *tbwNotifies;
#ifdef EYECU_MOBILE     // *** <<< eyeCU <<< ***
    QToolBox        *tlbNotifies;
    QMultiMap<int, NotificationType> FOrderedTypes;
    QStringList     plugTypeId;
#endif                  // *** >>> eyeCU >>> ***
};

#endif // NOTIFYKINDOPTIONSWIDGET_H
