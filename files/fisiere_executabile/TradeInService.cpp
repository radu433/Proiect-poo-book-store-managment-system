#include "../headere/TradeInService.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"



bool TradeInService::esteEligibila(const UnitateVanzare& vechi) {
    const auto pub = vechi.getProdusPrincipal();
    if (!pub)
        return false;
    // trebuie sa fie second-hand
    if (!vechi.esteSecondHand())
        return false;
    //  doar carti si reviste
    if (!std::dynamic_pointer_cast<Carte>(pub) &&
        !std::dynamic_pointer_cast<Revista>(pub)) {
        return false;
        }
    // conditie fizica minima
    if (vechi.getConditieFizica() == "Uzata")
        return false;

    return true;
}

double TradeInService::calculeazaValoareTradeIn(const UnitateVanzare & unitate_veche, int luni_folosite) {
    const auto pub = unitate_veche.getProdusPrincipal();
    if (!pub)
        throw DateInvalideException("Publicatie invalida pentru trade-in!");

    const double val_init = pub->getPretFinal();
    double factor = 1.0;

    if (std::dynamic_pointer_cast<Carte>(pub)) {
        factor = 0.40; // baza trade-in carte
    } else if (std::dynamic_pointer_cast<Revista>(pub)) {
        factor = 0.15; // baza trade-in revista
    } else {
        return 0.0;
    }

    if (unitate_veche.esteSecondHand()) {
        if (const std::string cond = unitate_veche.getConditieFizica(); cond == "Noua") factor *= 1.00;
        else if (cond == "Buna") factor *= 0.80;
        else if (cond == "Acceptabila") factor *= 0.60;
        else factor *= 0.40;
    }

    if (luni_folosite > 0) {
        const double depreciere_timp = std::max(0.0, 1.0 - (luni_folosite * 0.01));
        factor *= depreciere_timp;
    }

    if (auto carte = std::dynamic_pointer_cast<Carte>(pub)) {

        if (carte->calculeaza_valoarea_academica() >= 80.0)
            factor *= 1.15;

        if (carte->getRatingMediu() >= 4.0)
            factor *= 1.10;

        if (auto manual = std::dynamic_pointer_cast<Manual>(carte)) {
            if (manual->StareAprobare() == "Aprobat")
                factor *= 1.20;
        }
    }

    double valoare = val_init * factor;
    valoare = std::max(valoare, val_init * 0.10);
    valoare = std::min(valoare, val_init * 0.90);

    return valoare;

}

double TradeInService::proceseazaTradeIn(Client &client, const UnitateVanzare &vechi,AppState &app, const std::string &optiune) {
    if (!esteEligibila(vechi))
        throw DateInvalideException("Produs neeligibil pentru trade-in!");

    const int luni = vechi.calculeazaLuniDetinere();
    const double valoare = calculeazaValoareTradeIn(vechi, luni);

    if (valoare <= 0)
        throw DateInvalideException("Valoare trade-in invalida!");

    if (optiune == "BANI") {
        client.alimenteazaCont(valoare);
    } else if (optiune == "REDUCERE") {
        client.adugaredVit(3.0);
    } else if (optiune == "PUNCTE") {
        client.adaugaPCT(static_cast<int>(valoare * 2));
    } else {
        throw DateInvalideException("Optiune trade-in invalida!");
    }

    app.stocSH.push_back(vechi.clone());
    return valoare;
}
