QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwidget.cpp \
    blueray.cpp \
    bluerayManager.cpp \
    book.cpp \
    bookManager.cpp \
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
    music.cpp \
    musicManager.cpp \
    orderwidget.cpp \
    perproduct.cpp \
    popup.cpp \
    productInfo.cpp \
    productwidget.cpp \
    server.cpp \
    userInfo.cpp \
    orderInfo.cpp \
    userManager.cpp

HEADERS += \
    adminwidget.h \
    bluerayManager.h \
    bookManager.h \
    chattinglogwidget.h \
    client.h \
    customerswidget.h \
    editalbumwidget.h \
    editbookwidget.h \
    editproductwidget.h \
    blueray.h \
    book.h \
    commuInfo.h \
    customerwidget.h \
    info.h \
    join.h \
    mainController.h \
    mainwindow.h \
    musicManager.h \
    orderwidget.h \
    perproduct.h \
    popup.h \
    productinfo.h \
    productwidget.h \
    userInfo.h \
    userManager.h \
    server.h \
    orderInfo.h \
    music.h

FORMS += \
    adminwidget.ui \
    customerswidget.ui \
    editalbumwidget.ui \
    editbookwidget.ui \
    perproduct.ui \
    popup.ui \
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

#     userManager.h
