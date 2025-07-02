QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwidget.cpp \
    artist.cpp \
    blueray.cpp \
    book.cpp \
    chattinglogwidget.cpp \
    client.cpp \
    customerswidget.cpp \
    editalbumwidget.cpp \
    editbookwidget.cpp \
    editproductwidget.cpp \
    info.cpp \
    join.cpp \
    main.cpp \
    mainController.cpp \
    mainwindow.cpp \
    orderwidget.cpp \
    productInfo.cpp \
    productwidget.cpp \
    server.cpp \
    userInfo.cpp

HEADERS += \
    adminwidget.h \
    chattinglogwidget.h \
    client.h \
    customerswidget.h \
    editalbumwidget.h \
    editbookwidget.h \
    editproductwidget.h \
    artist.h \
    blueray.h \
    book.h \
    commuInfo.h \
    info.h \
    join.h \
    mainController.h \
    mainwindow.h \
    music.h \
    orderwidget.h \
    productinfo.h \
    productwidget.h \
    userInfo.h \
    userManager.h \
    customerwidget.h \
    server.h

FORMS += \
    adminwidget.ui \
    customerswidget.ui \
    editalbumwidget.ui \
    editbookwidget.ui \
    server.ui \
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

# HEADERS += \
#     adminwidget.h \
#     artist.h \
#     blueray.h \
#     book.h \
#     chattinglogwidget.h \
#     client.h \
#     commuInfo.h \
#     customerswidget.h \
#     editalbumwidget.h \
#     editbookwidget.h \
#     editproductwidget.h \
#     info.h \
#     join.h \
#     mainController.h \
#     mainwindow.h \
#     music.h \
#     orderwidget.h \
#     productInfo.h \
#     productwidget.h \
#     server.h \
#     userInfo.h \
#     userManager.h
