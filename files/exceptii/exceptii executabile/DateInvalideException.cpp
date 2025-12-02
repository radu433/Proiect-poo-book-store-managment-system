#include "../exceptii_headere/DateInvalideException.h" // Ajustează calea

// date invalide
DateInvalideException::DateInvalideException(const std::string& detalii)
    : LibrarieException("Date Invalide -> " + detalii){}