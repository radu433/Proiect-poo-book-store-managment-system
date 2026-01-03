#include "../headere/Revista.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

#include <set>
#include <ctime>

namespace {
    static const std::set<std::string> divertisment = {"Fashion", "Cancan", "Sport", "Lifestyle"};
    static const std::set<std::string> hobby = {"Auto", "IT Consumer", "Casa si Gradina"};
    static const std::set<std::string> profesional = {"Tehnologie", "Business", "Psihologie Practica"};
    static const std::set<std::string> academic = {"Medicala", "Stiintifica", "Juridica"};
    static const std::set<std::string> edituri_top = {"Polirom", "Humanitas", "Oxford", "Teora", "Nemira"};
}
    // constructor cu parametrii


Revista::Revista(const std::string &titlu, int cantitate, const std::string& data_publicatie,const int nr_pagini,
                double pret_baza,const std::string &frecventa,  int numar_editie,
                 const bool are_cadou_suplimentar, const std::string& tip, const std::string& ISSN, const std::string& editura) :
Publicatie(titlu,pret_baza,cantitate,data_publicatie,nr_pagini, editura),
    frecventa(frecventa), numar_editie(numar_editie),
    are_cadou_suplimentar(are_cadou_suplimentar), tip(tip),
    ISSN(ISSN) {
    if (numar_editie < 0) {
        throw DateInvalideException("Numarul editiei nu poate fi negativ!");
    }
    if (tip.empty()) {
        throw DateInvalideException("Tipul revistei nu poate fi gol!");
    }
    if (editura.empty()) {
        throw DateInvalideException("Revista trebuie sa apartina unei edituri!");
    }
    if (!verificareISSN(ISSN))
        throw DateInvalideException("ISSN-ul este introdus gresit!");
}
// clone
std::shared_ptr<Publicatie> Revista::clone() const {
    return std::make_shared<Revista>(*this);
}

bool Revista::verificareISSN(const std::string& ISSN) {
    if (ISSN.length()!=9 || ISSN[4]!='-')
        return false;
    int s=0;
    for (int i=0;i<7;i++) {
        const int index_c=(i<4)? i:i+1;
        if (!isdigit(ISSN[index_c]))
            return false;
        const int cif=ISSN[index_c]-'0';
        const int pondere=8-i;
        s+=cif*pondere;
    }
    char cif_control;
    if (s%11==0)
        cif_control='0';
      else {
          int v=11-s%11;
          if (v==10)
              cif_control='X';
          else {
              cif_control=v+'0';
          }
      }
    if (cif_control=='X') {
        return (ISSN[8]=='X'|| ISSN[8]=='x');
    }else {
        return ISSN[8]==cif_control;
    }
}

// afisare
void Revista::afisare(std::ostream &out) const {
   Publicatie::afisare(out);
        out<< "ISSN:          " << ISSN << "\n"
        << "Frecventa:     " << frecventa << "\n"
        << "Numar editie:  " << numar_editie << "\n"
        << "Cadou suplimentar:   " << (are_cadou_suplimentar ? "DA" : "NU") << "\n"
        << "Tip Revista:   " << tip << "\n";
}

// functii virtuale

double Revista::getPretFinal() const {
    double pret=pret_baza;
    if (divertisment.contains(tip))
        pret*=1.1;
    if (hobby.contains(tip))
        pret*=1.2;
    if (profesional.contains(tip))
        pret*=1.4;
    if (academic.contains(tip))
        pret*=1.5;
    if (are_cadou_suplimentar)
        pret+=10;
    if (frecventa == "lunara")
        pret *= 1.1;

    if (frecventa == "saptamanala")
        pret *= 0.95;
    if (editura == "Vogue" || editura == "Condé Nast" || editura == "Hearst") {
        pret *= 1.25;
    }
    if (editura == "Springer" || editura == "Nature Portfolio") {
        pret *= 1.50;
    }
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
    const std::time_t t=std::time(nullptr);
    const std::tm* tm_now=std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                   + (tm_now->tm_mon + 1) * 100
                   + tm_now->tm_mday;

    const int data_apritie =Data::parse(data_publicatie).toNumeric();

    if (data_actuala-data_apritie == 0) {
        valoare += 15.0;
    } else if (data_actuala-data_apritie == 10000) {
        valoare += 10.0;
    } else if (data_actuala-data_apritie <= 30000) {
        valoare += 5.0;
    }

    const double rating = getRatingMediu();
    valoare += rating * 2.0;

    if (esteBesteller()) {
        valoare += 5.0;
    }
    if (esteEditieSpeciala()) {
        valoare += 5.0;
    }
    if (edituri_top.contains(editura)) {
        valoare += 10.0;
    }

    return std::min(valoare, 100.0);

}

int Revista::timp_estimat_lecturii() const {
    if (tip=="Stiintifica")
        return nr_pagini*5;
    return nr_pagini;
}

void Revista::seteazaReducere(int procent, int durata_zilei) {
    reducere_procent=procent;
    reducere_expirare=std::time(nullptr)+durata_zilei*86400;
}

double Revista::calculeazaPrioritateRestoc() const {
    const std::time_t t = std::time(nullptr);
    const std::tm* tm_now = std::localtime(&t);

    const int data_actuala =
        (tm_now->tm_year + 1900) * 10000 +
        (tm_now->tm_mon + 1) * 100 +
        tm_now->tm_mday;

    const int data_aparitie = Data::parse(data_publicatie).toNumeric();

    double prioritate = 0.0;
    int stoc = getcantitate();

    if (stoc == 0)
        prioritate += 90.0;
    else if (stoc <= 5)
        prioritate += 70.0;
    else if (stoc <= 10)
        prioritate += 45.0;
    else if (stoc <= 25)
        prioritate += 20.0;

    if (esteBesteller())
        prioritate += 25.0;
    double rating = getRatingMediu();
    prioritate += rating * 4.0;

    prioritate += CalculeazaScorPopularitate() * 0.8;

    int vechime = data_actuala - data_aparitie;

    if (vechime == 0) {
        prioritate += 20.0;
    }
    else if (vechime <= 2000) {
        prioritate += 10.0;
    }
    else if (vechime >= 5000) {
        prioritate *= 0.7;
    }
    if (edituri_top.contains(editura))
        prioritate += 10.0;

    return prioritate;
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
    if (getRatingMediu()>=4.5 && numar_editie>=50)
        return true;

    return false;
}

std::string Revista::getIdentificator() const {
    return ISSN;
}




