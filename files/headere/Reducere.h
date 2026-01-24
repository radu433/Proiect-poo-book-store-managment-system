

#ifndef OOP_REDUCERE_H
#define OOP_REDUCERE_H
#include <memory>
#include <ctime>
#include "Publicatie.h"

class Reducere {
    std::shared_ptr<Publicatie> publicatie;
    int procent;
    std::time_t start;
    std::time_t sfarsit;

public:

    Reducere(std::shared_ptr<Publicatie> pub, int procent, std::time_t start, std::time_t sfarsit);

    bool esteActiva() const;
    bool seAplicaLa(const std::shared_ptr<Publicatie>& p) const;

    int getProcent() const;
    std::shared_ptr<Publicatie> getPublicatie() const;

    std::string serializare() const;
    static Reducere deserializare(const std::string& line, const std::vector<std::shared_ptr<Publicatie>>& publicatii);
};
#endif //OOP_REDUCERE_H