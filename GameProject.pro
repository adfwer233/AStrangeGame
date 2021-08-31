QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithm.cpp \
    archer.cpp \
    battlefieldwidget.cpp \
    battlefieldscene.cpp \
    battlefieldview.cpp \
    flightequipment.cpp \
    graphland.cpp \
    graphunit.cpp \
    infantary.cpp \
    main.cpp \
    mainwindow.cpp \
    normalland.cpp \
    obstacle.cpp \
    role.cpp \
    rolebuff.cpp \
    roleskill.cpp \
    rolestatuspanel.cpp \
    roundstatuspanel.cpp 

HEADERS += \
    algorithm.h \
    archer.h \
    battlefieldwidget.h \
    battlefieldscene.h \
    battlefieldview.h \
    flightequipment.h \
    graphland.h \
    graphunit.h \
    infantary.h \
    mainwindow.h \
    normalland.h \
    obstacle.h \
    role.h \
    rolebuff.h \
    roleskill.h \
    rolestatuspanel.h \
    statusConstant.h \
    roundstatuspanel.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
	gifs.qrc
