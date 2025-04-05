#pragma once

#include <iostream>

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

#define LOG(level, msg)            \
    do {                           \
    std::cout << msg << std::endl; \
    } while (0)
