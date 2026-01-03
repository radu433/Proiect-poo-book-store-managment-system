#include "../headere/CarteStiintifica.h"
#include <string>
#include <ctime>
#include "../exceptii/exceptii_headere/DateInvalideException.h"

// constructor cu parametrii
CarteStiintifica::CarteStiintifica(const std::string &titlu, const std::shared_ptr<Autor> &autor, int cantitate,
                                   const std::string &data_publicatie,
                                   const std::string &isbn, double pret_baza, const int numar_pagini,
                                   const std::string &editura, const std::string &domeniu,
                                   const std::string &nivel_academic,
                                   const int nr_referinte, const bool are_formule_diagrame) : Carte(titlu, autor,
        cantitate, data_publicatie, isbn, pret_baza, numar_pagini, editura),
    domeniu(domeniu), nivel_academic(nivel_academic), nr_referinte(nr_referinte),
    are_formule_diagrame(are_formule_diagrame) {
    if (domeniu.empty()) {
        throw DateInvalideException("Domeniul cartii stiintifice nu poate fi gol!");
    }
    if (nivel_academic.empty()) {
        throw DateInvalideException("Nivelul academic nu poate fi gol!");
    }

    if (nr_referinte < 0) {
        throw DateInvalideException("Numarul de referinte nu poate fi negativ: " + std::to_string(nr_referinte));
    }
}

int scorNivel(const std::string &nivel) {
    if (nivel == "Generala") return 1;
    if (nivel == "Liceu") return 2;
    if (nivel == "Universitar") return 3;
    if (nivel == "Masterat") return 4;
    if (nivel == "Doctorat") return 5;
    if (nivel == "Cercetare") return 6;

    return 0;
}

int CarteStiintifica::timp_estimat_lecturii() const {
    int minute_per_pagina = 8;
    if (scorNivel(nivel_academic) == 6 || scorNivel(nivel_academic) == 5)
        minute_per_pagina = 15;
    return nr_pagini * minute_per_pagina;
}

double CarteStiintifica::getPretFinal() const {
    double pret = pret_baza;
    const double impact = calculeazaFactorImpact();
    pret *= 1.3;
    if (impact >= 5 && impact <= 10)
        pret += 50;
    else pret += 10;
    return pret;
}

std::string CarteStiintifica::getTip() const { return nivel_academic; }


const std::string &CarteStiintifica::getDomeniu() const { return domeniu; }

bool CarteStiintifica::esteDeActualitate() const {
    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                             + (tm_now->tm_mon + 1) * 100
                             + tm_now->tm_mday;

    const int data_aparitie = Data::parse(data_publicatie).toNumeric();

    if (domeniu == "Informatica" || domeniu == "Medicina" || domeniu == "Robotica") {
        return (data_actuala - data_aparitie) <= 10000;
    }

    return (data_actuala - data_aparitie) <= 150000;
}


double CarteStiintifica::calculeaza_valoarea_academica() const {
    double valoare = 0.0;

    if (scorNivel(nivel_academic) == 6 || scorNivel(nivel_academic) == 5)
        valoare += 40;
    else if (scorNivel(nivel_academic) == 4 || scorNivel(nivel_academic) == 3)
        valoare += 30;
    else if (scorNivel(nivel_academic) == 2)
        valoare += 15;
    else valoare += 10;

    double impact = calculeazaFactorImpact();
    valoare += (impact * 3.0);

    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                             + (tm_now->tm_mon + 1) * 100
                             + tm_now->tm_mday;

    const int data_apritie = Data::parse(data_publicatie).toNumeric();

    if (domeniu == "IT" || domeniu == "Medicina") {
        if (data_actuala - data_apritie <= 20000) valoare += 20;
        else if (data_actuala - data_apritie <= 50000) valoare += 10;
        else
            valoare -= 10;
    } else {
        if (data_actuala - data_apritie <= 50000) valoare += 20;
        else if (data_actuala - data_apritie <= 150000) valoare += 10;
        else
            valoare += 5;
    }
    return std::min(valoare, 100.0);
}

void CarteStiintifica::seteazaReducere(int procent, int durata_zilei) {
    Carte::seteazaReducere(procent, durata_zilei);
}

double CarteStiintifica::calculeazaFactorImpact() const {
    double scor = 0.0;

    if (nr_referinte > 100)
        scor += 3.0;
    else if (nr_referinte > 50)
        scor += 2.0;
    else
        scor += 1.0;
    if (are_formule_diagrame)
        scor += 1.5;

    if (scorNivel(nivel_academic) == 6 || scorNivel(nivel_academic) == 5)
        scor += 4.5;
    else if (scorNivel(nivel_academic) == 4 || scorNivel(nivel_academic) == 3)
        scor += 3.5;
    else if (scorNivel(nivel_academic) == 2)
        scor += 2.5;
    else scor += 1.1;


    return scor;
}

bool CarteStiintifica::CompatibilaCuNivel(const std::string &nivel_studii) const {
    int nivelCititor = scorNivel(nivel_studii);
    int nivelCarte = scorNivel(nivel_academic);

    if (nivelCititor == 0 || nivelCarte == 0) {
        throw DateInvalideException("Nivel academic necunoscut!");
    }

    // Cititorul trebuie sa aiba nivel >= nivelul cartii
    return nivelCititor >= nivelCarte;
}

double CarteStiintifica::calculeazaPrioritateRestoc() const {
    return Carte::calculeazaPrioritateRestoc();
}

bool CarteStiintifica::areAutor(int idautor) const {
    return Carte::areAutor(idautor);
}


// operator<<
void CarteStiintifica::afisare(std::ostream &out) const {
    Carte::afisare(out);
    out << "  Domeniu: " << domeniu << "\n"
            << "  Nivel Academic: " << getTip() << "\n"
            << "  Impact: " << nr_referinte << " citari (Scor: " << calculeazaFactorImpact() << ")\n";

    if (!esteDeActualitate()) {
        out << "  [ATENTIE]: Continut posibil invechit!\n";
    }
}