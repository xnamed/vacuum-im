#ifndef NOTIFYWIDGETMOBILE_H
#define NOTIFYWIDGETMOBILE_H

#include <QWidget>

#include <definitions/resources.h>
#include <definitions/menuicons.h>
#include <utils/iconstorage.h>
#include <interfaces/inotifications.h>
#include "notifykindoptionswidgetm.h"

namespace Ui {
class NotifyWidgetMobile;
}

class NotifyWidgetMobile : public QWidget
{
    Q_OBJECT

public:
    NotifyWidgetMobile(ushort AKinds, QWidget *parent = 0);
    ~NotifyWidgetMobile();
protected slots:
    void onRButOff(bool AStatus);
    void onRButStatusBar(bool AStatus);
    void onRButToast(bool AStatus);
signals:
    void wdModify();
protected:
    void init();
    void setValue();
    void setEnableStatusBar(bool AStatus);
    void setEnableToast(bool AStatus);
	void setVisStatusBar(bool AStatus);

private:
    Ui::NotifyWidgetMobile *ui;
    ushort FKinds;

};

#endif // NOTIFYWIDGETMOBILE_H
