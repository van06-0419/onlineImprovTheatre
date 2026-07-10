cmake_minimum_required(VERSION 3.16)
project(ChatDemo)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network Widgets)

set(COMMON_SRC
    Common/CommonDef.h
    Common/Message.h Common/Message.cpp
    Common/Packet.h Common/Packet.cpp
    Common/Role.h Common/Role.cpp
    Common/Scene.h Common/Scene.cpp
    Common/User.h Common/User.cpp
)

# ChatServer 
add_executable(ChatServer
    Server/main.cpp
    Server/Server.h Server/Server.cpp
    Server/Room.h Server/Room.cpp
    Server/UserManager.h Server/UserManager.cpp
    ${COMMON_SRC}
)
# headPath
target_include_directories(ChatServer PRIVATE
    .
    Common
    Server
)
# Core+Network
target_link_libraries(ChatServer
    Qt6::Core
    Qt6::Network
)

# ChatClient
add_executable(ChatClient
    Client/main.cpp
    Client/Client.h Client/Client.cpp
    Client/LoginWindow.h Client/LoginWindow.cpp Client/LoginWindow.ui
    Client/ChatWindow.h Client/ChatWindow.cpp Client/ChatWindow.ui
    Client/LobbyWindow.h Client/LobbyWindow.cpp Client/LobbyWindow.ui
    Client/VoteWindow.h Client/VoteWindow.cpp Client/VoteWindow.ui
    ${COMMON_SRC}
)
target_include_directories(ChatClient PRIVATE
    .
    Common
    Client
)
# Widgets
target_link_libraries(ChatClient
    Qt6::Core
    Qt6::Network
    Qt6::Widgets
)
