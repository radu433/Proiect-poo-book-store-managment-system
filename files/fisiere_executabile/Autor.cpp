#include "../headere/Autor.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/AutorException.h"
#include<iostream>
int Autor::nextidautor=0;
// constructor fara parametrii
Autor::Autor() : nume("NECUNOSCUT"), prenume("NECUNOSCUT"), varsta(0), numar_premii(0), idautor(++nextidautor) {
}

// constructor cu parametrii
Autor::Autor(const std::string &nume, const std::string &prenume, int varsta) : nume(nume),
    prenume(prenume), varsta(varsta), numar_premii(0) ,idautor(++nextidautor){
    if (nume.empty() || prenume.empty()) {
        throw AutorInvalidException("Numele si prenumele autorului nu pot fi goale!");
    }
    if (varsta < 0) {
        throw AutorInvalidException("Varsta autorului nu poate fi negativa (" + std::to_string(varsta) + ")!");
    }
    if (varsta > 120) {
        throw AutorInvalidException("Varsta autorului este nerealista (" + std::to_string(varsta) + ")!");
    }
}

// getteri
const std::string &Autor::getNume() const {
    return nume;
}

const std::string &Autor::getprenume() const {
    return prenume;
}

int Autor::getvarsta() const {
    return varsta;
}


// supraincarcarea operatorului <<
std::ostream &operator<<(std::ostream &out, const Autor &obj) {
    out << "Autor:" << obj.getNume() << " " << obj.getprenume() << " Varsta:" << obj.getvarsta();
    return out;
};


// fuctii

size_t Autor::getcarti_scrise() const {
    return carti_scrise.size();
}

bool Autor::adauga_carte(const std::string &isbn) {
    for (const auto &c: carti_scrise) {
        if (c == isbn) {
            return false;
        }
    }
    carti_scrise.push_back(isbn);
    return true;
}

double Autor::calcproductivitate() const {
    if (varsta < 18) return 0.0;
    int ani_c = varsta - 18;
    if (ani_c == 0) return 0.0;
    return static_cast<double>(carti_scrise.size()) / ani_c;
}

int Autor::calcscorrenume() const {
    return carti_scrise.size() * 10 + numar_premii * 50;
}

std::string Autor::ierarhie_a() const {
    int scor_renume = calcscorrenume();
    double productivitate = calcproductivitate();


    int bonus_productivitate = static_cast<int>(productivitate * 50);

    int scor_total = scor_renume + bonus_productivitate;

    if (scor_total > 600)
        return "Legendar";

    if (scor_total > 350)
        return "Renumit";

    if (scor_total > 200)
        return "Consacrat";

    if (scor_total > 100)
        return "In ascensiune";

    return "Incepator";
}

int Autor::getidAutor() const {
    return idautor;
}
