QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
<<<<<<< HEAD
    chattinglogwidget.cpp \
    client.cpp \
    customerwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    orderwidget.cpp \
    productwidget.cpp

HEADERS += \
    chattinglogwidget.h \
    client.h \
    customerwidget.h \
    mainwindow.h \
    orderwidget.h \
    productwidget.h
=======
    artist.cpp \
    blueray.cpp \
    book.cpp \
    client.cpp \
    info.cpp \
    join.cpp \
    main.cpp \
    mainwindow.cpp \
    productInfo.cpp

HEADERS += \
    artist.h \
    blueray.h \
    book.h \
    client.h \
    commuInfo.h \
    info.h \
    join.h \
    mainwindow.h \
    music.h \
    productinfo.h
>>>>>>> 1977ef8ba06df5f1906c5d06ef932b3216099763

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
