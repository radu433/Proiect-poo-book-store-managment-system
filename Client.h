//
// Created by sindicat on 10/28/2025.
//

#ifndef OOP_CLIENT_H
#define OOP_CLIENT_H
#include <iostream>
#include <string>
#include <vector>

class Client {
private:
    std::string username;
    std::string email;
    std::string adresa;
    std::string telefon;
    double sold;
    int numarcomenzi;
    double totalcumparaturi;
    std::vector<std::string> istoric_isbn;
    int pct_fidelitate;
public:
    // constrctor cu parametrii

    Client(const std::string& username, const std::string& email, const std::string& adresa,
        const std::string& telefon);

    // destructor

    ~Client();

    //operator <<

    friend std::ostream& operator<<(std::ostream& out, const Client& obj);

    //functii

    [[nodiscard]] double getSold() const;

    void adaugacumparaturi(double suma);

    double calcdiscountpersonalizat() const;

    bool esteVIP()const;

    std::string ierarhie_clienti() const;

    void adaugaComanda(double valoare, const std::vector<std::string>& isbn_list);

    double foloseste_pct_fidelitate(int pct_utilizate) ;


    };



#endif //OOP_CLIENT_H