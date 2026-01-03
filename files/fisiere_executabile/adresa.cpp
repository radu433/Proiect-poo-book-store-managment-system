#include "../headere/adresa.h"
#include <sstream>
#include "../exceptii/exceptii_headere/DateInvalideException.h"

Adresa::Adresa(std::string const &judet, const std::string &oras, const std::string &strada, const int numar,
               const std::string &cod_postal) : judet(judet),
                                                oras(oras), strada(strada), numar(numar), cod_postal(cod_postal) {
    if (this->judet.empty() || this->oras.empty() || this->strada.empty()) {
        throw DateInvalideException("Adresa incompleta: Judetul, Orasul si Strada sunt obligatorii!");
    }
    if (this->cod_postal.length() != 6) {
        throw DateInvalideException("Cod postal invalid (" + this->cod_postal + "): Trebuie sa aiba exact 6 cifre.");
    }
    for (const char c: this->cod_postal) {
        if (!std::isdigit(c)) {
            throw DateInvalideException(
                "Cod postal invalid (" + this->cod_postal + "): Contine caractere non-numerice.");
        }
    }
}

// operator <<
std::ostream &operator<<(std::ostream &out, const Adresa &a) {
    out << a.strada << ", " << a.oras << ", " << a.cod_postal;
    return out;
}

// functii

void Adresa::modificaAdreasa(const std::string &jud, const std::string &o, const std::string &str,
                             const std::string &cp) {
    if (jud.empty() || o.empty() || str.empty()) {
        throw DateInvalideException("Nu puteti actualiza adresa cu campuri goale!");
    }

    if (cp.length() != 6) {
        throw DateInvalideException("Noul cod postal este invalid (lungime gresita)!");
    }

    for (const char c: cp) {
        if (!std::isdigit(c)) {
            throw DateInvalideException("Noul cod postal contine litere!");
        }
    }
    this->strada = str;
    this->oras = o;
    this->cod_postal = cp;
    this->judet = jud;
}

std::string Adresa::getAdresaCompleta() const {
    std::stringstream ss;
    ss << "Destinatar: " << "\n"
            << "Strada: " << this->strada << "\n"
            << "Oras: " << this->oras << ", Jud. " << this->judet << "\n"
            << "Cod Postal: " << this->cod_postal;

    return ss.str();
}