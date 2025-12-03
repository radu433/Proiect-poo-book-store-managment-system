#include "../headere/CarteStiintifica.h"
#include <string>
#include "../exceptii/exceptii_headere/DateInvalideException.h"

// constructor cu parametrii
CarteStiintifia::CarteStiintifia(const std::string &titlu, const std::shared_ptr<Autor> &autor, int cantitate, int an_publicare,
    const std::string &isbn, double pret_baza,const int numar_pagini,const std::string &domeniu, const std::string &nivel_academic,
    int nr_referinte, const bool are_formule_diagrame): Carte(titlu,autor,cantitate,an_publicare,isbn,pret_baza,numar_pagini),
domeniu(domeniu),nivel_academic(nivel_academic),nr_referinte(nr_referinte),are_formule_diagrame(are_formule_diagrame){
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

int CarteStiintifia::timp_estimat_lecturii() const {
    int minute_per_pagina=8;
    if (nivel_academic=="Cercetare" || nivel_academic=="Doctorat")
        minute_per_pagina=15;
    return numar_pagini*minute_per_pagina;
}

double CarteStiintifia::getPretFinal() const {
    double pret= pret_baza;
    const double impact=calculeazaFactorImpact();
    pret*=1.3;
    if (impact>=5 && impact<=10)
        pret+=50;
    else pret+=10;
    return pret;
}

std::string CarteStiintifia::getTip() const { return nivel_academic; }


 const std::string& CarteStiintifia::getDomeniu() const {return domeniu;}

bool CarteStiintifia::esteDeActualitate(const int an_curent) const {
    int vechime = an_curent - an_publicare;

    if (domeniu == "Informatica" || domeniu == "Medicina" || domeniu == "Robotica") {
        return vechime <= 5;
    }

    return vechime <= 15;

}


double CarteStiintifia::calculeaza_valoarea_academica() const {
double valoare=0.0;

    if (nivel_academic=="Cercetare" || nivel_academic=="Doctorat")
        valoare+=40;
    else if (nivel_academic=="Master" || nivel_academic=="Facultate")
        valoare+=30;
    else if (nivel_academic=="Liceu")
        valoare+=15;
    else valoare+=10;

    double impact=calculeazaFactorImpact();
    valoare+=(impact*3.0);

    int an_curent=2025;
    int vechime=an_curent-getAnPublicare();

if (domeniu=="IT" || domeniu=="Medicina") {
        if (vechime<=2) valoare+=20;
        else if (vechime<=5) valoare+=10;
        else
            valoare-=10;
    } else {
        if (vechime<=5) valoare+=20;
        else if (vechime<=15) valoare+=10;
        else
            valoare+=5;
    }
    return std::min(valoare,100.0);

}

double CarteStiintifia::calculeazaFactorImpact() const {
    double scor=0.0;

    if (nr_referinte>100)
        scor+=3.0;
    else if (nr_referinte>50 )
        scor+=2.0;
    else
        scor+=1.0;
    if (are_formule_diagrame)
        scor+=1.5;

    if (nivel_academic=="Cercetare" || nivel_academic=="Doctorat")
        scor+=4.5;
    else if (nivel_academic=="Master" || nivel_academic=="Facultate")
        scor+=3.5;
    else if (nivel_academic=="Liceu")
        scor+=2.5;
    else scor+=1.1;

    if (are_formule_diagrame)
        scor+=1.0;

    return scor;
}

bool CarteStiintifia::CopatibilaCuNivel(const std::string& nivel_studii) const {
    // Verifică dacă cartea e potrivită pentru nivelul de studii dat

    if (nivel_studii == "liceu") {
        return nivel_academic == "liceu" || nivel_academic == "popularizare";
    } else if (nivel_studii == "universitar") {
        return nivel_academic == "universitar" ||
               nivel_academic == "liceu";  // Poate fi și liceu pentru bază
    } else if (nivel_studii == "masterat" || nivel_studii == "doctorat") {
        return nivel_academic == "cercetare" ||
               nivel_academic == "universitar";
    } else if (nivel_studii == "general") {
        return nivel_academic == "popularizare";
    }

    return false;
}

// operator<<
 void CarteStiintifia::afisare(std::ostream &out) const {
     Carte::afisare(out);
    out << "  Domeniu: " << domeniu << "\n"
        << "  Nivel Academic: " << getTip() << "\n"
        << "  Impact: " << nr_referinte << " citari (Scor: " << calculeazaFactorImpact() << ")\n";

    if (!esteDeActualitate(2025)) {
        out << "  [ATENTIE]: Continut posibil invechit!\n";
    }
}




