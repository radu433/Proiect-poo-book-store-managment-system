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
      const Client* client;
     std::string stare_comanda;

     int id_comanda;
     static int global_id_comanda;
     time_t data_comanda;
     // healper

 

protected:

     void Curata_articole();

public:

     std::vector<std::string> extrageIdentificatori() const;

     explicit Comanda(const Client& client);
     
     // Constructor pt LOAD
     Comanda(int id, time_t data, const Client* client, const std::string& stare, const std::vector<ArticolComanda>& articole);

     // destructor
     ~Comanda()=default;

     // constructor de mutare
     Comanda(Comanda&& other) noexcept=default;

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
     std::string getUser() const;

     // functii
     void adaugaArticol(const std::shared_ptr<UnitateVanzare> &articol, int cantitate);

     void stergeArticol(int idx);

     [[nodiscard]] double calculeazaTotal() const;



     void valideazaComanda() const;

     void anuleazaComanda();

     std::shared_ptr<Client> getClient() const;

     std::string serializare() const;
     static Comanda deserializare(const std::string& line, const std::vector<Client>& clienti, const std::vector<std::shared_ptr<UnitateVanzare>>& stoc);
};
#endif //OOP_COMANDA_H