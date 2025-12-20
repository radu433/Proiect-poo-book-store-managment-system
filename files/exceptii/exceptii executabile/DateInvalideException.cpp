#include "../exceptii_headere/DateInvalideException.h"

// date invalide
DateInvalideException::DateInvalideException(const std::string &detalii)
    : LibrarieException("Date Invalide -> " + detalii) {
}