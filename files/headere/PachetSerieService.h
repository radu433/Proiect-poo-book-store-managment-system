

#ifndef OOP_PACHETSERIESERVICE_H
#define OOP_PACHETSERIESERVICE_H
#include "PachetSerie.h"

class PachetSerieService {
    public:
    static void adaugaUnitate(PachetSerie &pachet, const std::shared_ptr<UnitateVanzare> &unitate) {
        if (!unitate)
            throw DateInvalideException("Unitate invalida");

        auto noi = unitate->getListaIdentificatori();
        auto existente = pachet.getListaIdentificatori();
        for (const auto& id : noi) {
            if (std::ranges::find(existente, id) != existente.end())
                throw DateInvalideException("Produs deja in pachet");
        }

        pachet.adaugaIntern(unitate);
    }
};
#endif //OOP_PACHETSERIESERVICE_H