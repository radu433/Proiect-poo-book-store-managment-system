#ifndef OOP_PAGINAREEXCEPTION_H
#define OOP_PAGINAREEXCEPTION_H

#include "LibrarieException.h"

class PaginareException : public LibrarieException {
public:
    explicit PaginareException(const std::string& msg);
};

class NumarPaginiInvalidException : public PaginareException {
private:
    int nr_pagini_invalid;
public:
    explicit NumarPaginiInvalidException(int nr_pagini);
    [[nodiscard]] int getNrPagini() const { return nr_pagini_invalid; }
};

class PaginareInconsistentaException : public PaginareException {
private:
    int nr_pagini_declarat;
    int nr_pagini_real;
public:
    PaginareInconsistentaException(int declarat, int real);
};

#endif