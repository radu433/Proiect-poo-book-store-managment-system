
#ifndef OOP_COSSERVICE_H
#define OOP_COSSERVICE_H
#include "AppState.h"
#include "UnitateVanzare.h"
#include "Client.h"

class CosService {
public :
    static void stergeDinCons(Comanda* &comandaActiva, int idxA);

     static void adaugaPachetPredefinit(const AppState& app, Client* clientCurent,Comanda*& comandaActiva,
         int idxP);

    static void adaugaCarteIndividuala( Client* clientCurent,Comanda*& comandaActiva,
    int cantitate, const std::shared_ptr<Publicatie> &publicatie,bool esteSH,const std::string& conditie,int luni);
    static void adaugaPachetCreat( Client* clientCurent,Comanda*& comandaActiva,
    const std::shared_ptr<UnitateVanzare> &pachet);

    static void adaugaUnitataVanzare( AppState& app,Client* client,Comanda*& comanda,
    const std::shared_ptr<UnitateVanzare>& unitate);

};
#endif //OOP_COSSERVICE_H