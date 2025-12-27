

#ifndef OOP_CLIENTSERVICE_H
#define OOP_CLIENTSERVICE_H
#include "AppState.h"
#include "Client.h"



struct ClientStats {
    int totalClienti;
    int activi;
    int inactivi;
    int vip;
    int totalComenzi;
    double totalVanzari;

    std::shared_ptr<Client> maxComenzi;
    std::shared_ptr<Client> maxCheltuieli;
};
struct ClientDetalii {
    std::string username;
    std::string email;
    double sold;
    int nrComenzi;
    double totalCumparaturi;
    int puncteFidelitate;
    std::string ierarhie;
};
class ClientService {
public:
    static ClientStats calculeazaStatistici(const AppState& app);

    static ClientDetalii getDetaliiClient(const Client& c);
};

inline ClientStats ClientService::calculeazaStatistici(const AppState &app) {
    ClientStats stats{};

    stats.totalClienti = static_cast<int>(app.clienti.size());
    stats.activi = 0;
    stats.inactivi = 0;
    stats.vip = 0;
    stats.totalComenzi = 0;
    stats.totalVanzari = 0.0;

    stats.maxComenzi = nullptr;
    stats.maxCheltuieli = nullptr;

    for (const auto& c : app.clienti) {
        const int nrC = c->getNumarComenzi();
        const double totalV = c->getTotalCumparaturi();

        stats.totalComenzi += nrC;
        stats.totalVanzari += totalV;

        if (nrC > 0)
            stats.activi++;

        if (c->esteVIP())
            stats.vip++;

        if (!stats.maxComenzi || nrC > stats.maxComenzi->getNumarComenzi()) {
            stats.maxComenzi = c;
        }

        if (!stats.maxCheltuieli || totalV > stats.maxCheltuieli->getTotalCumparaturi()) {
            stats.maxCheltuieli = c;
        }
    }

    stats.inactivi = stats.totalClienti - stats.activi;

    return stats;
}

inline ClientDetalii ClientService::getDetaliiClient(const Client &c) {
    ClientDetalii det;

    det.username = c.getUsername();
    det.email = c.getEmail();
    det.sold = c.getSold();
    det.nrComenzi = c.getNumarComenzi();
    det.totalCumparaturi = c.getTotalCumparaturi();
    det.puncteFidelitate = c.getPunctedeFideliate();
    det.ierarhie = c.ierarhie_clienti();

    return det;
}
#endif //OOP_CLIENTSERVICE_H
