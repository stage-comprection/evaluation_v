TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
LIBS += -lpthread

SOURCES += \
    src/main.cpp \
    src/master.cpp \
    src/file_handling.cpp \
    src/computations.cpp \
    src/utils.cpp

HEADERS += \
    src/master.h \
    src/file_handling.h \
    src/settings.h \
    src/output.h \
    src/read.h \
    src/computations.h \
    src/utils.h
