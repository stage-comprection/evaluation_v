TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS_WARN_OFF -= -Wmaybe-uninitialized -Wformat
LIBS += -lpthread

SOURCES += \
    src/main.cpp \
    src/master.cpp \
    src/file_handling.cpp \
    src/computations.cpp \
    src/utils.cpp \
    src/binary.cpp \
    src/read_storage.cpp \
    src/mapping/mapping_utils.cpp \
    src/mapping/indexing.cpp \
    src/mapping/mapping.cpp

HEADERS += \
    src/master.h \
    src/file_handling.h \
    src/settings.h \
    src/output.h \
    src/computations.h \
    src/utils.h \
    src/binary.h \
    src/read_storage.h \
    src/mapping/BBhash.h \
    src/mapping/mapping_utils.h \
    src/mapping/indexing.h \
    src/mapping/mapping.h
