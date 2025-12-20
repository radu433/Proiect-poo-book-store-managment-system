#ifndef OOP_DATAEXCEPTION_H
#define OOP_DATAEXCEPTION_H
#include "LibrarieException.h"

class DataException : public LibrarieException {
public:
    explicit DataException(const std::string &msg) : LibrarieException(msg) {
    };
};
#endif //OOP_DATAEXCEPTION_H