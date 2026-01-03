#ifndef OOP_CLIENT_H
#define OOP_CLIENT_H
#include "Carte.h"
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "adresa.h"
#include "PublicatieSugestie.h"

#include "../exceptii/exceptii_headere/LibrarieException.h"
class Comanda;

class Client {
private:
    std::string username;
    std::string email;
    Adresa adresa_livrare;
    std::string telefon;
    std::string parola;
    double sold;
    int numarcomenzi;
    double totalcumparaturi;
    std::vector<std::string> istoric_identificatori;
    int pct_fidelitate;
    std::vector<std::shared_ptr<Publicatie> > istoric_cumparaturi;
    std::shared_ptr<Comanda> comandaActiva = nullptr;
    double reducereLaUrmC=0.0;

public:
    // constrctor cu parametrii

    Client(const std::string &username, const std::string &email, const Adresa &adresa,
           const std::string &telefon, const std::string &parola);

    // destructor

    ~Client();

    //operator <<

    friend std::ostream &operator<<(std::ostream &out, const Client &obj);

    //functii
    void alimenteazaCont(double suma);

    void plateste(double suma);

    [[nodiscard]] double getSold() const;

     void adaugacumparaturi(double suma);

    double calcdiscountpersonalizat() const;

    bool esteVIP() const;

    std::string ierarhie_clienti() const;

    void adaugaComanda(double valoare, const std::vector<std::shared_ptr<Carte> > &carti_cumparate);

    std::vector<PublicatieSugestie> genereazaSugestii() const;


    double foloseste_pct_fidelitate(int pct_utilizate);

    [[nodiscard]] const std::string &getUsername() const;

    [[nodiscard]] const std::string &getEmail() const;

    [[nodiscard]] int getNumarComenzi() const;

    [[nodiscard]] double getTotalCumparaturi() const;

    [[nodiscard]] int getPunctedeFideliate() const;

    [[nodiscard ]] std::string getTelefon() const;

    bool verificaParola(const std::string &parolaIntrodusa) const;

    void seteazaParola(std::string &parolac) const;

    [[nodiscard]] const std::vector<std::string>& getIstoricIdentificatori() const;

    void adaugaPCT(int pct);

    void adugaredVit(double reducere);

    bool aCumparatPub(const std::string& identificator) const ;


};


#endif //OOP_CLIENT_H