#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T16:40:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FaceTracking
TEMPLATE = app
CONFIG += c++11

QMAKE_CXXFLAGS+= -fopenmp
QMAKE_LFLAGS +=  -fopenmp

INCLUDEPATH += C:/InstallFiles/OpenCV3.1.0/build/include \
                              C:/InstallFiles/OpenCV3.1.0/build/include/opencv \
                              C:/InstallFiles/OpenCV3.1.0/build/include/opencv2 \
                              ./SeetaFaceDetection/include

LIBS += C:/InstallFiles/OpenCV3.1.0/build/x86/mingw/lib/libopencv_*

SOURCES += main.cpp\
        mainwidget.cpp \
    SeetaFaceDetection/src/classifier/lab_boosted_classifier.cpp \
    SeetaFaceDetection/src/classifier/mlp.cpp \
    SeetaFaceDetection/src/classifier/surf_mlp.cpp \
    SeetaFaceDetection/src/feat/lab_feature_map.cpp \
    SeetaFaceDetection/src/feat/surf_feature_map.cpp \
    SeetaFaceDetection/src/io/lab_boost_model_reader.cpp \
    SeetaFaceDetection/src/io/surf_mlp_model_reader.cpp \
    SeetaFaceDetection/src/test/facedetection_test.cpp \
    SeetaFaceDetection/src/util/image_pyramid.cpp \
    SeetaFaceDetection/src/util/nms.cpp \
    SeetaFaceDetection/src/face_detection.cpp \
    SeetaFaceDetection/src/fust.cpp

HEADERS  += mainwidget.h \
    SeetaFaceDetection/include/classifier/lab_boosted_classifier.h \
    SeetaFaceDetection/include/classifier/mlp.h \
    SeetaFaceDetection/include/classifier/surf_mlp.h \
    SeetaFaceDetection/include/feat/lab_feature_map.h \
    SeetaFaceDetection/include/feat/surf_feature_map.h \
    SeetaFaceDetection/include/io/lab_boost_model_reader.h \
    SeetaFaceDetection/include/io/surf_mlp_model_reader.h \
    SeetaFaceDetection/include/util/image_pyramid.h \
    SeetaFaceDetection/include/util/math_func.h \
    SeetaFaceDetection/include/util/nms.h \
    SeetaFaceDetection/include/classifier.h \
    SeetaFaceDetection/include/common.h \
    SeetaFaceDetection/include/detector.h \
    SeetaFaceDetection/include/face_detection.h \
    SeetaFaceDetection/include/feature_map.h \
    SeetaFaceDetection/include/fust.h \
    SeetaFaceDetection/include/model_reader.h

RESOURCES += \
    resources.qrc

DISTFILES += \
    SeetaFaceDetection/model/seeta_fd_frontal_v1.0.bin
