TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cc \
        iteration1.cc \
        iteration2.cc \
        iteration3.cc \
        iteration4.cc

HEADERS += \
        CLI11.hh \
        iteration1.hh \
        iteration2.hh \
        iteration3.hh \
        iteration4.hh
