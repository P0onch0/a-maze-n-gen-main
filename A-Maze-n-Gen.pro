QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += include \
               src

SOURCES += \
    src/main.cpp \
    src/cell.cpp \
    src/maze.cpp \
    src/coordinate.cpp \
    src/gui/mazesizeradiobutton.cpp \
    src/gui/startstoppushbutton.cpp \
    src/gui/mainwindow.cpp \
    src/gui/basewidgetmenu.cpp \
    src/gui/algorithmgeneratormenu.cpp \
    src/gui/fieldsizemenu.cpp \
    src/gui/mazearea.cpp

HEADERS += \
    include/cell.h \
    include/maze.h \
    include/coordinate.h \
    include/gui/mazesizeradiobutton.h \
    include/gui/startstoppushbutton.h \
    include/gui/algorithmgeneratormenu.h \
    include/gui/basewidgetmenu.h \
    include/gui/fieldsizemenu.h \
    include/gui/mainwindow.h \
    include/gui/mazearea.h

RC_FILE = resources/resources.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
