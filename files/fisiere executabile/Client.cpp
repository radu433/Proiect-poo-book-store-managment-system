#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/ClientExceptions.h"
#include "../headere/Client.h"
#include "../headere/Manual.h"
#include "../headere/CarteStiintifica.h"
#include "../headere/Revista.h"

// constructori cu parametrii

Client::Client(const std::string &username, const std::string &email, const Adresa &adresa,
const std::string &telefon) : username(username), email(email),
    adresa_livrare(adresa), telefon(telefon),sold(0.0),numarcomenzi(0),totalcumparaturi(0),pct_fidelitate(0) {
    if (username.empty()) {
        throw ClientInvalidException("Numele utilizatorului nu poate fi gol!");
    }
    if (email.find('@') == std::string::npos) {
        throw ClientInvalidException("Email invalid: " + email);
    }
}
// destructor
Client::~Client() {
    std::cout << "Clientul a fost sters" << std::endl;
}


// operator <<
std::ostream & operator<<(std::ostream &out, const Client &obj) {
    out << "Client: " << obj.username << " (" << obj.email << ")\n"
        << "Adresa: " << obj.adresa_livrare << ", Tel: " << obj.telefon << "\n"
        << "Istoric: " << obj.numarcomenzi << " comenzi, "
        << obj.totalcumparaturi << " RON cheltuiti.";
    return out;
}
//functii
void Client::alimenteazaCont(double suma) {
    if (suma <= 0) {
        throw DateInvalideException("Suma de alimentare trebuie sa fie pozitiva!");
    }
    sold += suma;
    std::cout << "Cont alimentat cu " << suma << " RON. Sold curent: " << sold << " RON.\n";
}

void Client::plateste(double suma) {
    if (suma > sold) {
        // Aruncăm excepția specifică de sold
        throw SoldInsuficientException(sold, suma);
    }
    sold -= suma;
}


double Client::getSold() const { return sold;}

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
    else if (totalcumparaturi>1000.0)
        discount+=10;
    else if (totalcumparaturi>500.0)
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

void Client::adaugaComanda(double valoare, const std::vector<std::shared_ptr<Carte>>& carti_cumparate) {
    if (valoare<=0) {
        throw DateInvalideException("Valoarea comenzii invalida (<=0)!");

    }
    plateste(valoare);
    adaugacumparaturi(valoare);
    std::map<std::string, int> statistica_preferinte;
    for (const auto& carte: carti_cumparate) {
        istoric_cumparaturi.push_back(carte);
        if (const auto manual = std::dynamic_pointer_cast<Manual>(carte)) {
            statistica_preferinte["Manuale"]++;
            statistica_preferinte["Clasa " + std::to_string(manual->getclasa())]++;
        }
        else if (auto stiintifica = std::dynamic_pointer_cast<CarteStiintifia>(carte)) {
            statistica_preferinte["Stiinta"]++;
            statistica_preferinte["Domeniu: " + stiintifica->getDomeniu()]++;
        }
        else if (auto revista = std::dynamic_pointer_cast<Revista>(carte)) {
            statistica_preferinte["Reviste"]++;
        }
    }
    std::cout << "\n--- ANALIZA PREFERINTE ---\n";

    if (statistica_preferinte["Manuale"] > 0) {
        std::cout << "Vad ca ai cumparat Manuale. Iti recomandam si culegeri pentru aceeasi clasa!\n";
    }
    if (statistica_preferinte["Stiinta"] > 1) {
        std::cout << "Esti pasionat de Stiinta! Avem noi enciclopedii in stoc.\n";
    }
    if (statistica_preferinte["Reviste"] > 0) {
        std::cout << " Nu uita sa te abonezi la revistele tale preferate pentru a primi editia lunar!\n";
    }

    std::cout << "Comanda inregistrata cu succes. Multumim!\n";
}

double Client::foloseste_pct_fidelitate(int pct_utilizate) {
    if (pct_utilizate <= 0) {
        throw DateInvalideException("Numar puncte invalid!");
    }

    if (pct_utilizate> pct_fidelitate) {
        throw DateInvalideException(
            "Puncte insuficiente! Disponibil: " + std::to_string(pct_fidelitate) +
            ", Cerut: " + std::to_string(pct_utilizate)
        );
    }

    pct_fidelitate -= pct_utilizate;
    double reducere = pct_utilizate * 0.1;  // 1 punct = 0.1 RON

    std::cout << "S-au folosit " << pct_utilizate << " puncte fidelitate\n"
              << "Reducere aplicata: " << reducere << " RON\n"
              << "Puncte ramase: " << pct_utilizate << "\n";

    return reducere;
}







