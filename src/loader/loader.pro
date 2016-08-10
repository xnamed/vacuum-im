include(../make/config.inc)

TARGET             = $$EYECU_LOADER_NAME
TEMPLATE           = app
QT                += xml svg
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS              += -L../libs
LIBS              += -l$$EYECU_UTILS_NAME

DEPENDPATH        += ..
INCLUDEPATH       += ..
DESTDIR            = ../..

equals(QMAKE_HOST.os, Windows)|equals(QMAKE_HOST.os, os2): MAYBE_QUOTE=\\\"
else: MAYBE_QUOTE=

DEFINES  += EXTRA_TRANSLATORS=\"$${MAYBE_QUOTE}$$EYECU_LOADER_NAME;$$EYECU_UTILS_NAME;qtgeo;$${MAYBE_QUOTE}\"

include(loader.pri)
#Appication icon
win32:RC_FILE      = loader.rc
os2:  RC_FILE      = loader2.rc

#symbian
symbian {
    TARGET.UID3 = 0xe3837d66
    # TARGET.CAPABILITY +=
    TARGET.EPOCSTACKSIZE = 0x14000
    TARGET.EPOCHEAPSIZE = 0x020000 0x800000
}

#GIT Info
GIT_HASH = $$system(git log -n 1 --format=%H)
GIT_DATE = $$system(git log -n 1 --format=%ct)
GIT_DATE = $$find(GIT_DATE,^\\d*)
!isEmpty(GIT_DATE) {
  equals(QMAKE_HOST.os, Windows)|equals(QMAKE_HOST.os, os2) {
    WIN_OUT_PWD = $$replace(OUT_PWD, /, \\)
    system(mkdir $${WIN_OUT_PWD} & echo $${LITERAL_HASH}define GIT_HASH \"$${GIT_HASH}\" > $${WIN_OUT_PWD}\\gitinfo.h) {
      system(echo $${LITERAL_HASH}define GIT_DATE \"$${GIT_DATE}\" >> $${WIN_OUT_PWD}\\gitinfo.h)
      DEFINES         += GITINFO
      QMAKE_DISTCLEAN += $${OUT_PWD}/gitinfo.h
    }
  } else {
    system(mkdir -p $${OUT_PWD} && echo \\$${LITERAL_HASH}define GIT_HASH \\\"$${GIT_HASH}\\\" > $${OUT_PWD}/gitinfo.h) {
      system(echo \\$${LITERAL_HASH}define GIT_DATE \\\"$${GIT_DATE}\\\" >> $${OUT_PWD}/gitinfo.h)
      DEFINES         += GITINFO
      QMAKE_DISTCLEAN += $${OUT_PWD}/gitinfo.h
    }
  }
}

#Install
target.path = $$INSTALL_BINS            #for android = /lib/armeabi-v7a OR /lib/armeabi
resources.path = $$INSTALL_RESOURCES    #for android = /assets/resources
resources.files = ../../resources/*
documents.path = $$INSTALL_DOCUMENTS
documents.files = ../../AUTHORS ../../CHANGELOG ../../README ../../COPYING ../../TRANSLATORS
INSTALLS += target resources documents

#Translation
TRANS_BUILD_ROOT   = $${OUT_PWD}/../..
TRANS_SOURCE_ROOT  = ..
include(../translations/languages.inc)

#Linux desktop install
unix:!macx:!android {
  icons.path       = $$INSTALL_PREFIX/$$INSTALL_RES_DIR/pixmaps
  icons.files      = ../../resources/menuicons/shared/eyecu.png
  INSTALLS        += icons
  desktop.path     = $$INSTALL_PREFIX/$$INSTALL_RES_DIR/applications
  desktop.files    = ../../src/packages/linux/*.desktop
  INSTALLS        += desktop
}

#MacOS Install
macx {
  UTILS_LIB_NAME   = lib$${EYECU_UTILS_NAME}.$${EYECU_UTILS_ABI}.dylib
  UTILS_LIB_LINK   = lib$${EYECU_UTILS_NAME}.dylib

  lib_utils.path   = $$INSTALL_LIBS
  lib_utils.extra  = cp -f ../libs/$$UTILS_LIB_NAME $(INSTALL_ROOT)$$INSTALL_LIBS/$$UTILS_LIB_NAME && \
                     ln -sf $$UTILS_LIB_NAME $(INSTALL_ROOT)$$INSTALL_LIBS/$$UTILS_LIB_LINK
  INSTALLS        += lib_utils

  name_tool.path   = $$INSTALL_BINS
  name_tool.extra  = install_name_tool -change $$UTILS_LIB_NAME @executable_path/../Frameworks/$$UTILS_LIB_NAME $(INSTALL_ROOT)$$INSTALL_BINS/$$INSTALL_APP_DIR/Contents/MacOS/$$VACUUM_LOADER_NAME
  INSTALLS        += name_tool

  sdk_utils.path   = $$INSTALL_INCLUDES/utils
  sdk_utils.files  = ../utils/*.h
  INSTALLS        += sdk_utils

  #Dirty hack to install utils translations
  TARGET           = $$EYECU_UTILS_NAME
  include(../translations/languages.inc)
  TARGET           = $$EYECU_LOADER_NAME

  ICON              = ../../eyecu.icns
  QMAKE_INFO_PLIST  = ../packages/macosx/Info.plist

  en_lproj.path     = $$INSTALL_RESOURCES/en.lproj/
  en_lproj.files    = ../packages/macosx/InfoPlist.strings
  INSTALLS         += en_lproj
}

#Android Install
greaterThan(QT_VERSION, 5.5) {
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_EXTRA_LIBS = \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Multimedia.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Network.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5PrintSupport.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Script.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Sql.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Svg.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5AndroidExtras.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Util.so \
        C:/QT/Qt5.6/5.6/android_armv7/lib/libQt5Geo.so \
        C:/QT/Qt5.6/5.6/android_armv7/plugins/sqldrivers/libqsqlite.so
    }
    #contains(ANDROID_TARGET_ARCH,armeabi) {
    #    ANDROID_EXTRA_LIBS = \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Multimedia.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Network.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5PrintSupport.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Script.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Sql.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Svg.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5AndroidExtras.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Util.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/lib/libQt5Geo.so \
    #    C:/QT/Qt5.6/5.6/android_armv5/plugins/sqldrivers/libqsqlite.so
    #}
    contains(ANDROID_TARGET_ARCH,android-x86) {
        ANDROID_EXTRA_LIBS = \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5Multimedia.so \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5Network.so \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5PrintSupport.so \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5Script.so \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5Sql.so \
        C:/QT/Qt5.6/5.6/android_86/lib/libQt5Svg.so \
        C:/QT/Qt5.6/5.6/android_86/lib/libQt5AndroidExtras.so \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5Util.so \
        C:/QT/Qt5.6/5.6/android_x86/lib/libQt5Geo.so \
        C:/QT/Qt5.6/5.6/android_x86/plugins/sqldrivers/libqsqlite.so
    }
message("1.CUR QT_VERSION" = $$QT_VERSION)
message("1.ANDROID_TARGET_ARCH" = $$ANDROID_TARGET_ARCH)
} else {
    contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
        ANDROID_EXTRA_LIBS = \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Multimedia.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Network.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5PrintSupport.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Script.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Sql.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Svg.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5AndroidExtras.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Util.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/lib/libQt5Geo.so \
        C:/QT/Qt5.5.1/5.5/android_armv7/plugins/sqldrivers/libqsqlite.so
    }
    contains(ANDROID_TARGET_ARCH,armeabi) {
        ANDROID_EXTRA_LIBS = \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Multimedia.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Network.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5PrintSupport.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Script.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Sql.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Svg.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5AndroidExtras.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Util.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/lib/libQt5Geo.so \
        C:/QT/Qt5.5.1/5.5/android_armv5/plugins/sqldrivers/libqsqlite.so
    }
    #contains(ANDROID_TARGET_ARCH,android-x86) {
    #    ANDROID_EXTRA_LIBS = \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Multimedia.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Network.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5PrintSupport.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Script.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Sql.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Svg.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5AndroidExtras.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Util.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/lib/libQt5Geo.so \
    #    C:/QT/Qt5.5.1/5.5/android_x86/plugins/sqldrivers/libqsqlite.so
    #}
message("2.CUR QT_VERSION" = $$QT_VERSION)
message("2.ANDROID_TARGET_ARCH" = $$ANDROID_TARGET_ARCH)
}

DISTFILES += \
    android/AndroidManifest.xml \
    android/res/values/libs.xml \
    android/build.gradle \
    android/src/rws/org/eyecu/NotificationClient.java

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
