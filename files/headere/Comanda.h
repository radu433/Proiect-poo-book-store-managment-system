#ifndef OOP_COMANDA_H
#define OOP_COMANDA_H
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include <string>
#include <memory>
#include <ctime>

#include "ArticolComanda.h"
#include "Client.h"
#include "UnitateVanzare.h"
class Client;

class Comanda {
private:

     // date obiecte comandate
     std::vector<ArticolComanda> articole;
     Client* client;
     std::string stare_comanda;

     int id_comanda;
     static int global_id_comanda;
     time_t data_comanda;
     // healper

     std::vector<std::shared_ptr<Carte>> extrageCarti() const;

protected:

     void Curata_articole();

public:

     std::vector<std::string> extrageIdentificatori() const;

     explicit Comanda(Client& client);



     //constructor pt load
     //Comanda(int id, time_t data, std::shared_ptr<Client> client,const std::vector<ArticolComanda>& articole,const std::string& stare);

     // destructor
     ~Comanda()=default;

     // constructor de mutare
     Comanda(const Comanda&& other) noexcept=default;

     // operator =
     Comanda& operator=(Comanda&& other) =delete;

     // functie swap pt opertor <<
     friend  void swap(Comanda& unu,Comanda& doi ) noexcept;

     friend std::ostream& operator<<(std::ostream& out, const Comanda& cmd);

     // getteri
     [[nodiscard]] const std::string& getStare() const;
     [[nodiscard]] int getId() const;
     [[nodiscard]] std::string getDataFormatata() const;
     [[nodiscard]] int getNumarArticole() const;
     const std::vector<ArticolComanda>& getArticole() const;

     // functii
     void adaugaArticol(const std::shared_ptr<UnitateVanzare> &articol, int cantitate);

     void stergeArticol(int idx);

     [[nodiscard]] double calculeazaTotal() const;



     void valideazaComanda() const;

     void anuleazaComanda();

     std::shared_ptr<Client> getClient() const;
};
#endif //OOP_COMANDA_H