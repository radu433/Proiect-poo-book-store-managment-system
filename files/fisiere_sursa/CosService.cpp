#include "../headere/CosService.h"
#include "../headere/PachetSerie.h"
#include"../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/ComandaExceptions.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../headere/CarteIndividuala.h"
#include "../headere/Comanda.h"
#include "../headere/RevistaIndividuala.h"


void CosService::stergeDinCos(const std::shared_ptr<Comanda>& comandaActiva, const int idxA) {
    if (!comandaActiva)
        throw ComandaGoalaException();
    comandaActiva->stergeArticol(idxA);
}

void CosService::adaugaPachetPredefinit(const AppState &app, const std::shared_ptr<Client>& clientCurent,
    std::shared_ptr<Comanda> &comandaActiva, const int idxP) {
    if (!clientCurent) throw DateInvalideException("Client invalid!");
    if (idxP < 0 || idxP >= static_cast<int>(app.getPachetePredefinite().size()))
        throw DateInvalideException("Index pachet invalid");

    const auto unitate = app.getPachetePredefinite()[idxP].clone();

    if (!comandaActiva) {
        comandaActiva = std::make_shared<Comanda>(*clientCurent);
    }
    if (comandaActiva) {
        comandaActiva->adaugaArticol(unitate, 1);
    }
}



void CosService::adaugaCarteIndividuala( const std::shared_ptr<Client>& clientCurent,
                                        std::shared_ptr<Comanda>& comandaActiva,  int cantitate,
                                        const std::shared_ptr<Publicatie> &publicatie,bool esteSH,const std::string& conditie,int luni) {
    if (!publicatie)
        throw DateInvalideException("Publicatia este inexistenta!");
    if (cantitate <= 0)
        throw DateInvalideException("Cantitate invalida!");

    std::shared_ptr<UnitateVanzare> unitate;

    if (auto carte = std::dynamic_pointer_cast<Carte>(publicatie)) {
         unitate =std::make_shared<CarteIndividuala>(carte);
        if (esteSH && unitate) {
            unitate->marcheazaSecondHand(conditie, luni);
        }

    } else if (auto revista = std::dynamic_pointer_cast<Revista>(publicatie)) {
         unitate = std::make_shared<RevistaIndividuala>(revista);

        if (esteSH && unitate) {
            unitate->marcheazaSecondHand(conditie, luni);
        }

    } else {
        throw DateInvalideException("Publicatia nu poate fi adaugata individual!");
    }

    if (!comandaActiva)
        comandaActiva =std::make_shared<Comanda>(*clientCurent);

    comandaActiva->adaugaArticol(unitate, cantitate);
}

void CosService::adaugaPachetCreat(const std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva,
    const std::shared_ptr<UnitateVanzare> &pachet) {
    if (!pachet)
        throw DateInvalideException("Pachet invalid!");

    if (!comandaActiva)
        comandaActiva =std::make_shared<Comanda>(*clientCurent);

    comandaActiva->adaugaArticol(pachet, 1);
}

void CosService::adaugaUnitateVanzare(AppState& app, const std::shared_ptr<Client>& client,std::shared_ptr<Comanda>& comanda,
    const std::shared_ptr<UnitateVanzare>& unitate) {
    if (!client) throw DateInvalideException("Client invalid!");
    if (!unitate || !unitate->valideazaDisponibilitate()) {
        throw StocException("Produs indisponibil pentru vanzare!");
    }
    if (!comanda) {
        comanda = std::make_shared<Comanda>(*client);
    }
    comanda->adaugaArticol(unitate, 1);
}


