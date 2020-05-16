#include <BitDescriptor.hpp>
#include <cmath>


BitDescriptor::BitDescriptor(size_t start, size_t end, std::string description, Type type) : start(start), end(end), description(std::move(description)), type(type) {
    if(start == 63 && end == 0)
        mask = 0xFFFFFFFFFFFFFFFF;
    else if(start != end) {
        mask = std::pow(2, (start - end) + 1) - 1;
    }

    auto pos = this->description.find_first_of("\n.:[");
    name = this->description.substr(0, pos);
    if(name.find("Reserved") != std::string::npos)
        this->type = Type::RESERVED;
    else if(start == end)
        this->type = Type::BOOL;
}