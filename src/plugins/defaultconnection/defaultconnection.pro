TARGET = defaultconnection
os2:TARGET_SHORT = dfltconn
include(defaultconnection.pri)
include(../plugins.inc)
lessThan(QT_MAJOR_VERSION, 5) {
include(../../thirdparty/jdns/jdns.pri)
DEPENDPATH  += ../../thirdparty/jdns
INCLUDEPATH += ../../thirdparty/jdns
}
