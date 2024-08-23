TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

TARGET = test

SOURCES += \
        test.cc \
        missing.cc \
        missing2.cc

HEADERS += \
        test.hh
