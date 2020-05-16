add_library(Registers STATIC MSRRegister.cpp CPUIDRegister.cpp BitDescriptor.cpp)
target_include_directories(Registers PUBLIC ../include)

add_executable(RyzMasterRace-qt RyzMasterRace-qt.cpp)
target_link_libraries(RyzMasterRace-qt Registers Qt5::Core Qt5::Widgets)