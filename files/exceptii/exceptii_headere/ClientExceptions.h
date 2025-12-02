

#ifndef OOP_CLIENTEXCEPTIONS_H
#define OOP_CLIENTEXCEPTIONS_H
#include "LibrarieException.h"
#include <string>
class ClientException : public LibrarieException {
    public:
    explicit ClientException(const std::string& msg);
};

class ClientInvalidException : public ClientException {
    public:
    explicit ClientInvalidException(const std::string& detalii);
};
class SoldInsuficientException : public ClientException {
    public:
    SoldInsuficientException(double sold_curent, double total_necesar);
};
#endif //OOP_CLIENTEXCEPTIONS_H