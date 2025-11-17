QT       += core gui

# Conditional print support - not available on Android
!android: QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# Android specific configuration
android {
    # Qt 6: androidextras is integrated into core, no need to add it
    # Qt 5: add androidextras if needed
    lessThan(QT_MAJOR_VERSION, 6): QT += androidextras

    # Android package information
    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    # Minimum Android API level
    # Qt 6.7+ requires API 28 minimum
    # Qt 6.2-6.6 requires API 23 minimum
    ANDROID_MIN_SDK_VERSION = 28
    ANDROID_TARGET_SDK_VERSION = 33

    # Android permissions
    ANDROID_PERMISSIONS += \
        android.permission.READ_EXTERNAL_STORAGE \
        android.permission.WRITE_EXTERNAL_STORAGE \
        android.permission.INTERNET

    # Features
    ANDROID_FEATURES += \
        android.hardware.touchscreen
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Define Android build
android: DEFINES += ANDROID_BUILD

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    tools.cpp

HEADERS += \
    mainwindow.h \
    tools.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
    # Android deployment settings
    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target
}

RESOURCES += \
    resources.qrc
