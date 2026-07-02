cmake_minimum_required(VERSION 3.16)
project(onlineImprovTheatre)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)
add_executable(testCommon
    main.cpp

    Common/Message.cpp
    Common/Packet.cpp
    Common/Role.cpp
    Common/Scene.cpp
    Common/User.cpp
)

target_include_directories(testCommon PRIVATE
    Common
)

target_link_libraries(testCommon
    Qt6::Core
    Qt6::Network
)