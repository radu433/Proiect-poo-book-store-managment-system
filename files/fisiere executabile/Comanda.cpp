#include "../headere/Comanda.h"
#include "../headere/Client.h"
#include "../headere/PachetSerie.h"
#include "../headere/UnitateVanzare.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere//ClientExceptions.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../exceptii/exceptii_headere/ComandaExceptions.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <utility>
#include <memory>
#include <ctime>

#include "../headere/CarteIndividuala.h"
int Comanda::global_id_comanda=0;
// healper
std::vector<std::string> Comanda::extrageISBN() const {
    std::vector<std::string> isbn_list;
    for (const auto& art : articole) {
        std::vector<std::string> isbn_uri_articol = art->getListaIdentificatori();
        isbn_list.insert(
           isbn_list.end(),
           isbn_uri_articol.begin(),
           isbn_uri_articol.end() );
    }
    return isbn_list;
}

std::vector<std::shared_ptr<Carte>> Comanda::extrageCarti() const {
    std::vector<std::shared_ptr<Carte>> lista_carti;

    for (const auto &unitate: articole) {
        // CAZ 1: PACHET
        if (auto pachet = std::dynamic_pointer_cast<PachetSerie>(unitate)) {
            for (const auto &pub: pachet->getContinut()) {
                if (auto carte = std::dynamic_pointer_cast<Carte>(pub)) {
                    lista_carti.push_back(carte);
                }
            }
        }
        // CAZ 2: ARTICOL INDIVIDUAL
        else {
            auto pub = unitate->getProdusPrincipal();
            if (auto carte = std::dynamic_pointer_cast<Carte>(pub)) {
                lista_carti.push_back(carte);
            }
        }
    }

    return lista_carti;
}

// constructor cu parametrii
Comanda::Comanda(const std::shared_ptr<Client> &client) : client(std::move(client)), stare_comanda("Noua"),
                                                          id_comanda(++global_id_comanda),data_comanda(std::time(nullptr)) {
    if (!this->client)
    { throw ClientInvalidException("u se poate crea o comanda fara un client valid!");}
    std::cout << "Comanda #" << id_comanda << " creata la " << getDataFormatata() << "\n";
}

// destructor
Comanda::~Comanda() {
   Curata_articole();
    std::cout<<"Comanda #"<<id_comanda<<" a fost stearsa la "<< getDataFormatata()<<"\n";
}

// constructor de copiere

Comanda::Comanda(const Comanda &other):client(other.client), stare_comanda(other.stare_comanda),
                                        id_comanda(++global_id_comanda), data_comanda(std::time(nullptr)) {
    articole.reserve(other.articole.size());
    for (const auto& art :other.articole) {
        articole.push_back(art->clone());
    }
    std::cout << "DEBUG: Comanda #" << id_comanda << " a fost copiata prin CLONE.\n";

}

// swap si operator=

void swap(Comanda &unu, Comanda &doi) noexcept {
    std::swap(unu.client, doi.client);
    std::swap(unu.articole, doi.articole);
    std::swap(unu.stare_comanda, doi.stare_comanda);
    std::swap(unu.id_comanda,doi.id_comanda);
}

Comanda& Comanda::operator=(Comanda other) {
    swap(*this, other);
    return *this;
}


// operator <<
std::ostream &operator<<(std::ostream &out, const Comanda &cmd) {
    out << "========== COMANDA #" << cmd.id_comanda << " ==========\n";
    out << "Data: " << cmd.getDataFormatata() << "\n";
    out << "Status: " << cmd.stare_comanda << "\n";

    if (cmd.client) {
        out << "Client: " << *cmd.client << "\n";
    }

    out << "Articole in cos: " << cmd.articole.size() << "\n";

    for (size_t i = 0; i < cmd.articole.size(); ++i) {
        out << "  [" << i << "] " << *cmd.articole[i] << "\n";
        out << "     [DETALII]: " << cmd.articole[i]->getDescriere() << "\n";
    }

    out << "TOTAL COMANDA: " << cmd.calculeazaTotal() << " RON\n";
    out << "====================================\n";
    return out;
}
// getteri
 const std::string& Comanda::getStare() const {
    return stare_comanda;
}

int Comanda::getId() const {return id_comanda;}

std::string Comanda::getDataFormatata() const {
    const std::tm* tm_info = std::localtime(&data_comanda);
    std::stringstream ss;
    ss << std::put_time(tm_info, "%d/%m/%Y %H:%M:%S");
    return ss.str();
}

int Comanda::getNumarArticole() const {
    return static_cast<int>(articole.size());
}
// functii
void Comanda::valideazaComanda() const {
    if (!client) {
        throw ClientInvalidException("Pointerul catre client este NULL!");
    }
    if (articole.empty()) {
        throw ComandaGoalaException();
    }
    if (stare_comanda=="Anulata") {
        throw ComandaAnulataException(id_comanda);
    }
    if (stare_comanda=="Finalizata") {
        throw ComandaFinalizataException(id_comanda);
    }
    for (const auto& art:articole) {
        if (!art) {
            throw ComandaInvalidaException("Articol invalid gasit in lista de produse.");
        }
        if (!art->valideazaDisponibilitate()){
            throw StocInsuficientException(art->getNumeProdus(), 0, 1);

        }

    }
    std::cout << "Comanda #" << id_comanda << " este valida!\n";
}

void Comanda::stergeArticol(int idx) {
    if (idx < 0 || idx >= static_cast<int>(articole.size())) {
        throw DateInvalideException("Index articol invalid!");
    }
    articole[idx]->getProdusPrincipal()->cresteCantitatea(1);

    // eliminam articolul din cos
    articole.erase(articole.begin() + idx);
}

void Comanda::anuleazaComanda() {
    if (stare_comanda=="Finalizata") {
        throw ComandaFinalizataException(id_comanda);
    }
    if (stare_comanda=="Anulata")
        std::cout<<"Comanda este deja anulata!";

    stare_comanda="Anulata";
    std::cout<<"Comanda #"<<id_comanda<<"a fost anulata! ";
    std::cout<<"Articole anulate:"<<articole.size()<<"\n";
}

std::shared_ptr<Client> Comanda::getClient() const {
    return client;
}

void Comanda::adaugaArticol(const std::shared_ptr<UnitateVanzare> &articol, int cantitate) {
    if (!articol)
        throw ComandaInvalidaException("Incercare de adaugare articol invalid.");
    if (cantitate<=0)
        throw std::invalid_argument("Stoc insuficient!");
    if (stare_comanda!="Noua" && stare_comanda!="In procesare")
        throw ComandaInvalidaException("Nu se pot adauga produse. Comanda este: " + stare_comanda);
    if (!articol->verificaStocSuficient(cantitate)) {
        throw StocInsuficientException(articol->getDescriere(), articol->getCantitate(), cantitate);
    }
    for (int i = 0; i < cantitate; ++i) {
        articole.push_back(articol->clone());
    }

    std::cout << "Succes: S-au adaugat " << cantitate << " x "
              << articol->getDescriere() << "\n";
}

double Comanda::calculeazaTotal() const {
    if (articole.empty())
        return 0.0;
    double total = 0.0;
    int numar_pachete=0;
    for (const auto& art: articole) {
        double pret_articol=art->getPretcomanda();

        if (auto pachet= std::dynamic_pointer_cast<PachetSerie>(art)) {
            std::cout<<"Pachet detecta: "<<pachet->getDescriere()<< "- Reducere exctra 10%\n";
            pret_articol*=0.9;
            numar_pachete++;
        }
        total+=pret_articol;
    }
    if (numar_pachete >= 3) {
        double reducere_multipachete = 50.0;
        total-=reducere_multipachete;
        std::cout<<"Bonus 3+ pachete: -"<< reducere_multipachete << "RON\n";
    }
    double discount_client=0.0;
    std::string ierarhie=client->ierarhie_clienti();

    if (ierarhie == "Platinum") {
        discount_client = 0.25;
    } else if (ierarhie == "Gold") {
        discount_client = 0.20;
    } else if (ierarhie == "Silver") {
        discount_client = 0.15;
    } else if (ierarhie == "Bronze") {
        discount_client = 0.10;
    } else {
        discount_client = 0.0;
    }
    double reducere=total*discount_client;
    total = total - reducere;

    if (articole.size()>=15) {
        double reducere_volum=total*0.05;
        total-=reducere_volum;
        std::cout<<"Reducere volum aplicata:-"<< reducere_volum<<"RON\n";
    }
    return total;
}

double Comanda::finalizareComanda() {
    valideazaComanda();

    if (stare_comanda == "Finalizata") {
        throw ComandaFinalizataException(id_comanda);
    }
    if (stare_comanda == "Anulata") {
        throw ComandaAnulataException(id_comanda);
    }
    const double total=calculeazaTotal();

    if (client->getSold()<total) {
        throw SoldInsuficientException(client->getSold(), total);
    }
    for (const auto& art: articole) {
        if (const auto pachet=std::dynamic_pointer_cast<PachetSerie>(art)) {
            if (!pachet->verificaCompletitudine()) {
                throw DateInvalideException(
                    "Pachetul '" + pachet->getNumeProdus() + "' este incomplet si nu poate fi procesat!");
            }
        }
        std::cout << "Procesare stoc: " << art->getDescriere() << "\n";
        art->scadeStoc(1);
    }
    std::vector<std::shared_ptr<Carte>> lista_carti = extrageCarti();

    // actualizare client
    client->adaugaComanda(total, lista_carti);

    // schimbare de stare
    stare_comanda = "Finalizata";

    std::cout << "\n========== COMANDA FINALIZATA ==========\n";
    std::cout << "Comanda #" << id_comanda << " finalizata cu succes!\n";
    std::cout << "Total: " << total << " RON\n";
    std::cout << "Client: " << client->ierarhie_clienti() << "\n";
    std::cout << "========================================\n\n";

    return total;
}
//...