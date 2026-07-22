cmake_minimum_required(VERSION 3.16)

project(ImprovTheatre LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Network
    Multimedia
)
qt_standard_project_setup()

include_directories(${CMAKE_SOURCE_DIR}/Common)

qt_add_executable(ImprovClient
    Client/main.cpp
    Client/LoginWindow.h
    Client/LoginWindow.cpp
    Client/LoginWindow.ui
    Client/GameWindow.h
    Client/GameWindow.cpp
    Client/GameWindow.ui
    Client/AudioManager.h
    Client/AudioManager.cpp
    Client/GameLogic.h
    Client/GameLogic.cpp
    Client/NetworkClient.h
    Client/NetworkClient.cpp
)

target_link_libraries(ImprovClient PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Multimedia
)

if (WIN32)
    set_target_properties(ImprovClient PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()

qt_add_executable(ImprovServer
    Server/main.cpp
    Server/Server.h
    Server/Server.cpp
)

target_link_libraries(ImprovServer PRIVATE
    Qt6::Core
    Qt6::Network
)

if (WIN32 AND Qt6_FOUND)
    find_program(QT_WINDEPLOY_EXECUTABLE windeployqt PATHS ${Qt6_DIR}/bin)
    if (QT_WINDEPLOY_EXECUTABLE)
        install(CODE "
            execute_process(COMMAND ${QT_WINDEPLOY_EXECUTABLE} \$ENV{DESTDIR}\${CMAKE_INSTALL_BINDIR}/ImprovClient.exe)
            execute_process(COMMAND ${QT_WINDEPLOY_EXECUTABLE} \$ENV{DESTDIR}\${CMAKE_INSTALL_BINDIR}/ImprovServer.exe)
        ")
    endif()
endif()

install(TARGETS ImprovClient ImprovServer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)