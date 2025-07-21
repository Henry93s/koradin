QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addAtUserList.cpp \
    adminwidget.cpp \
    blueray.cpp \
    bluerayManager.cpp \
    bluerayitem.cpp \
    book.cpp \
    bookManager.cpp \
    bookitem.cpp \
    chattinglogwidget.cpp \
    chattingroom.cpp \
    client.cpp \
    clienthandler.cpp \
    commuInfo.cpp \
    createnewroom.cpp \
    clientBluerayService.cpp \
    clientBookService.cpp \
    clientHomeService.cpp \
    clientMusicService.cpp \
    clientService.cpp \
    customerswidget.cpp \
    editalbumwidget.cpp \
    editbookwidget.cpp \
    editproductwidget.cpp \
    info.cpp \
    join.cpp \
    logManager.cpp \
    main.cpp \
    mainController.cpp \
    mainwindow.cpp \
    music.cpp \
    musicManager.cpp \
    musicitem.cpp \
    order.cpp \
    orderManager.cpp \
    orderitem.cpp \
    orderwidget.cpp \
    perchatting.cpp \
    perproduct.cpp \
    popup.cpp \
    productInfo.cpp \
    productwidget.cpp \
    server.cpp \
    userInfo.cpp \
    orderInfo.cpp \
    userManager.cpp \
    useritem.cpp \
    whisperdialog.cpp

HEADERS += \
    MapToVector.h \
    addAtUserList.h \
    adminwidget.h \
    bluerayManager.h \
    bluerayitem.h \
    bookManager.h \
    bookitem.h \
    chattinglogwidget.h \
    chattingroom.h \
    client.h \
    clienthandler.h \
    createnewroom.h \
    clientBluerayService.h \
    clientBookService.h \
    clientHomeService.h \
    clientMusicService.h \
    clientService.h \
    commonVariable.h \
    customerswidget.h \
    editalbumwidget.h \
    editbookwidget.h \
    editproductwidget.h \
    blueray.h \
    book.h \
    commuInfo.h \
    customerwidget.h \
    idproduct.h \
    info.h \
    join.h \
    logManager.h \
    mainController.h \
    mainwindow.h \
    musicManager.h \
    musicitem.h \
    order.h \
    orderManager.h \
    orderitem.h \
    orderwidget.h \
    perchatting.h \
    perproduct.h \
    popup.h \
    productInfo.h \
    productwidget.h \
    tcpcommudefines.h \
    userInfo.h \
    userManager.h \
    server.h \
    orderInfo.h \
    music.h \
    useritem.h \
    whisperdialog.h
    uuidCompare.h

FORMS += \
    adminwidget.ui \
    chattingroom.ui \
    createnewroom.ui \
    bluerayitem.ui \
    bookitem.ui \
    customerswidget.ui \
    editalbumwidget.ui \
    editbookwidget.ui \
    perchatting.ui \
    musicitem.ui \
    orderitem.ui \
    perproduct.ui \
    popup.ui \
    server.ui \
    client.ui \
    join.ui \
    mainwindow.ui
TRANSLATIONS += \
    Koradin_ko_KR.ts \
    Koradin_en_US.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    .gitmessage.txt

#     userManager.h
