add_executable(cmd cmd.cpp)

add_library(MSRRegister STATIC MSRRegister.cpp)
target_include_directories(MSRRegister PUBLIC ../include)