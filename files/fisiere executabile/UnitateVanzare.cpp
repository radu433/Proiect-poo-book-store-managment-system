#include "../headere/UnitateVanzare.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include <sstream>
#include <stdexcept>

int UnitateVanzare::id_contor_global = 0;

// --- CONSTRUCTORI ---

UnitateVanzare::UnitateVanzare(std::shared_ptr<Carte> produs)
    : id_unic(++id_contor_global),
      produs_principal(std::move(produs))
{
    if (!produs_principal) {
        throw DateInvalideException("Eroare critica: UnitateVanzare creata cu un pointer null!");
    }
}

UnitateVanzare::UnitateVanzare(const UnitateVanzare &other)
    : id_unic(++id_contor_global), // ID nou la copiere
      produs_principal(other.produs_principal)
{
}

// --- GETTERE ---

std::shared_ptr<Carte> UnitateVanzare::getProdusPrincipal() const {
    return produs_principal;
}

int UnitateVanzare::getId() const {
    return id_unic;
}

std::string UnitateVanzare::getNumeProdus() const {
    return produs_principal->gettitlu();
}

int UnitateVanzare::getCantitate() const { // Corectat typo
    return produs_principal->getcantitate();
}

std::string UnitateVanzare::getISBN() const {
    if (produs_principal) {
        return produs_principal->getISBN();
    }
    return "";
}

// --- LOGICĂ ---

bool UnitateVanzare::valideazaDisponibilitate() const {
    // Apelează funcția virtuală care știe să verifice pachetele corect
    return verificaStocSuficient(1);
}

// --- AFIȘARE ---

void UnitateVanzare::afisare(std::ostream &out) const {
    out << "[ID:" << id_unic << "] " << getNumeProdus();
}

std::ostream & operator<<(std::ostream &out, const UnitateVanzare &obj) {
    obj.afisare(out);
    return out;
}