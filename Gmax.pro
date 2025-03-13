QT       += core gui
QT += openglwidgets opengl
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
    QtDock/QtDock.pri \
    images/bag-handle-outline.svg \
    images/bug-outline.svg \
    images/color_lens.svg \
    images/document-outline.svg \
    images/folder-outline.svg \
    images/folder.svg \
    images/folder_open.svg \
    images/game-controller-outline.svg \
    images/help-circle-outline.svg \
    images/home-outline.svg \
    images/lock_outline.svg \
    images/note_add.svg \
    images/play-outline.svg \
    images/play.svg \
    images/save-outline.svg \
    images/settings-outline.svg \
    images/stop-outline.svg

RESOURCES += \
    Gmax.qrc
