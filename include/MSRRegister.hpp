#pragma once

#include <array>
#include <cstdio>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

class MSRException : public std::exception {
    std::string msg;

public:
    explicit MSRException(std::string msg) : msg(std::move(msg)) {}

    const char *what() const noexcept override {
        return msg.c_str();
    }
};

class MSRRegister {
private:
    class Value {
    public:
        const size_t start, end;
        const std::string description;
        int64_t mask = 1;

        Value(size_t start, size_t end, std::string description);

        std::string getName() const;
    };

    class Register {
    public:
        const std::string MSRname, name, comments, lthree;
        const std::vector<size_t> offsets;
        const std::vector<Value> values;

        Register(std::string MSRname, std::string name, std::string comments, std::string lthree,
                 std::vector<size_t> offsets,
                 std::vector<Value> values) : MSRname(std::move(MSRname)), name(std::move(name)), comments(std::move(comments)), lthree(std::move(lthree)), offsets(std::move(offsets)), values(std::move(values)) {

            size_t s = 0;
            for (auto v : this->values) {
                s += v.start - v.end + 1;
            }

            if (s != 64)
                printf("Warning : \'%s\' values not equal to 64 (s = %zu).\n", this->name.c_str(), s);
        }
    };

    static const std::vector<Register> Registers;

    unsigned int numOfCPU = 0;
    std::vector<int> filesStreams;

    void open();

    uint64_t read(int cpu, unsigned int addr);

public:
    void readSMR();

    friend std::ostream &operator<<(std::ostream &os, MSRRegister const &m);
};