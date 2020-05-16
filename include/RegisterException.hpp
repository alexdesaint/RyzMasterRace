#pragma once
#include <exception>

class RegisterException : public std::exception {
    std::string msg;

public:
    explicit RegisterException(std::string msg) : msg(std::move(msg)) {}

    const char *what() const noexcept override {
        return msg.c_str();
    }
};