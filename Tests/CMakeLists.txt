cmake_minimum_required(VERSION 3.19)
project(CzhWidgetTests LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Test Network SerialPort)

qt_standard_project_setup()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

qt_add_executable(CommTest
    CommTest.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/CommProtocol.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/CommConfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/ICommInterface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/SerialComm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/TcpClientComm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/TcpServerComm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/UdpComm.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/CommFactory.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/CommManager.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/SerialComm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/TcpClientComm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/TcpServerComm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/UdpComm.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/CommFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../Services/Comm/CommManager.cpp
)

add_test(NAME CommTest COMMAND CommTest)

target_link_libraries(CommTest
    PRIVATE
        Qt::Core
        Qt::Test
        Qt::Network
        Qt::SerialPort
)
