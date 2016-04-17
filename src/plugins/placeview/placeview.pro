TARGET = placeview 
os2:TARGET_SHORT = plceview
include(placeview.pri) 
include(../plugins.inc)

!greaterThan(QT_VERSION,5.6): !android: QT += webkit

!greaterThan(QT_VERSION,5.6): greaterThan(QT_MAJOR_VERSION, 4) {
    QT += webkitwidgets
    QT += geo util
}
else: CONFIG += geo util
