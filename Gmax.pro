QT       += core gui opengl
QT += widgets opengl openglwidgets
LIBS += -lopengl32
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    newprojectdialog.cpp \
    window.cpp \
    workspace2dwidget.cpp \
    workspace3dwidget.cpp

HEADERS += \
    mainwindow.h \
    newprojectdialog.h \
    window.h \
    workspace2dwidget.h \
    workspace3dwidget.h

FORMS += \
    mainwindow.ui \
    newprojectdialog.ui \
    window.ui
include($$PWD/QtDock/QtDock.pri)
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    G-max.rc \
    QtDock/QtDock.pri

RESOURCES += \
    Gmax.qrc
RC_ICONS = G-max.ico
RC_FILE += G-max.rc
