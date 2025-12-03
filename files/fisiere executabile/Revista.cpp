#include "../headere/Revista.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

#include <set>

namespace {
    static const std::set<std::string> divertisment = {"Fashion", "Cancan", "Sport", "Lifestyle"};
    static const std::set<std::string> hobby = {"Auto", "IT Consumer", "Casa si Gradina"};
    static const std::set<std::string> profesional = {"Tehnologie", "Business", "Psihologie Practica"};
    static const std::set<std::string> academic = {"Medicala", "Stiintifica", "Juridica"};
}
    // constructor cu parametrii


Revista::Revista(const std::string &titlu, const std::shared_ptr<Autor> &autor, int cantitate, int an_publicare,
                 const std::string &isbn, double pret_baza,const int numar_pagini,const std::string &frecventa,  int numar_editie,
                 const bool are_cadou_suplimentar, const std::string& tip) : Carte(titlu, autor, cantitate, an_publicare, isbn, pret_baza, numar_pagini),
                                                                             frecventa(frecventa), numar_editie(numar_editie),
                                                                             are_cadou_suplimentar(are_cadou_suplimentar),tip(tip) {
    if (numar_editie < 0) {
        throw DateInvalideException("Numarul editiei nu poate fi negativ!");
    }
    if (tip.empty()) {
        throw DateInvalideException("Tipul revistei nu poate fi gol!");
    }
}
// afisare
void Revista::afisare(std::ostream &out) const {
    Carte::afisare(out);
    out << "Tip revista: " << tip
        <<"Numar pagini:" << numar_pagini<<"\n"
        << ", Frecventa: " << frecventa << "\n"
        << "Editia nr: " << numar_editie <<"\n";
}

// functii virtuale

double Revista::getPretFinal() const {
    double pret=pret_baza;
    if (tip == "Fashion" || tip == "Cancan" || tip == "Sport" || tip == "Lifestyle")
        pret*=1.1;
    if (tip == "Tehnologie" || tip == "Business" || tip == "Psihologie Practica")
        pret*=1.2;
    if (tip == "Auto" || tip == "IT Consumer"|| tip == "Casa si Gradina")
        pret*=1.4;
    if (tip == "Medicala" || tip == "Stiintifica" || tip == "Juridica")
        pret*=1.5;
    if (are_cadou_suplimentar)
        pret+=10;
    return pret;
}

std::string Revista::getTip() const {return tip;}

double Revista::calculeaza_valoarea_academica() const {
    double valoare=0.0;

    if (divertisment.contains(tip)) {
        valoare+=5.0;
    }
    else if (hobby.contains(tip)) {
        valoare+=10.0;
    }
    else if (profesional.contains(tip)) {
        valoare+=15.0;

    }
    else if (academic.contains(tip)) {
        valoare+=20.0;
    }

    if (numar_editie >= 500) {
        valoare += 25.0;
    } else if (numar_editie >= 200) {
        valoare += 20.0;
    } else if (numar_editie >= 100) {
        valoare += 15.0;
    } else if (numar_editie >= 50) {
        valoare += 10.0;
    } else if (numar_editie >= 10) {
        valoare += 5.0;
    }
    int an_curent = 2025;
    int vechime = an_curent - an_publicare;

    if (vechime == 0) {
        valoare += 15.0;
    } else if (vechime == 1) {
        valoare += 10.0;
    } else if (vechime <= 3) {
        valoare += 5.0;
    }

    double rating = calculeaza_raitingmin();
    valoare += rating * 2.0;

    if (esteBestseller()) {
        valoare += 5.0;
    }
    if (esteEditieSpeciala()) {
        valoare += 5.0;
    }

    return std::min(valoare, 100.0);

}

int Revista::timp_estimat_lecturii() const {
    if (tip=="Stiintifica")
        return numar_pagini*5;
    return numar_pagini;
}

bool Revista::esteEditieSpeciala() const {
    return (numar_editie % 100 == 0) ||
           (numar_editie % 50 == 0);

}

double Revista::calculeaza_frecventa_citire() const {
    double scor=0.0;
    if (frecventa=="Saptamanala")
        scor=100.0;
    else if (frecventa=="Lunar")
        scor=80.0;
    else if (frecventa=="Trimestrial")
        scor=30.0;

    if (academic.contains(tip))
        scor*=1.5;
    else if (profesional.contains(tip))
        scor*=1.3;
    else if (hobby.contains(tip))
        scor*=1.2;
    else
        scor*=1.1;

    return scor;
}

bool Revista::este_colectionabila() const {
    if (esteEditieSpeciala())
        return true;
    if (numar_editie >= 200 && (academic.contains(tip) || profesional.contains(tip))) {
        return true;

    }
    if (calculeaza_raitingmin()>=4.5 && numar_editie>=50)
        return true;

    return false;
}



