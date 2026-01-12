#include "../exceptii_headere/PaginareException.h"

PaginareException::PaginareException(const std::string& msg)
    : LibrarieException("Eroare Paginare: " + msg) {}

NumarPaginiInvalidException::NumarPaginiInvalidException(int nr_pagini)
    : PaginareException("Numar pagini invalid: " + std::to_string(nr_pagini)),
      nr_pagini_invalid(nr_pagini) {}

PaginareInconsistentaException::PaginareInconsistentaException(int declarat, int real)
    : PaginareException("Inconsistenta: declarat " + std::to_string(declarat) + 
                       " pagini, dar are " + std::to_string(real)),
      nr_pagini_declarat(declarat),
      nr_pagini_real(real) {}