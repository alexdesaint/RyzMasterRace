#include <CPUIDRegister.hpp>
#include <iostream>

int main() {
    std::cout << CPUIDRegister::Registers[1].toString() << CPUIDRegister::Registers[3].toString() << CPUIDRegister::Registers[2].toString() << std::endl;

    for(auto &r : CPUIDRegister::Registers) {
        if(r.type == Type::STRING) {
            printf("%-106s %-4s\n", r.name, r.toString().c_str());
        } else {
            printf("%-100s 0x%08X\n", r.name, r.data);
            for(auto &bd : r.values) {
                auto v = (r.data >> bd.end) & bd.mask;
                switch (bd.type) {
                    case Type::BOOL:
                        printf("     %-96s (%lu) %s\n", bd.name.c_str(), v, v ? " True" : "False");
                        break;
                    case Type::INT:
                        printf("     %-90s %15lu\n", bd.name.c_str(), v);
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return 0;
}