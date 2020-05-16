cmake_minimum_required(VERSION 3.10)

project(RyzMasterRace)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_subdirectory(src)
add_subdirectory(test)