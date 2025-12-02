#include "../headere/Manual.h"
#include "../headere/Carte.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include <string>

Manual::Manual(const std::string &titlu,const std::shared_ptr<Autor> &autor, double pret_baza, int cantitae, int an_publicare,
    const std::string &isbn,const int numar_pagini,const std::string &materie, int clasa)
    : Carte(titlu, autor, cantitae, an_publicare, isbn, pret_baza, numar_pagini), materie(materie),
      clasa(clasa) {
    if (clasa < 0 || clasa > 13) {
        throw DateInvalideException("Clasa manualului trebuie sa fie intre 0 (Pregatitoare) si 13. Ati introdus: " + std::to_string(clasa));
    }

    if (materie.empty()) {
        throw DateInvalideException("Materia manualului nu poate fi goala!");
    }
}


std::string Manual::getCicluScolar() const {
    if (clasa>=0 &&clasa<=4)
        return "Primara";
    else if (clasa>=5 &&clasa<=8)
        return "Generala";
    return "Liceal";
}

int Manual::getclasa() const {return clasa;}


std::string Manual::StareAprobare(int an_curent) const {\
    int vechime=an_curent-an_publicare;
        if (vechime<0)
            return "Publicare Viitoare";
        else if (vechime==0)
            return "NOU";
    if (vechime>=1 && vechime<=2)
        return "Aprobat";
    if (vechime>=3 && vechime<=4)
        return "Necesita actualizare";
    return "Expirat";
}

double Manual::calculeazaPrioritateRestoc(int an_curent, int luna_curenta) const {
    if (luna_curenta < 1 || luna_curenta > 12) {
        throw DateInvalideException("Luna invalida pentru calcul stoc: " + std::to_string(luna_curenta));
    }
    double prioritate=0.0;
    int stoc=getcantitate();
    if (stoc==0)
        prioritate+=100.0;
    else if (stoc<=5)
        prioritate+=80.0;
    else if (stoc<=10)
        prioritate+=50.0;
    else if (stoc<=30)
        prioritate+=20.0;
    if (esteBestseller())
        prioritate+=20.0;
    //daca are rating bun
    double rating=calculeaza_raitingmin();
        prioritate+=rating*3.0;
    // daca e inceput de an scolar
    if (luna_curenta>=9 && luna_curenta<=10)
        prioritate*=1.5;
    //dupa vechime
    int vechime=an_curent-an_publicare;

    if (vechime == 0) {
        prioritate += 30.0;
    } else if (vechime <= 2) {
        prioritate += 15.0;
    }


    if (vechime >= 5) {
        prioritate *= 0.5;
    }

    return prioritate;
}

double Manual::CalculeazaRelevanta(int clasa_elev, const std::vector<std::string> &materii_preferate,
    int an_curent) const {
    if (clasa_elev < 0 || clasa_elev > 13) {
        throw DateInvalideException("Clasa elevului invalida!");
    }
// potrivire clase
    int scor=0.0;

    if (clasa==clasa_elev)
        scor+=50.0;
    else if (std::abs(clasa-clasa_elev)==1)
        scor+=25.0;
    else
        return 0.0;

// materia e in prefertinte

    for (const auto& materie_pref : materii_preferate) {
        if (materie == materie_pref) {
            scor += 30.0;
            break;
        }
    }
// rating
    const double rating=calculeaza_raitingmin();
    scor+=rating*8.0;
//actualitate
    int vechime = an_curent - an_publicare;
    if (vechime == 0) {
        scor += 15.0;
    } else if (vechime <= 2) {
        scor += 10.0;
    } else if (vechime <= 4) {
        scor += 5.0;
    }


    //  Disponibilitate
    if (este_disponibila()) {
        scor += 10.0;
    } else {
        scor *= 0.5;
    }

    // Bonus pentru bestseller
    if (esteBestseller()) {
        scor += 15.0;
    }

    return scor;
}

double Manual::calculeaza_valoarea_academica() const {
    double valoare=0.0;
    if (clasa>=9) {
        valoare+=30.0;
    } else if (clasa>=5) {
        valoare+=20.0;
    }else {
        valoare+=10.0;
    }
    const std::vector<std::string> materie_imp={
        "Matematica","Fizica", "Limba Romana","Informatica","Chimie""Limba Engleza"
    };
    bool este_materie_importanta=false;
    for (const auto& mat_imp: materie_imp) {
        if (materie.find(mat_imp)!=std::string::npos) {
            este_materie_importanta=true;
            break;
        }
    }
    if (este_materie_importanta) {
        valoare += 15.0;
    } else {
        valoare += 10.0;
    }

    int an_curent=2025;

    if (const std::string stare_aprobare=StareAprobare(an_curent); stare_aprobare == "NOU") {
        valoare += 25.0;
    } else if (stare_aprobare == "Aprobat,"|| stare_aprobare==" Necesita actualizare") {
        valoare += 15.0;
    } else {
        valoare += 5.0;
    }
    const double rating=calculeaza_raitingmin();
    valoare+=rating*2.0;
    if(esteBestseller())
        valoare+=5;
    return std::min(valoare,100.0);
}

int Manual::timp_estimat_lecturii() const {
    const int min_per_pagina=(clasa>=9)? 6:4;
    return numar_pagini*min_per_pagina;
}

std::string Manual::getTip() const { return materie;}

double Manual::getPretFinal() const {
    double pret= getpret_baza();
    pret*=1.05;
    const std::string stare=StareAprobare(2025);

    if (stare=="Necesita actualizare ")
        pret*=0.8;
    if (stare=="Expirat")
        pret*=0.5;
    return pret;
}


// operator <<
void Manual::afisare(std::ostream& out) const {
    Carte::afisare(out);
    out<<"Tip:"<<getTip()<<"\n"<<"Clasa:"<<clasa<<"\n";
    out<<"Materie:"<< materie<<"\n"
    << "Stare:"<<StareAprobare(2025)<<"\n";
}

