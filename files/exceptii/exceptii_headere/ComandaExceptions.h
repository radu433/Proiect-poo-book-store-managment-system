//
// Created by sindicat on 11/25/2025.
//

#ifndef OOP_COMANDAEXCEPTIONS_H
#define OOP_COMANDAEXCEPTIONS_H
#include "LibrarieException.h"
#include <string>
class ComandaException : public LibrarieException {
    public:
    explicit ComandaException(const std::string & msg);
};
class ComandaInvalidaException : public ComandaException {
public:
   explicit  ComandaInvalidaException(const std::string& actiune);
};
class ComandaFinalizataException : public ComandaException {
    public:
    explicit ComandaFinalizataException(int id_comanda);

};
class ComandaAnulataException : public ComandaException {
    public:
    explicit ComandaAnulataException(int id_comanda);
};
class ComandaGoalaFinalizareException : public ComandaException {
  public:
    explicit ComandaGoalaFinalizareException();
};
class ComandaGoalaException : public ComandaException {
    public:
    explicit ComandaGoalaException();
};
#endif //OOP_COMANDAEXCEPTIONS_H