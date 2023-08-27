QT       += core gui openglwidgets
LIBS     += -framework GLUT -framework OpenGL -framework Cocoa

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../back/affine.c \
    ../back/viewer_back.c \
    main.cpp \
    mainwindow.cpp \
    winopengl.cpp

HEADERS += \
    ../back/viewer_back.h \
    mainwindow.h \
    winopengl.h

FORMS += \
    mainwindow.ui

ICON =  3d.icns

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    3d.icns
