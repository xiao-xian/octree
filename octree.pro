#-------------------------------------------------
#
# Project created by QtCreator 2013-03-01T23:34:11
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = octree
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    octree.cpp \
    shader.cpp \
    helpers.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    octree.h \
    data_structure.h \
    shader.h \
    mesh.h \
    helpers.h \
    unitcube.h


#INCLUDEPATH += $$PWD/../../../../../../opt/local/include/boost

INCLUDEPATH += /user/boost/include

OTHER_FILES += \
    shader/phong.vert \
    shader/phong.frag
