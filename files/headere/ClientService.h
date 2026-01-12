

#ifndef OOP_CLIENTSERVICE_H
#define OOP_CLIENTSERVICE_H
#include "AppState.h"
#include "Client.h"
#include "Publicatie.h"

enum class TipPublicatie;

struct ClientStats {
    int totalClienti=0;
    int activi=0;
    int inactivi=0;
    int vip=0;
    int totalComenzi=0;
    double totalVanzari=0.0;

    const Client* maxComenzi=nullptr;
    const Client* maxCheltuieli=nullptr;
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

    static std::vector<PublicatieSugestie> genereazaSugestii(const Client& client, const AppState& app);
};

inline ClientStats ClientService::calculeazaStatistici(const AppState &app) {
    ClientStats stats{};

    stats.totalClienti = static_cast<int>(app.getClient().size());

    for (const auto& c : app.getClient()) {
        const int nrC = c.getNumarComenzi();
        const double totalV = c.getTotalCumparaturi();

        stats.totalComenzi += nrC;
        stats.totalVanzari += totalV;

        if (nrC > 0)
            stats.activi++;

        if (c.esteVIP())
            stats.vip++;

        if (!stats.maxComenzi || nrC > stats.maxComenzi->getNumarComenzi()) {
            stats.maxComenzi = &c;
        }

        if (!stats.maxCheltuieli || totalV > stats.maxCheltuieli->getTotalCumparaturi()) {
            stats.maxCheltuieli = &c;
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

inline std::vector<PublicatieSugestie> ClientService::genereazaSugestii(const Client& client, const AppState& app) {
    std::map<TipPublicatie, int> statistica;

    for (const auto& id: client.getIstoricIdentificatori()) {
        auto pub=app.gasestePublicatie(id);
        if (!pub)
            continue;
        statistica[pub->getTipPub()]++;

    }
    auto count = [&](TipPublicatie tip) {
        auto it = statistica.find(tip);
        return (it != statistica.end()) ? it->second : 0;
    };
    std::vector<PublicatieSugestie> sugestii;
    if (count(TipPublicatie::Manual) > 0) {
        sugestii.emplace_back("Manuale","Ai cumparat manuale – iti recomandam auxiliare.","Manual");
    }
    if (count(TipPublicatie::Cartestiintifica) > 1) {
        sugestii.emplace_back("Stiinta","Pare ca iti place stiinta – vezi enciclopediile noastre.","Stiinta");
    }

    if (count(TipPublicatie::Revista) > 0) {
        sugestii.emplace_back("Reviste","Te poti abona la revistele tale preferate.","Revista");
    }

    return sugestii;


}
#endif //OOP_CLIENTSERVICE_H
