cmake_minimum_required(VERSION 3.10)


# set the project name
project(RyzMasterRace)

# add the executable
add_executable(cmd src/cmd.cpp)
target_include_directories(cmd PUBLIC include)