TEMPLATE = app
TARGET = NYSSE

QT += core gui widgets network multimedia

CONFIG += c++14

SOURCES += \
    actors/bonusbag.cpp \
    actors/player.cpp \
    core/city.cpp \
    core/statistics.cpp \
    creategame.cpp \
    graphics/actoritem.cpp \
    graphics/bomb.cpp \
    graphics/bonusbagitem.cpp \
    graphics/mainwindow.cpp \
    graphics/startdialog.cpp \
    graphics/usergraphicsitem.cpp \
    main.cc \

win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/release/ -lCourseLib
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/debug/ -lCourseLib
else:unix: LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/ -lCourseLib

INCLUDEPATH += \
    $$PWD/../Course/CourseLib

DEPENDPATH += \
    $$PWD/../Course/CourseLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/libCourseLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/libCourseLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/CourseLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/CourseLib.lib
else:unix: PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/libCourseLib.a

HEADERS += \
    actors/bonusbag.h \
    actors/player.hh \
    core/city.hh \
    core/statistics.hh \
    graphics/actoritem.hh \
    graphics/bomb.hh \
    graphics/bonusbagitem.h \
    graphics/mainwindow.hh \
    graphics/startdialog.hh \ \
    graphics/usergraphicsitem.hh

FORMS += \
    graphics/mainwindow.ui \
    graphics/startdialog.ui \

RESOURCES += \
    res.qrc


