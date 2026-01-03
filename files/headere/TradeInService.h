

#ifndef OOP_TRADEINSERVICE_H
#define OOP_TRADEINSERVICE_H
#include <string>
#include "../headere/UnitateVanzare.h"
#include "../headere/CarteIndividuala.h"
#include "../headere/AppState.h"
#include "../headere/Carte.h"
#include "../headere/Revista.h"
#include "../headere/Manual.h"
#include "../headere/Client.h"
#include <memory>


class TradeInService {
    public:
    static bool esteEligibila(const UnitateVanzare& vechi);

    static double calculeazaValoareTradeIn(const UnitateVanzare& carte_sh, int luni_folosite);

    static double proceseazaTradeIn(Client& client, const UnitateVanzare &carte_sh,
                                    AppState &app, const std::string &optiune);
};
#endif //OOP_TRADEINSERVICE_H