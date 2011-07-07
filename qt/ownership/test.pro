TARGET = test

QT += core
QT += gui

CONFIG += console

INCLUDEPATH += $$(BOOSTPATH)

build_pass:CONFIG(release, debug|release) {
    win32 {
        QMAKE_CXXFLAGS += /W4 /wd4819 /wd4127 /wd4512 /wd4121 /wd4250
        QMAKE_LFLAGS += /MAP
        LIBS += -L$$(BOOSTLIBPATH)
    }
    unix {
        QMAKE_CXXFLAGS += -std=c++0x -Os -O3
        QMAKE_LFLAGS += -Wl,-Map=le.map
        !NO_BOOST_FILESYSTEM {
            LIBS += -L$$(BOOSTLIBPATH) -lboost_filesystem
        }
    }
}
build_pass:CONFIG(debug, debug|release) {
    win32 {
        QMAKE_CXXFLAGS += /W4 /wd4819 /wd4127 /wd4512 /wd4121 /wd4250
        QMAKE_LFLAGS += /MAP
        LIBS += -L$$(BOOSTLIBPATH)
    }
    unix {
        QMAKE_CXXFLAGS += -std=c++0x -g
        QMAKE_LFLAGS += -Wl,-Map=le.map
        !NO_BOOST_FILESYSTEM {
            LIBS += -L$$(BOOSTLIBPATH) -lboost_filesystem
        }
    }
}

INCLUDEPATH += $$(BOOSTPATH)
CONFIG += -Wall

#HEADERS += test.hpp
SOURCES += test.cpp
