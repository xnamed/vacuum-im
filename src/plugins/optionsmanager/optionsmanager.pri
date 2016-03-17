FORMS = \
        editprofilesdialog.ui \
        logindialog.ui

isEqual(EYECU_MOBILE,true){
    message(Mobile!= EYECU_MOBILE)
    FORMS += optionsdialogmobile.ui
} else {
    message(Not Mobile!= EYECU_MOBILE)
    FORMS += optionsdialog.ui
}

HEADERS = optionsmanager.h \
          optionsdialog.h \
          optionsdialogwidget.h \
          optionsdialogheader.h \
          editprofilesdialog.h \
          logindialog.h
#          newoptionsdialogclass.h

SOURCES = optionsmanager.cpp \
          optionsdialog.cpp \
          optionsdialogwidget.cpp \
          optionsdialogheader.cpp \
          editprofilesdialog.cpp \
          logindialog.cpp
#    newoptionsdialogclass.cpp
