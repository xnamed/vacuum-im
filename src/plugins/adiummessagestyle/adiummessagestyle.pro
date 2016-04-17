TARGET = adiummessagestyle 
os2: TARGET_SHORT	= adiummsg
!greaterThan(QT_VERSION,5.6):!android: QT += webkit
greaterThan(QT_MAJOR_VERSION,4):!greaterThan(QT_VERSION,5.6):  QT += webkitwidgets
QT -= phonon xmlpatterns
include(adiummessagestyle.pri) 
include(../plugins.inc) 
