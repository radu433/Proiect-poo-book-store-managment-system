#ifndef OOP_PAGINAREEXCEPTION_H
#define OOP_PAGINAREEXCEPTION_H

#include "LibrarieException.h"

class PaginareException : public LibrarieException {
public:
    explicit PaginareException(const std::string& msg);
};

class NumarPaginiInvalidException : public PaginareException {
public:
    explicit NumarPaginiInvalidException(int nr_pagini);
};



#endif