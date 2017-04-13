#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T16:40:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceTracking
TEMPLATE = app

INCLUDEPATH += C:/InstallFiles/OpenCV3.1.0/build/include \
                              C:/InstallFiles/OpenCV3.1.0/build/include/opencv \
                              C:/InstallFiles/OpenCV3.1.0/build/include/opencv2

LIBS += C:/InstallFiles/OpenCV3.1.0/build/x86/mingw/lib/libopencv_*

SOURCES += main.cpp\
        mainwidget.cpp

HEADERS  += mainwidget.h

RESOURCES += \
    resources.qrc
