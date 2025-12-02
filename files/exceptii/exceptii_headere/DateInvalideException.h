#ifndef OOP_DATEINVALIDEXCEPTION_H
#define OOP_DATEINVALIDEXCEPTION_H

#include "LibrarieException.h"
#include <string>

class DateInvalideException : public LibrarieException {
public:
    explicit DateInvalideException(const std::string& detalii);
};

#endif //OOP_DATEINVALIDEXCEPTION_H