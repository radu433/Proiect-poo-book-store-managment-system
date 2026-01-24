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
std::vector<std::string> Comanda::extrageIdentificatori() const {
    std::vector<std::string> idf_list;

    for (const auto& art : articole) {
        if (!art.getUnitate()) continue;

        for (int i = 0; i < art.getCantitate(); ++i) {
            auto lista = art.getUnitate()->getListaIdentificatori();
            idf_list.insert(idf_list.end(), lista.begin(), lista.end());
        }
    }
    return idf_list;
}

Comanda::Comanda(const Client &client1): client(&client1),stare_comanda("Noua"),id_comanda(++global_id_comanda),data_comanda(std::time(nullptr))
{
}

Comanda::Comanda(int id, time_t data, const Client* client, const std::string& stare, const std::vector<ArticolComanda>& articole)
    : articole(articole), client(client), stare_comanda(stare), id_comanda(id), data_comanda(data) {
    if (id > global_id_comanda) global_id_comanda = id;
}

// operator <<
std::ostream& operator<<(std::ostream& out, const Comanda& cmd) {
    out << "========== COMANDA #" << cmd.id_comanda << " ==========\n";
    out << "Data: " << cmd.getDataFormatata() << "\n";
    out << "Status: " << cmd.stare_comanda << "\n";


    out << "Articole in cos: " << cmd.articole.size() << "\n";

    for (size_t i = 0; i < cmd.articole.size(); ++i) {
        const auto& art = cmd.articole[i];

        auto unitate = art.getUnitate();
        if (!unitate) {
            out << "  [" << i << "] Articol invalid\n";
            continue;
        }

        out << "  [" << i << "] "
            << unitate->getDescriere()
            << " x" << art.getCantitate()
            << " | Subtotal: " << art.getSubtotal() << " RON\n";
    }

    out << "------------------------------------\n";
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

    if (articole.empty())
        throw ComandaGoalaFinalizareException();
    if (stare_comanda == "Anulata")
        throw ComandaAnulataException(id_comanda);
    if (stare_comanda == "Finalizata")
        throw ComandaFinalizataException(id_comanda);

    for (const auto& art : articole) {
        if (!art.getUnitate())
            throw ComandaInvalidaException("Articol NULL");
        if (!art.getUnitate()->verificaStocSuficient(art.getCantitate()))
            throw StocInsuficientException(
                art.getUnitate()->getDescriere(),
                art.getUnitate()->getCantitate(),
                art.getCantitate());
    }

}

void Comanda::stergeArticol(int idx) {
    if (idx < 0 || idx >= static_cast<int>(articole.size()))
        throw DateInvalideException("Index invalid");

    articole.erase(articole.begin() + idx);
}

void Comanda::anuleazaComanda() {
    if (stare_comanda == "Finalizata")
        throw ComandaFinalizataException(id_comanda);

    stare_comanda = "Anulata";
    std::cout << "Comanda #" << id_comanda << " anulata\n";
}


const std::vector<ArticolComanda> & Comanda::getArticole() const {
    return articole;
}

std::string Comanda::getUser() const {
    return client->getUsername();
}

void Comanda::adaugaArticol(const std::shared_ptr<UnitateVanzare> &unitate, int cantitate) {
    if (!unitate)
        throw ComandaInvalidaException("Articol invalid");
    if (cantitate <= 0)
        throw DateInvalideException("Cantitate invalida");

    if (!unitate->verificaStocSuficient(cantitate))
        throw StocInsuficientException(unitate->getDescriere(),
                                       unitate->getCantitate(),
                                       cantitate);

    articole.emplace_back(unitate, cantitate);

    std::cout << "Adaugat: " << cantitate << " x "
              << unitate->getDescriere() << "\n";
}

double Comanda::calculeazaTotal() const {
    double total = 0.0;
    int nrPachete = 0;
    int nrUnitati = 0;

   for (const auto& art : articole) {
       auto unitate = art.getUnitate();
       if (!unitate) continue;
       total += unitate->getPretFinUnitate()* art.getCantitate();
       if (unitate->estePPredefinit())
           nrPachete+=art.getCantitate();
       nrUnitati+=art.getCantitate();
   }
    if (nrPachete >= 3) {
       total -= 50.0;
   }
    if (nrUnitati >= 15) {
        total *= 0.95;
    }
    return total;
}

std::shared_ptr<Client> Comanda::getClient() const {
    if (!client) return nullptr;
    return std::make_shared<Client>(*client);
}

static std::vector<std::string> split_cmd(const std::string& s, const char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::string Comanda::serializare() const {
    std::stringstream ss;
    ss << id_comanda << "|" << data_comanda << "|" << client->getEmail() << "|" << stare_comanda << "|";
    for (size_t i = 0; i < articole.size(); ++i) {
        if (articole[i].getUnitate()) {
            ss << articole[i].getUnitate()->getId() << "," << articole[i].getCantitate();
            if (i < articole.size() - 1) ss << ";";
        }
    }
    return ss.str();
}

Comanda Comanda::deserializare(const std::string& line, const std::vector<Client>& clienti, const std::vector<std::shared_ptr<UnitateVanzare>>& stoc) {
    const auto v = split_cmd(line, '|');
    if (v.size() < 4) throw std::runtime_error("Linie comanda invalida!");

    const int id = std::stoi(v[0]);
    const time_t data = std::stoll(v[1]);
    const std::string email = v[2];
    const std::string stare = v[3];
    const Client* ptrClient = nullptr;
    for (const auto& c : clienti) {
        if (c.getEmail() == email) {
            ptrClient = &c;
            break;
        }
    }
    if (!ptrClient) throw std::runtime_error("Clientul " + email + " nu a fost gasit la deserializarea comenzii!");

    std::vector<ArticolComanda> articole_load;
    if (v.size() > 4 && !v[4].empty()) {
        for (const auto arts = split_cmd(v[4], ';'); const auto& art_str : arts) {
            if (auto pair = split_cmd(art_str, ','); pair.size() == 2) {
                const int idUnit = std::stoi(pair[0]);
                int cant = std::stoi(pair[1]);
                std::shared_ptr<UnitateVanzare> unitGasit = nullptr;
                for (const auto& u : stoc) {
                    if (u->getId() == idUnit) {
                        unitGasit = u;
                        break;
                    }
                }
                if (unitGasit) {
                    articole_load.emplace_back(unitGasit, cant);
                }
            }
        }
    }

    return Comanda(id, data, ptrClient, stare, articole_load);
}