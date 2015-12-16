HEADERS   += qxtglobal.h \
             qxtglobalshortcut.h \
             qxtglobalshortcut_p.h

SOURCES   += qxtglobal.cpp \
             qxtglobalshortcut.cpp

win32 {
  SOURCES += qxtglobalshortcut_win.cpp
} else:macx {
  SOURCES += qxtglobalshortcut_mac.cpp
} else:os2 {
  SOURCES += qxtglobalshortcut_os2.cpp
} else:haiku {
  SOURCES += qxtglobalshortcut_haiku.cpp
} else:!android {
  SOURCES += qxtglobalshortcut_x11.cpp
}
