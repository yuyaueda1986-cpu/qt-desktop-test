cmake_minimum_required(VERSION 3.16)
project(qt-desktop-test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SOURCES
    src/main.cpp
    src/config.cpp
    src/widgetfactory.cpp
    src/shapewidget.cpp
    src/mainwindow.cpp
)

set(HEADERS
    src/config.h
    src/widgetfactory.h
    src/shapewidget.h
    src/mainwindow.h
)

add_executable(qt-desktop-test ${SOURCES} ${HEADERS})

target_link_libraries(qt-desktop-test PRIVATE Qt6::Widgets)
