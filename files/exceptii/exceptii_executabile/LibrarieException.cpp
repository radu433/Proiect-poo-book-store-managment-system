#include "../exceptii_headere/LibrarieException.h"

LibrarieException::LibrarieException(const std::string &mesaj): std::runtime_error("Eroare librarie:"+ mesaj),
mesaj(mesaj)
{}

const char * LibrarieException::what() const noexcept {
    return mesaj.c_str();
}
