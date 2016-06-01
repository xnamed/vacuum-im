FORMS   = \
    setuppluginsdialog.ui \
    setuppluginsdialog2.ui

anonymous:  FORMS += aboutbox_anonymous.ui
!anonymous: FORMS += aboutbox.ui

HEADERS = \
    pluginmanager.h \
    aboutbox.h \
    setuppluginsdialog.h \
    $$PWD/styleeyecu.h

SOURCES = \
    main.cpp \
    pluginmanager.cpp \
    aboutbox.cpp \
    setuppluginsdialog.cpp \
    $$PWD/styleeyecu.cpp
