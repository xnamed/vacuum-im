include(../make/config.inc)

TARGET             = $$EYECU_UTILS_NAME
os2: TARGET_SHORT  = $$EYECU_UTILS_SNAME
TEMPLATE           = lib
VERSION            = $$EYECU_UTILS_ABI
CONFIG            += dll
QT                += xml network
greaterThan(QT_MAJOR_VERSION, 4): QT += util widgets
else: CONFIG += util

DEFINES           += UTILS_DLL QXT_STATIC

DEPENDPATH        += ..
INCLUDEPATH       += ..

DESTDIR            = ../libs
win32|os2 {
  DLLDESTDIR       = ..\\..
  QMAKE_DISTCLEAN += $${DLLDESTDIR}\\$${TARGET}.dll
}

LIBS              += -L../libs
LIBS              += -lidle -lidn -lminizip -lzlib
!android: LIBS    += -lqxtglobalshortcut
macx {
  QMAKE_LFLAGS    += -framework Carbon -framework IOKit -framework Cocoa
} else:unix:!haiku:!android {
  LIBS            += -lXss
  CONFIG          += x11
} else:win32 {
  LIBS            += -luser32
} else:os2 {
  LIBS            += -lpmhotkey
}

include(utils.pri)

#Install (for MacOS in loader.pro because instalation directory will be removed in loader installer)
!macx:!android {
  target.path      = $$INSTALL_LIBS
  INSTALLS        += target

  sdk_utils.path   = $$INSTALL_INCLUDES/utils
  sdk_utils.files  = *.h
  INSTALLS        += sdk_utils

  #Translation
  TRANS_BUILD_ROOT   = $${OUT_PWD}/../..
  TRANS_SOURCE_ROOT  = ..
  include(../translations/languages.inc)
}
