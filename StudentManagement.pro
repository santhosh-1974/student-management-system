QT += core gui widgets sql

SOURCES += \
        admindashboard.cpp \
        authmanager.cpp \
        database.cpp \
        loginwindow.cpp \
        main.cpp \
        mainwindow.cpp \
        requestmanager.cpp \
        studentdashboard.cpp \
        studentmanager.cpp

HEADERS += \
    admindashboard.h \
    authmanager.h \
    database.h \
    loginwindow.h \
    mainwindow.h \
    requestmanager.h \
    studentdashboard.h \
    studentmanager.h

FORMS += \
    admindashboard.ui \
    loginwindow.ui \
    mainwindow.ui \
    studentdashboard.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
