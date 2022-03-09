#ifndef TREESTRUCTURE_NOSUCHELEMENTEXCEPTION_H
#define TREESTRUCTURE_NOSUCHELEMENTEXCEPTION_H

#include <exception>

struct NoSuchElementException : public std::exception {
    const char* what () const noexcept override {
        return "No Such Element";
    }
};

#endif //TREESTRUCTURE_NOSUCHELEMENTEXCEPTION_H
