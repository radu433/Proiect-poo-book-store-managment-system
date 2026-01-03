#include "../headere/CosService.h"
#include "../headere/PachetSerie.h"
#include"../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/ComandaExceptions.h"
#include "../headere/CarteIndividuala.h"
#include "../headere/Comanda.h"
#include "../headere/RevistaIndividuala.h"


void CosService::stergeDinCons(const std::shared_ptr<Comanda> &comandaActiva, const int idxA) {
    if (!comandaActiva)
        throw ComandaGoalaException();
    comandaActiva->stergeArticol(idxA);
}

void CosService::adaugaPachetPredefinit(const AppState &app, std::shared_ptr<Client> &clientCurent,
    std::shared_ptr<Comanda> &comandaActiva, const int idxP) {
    if (idxP < 0 || idxP >= static_cast<int>(app.pachetePredefinite.size()))
        throw DateInvalideException("Index pachet invalid");

    const auto unitate = app.pachetePredefinite[idxP]->clone();

    if (!comandaActiva)
        comandaActiva = std::make_shared<Comanda>(clientCurent);

    comandaActiva->adaugaArticol(unitate, 1);
}



void CosService::adaugaCarteIndividuala( std::shared_ptr<Client> &clientCurent,
                                        std::shared_ptr<Comanda> &comandaActiva,  int cantitate, const std::shared_ptr<Publicatie> &publicatie) {
    if (!publicatie)
        throw DateInvalideException("Publicatia este inexistenta!");
    if (cantitate<=0)
        throw DateInvalideException("Cantitate invalida!");
    std::shared_ptr<UnitateVanzare> unitate;

    if (auto carte = std::dynamic_pointer_cast<Carte>(publicatie)) {
        unitate = std::make_shared<CarteIndividuala>(carte, false, "Noua");
    }
    else if (auto revista = std::dynamic_pointer_cast<Revista>(publicatie)) {
        unitate = std::make_shared<RevistaIndividuala>(revista);
    }
    else {
        throw DateInvalideException("Publicatia nu poate fi adaugata individual!");
    }
    if (!comandaActiva)
        comandaActiva=std::make_shared<Comanda>(clientCurent);
    comandaActiva->adaugaArticol(unitate,cantitate);
}

void CosService::adaugaPachetCreat(std::shared_ptr<Client> &clientCurent, std::shared_ptr<Comanda> &comandaActiva,
    std::shared_ptr<UnitateVanzare> pachet) {
    if (!pachet)
        throw DateInvalideException("Pachet invalid!");

    if (!comandaActiva)
        comandaActiva = std::make_shared<Comanda>(clientCurent);

    comandaActiva->adaugaArticol(pachet, 1);
}

void CosService::adaugaUnitataVanzare(std::shared_ptr<Client> &client, std::shared_ptr<Comanda> &comanda,
    const std::shared_ptr<UnitateVanzare> &unitate) {
    if (!client)
        throw DateInvalideException("Client invalid!");

    if (!comanda)
        comanda = std::make_shared<Comanda>(client);

    comanda->adaugaArticol(unitate, 1);
}


