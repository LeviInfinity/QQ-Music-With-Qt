#-------------------------------------------------
#
# Project created by QtCreator 2016-12-30T16:01:32
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets script

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = QQMusic
TEMPLATE = app
RC_ICONS = ./images/qqmusic.ico 
RESOURCES += ./images/img.qrc
INCLUDEPATH += F:/OpenCV/opencv/build/include/opencv2 F:/OpenCV/opencv/build/include/opencv F:/OpenCV/opencv/build/include
LIBS += -LF:/OpenCV/opencv/build/x86/vc12/lib/ -lopencv_highgui248 -lopencv_core248 -lopencv_imgproc248

SOURCES += main.cpp\
        mainwidget.cpp \
    titlebar.cpp \
    mypushbutton.cpp \
    controlwidget.cpp \
    playlistwidget.cpp \
    trylistenwidget.cpp \
    mylovewidget.cpp \
    playhistorywidget.cpp \
    localmusicwidget.cpp \
    searchwidget.cpp \
    showlrcwidget.cpp

HEADERS  += mainwidget.h \
    titlebar.h \
    mypushbutton.h \
    controlwidget.h \
    playlistwidget.h \
    trylistenwidget.h \
    mylovewidget.h \
    playhistorywidget.h \
    localmusicwidget.h \
    searchwidget.h \
    showlrcwidget.h
