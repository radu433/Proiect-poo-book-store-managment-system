

#ifndef OOP_COMANDA_H
#define OOP_COMANDA_H
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "Client.h"
#include <string>
#include <memory>
#include <ctime>

#include "UnitateVanzare.h"

class Comanda {
private:
     // date client
     std::shared_ptr <Client> client;

     // date obiecte comandate(carte individuala pachete de carti  reviste)
     std::vector<std::shared_ptr<UnitateVanzare>> articole;

     std::string stare_comanda;

     int id_comanda;
     static int global_id_comanda;
     time_t data_comanda;
// healper

     std::vector<std::shared_ptr<Carte>> extrageCarti() const;

protected:

     void Curata_articole();

public:

     std::vector<std::string> extrageISBN() const;

     // constructor cu parametruu
     explicit Comanda(std::shared_ptr<Client> client);

     // destructor
     ~Comanda();

     // constructor de copiere
     Comanda(const Comanda& other);

     // operator =
     Comanda& operator=(Comanda other);

     // functie swap pt opertor <<
     friend  void swap(Comanda& unu,Comanda& doi ) noexcept;

     friend std::ostream& operator<<(std::ostream& out, const Comanda& cmd);

     // getteri
     [[nodiscard]] const std::string& getStare() const;
     [[nodiscard]] int getId() const;
     [[nodiscard]] std::string getDataFormatata() const;
     [[nodiscard]] int getNumarArticole() const;

     // functii
     void adaugaArticol(const std::shared_ptr<UnitateVanzare> &articol, int cantitate);

     [[nodiscard]] double calculeazaTotal() const;

     double finalizareComanda();

     void valideazaComanda() const;

     void anuleazaComanda();





};
#endif //OOP_COMANDA_H