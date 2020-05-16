#pragma once

#include <BitDescriptor.hpp>
#include <utility>

class Register {
public:
    const int size;
    const std::vector<BitDescriptor> bitDescriptors;

    char const *name, *description;

    Type type = Type::REGISTER;/// a REGISTER by default, can also be a STRING_REGISTER (a register describing a string)

    const std::vector<size_t> offsets;      /// some register are in multiple placements
    std::vector<std::vector<uint64_t>> data;/// by offsets and by CPU
    std::vector<std::vector<bool>> errors;  /// Data that could not be read (offsets, CPU)

    virtual std::string toString() {
        return std::string();
    }


    Register(std::vector<BitDescriptor> bitDescriptors, int size, char const *name, char const *description, std::vector<size_t> offsets, Type type = Type::REGISTER) : bitDescriptors(std::move(bitDescriptors)),
                                                                                                                                                                        size(size),
                                                                                                                                                                        name(name),
                                                                                                                                                                        description(description),
                                                                                                                                                                        offsets(std::move(offsets)),
                                                                                                                                                                        type(type) {
        size_t s = 0;
        for (auto v : this->bitDescriptors)
            s += v.start - v.end + 1;

        if (s != size)
            printf("Warning in \'%s\' : sum of the bits descriptor is not equal to the size if the register (%u != %zu).\n", name, size, s);
    }
};