#include "../headere/Manual.h"
#include "../headere/Carte.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include <string>
#include <set>
#include <ctime>
static const std::set<std::string> edituri_educationale_top = {
    "Didactica si Pedagogica",
    "Aramis",
    "Corint",
    "Art",
    "Paralela 45",
    "Booklet"
};

Manual::Manual(const std::string &titlu, const std::shared_ptr<Autor> &autor, double pret_baza, int cantitate,
               const std::string &data_publicatie,
               const std::string &isbn, const int numar_pagini, const std::string &editura, const std::string &materie,
               int clasa)
    : Carte(titlu, autor, cantitate, data_publicatie, isbn, pret_baza, numar_pagini, editura), materie(materie),
      clasa(clasa) {
    if (clasa < 0 || clasa > 13) {
        throw DateInvalideException(
            "Clasa manualului trebuie sa fie intre 0 (Pregatitoare) si 13. Ati introdus: " + std::to_string(clasa));
    }

    if (materie.empty()) {
        throw DateInvalideException("Materia manualului nu poate fi goala!");
    }
}


std::string Manual::getCicluScolar() const {
    if (clasa >= 0 && clasa <= 4)
        return "Primara";
    else if (clasa >= 5 && clasa <= 8)
        return "Generala";
    return "Liceal";
}

int Manual::getclasa() const { return clasa; }


std::string Manual::StareAprobare() const {\
    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                             + (tm_now->tm_mon + 1) * 100
                             + tm_now->tm_mday;

    const int data_apritie = Data::parse(data_publicatie).toNumeric();

    if (data_actuala - data_apritie < 0)
        return "Publicare Viitoare";
    else if (data_actuala - data_apritie == 0)
        return "NOU";
    if (data_actuala - data_apritie >= 10000 && data_actuala - data_apritie <= 20000)
        return "Aprobat";
    if (data_actuala - data_apritie >= 30000 && data_actuala - data_apritie <= 40000)
        return "Necesita actualizare";
    return "Expirat";
}

double Manual::calculeazaPrioritateRestoc() const {
    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                             + (tm_now->tm_mon + 1) * 100
                             + tm_now->tm_mday;

    const int data_apritie = Data::parse(data_publicatie).toNumeric();

    double prioritate = 0.0;
    int stoc = getcantitate();
    if (stoc == 0)
        prioritate += 100.0;
    else if (stoc <= 5)
        prioritate += 80.0;
    else if (stoc <= 10)
        prioritate += 50.0;
    else if (stoc <= 30)
        prioritate += 20.0;
    if (esteBesteller())
        prioritate += 20.0;

    //daca are rating bun
    double rating = getRatingMediu();
    prioritate += rating * 3.0;
    // daca e inceput de an scolar

    if (data_actuala / 100 % 100 >= 9 && data_actuala / 100 % 100 <= 11)
        prioritate *= 1.5;

    //dupa vechime


    if (data_actuala - data_apritie == 0) {
        prioritate += 30.0;
    } else if (data_actuala - data_apritie <= 20000) {
        prioritate += 15.0;
    }


    if (data_actuala - data_apritie >= 50000) {
        prioritate *= 0.5;
    }
    if (edituri_educationale_top.contains(editura))
        prioritate += 10.0;

    return prioritate;
}

double Manual::CalculeazaRelevanta(const int clasa_elev, const std::vector<std::string> &materii_preferate) const {
    if (clasa_elev < 0 || clasa_elev > 13) {
        throw DateInvalideException("Clasa elevului invalida!");
    }
    // potrivire clase
    int scor = 0.0;

    if (clasa == clasa_elev)
        scor += 50.0;
    else if (std::abs(clasa - clasa_elev) == 1)
        scor += 25.0;
    else
        return 0.0;

    // materia e in prefertinte

    for (const auto &materie_pref: materii_preferate) {
        if (materie == materie_pref) {
            scor += 30.0;
            break;
        }
    }
    // rating
    const double rating = getRatingMediu();
    scor += rating * 8.0;
    //actualitate
    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                             + (tm_now->tm_mon + 1) * 100
                             + tm_now->tm_mday;

    const int data_apritie = Data::parse(data_publicatie).toNumeric();
    if (data_actuala - data_apritie == 0) {
        scor += 15.0;
    } else if (data_actuala - data_apritie <= 2) {
        scor += 10.0;
    } else if (data_actuala - data_apritie <= 4) {
        scor += 5.0;
    }


    //  Disponibilitate
    if (cantitate > 0) {
        scor += 10.0;
    } else {
        scor *= 0.5;
    }

    // Bonus pentru bestseller
    if (esteBesteller()) {
        scor += 15.0;
    }
    if (edituri_educationale_top.contains(editura))
        scor += 10.0;

    return std::min(scor, 100);
}

double Manual::calculeaza_valoarea_academica() const {
    double valoare = 0.0;
    if (clasa >= 9) {
        valoare += 30.0;
    } else if (clasa >= 5) {
        valoare += 20.0;
    } else {
        valoare += 10.0;
    }
    const std::vector<std::string> materie_imp = {
        "Matematica", "Fizica", "Limba Romana", "Informatica", "Chimie""Limba Engleza"
    };
    bool este_materie_importanta = false;
    for (const auto &mat_imp: materie_imp) {
        if (materie.find(mat_imp) != std::string::npos) {
            este_materie_importanta = true;
            break;
        }
    }
    if (este_materie_importanta) {
        valoare += 15.0;
    } else {
        valoare += 10.0;
    }


    if (const std::string stare_aprobare = StareAprobare(); stare_aprobare == "NOU") {
        valoare += 25.0;
    } else if (stare_aprobare == "Aprobat," || stare_aprobare == " Necesita actualizare") {
        valoare += 15.0;
    } else {
        valoare += 5.0;
    }
    const double rating = getRatingMediu();
    valoare += rating * 2.0;
    if (esteBesteller())
        valoare += 5;
    if (edituri_educationale_top.contains(editura))
        valoare += 10.0;

    return std::min(valoare, 100.0);
}

int Manual::timp_estimat_lecturii() const {
    const int min_per_pagina = (clasa >= 9) ? 6 : 4;
    return nr_pagini * min_per_pagina;
}

std::string Manual::getTip() const { return materie; }

double Manual::getPretFinal() const {
    double pret = getpretbaza();
    pret *= 1.05;
    if (edituri_educationale_top.contains(editura))
        pret += 10.0;
    const std::string stare = StareAprobare();

    if (stare == "Necesita actualizare ")
        pret *= 0.8;
    if (stare == "Expirat")
        pret *= 0.5;
    return pret;
}

void Manual::seteazaReducere(int procent, int durata_zilei) {
    Carte::seteazaReducere(procent, durata_zilei);
}


// operator <<
void Manual::afisare(std::ostream &out) const {
    Carte::afisare(out);
    out << "Clasa:" << clasa << "\n";
    out << "Materie:" << materie << "\n"
            << "Stare:" << StareAprobare() << "\n";
}