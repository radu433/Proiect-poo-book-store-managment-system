

#ifndef OOP_AUTOREXCEPTION_H
#define OOP_AUTOREXCEPTION_H
#include "LibrarieException.h"
class AutorException : public LibrarieException {
    public:
    explicit AutorException(const std::string& msg);
};
class AutorInvalidException : public AutorException {
    public:
    explicit AutorInvalidException(const std::string& msg);
};

#endif //OOP_AUTOREXCEPTION_H