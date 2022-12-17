#pragma once
#include "engine.h"
struct OtherException : public std::exception {
    const char* what() const throw() {
        return "Exception";
    }
};