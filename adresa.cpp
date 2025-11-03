#include "adresa.h"
#include <sstream>

Adresa::Adresa(std::string const &judet, const std::string &oras, const std::string &strada, const std::string &cod_postal):judet(judet),
oras(oras), strada(strada), cod_postal(cod_postal){
}

void Adresa::modificaAdreasa(const std::string &jud, const std::string &o, const std::string &str,
    const std::string &cp) {
    this->strada = str;
    this->oras = o;
    this->cod_postal = cp;
    this->judet = judet;
}

bool Adresa::esteValida() const {
    if (strada.empty() || oras.empty() || cod_postal.empty() || judet.empty()) {
        return false;
    }

    if (cod_postal.length() != 6) {
        return false;
    }

    for (char const &c : cod_postal) {
        if (std::isdigit(c) == 0) {
            return false; // Nu e cifră
        }
    }

    return true;
}

std::string Adresa::getAdresaCompleta() const {
    std::stringstream ss;
    ss << "Destinatar: " << "\n"
       << "Strada: " << this->strada << "\n"
       << "Oras: " << this->oras << ", Jud. " << this->judet << "\n"
       << "Cod Postal: " << this->cod_postal;

    return ss.str();
}



std::ostream & operator<<(std::ostream &out, const Adresa &a) {
    out << a.strada << ", " << a.oras << ", " << a.cod_postal;
    return out;
}
