
#ifndef OOP_STOCEXCEPTION_H
#define OOP_STOCEXCEPTION_H
#include "../exceptii_headere/LibrarieException.h"
#include <string>
#include <exception>

class StocException : public LibrarieException {
public:
    explicit StocException(const std::string& msg);
};
class StocInvalidException : public StocException {
public:
    explicit StocInvalidException(int valoare_invalida);
};
class StocInsuficientException : public StocException {
private:
    int stoc_avut;
    int stoc_cerut;
public:
    StocInsuficientException(const std::string& produs, int avut, int cerut);

    // Getteri
    int getDiferenta() const;
};


#endif //OOP_STOCEXCEPTION_H