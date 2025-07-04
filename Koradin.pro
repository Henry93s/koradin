QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adminwidget.cpp \
    blueray.cpp \
    book.cpp \
    chattinglogwidget.cpp \
    chattingroom.cpp \
    client.cpp \
    commuInfo.cpp \
    createnewroom.cpp \
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
    perchatting.cpp \
    perproduct.cpp \
    productInfo.cpp \
    productwidget.cpp \
    server.cpp \
    userInfo.cpp \
    orderInfo.cpp \
    userManager.cpp

HEADERS += \
    adminwidget.h \
    chattinglogwidget.h \
    chattingroom.h \
    client.h \
    createnewroom.h \
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
    orderwidget.h \
    perchatting.h \
    perproduct.h \
    productinfo.h \
    productwidget.h \
    tcpcommudefines.h \
    userInfo.h \
    userManager.h \
    server.h \
    orderInfo.h \
    music.h

FORMS += \
    adminwidget.ui \
    chattingroom.ui \
    createnewroom.ui \
    customerswidget.ui \
    editalbumwidget.ui \
    editbookwidget.ui \
    perchatting.ui \
    perproduct.ui \
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
