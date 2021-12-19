#ifndef ERRORS_H
#define ERRORS_H

#include <string>
#include <exception>

class BaseError: public std::exception
{
public:
    const char* what() const noexcept override{
        return "Unknown Error";
    }
};

class MemoryError: public BaseError
{
public:
    const char* what() const noexcept override{
        return "Memory Error";
    }
};

#endif // ERRORS_H
