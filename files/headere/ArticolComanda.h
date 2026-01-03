
#ifndef OOP_ARTICOLCOMANDA_H
#define OOP_ARTICOLCOMANDA_H
#include <memory>

#include "UnitateVanzare.h"

class ArticolComanda {
private:
    std::shared_ptr<UnitateVanzare> unitate;
    int cantitate;
public:
    ArticolComanda(std::shared_ptr<UnitateVanzare> u, int c)
        : unitate(u), cantitate(c) {}

    std::shared_ptr<UnitateVanzare> getUnitate() const {
        return unitate;
    }

    int getCantitate() const {
        return cantitate;
    }

    double getSubtotal() const {
        return unitate->getPretcomanda() * cantitate;
    }
};
#endif //OOP_ARTICOLCOMANDA_H