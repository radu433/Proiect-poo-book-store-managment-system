#include "../headere/Manual.h"
#include "../headere/Carte.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include <string>
#include <set>
#include <ctime>
#include <sstream>
static const std::set<std::string> edituri_educationale_top = {
    "Didactica si Pedagogica",
    "Aramis",
    "Corint",
    "Art",
    "Paralela 45",
    "Booklet"
};

Manual::Manual(const std::string &titlu, const Autor *autor, double pret_baza, int cantitate,
    const std::string &data_publicatie, const std::string &isbn, const int numar_pagini, const std::string &editura,
    const std::string &materie, int clasa, int nr_vanzari, const std::vector<int> &ratinguri):
Carte(titlu, autor, cantitate, data_publicatie, isbn, pret_baza, numar_pagini, editura,nr_vanzari,ratinguri),materie(materie),clasa(clasa){
}

// constructor arametrii

Manual::Manual(const std::string &titlu, const  Autor* autor, double pret_baza, int cantitate,
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
        "Matematica", "Fizica", "Limba Romana", "Informatica", "Chimie","Limba Engleza"};
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


std::string Manual::getTip() const { return "Manual"; }

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

TipPublicatie Manual::getTipPub() const {
    return TipPublicatie::Manual;
}

static std::vector<std::string> split_manual(const std::string& s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::string Manual::serializare() const {
    std::stringstream ss;
    ss << Carte::serializare() << "|" << materie << "|" << clasa;
    return ss.str();
}

std::shared_ptr<Manual> Manual::dinString(const std::string &line, const std::vector<std::shared_ptr<Autor>> &autori) {
    const auto v = split_manual(line, '|');
    if (v.size() < 13) {
        throw LibrarieException("Linie invalida pentru deserializare Manual!");
    }
    std::string titlu = v[2];
    double pret = std::stod(v[3]);
    int cantitate = std::stoi(v[4]);
    std::string data = v[5];
    int pagini = std::stoi(v[6]);
    std::string editura = v[7];
    int vanzari = std::stoi(v[8]);
    std::vector<int> ratings;
    if (!v[9].empty()) {
        for (const auto r_str = split_manual(v[9], ','); const auto& s : r_str) {
             if(!s.empty()) ratings.push_back(std::stoi(s));
        }
    }

    const int idAutor = std::stoi(v[10]);
    std::string isbn = v[1];
    
    std::string materie = v[11];
    int clasa = std::stoi(v[12]);
    const Autor* ptrAutor = nullptr;
    for (const auto& a : autori) {
        if (a->getidAutor() == idAutor) {
            ptrAutor = a.get();
            break;
        }
    }

    if (!ptrAutor) {
        throw LibrarieException("Autorul ID " + std::to_string(idAutor) + " lipsa pt Manual!");
    }

    return std::make_shared<Manual>(titlu, ptrAutor, pret, cantitate, data, isbn, pagini, editura, materie, clasa, vanzari, ratings);
}



// operator <<
void Manual::afisare(std::ostream &out) const {
    Carte::afisare(out);
    out << "  [Detalii Manual]\n"
        << "  Materia:      " << materie << "\n"
        << "  Clasa:        " << clasa << "\n"
        << "  Stare:        " << StareAprobare() << "\n";
}
