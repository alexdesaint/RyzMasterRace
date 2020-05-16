#include <array>
#include <cpuid.h>
#include <utility>
#include <vector>

#include <RegisterException.hpp>
#include <Register.hpp>
#include <Type.hpp>

class CPUIDRegister {
public:
    struct Register  : public ::Register{
        char const *posName, *nameSpace, *disposition, *position;

        uint32_t addr = 0, c = 0;
        char pos = ' ';

        std::string toString() final {
            std::string ret;
            ret += ((char)((data[0][0]) & 0xFF));
            ret += ((char)((data[0][0] >> 8) & 0xFF));
            ret += ((char)((data[0][0] >> 16) & 0xFF));
            ret += ((char)((data[0][0] >> 24) & 0xFF));
            return ret;
        }

        void read() {
            std::array<uint32_t, 4> ret{1};
            __cpuid_count(addr, c, ret[0], ret[1], ret[2], ret[3]);

            data[0][0] = ret[pos - 'A'];
        }

        Register(char const *posName, char const *name, char const *nameSpace, char const *description, char const *disposition, char const *position, std::vector<BitDescriptor> values, Type type = Type::REGISTER) :
                posName(posName), nameSpace(nameSpace), disposition(disposition), position(position), ::Register(std::move(values), 32, name, description, {0}, type)
        {
            sscanf(position, "CPUID_Fn%8X_E%cX_x%2X", &addr, &pos, &c);

            if(pos != 'A' && pos != 'B' && pos != 'C' && pos != 'D')
                throw RegisterException(std::string("invalid register position : ") + position);

            data.resize(1);
            data[0].resize(1);
            errors.resize(1);
            errors[0].resize(1);

            read();
        }
    };

    static std::vector<CPUIDRegister::Register> Registers;
};