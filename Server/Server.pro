QT += core network
CONFIG += console c++17
CONFIG -= app_bundle
TEMPLATE = app

SOURCES += \
    main.cpp \
    Server.cpp

HEADERS += \
    Server.h \
    ../Common/Protocol.h
