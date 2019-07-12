#-------------------------------------------------
#
# Project created by QtCreator 2019-03-06T13:06:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ORO_lab1
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    image.cpp \
    capturewindow.cpp \
    capturewindow.cpp \
    image.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
        mainwindow.h \
    mainwindow.h \
    image.h \
    capturewindow.h \
    capturewindow.h \
    image.h \
    mainwindow.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Images/Лб_1_001.jpg \
    Images/Лб_1_002.jpg \
    Images/Лб_1_003.jpg \
    Images/Лб_1_004.jpg \
    Images/Лб_1_005.jpg \
    Images/Лб_1_006.jpg \
    Images/Лб_1_007.jpg \
    Images/Лб_1_008.jpg \
    Images/Лб_1_009.jpg \
    Images/Лб_1_010.jpg \
    Images/Лб_1_011.jpg \
    Images/Лб_1_012.jpg \
    Images/Лб_1_013.jpg \
    Images/Лб_1_014.jpg \
    Images/Лб_1_015.jpg \
    Images/Лб_1_016.jpg \
    Images/Лб_1_017.jpg \
    Images/Лб_1_018.jpg \
    Images/Лб_1_019.jpg \
    Images/Лб_1_020.jpg \
    Images/Лб_1_021.jpg \
    Images/Лб_1_022.jpg \
    Images/Лб_1_023.jpg \
    Images/Лб_1_024.jpg \
    Images/Лб_1_025.jpg \
    Images/Лб_1_026.jpg \
    Images/Лб_1_027.jpg \
    Images/Лб_1_028.jpg \
    Images/Лб_1_029.jpg \
    Images/Лб_1_030.jpg \
    Images/Лб_1_031.jpg \
    Images/Лб_1_032.jpg \
    Images/Лб_1_001.jpg \
    Images/Лб_1_002.jpg \
    Images/Лб_1_003.jpg \
    Images/Лб_1_004.jpg \
    Images/Лб_1_005.jpg \
    Images/Лб_1_006.jpg \
    Images/Лб_1_007.jpg \
    Images/Лб_1_008.jpg \
    Images/Лб_1_009.jpg \
    Images/Лб_1_010.jpg \
    Images/Лб_1_011.jpg \
    Images/Лб_1_012.jpg \
    Images/Лб_1_013.jpg \
    Images/Лб_1_014.jpg \
    Images/Лб_1_015.jpg \
    Images/Лб_1_016.jpg \
    Images/Лб_1_017.jpg \
    Images/Лб_1_018.jpg \
    Images/Лб_1_019.jpg \
    Images/Лб_1_020.jpg \
    Images/Лб_1_021.jpg \
    Images/Лб_1_022.jpg \
    Images/Лб_1_023.jpg \
    Images/Лб_1_024.jpg \
    Images/Лб_1_025.jpg \
    Images/Лб_1_026.jpg \
    Images/Лб_1_027.jpg \
    Images/Лб_1_028.jpg \
    Images/Лб_1_029.jpg \
    Images/Лб_1_030.jpg \
    Images/Лб_1_031.jpg \
    Images/Лб_1_032.jpg

RESOURCES += \
    resources.qrc
