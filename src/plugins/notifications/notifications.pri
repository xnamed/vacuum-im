FORMS   = notifywidget.ui

HEADERS = notifications.h \
          notifywidget.h \
          notifytextbrowser.h \

SOURCES = notifications.cpp \
          notifywidget.cpp \
          notifytextbrowser.cpp \

equals(EYECU_MOBILE,"") {
    HEADERS += notifykindoptionswidget.h
    SOURCES += notifykindoptionswidget.cpp
}

equals(EYECU_MOBILE,true) {
    FORMS += notifywidgetmobile.ui

    HEADERS += notifykindoptionswidgetm.h
    HEADERS += notifywidgetmobile.h

    SOURCES += notifykindoptionswidgetm.cpp
    SOURCES += notifywidgetmobile.cpp
}
