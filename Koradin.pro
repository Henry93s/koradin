QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blueray.cpp \
    book.cpp \
    chattinglogwidget.cpp \
    client.cpp \
    customerwidget.cpp \
    info.cpp \
    join.cpp \
    main.cpp \
    mainController.cpp \
    mainwindow.cpp \
    orderwidget.cpp \
    productInfo.cpp \
    productwidget.cpp \
    userInfo.cpp \
    #artist.cpp \
    orderInfo.cpp \
    userManager.cpp

HEADERS += \
    blueray.h \
    book.h \
    chattinglogwidget.h \
    client.h \
    customerwidget.h \
    info.h \
    join.h \
    mainController.h \
    mainwindow.h \
    orderwidget.h \
    productinfo.h \
    productwidget.h \
    userInfo.h \
    orderInfo.h \
    userManager.h \
    commuInfo.h \
    music.h

FORMS += \
    client.ui \
    join.ui \
    mainwindow.ui

TRANSLATIONS += \
    Koradin_ko_KR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitmessage.txt
