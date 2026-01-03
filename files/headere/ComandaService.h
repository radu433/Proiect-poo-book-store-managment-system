

#ifndef OOP_COMANDASERVICE_H
#define OOP_COMANDASERVICE_H
#include "AppState.h"
#include "Comanda.h"

struct StatisticiComenzi {
    int total;
    int finalizate;
    int anulate;
    double valoareTotala;
    double valoareMedie;
};
class ComandaService {
public:
    static StatisticiComenzi calculeazaStatistici(const AppState& app);
};

inline StatisticiComenzi ComandaService::calculeazaStatistici(const AppState &app) {
    StatisticiComenzi s{};
    s.total = app.comenzi.size();

    for (const auto& c : app.comenzi) {
        if (c->getStare() == "Finalizata")
            s.finalizate++;
        else if (c->getStare() == "Anulata")
            s.anulate++;

        s.valoareTotala += c->calculeazaTotal();
    }

    if (s.total > 0)
        s.valoareMedie = s.valoareTotala / s.total;

    return s;
}
#endif //OOP_COMANDASERVICE_H
