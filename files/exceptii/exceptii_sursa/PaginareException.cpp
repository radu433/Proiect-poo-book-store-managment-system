#include "../exceptii_headere/PaginareException.h"

PaginareException::PaginareException(const std::string& msg)
    : LibrarieException("Eroare Paginare: " + msg) {}

NumarPaginiInvalidException::NumarPaginiInvalidException(int nr_pagini)
    : PaginareException("Numar pagini invalid: " + std::to_string(nr_pagini)){}

