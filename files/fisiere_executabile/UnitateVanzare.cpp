#include "../headere/UnitateVanzare.h"
#include "../headere/Revista.h"

#include <iomanip>

#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include <sstream>
#include <stdexcept>

int UnitateVanzare::id_contor_global = 0;

// --- CONSTRUCTORI ---

UnitateVanzare::UnitateVanzare(std::shared_ptr<Publicatie> produs)
    : id_unic(++id_contor_global),
      produs_principal(std::move(produs))
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

UnitateVanzare::UnitateVanzare(const UnitateVanzare &other)
    : id_unic(++id_contor_global), // ID nou la copiere
      produs_principal(other.produs_principal)
{
}

// --- GETTERE ---

std::shared_ptr<Publicatie> UnitateVanzare::getProdusPrincipal() const {
    return produs_principal;
}

int UnitateVanzare::getId() const {
    return id_unic;
}

std::string UnitateVanzare::getNumeProdus() const {
    return produs_principal->getTitlu();
}

int UnitateVanzare::getCantitate() const { // Corectat typo
    return produs_principal->getcantitate();
}



// --- LOGICĂ ---
int UnitateVanzare::calculeazaLuniDetinere() const {
    return calculeazaZileDetinere() / 30;
}

bool UnitateVanzare::valideazaDisponibilitate() const {
    // Apelează funcția virtuală care știe să verifice pachetele corect
    return verificaStocSuficient(1);
}

void UnitateVanzare::marcheazaSecondHand(const std::string &conditie, int luni_vechime) {
    este_second_hand = true;
    conditie_fizica = conditie;

    const time_t acum = std::time(nullptr);
    data_cumparare = acum - luni_vechime * 30 * 24 * 60 * 60;
}

int UnitateVanzare::calculeazaZileDetinere() const {
    time_t acum = std::time(nullptr);
    double sec = std::difftime(acum, data_cumparare);
    return static_cast<int>(sec / (60 * 60 * 24));
}

std::string UnitateVanzare::getDataCumparareFormatata() const {
    const std::tm* tm_info = std::localtime(&data_cumparare);
    if (!tm_info) return "Data invalida";

    std::ostringstream ss;
    ss << std::put_time(tm_info, "%d/%m/%Y");
    return ss.str();
}

// --- AFIȘARE ---

void UnitateVanzare::afisare(std::ostream &out) const {
    out << "[ID:" << id_unic << "] " << getNumeProdus();
}

std::ostream & operator<<(std::ostream &out, const UnitateVanzare &obj) {
    obj.afisare(out);
    return out;
}