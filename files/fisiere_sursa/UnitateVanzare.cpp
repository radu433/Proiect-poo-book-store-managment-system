#include "../headere/UnitateVanzare.h"
#include "../headere/Revista.h"
#include "../headere/Carte.h"
#include "../headere/Publicatie.h"
#include "../headere/CarteIndividuala.h"
#include "../headere/RevistaIndividuala.h"
#include "../headere/PachetSerie.h"

#include <iomanip>
#include <vector>

#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include <sstream>
#include <stdexcept>

int UnitateVanzare::id_contor_global = 0;

//constructori

UnitateVanzare::UnitateVanzare(std::shared_ptr<Carte> carte)
    : id_unic(++id_contor_global),
      produs_principal(std::move(carte))
{
    if (!produs_principal) {
        throw DateInvalideException("Eroare critica: UnitateVanzare creata cu un pointer null!");
    }
}

UnitateVanzare::UnitateVanzare(std::shared_ptr<Revista> revista): id_unic(++id_contor_global),
      produs_principal(std::move(revista))
{
    if (!produs_principal) {
        throw DateInvalideException(
            "Eroare critica: UnitateVanzare creata cu revista nullptr!");
    }
}

UnitateVanzare::UnitateVanzare(int id, std::shared_ptr<Publicatie> produs) : id_unic(id), produs_principal(std::move(produs))
{
    if (id > id_contor_global) id_contor_global = id;
    if (!produs_principal) throw DateInvalideException("UnitateVanzare null!");
}

UnitateVanzare::UnitateVanzare(const UnitateVanzare &other)
    : id_unic(++id_contor_global), // ID nou la copiere
      produs_principal(other.produs_principal)
{
}

//geteri

std::shared_ptr<Publicatie> UnitateVanzare::getProdusPrincipal() const {
    return produs_principal;
}

int UnitateVanzare::getId() const {
    return id_unic;
}

std::string UnitateVanzare::getNumeProdus() const {
    return produs_principal->getTitlu();
}

int UnitateVanzare::getCantitate() const {
    return produs_principal->getcantitate();
}

int UnitateVanzare::calculeazaLuniDetinere() const {
    return calculeazaZileDetinere() / 30;
}

bool UnitateVanzare::valideazaDisponibilitate() const {

    return verificaStocSuficient(1);
}

std::string UnitateVanzare::getTitlu() const {return produs_principal->getTitlu();}

void UnitateVanzare::marcheazaSecondHand(const std::string &conditie, int luni_vechime) {
    este_second_hand = true;
    conditie_fizica = conditie;

    const time_t acum = std::time(nullptr);
    data_cumparare = acum - luni_vechime * 30 * 24 * 60 * 60;
}

double UnitateVanzare::getPretFinUnitate() const {
    return produs_principal->getPretFinal();
}
int UnitateVanzare::calculeazaZileDetinere() const {
    time_t acum = std::time(nullptr);
    double sec = std::difftime(acum, data_cumparare);
    return static_cast<int>(sec / (60 * 60 * 24));
}


//afisare

void UnitateVanzare::afisare(std::ostream &out) const {
    out << "[ID:" << id_unic << "] " << getNumeProdus();
}

std::ostream & operator<<(std::ostream &out, const UnitateVanzare &obj) {
    obj.afisare(out);
    return out;
}

std::string UnitateVanzare::serializare() const {
    std::stringstream ss;
    ss << id_unic << "|" << conditie_fizica << "|" << este_second_hand << "|" << data_cumparare << "|" 
       << (produs_principal ? produs_principal->getIdentificator() : "");
    return ss.str();
}

static std::vector<std::string> split_uv(const std::string& s) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item,'|')) {
        elems.push_back(item);
    }
    return elems;
}

std::shared_ptr<UnitateVanzare> UnitateVanzare::fabricare(const std::string& line, const std::vector<std::shared_ptr<Publicatie>>& publicatii) {
    const auto v = split_uv(line);
    if (v.empty() || v.size() < 6) return nullptr;

    const std::string tip = v[0];
    int id = std::stoi(v[1]);
    const std::string conditie = v[2];
    const bool esteSH = (std::stoi(v[3]) != 0);
    const time_t data = std::stoll(v[4]);
    const std::string idPub = v[5];
    auto findPub = [&](const std::string& id_cautat) -> std::shared_ptr<Publicatie> {
        for (const auto& p : publicatii) {
            if (p->getIdentificator() == id_cautat) return p;
        }
        return nullptr;
    };

    const std::shared_ptr<Publicatie> pub = findPub(idPub);

    if (!pub && tip != "PachetSerie") {
         return nullptr;
    }
    std::shared_ptr<UnitateVanzare> uv = nullptr;
    if (tip == "CarteIndividuala") {
        if (auto carte = std::dynamic_pointer_cast<Carte>(pub)) {
            uv = std::make_shared<CarteIndividuala>(id, carte);
        }
    }
    else if (tip == "RevistaIndividuala") {
        if (auto rev = std::dynamic_pointer_cast<Revista>(pub)) {
            uv = std::make_shared<RevistaIndividuala>(id, rev);
        }
    }
    else if (tip == "PachetSerie") {
        uv = PachetSerie::deserializare(line, publicatii);
    }

    if (uv) {
        uv->initDateSH(esteSH, conditie, data);
    }
    
    return uv;
}