#include "../headere/CosService.h"
#include "../headere/PachetSerie.h"
#include"../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/ComandaExceptions.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../headere/CarteIndividuala.h"
#include "../headere/Comanda.h"
#include "../headere/RevistaIndividuala.h"


void CosService::stergeDinCons( Comanda*& comandaActiva, const int idxA) {
    if (!comandaActiva)
        throw ComandaGoalaException();
    comandaActiva->stergeArticol(idxA);
}

void CosService::adaugaPachetPredefinit(const AppState &app, Client* clientCurent,
    Comanda* &comandaActiva, const int idxP) {
    if (idxP < 0 || idxP >= static_cast<int>(app.getPachetePredefinite().size()))
        throw DateInvalideException("Index pachet invalid");

    const auto unitate = app.getPachetePredefinite()[idxP].clone();

    if (!comandaActiva) {


    }
    comandaActiva->adaugaArticol(unitate, 1);
}



void CosService::adaugaCarteIndividuala( Client* clientCurent,
                                        Comanda* &comandaActiva,  int cantitate,
                                        const std::shared_ptr<Publicatie> &publicatie,bool esteSH,const std::string& conditie,int luni) {
    if (!publicatie)
        throw DateInvalideException("Publicatia este inexistenta!");
    if (cantitate <= 0)
        throw DateInvalideException("Cantitate invalida!");

    std::shared_ptr<UnitateVanzare> unitate;

    if (auto carte = std::dynamic_pointer_cast<Carte>(publicatie)) {
        unitate = std::make_shared<CarteIndividuala>(carte);

        if (esteSH) {
            unitate->marcheazaSecondHand(conditie, luni);
        }

    } else if (auto revista = std::dynamic_pointer_cast<Revista>(publicatie)) {
        unitate = std::make_shared<RevistaIndividuala>(revista);

        if (esteSH) {
            unitate->marcheazaSecondHand(conditie, luni);
        }

    } else {
        throw DateInvalideException("Publicatia nu poate fi adaugata individual!");
    }

    if (!comandaActiva)
        comandaActiva = std::make_shared<Comanda>(clientCurent);

    comandaActiva->adaugaArticol(unitate, cantitate);
}

void CosService::adaugaPachetCreat(Client* clientCurent, Comanda* &comandaActiva,
    const std::shared_ptr<UnitateVanzare> &pachet) {
    if (!pachet)
        throw DateInvalideException("Pachet invalid!");

    if (!comandaActiva)
        comandaActiva = std::make_shared<Comanda>(clientCurent);

    comandaActiva->adaugaArticol(pachet, 1);
}

void CosService::adaugaUnitataVanzare(AppState& app,Client* client,Comanda*& comanda,
    const std::shared_ptr<UnitateVanzare>& unitate) {
    if (!client)
        throw DateInvalideException("Client invalid!");

    if (!unitate->valideazaDisponibilitate()) {
        throw StocException("Produs indisponibil pentru vanzare!");
    }
    if (!comanda) {
        app.adaugaComanda(Comanda(*client));
        comanda = &app.getComanda().back();
    }


    comanda->adaugaArticol(unitate, 1);
}


