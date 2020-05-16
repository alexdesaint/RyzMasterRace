add_executable(MSRtest MSRtest.cpp)
target_link_libraries(MSRtest Registers)

add_executable(CPUIDtest CPUIDtest.cpp)
target_link_libraries(CPUIDtest Registers)

add_executable(PCItest PCItest.c)
target_link_libraries(PCItest pci)