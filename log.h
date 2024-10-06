#pragma once
#include<iostream>

#define LOG() LogLine(__FILE_NAME__, __LINE__).stream()

class LogLine {
public:
    LogLine(const char* file, int line) {
        std::cout << file << ":" << line << " ";
    }

    ~LogLine() {
        std::cout << std::endl;
    }

    std::ostream& stream() {
        return std::cout;
    }
};


