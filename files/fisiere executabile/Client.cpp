#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/ClientExceptions.h"
#include "../headere/Client.h"
#include "../headere/Manual.h"
#include "../headere/CarteStiintifica.h"
#include "../headere/Revista.h"
#include <algorithm>

// constructori cu parametrii

Client::Client(const std::string &username, const std::string &email, const Adresa &adresa,
               const std::string &telefon, const std::string &parola) : username(username), email(email),
                                                                        adresa_livrare(adresa), telefon(telefon),
                                                                        parola(parola), sold(0.0), numarcomenzi(0),
                                                                        totalcumparaturi(0), pct_fidelitate(0) {
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
std::ostream &operator<<(std::ostream &out, const Client &obj) {
    out << "Client: " << obj.username << " (" << obj.email << ")\n"
            << "Sold: " << obj.sold << " RON\n"
            << "Adresa: " << obj.adresa_livrare << ", Tel: " << obj.telefon << "\n"
            << "Status: ";
    if (obj.totalcumparaturi >= 1000.0)
        out << "Gold";
    else if (obj.totalcumparaturi >= 500.0)
        out << "Silver";
    else if (obj.numarcomenzi >= 3)
        out << "Bronze";
    else
        out << "Standard";

    out << " (Pct: " << obj.pct_fidelitate << ")\n"
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


double Client::getSold() const { return sold; }

void Client::adaugacumparaturi(double suma) {
    if (suma > 0) {
        totalcumparaturi += suma;
        numarcomenzi++;
        pct_fidelitate += static_cast<int>(suma / 10);
        std::cout << "Cumparatura adaugara: " << suma << "RON pentru: " << username << std::endl;
    }
}

double Client::calcdiscountpersonalizat() const {
    double discount = 0;
    if (totalcumparaturi > 2000.0)
        discount += 20;
    else if (totalcumparaturi > 1000.0)
        discount += 10;
    else if (totalcumparaturi > 500.0)
        discount += 5;
    if (numarcomenzi > 35)
        discount += 5;
    else if (numarcomenzi > 15)
        discount += 2;
    discount += pct_fidelitate / 100;
    return std::min(discount, 50.0);
}

bool Client::esteVIP() const {
    if (totalcumparaturi > 150.0 && numarcomenzi > 2 && pct_fidelitate > 10)
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

void Client::adaugaComanda(double valoare, const std::vector<std::shared_ptr<Carte> > &carti_cumparate) {

        if (valoare <= 0)
            throw DateInvalideException("Valoarea comenzii invalida!");

        plateste(valoare);
        adaugacumparaturi(valoare);

        for (const auto& carte : carti_cumparate) {
            istoric_cumparaturi.push_back(carte);
        }
}

std::vector<PublicatieSugestie> Client::genereazaSugestii() const {
    std::map<std::string, int> statistica;
    std::vector<PublicatieSugestie> sugestii;

    for (const auto& carte : istoric_cumparaturi) {
        if (!carte) continue;

        if (auto manual = std::dynamic_pointer_cast<Manual>(carte)) {
            statistica["Manuale"]++;
            statistica["Clasa_" + std::to_string(manual->getclasa())]++;
        }
        else if (auto stiintifica = std::dynamic_pointer_cast<CarteStiintifica>(carte)) {
            statistica["Stiinta"]++;
            statistica["Domeniu_" + stiintifica->getDomeniu()]++;
        }
        else if (std::dynamic_pointer_cast<Revista>(carte)) {
            statistica["Reviste"]++;
        }
    }

    if (statistica["Manuale"] > 0) {
        sugestii.emplace_back(
            "Manuale si culegeri",
            "Iti recomandam culegeri pentru aceeasi clasa.",
            "Manuale"
        );
    }

    if (statistica["Stiinta"] > 1) {
        sugestii.emplace_back(
            "Enciclopedii stiintifice",
            "Esti pasionat de stiinta! Avem enciclopedii noi.",
            "Stiinta"
        );
    }

    if (statistica["Reviste"] > 0) {
        sugestii.emplace_back(
            "Abonamente reviste",
            "Aboneaza-te pentru a primi editii lunare.",
            "Reviste"
        );
    }

    return sugestii;
}

double Client::foloseste_pct_fidelitate(int pct_utilizate) {
    if (pct_utilizate <= 0) {
        throw DateInvalideException("Numar puncte invalid!");
    }

    if (pct_utilizate > pct_fidelitate) {
        throw DateInvalideException(
            "Puncte insuficiente! Disponibil: " + std::to_string(pct_fidelitate) +
            ", Cerut: " + std::to_string(pct_utilizate)
        );
    }

    pct_fidelitate -= pct_utilizate;
    double reducere = pct_utilizate * 0.1; // 1 punct = 0.1 RON

    std::cout << "S-au folosit " << pct_utilizate << " puncte fidelitate\n"
            << "Reducere aplicata: " << reducere << " RON\n"
            << "Puncte ramase: " << pct_utilizate << "\n";

    return reducere;
}

const std::string &Client::getUsername() const {
    return username;
}

const std::string &Client::getEmail() const {
    return email;
}

int Client::getNumarComenzi() const {
    return numarcomenzi;
}

double Client::getTotalCumparaturi() const {
    return totalcumparaturi;
}

int Client::getPunctedeFideliate() const {
    return pct_fidelitate;
}

std::string Client::getTelefon() const {
    return telefon;
}

bool Client::verificaParola(const std::string &parolaIntrodusa) const {
    return parolaIntrodusa == parola;
}

void Client::seteazaParola(std::string &parolac) const {
    if (parola.size() < 6)
        throw LibrarieException("Parola prea scurta!");
    parolac = parola;
}

const std::vector<std::string> & Client::getIstoricIdentificatori()const {
    return istoric_identificatori;
}

void Client::adaugaPCT(int pct) {
    if (pct<0)
        throw DateInvalideException("Numar puncte invalid!");
    pct_fidelitate+=pct;
}

void Client::adugaredVit(double reducere) {
    if (reducere<0)
        throw DateInvalideException("Priocentaj gresit!");
    reducereLaUrmC+=reducere;
}

bool Client::aCumparatPub(const std::string &identificator) const {
    return std::ranges::find(istoric_identificatori,identificator) != istoric_identificatori.end();
}
