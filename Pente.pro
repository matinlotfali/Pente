#-------------------------------------------------
#
# Project created by QtCreator 2015-08-19T17:00:26
#
#-------------------------------------------------

QMAKE_CFLAGS -= -O2
QMAKE_CFLAGS -= -O1
QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS -= -O1
QMAKE_CFLAGS = -O3
QMAKE_CXXFLAGS = -O3

QMAKE_CXXFLAGS += -std=c++11

QT += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pente
TEMPLATE = app

SOURCES += main.cpp\
    game.cpp \
    menu.cpp \
    opengl.cpp \
    mainform.cpp

HEADERS  += \
    game.h \
    menu.h \
    opengl.h \
    mainform.h

RESOURCES += \
    sounds.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
