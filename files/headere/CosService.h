
#ifndef OOP_COSSERVICE_H
#define OOP_COSSERVICE_H
#include "AppState.h"
#include "UnitateVanzare.h"

class CosService {
public :
    static void stergeDinCons(const std::shared_ptr<Comanda> &comandaActiva, int idxA);

     static void adaugaPachetPredefinit(const AppState& app,std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva,
         int idxP);

    static void adaugaCarteIndividuala(std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva,
    int cantitate, const std::shared_ptr<Publicatie> &publicatie,bool esteSH,const std::string& conditie,int luni);
    static void adaugaPachetCreat(std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva,
    const std::shared_ptr<UnitateVanzare> &pachet);

    static void adaugaUnitataVanzare(std::shared_ptr<Client>& client,std::shared_ptr<Comanda>& comanda,
    const std::shared_ptr<UnitateVanzare>& unitate);

};
#endif //OOP_COSSERVICE_H