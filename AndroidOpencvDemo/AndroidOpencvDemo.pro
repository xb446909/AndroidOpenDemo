#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T12:55:14
#
#-------------------------------------------------

QT       += core gui androidextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AndroidOpencvDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    androidimagepicker.cpp \
    imageproc.cpp \
    cameracap.cpp

HEADERS  += mainwindow.h \
    androidimagepicker.h \
    imageproc.h \
    cameracap.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

contains(ANDROID_TARGET_ARCH,x86) {
    ANDROID_EXTRA_LIBS = \
        $$PWD/../sdk/native/libs/x86/libopencv_java3.so
}

unix:!macx: LIBS += -L$$PWD/../sdk/native/libs/x86/ -lopencv_java3 -lopencv_core

INCLUDEPATH += $$PWD/../sdk/native/jni/include
DEPENDPATH += $$PWD/../sdk/native/jni/include
