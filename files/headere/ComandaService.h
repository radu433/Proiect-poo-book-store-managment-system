

#ifndef OOP_COMANDASERVICE_H
#define OOP_COMANDASERVICE_H
#include "AppState.h"
#include "Comanda.h"
#include "../exceptii/exceptii_headere/ClientExceptions.h"

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

    static double finalizeazaComanda (Comanda&& cmd,Client& client, AppState& app);
};

inline StatisticiComenzi ComandaService::calculeazaStatistici(const AppState &app) {
    StatisticiComenzi s{};
    s.total = app.getComanda().size();

    for (const auto& c : app.getComanda()) {
        if (c.getStare() == "Finalizata")
            s.finalizate++;
        else if (c.getStare() == "Anulata")
            s.anulate++;

        s.valoareTotala += c.calculeazaTotal();
    }

    if (s.total > 0)
        s.valoareMedie = s.valoareTotala / s.total;

    return s;
}

inline double ComandaService::finalizeazaComanda(Comanda &&cmd, Client &client, AppState& app) {
    cmd.valideazaComanda();

    double total = cmd.calculeazaTotal();

    const double discount = client.calcdiscountpersonalizat();
    total -= total * discount;

    if (client.getSold() < total)
        throw SoldInsuficientException(client.getSold(), total);

    for (const auto& art : cmd.getArticole()) {
        art.getUnitate()->scadeStoc(art.getCantitate());
    }

    const auto ids = cmd.extrageIdentificatori();
    client.plateste(total);
    client.finalizeazaComanda(total, ids);
    
    app.adaugaComanda(std::move(cmd));

    return total;
}
#endif //OOP_COMANDASERVICE_H
