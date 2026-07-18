QT += core gui widgets network multimedia multimedia
CONFIG += c++17 console
TEMPLATE = app

SOURCES += \
    main.cpp \
    LoginWindow.cpp \
    GameWindow.cpp \
    NetworkClient.cpp \
    AudioManager.cpp \
    GameLogic.cpp

HEADERS += \
    LoginWindow.h \
    GameWindow.h \
    NetworkClient.h \
    AudioManager.h \
    GameLogic.h \
    ../Common/Protocol.h

FORMS += \
    LoginWindow.ui \
    GameWindow.ui