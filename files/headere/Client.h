
#ifndef OOP_CLIENT_H
#define OOP_CLIENT_H
#include "Carte.h"
#include <map>
#include <iostream>
#include <string>
#include <vector>
#include "adresa.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"

class Client {
private:
    std::string username;
    std::string email;
    Adresa adresa_livrare;
    std::string telefon;
    double sold;
    int numarcomenzi;
    double totalcumparaturi;
    std::vector<std::string> istoric_isbn;
    int pct_fidelitate;
    std::vector<std::shared_ptr<Carte>> istoric_cumparaturi;
public:
    // constrctor cu parametrii

    Client(const std::string& username, const std::string& email, const Adresa& adresa,
        const std::string& telefon);

    // destructor

    ~Client();

    //operator <<

    friend std::ostream& operator<<(std::ostream& out, const Client& obj);

    //functii
    void alimenteazaCont(double suma);

    void plateste(double suma);

    [[nodiscard]] double getSold() const;

    void adaugacumparaturi(double suma);

    double calcdiscountpersonalizat() const;

    bool esteVIP()const;

    std::string ierarhie_clienti() const;

    void adaugaComanda(double valoare, const std::vector<std::shared_ptr<Carte>>& carti_cumparate);

    double foloseste_pct_fidelitate(int pct_utilizate) ;


    };



#endif //OOP_CLIENT_H