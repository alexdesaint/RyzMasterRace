#pragma once

#include <string>
#include <Type.hpp>

class BitDescriptor {
public:
    const size_t start, end;
    const std::string description;
    std::string name;
    int64_t mask = 1;
    Type type;

    BitDescriptor(size_t start, size_t end, std::string description, Type type = Type::INT);
};