//
// Created by sindicat on 10/28/2025.
//
#include "Client.h"
// constructori cu parametrii

Client::Client(const std::string &username, const std::string &email, const std::string &adresa,
const std::string &telefon) : username(username), email(email),
    adresa(adresa), telefon(telefon),sold(0.0),numarcomenzi(0),totalcumparaturi(0) {
}
// destructor
Client::~Client() {
    std::cout << "Clientul a fost sters" << std::endl;
}
// operator <<
std::ostream & operator<<(std::ostream &out, const Client &obj) {
    out << "Client: " << obj.username << " (" << obj.email << ")\n"
        << "Adresa: " << obj.adresa << ", Tel: " << obj.telefon << "\n"
        << "Istoric: " << obj.numarcomenzi << " comenzi, "
        << obj.totalcumparaturi << " RON cheltuiti.";
    return out;
}
//functii

void Client::adaugacumparaturi(double suma) {
    if (suma >0) {
        totalcumparaturi+=suma;
        numarcomenzi++;
        pct_fidelitate+=static_cast<int>(suma/10);
        std::cout<<"Cumparatura adaugara: "<<suma<<"RON pentru: "<<username<<std::endl;
    }
}

double Client::calcdiscountpersonalizat() const {
    double discount = 0;
    if (totalcumparaturi > 2000.0)
        discount+=20;
    if (totalcumparaturi>1000.0)
        discount+=10;
    if (totalcumparaturi>500.0)
        discount+=5;
    if (numarcomenzi>35)
        discount+=5;
    else if (numarcomenzi>15)
        discount+=2;
    discount+=pct_fidelitate/100;
    return std::min(discount,50.0);
}

bool Client::esteVIP() const {
    if (totalcumparaturi>1500.0 && numarcomenzi>25 && pct_fidelitate>50)
        return true;
    else
        return false;
}

std::string Client::ierarhie_clienti() const {
    if (esteVIP()) {
        return "Platinum";
    }
    if (totalcumparaturi >= 1000.0) {
        return "Gold";
    }
    if (totalcumparaturi >= 500.0) {
        return "Silver";
    }
    if (numarcomenzi >= 3) {
        return "Bronze";
    }
    return "Standard";
}

void Client::adaugaComanda(double valoare, const std::vector<std::string> &isbn_list) {
    if (valoare<=0) {
        std::cout<<"Valoare invalida pentru o comanda"<<std::endl;

    }
    totalcumparaturi+=valoare;
    numarcomenzi++;

    pct_fidelitate+=static_cast<int>(valoare/10);

    for (const auto& isbn : isbn_list) {
        istoric_isbn.push_back(isbn);
    }
    std::cout<<"Comandă adăugată pentru " << username
              << " - Valoare: " << valoare << " RON\n"
              << "Puncte fidelitate câștigate: " << static_cast<int>(valoare / 10)
              << " (Total: " << pct_fidelitate << ")\n";
}

double Client::foloseste_pct_fidelitate(int pct_utilizate) {
    if (pct_utilizate <= 0) {
        std::cout << "Numar invalid de puncte!\n";
        return 0.0;
    }

    if (pct_utilizate> pct_fidelitate) {
        std::cout << "Puncte insuficiente! Disponibile: "
                  <<pct_utilizate << "\n";
        return 0.0;
    }

    pct_fidelitate -= pct_utilizate;
    double reducere = pct_utilizate * 0.1;  // 1 punct = 0.1 RON

    std::cout << "S-au folosit " << pct_utilizate << " puncte fidelitate\n"
              << "Reducere aplicata: " << reducere << " RON\n"
              << "Puncte ramase: " << pct_utilizate << "\n";

    return reducere;
}







