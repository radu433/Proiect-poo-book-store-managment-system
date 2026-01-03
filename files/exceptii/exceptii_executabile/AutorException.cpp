#include "../exceptii_headere/AutorException.h"

AutorException::AutorException(const std::string &msg) : LibrarieException("Eroare autor: "+ msg) {
}

AutorInvalidException::AutorInvalidException(const std::string &msg) : AutorException("Date incorecte: " + msg){
}

