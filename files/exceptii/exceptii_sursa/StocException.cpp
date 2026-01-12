#include "../exceptii_headere/StocException.h"

// implementare constructor baza
StocException::StocException(const std::string &mesaj)
    : LibrarieException("Problema Stoc: " + mesaj)
{
}
// stoc invalid
StocInvalidException::StocInvalidException(int valoare_invalida)
    : StocException("Valoare negativa detectata (" + std::to_string(valoare_invalida) + ")! Stocul nu poate fi negativ.")
{
}
// stoc insuficient
StocInsuficientException::StocInsuficientException(const std::string &produs, int avut, int cerut)
    : StocException("Insuficient pentru '" + produs +
                    "'. Avut: " + std::to_string(avut) +
                    ", Cerut: " + std::to_string(cerut)),
      stoc_avut(avut),
      stoc_cerut(cerut)
{
}

int StocInsuficientException::getDiferenta() const {
    return stoc_cerut - stoc_avut;
}