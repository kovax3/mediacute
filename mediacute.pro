#-------------------------------------------------
#
# Project created by QtCreator 2012-04-16T14:46:23
#
#-------------------------------------------------

QT       += core gui phonon

TARGET = mediacute
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    dialogopen.cpp \
    dialogscript.cpp

HEADERS  += mainwindow.h \
    dialogopen.h \
    dialogscript.h

FORMS    += mainwindow.ui \
    dialogopen.ui \
    dialogscript.ui

RESOURCES += \
    data.qrc



# install
 desktopfile.files = mediacute.desktop
 desktopfile.path = /usr/share/applications
 target.path = /usr/bin
 icon48.files = icons/48x48/mediacute.png
 icon48.path = /usr/share/icons/hicolor/48x48/apps
 icon128.files =icons/128x128/mediacute.png
 icon128.path = /usr/share/icons/hicolor/128x128/apps

 INSTALLS +=    desktopfile \
                target \
                icon48 \
                icon128
