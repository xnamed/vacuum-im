TARGET = adiummessagestyle 
os2: TARGET_SHORT	= adiummsg
!android: QT += webkit
greaterThan(QT_MAJOR_VERSION, 4): QT += webkitwidgets
QT -= phonon xmlpatterns
include(adiummessagestyle.pri) 
include(../plugins.inc) 
