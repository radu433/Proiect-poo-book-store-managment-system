
#include "../headere/Carte.h"
#include "../headere/Autor.h"
#include <iostream>
#include <utility>
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
int Carte::total_carti_create = 0;
double Carte::venituri_totale = 0.0;
// constructor cu parametrii

Carte::Carte(const std::string& titlu, std::shared_ptr<Autor> autor, int cantitate, int an_publicare, const std::string & isbn,
    double pret_baza,const int numar_pagini) :
titlu(titlu), autor(std::move(autor)),cantitate(cantitate), numar_vanzari(0), an_publicare(an_publicare),isbn(isbn),
pret_baza(pret_baza),numar_pagini(numar_pagini) {
    if (pret_baza < 0) {
        throw DateInvalideException("Pretul de baza nu poate fi negativ!");
    }
    if (titlu.empty()) {
        throw DateInvalideException("Titlul cartii nu poate fi gol!");
    }
    if (cantitate < 0) {
        throw DateInvalideException("Stocul initial nu poate fi negativ!");
    }

    total_carti_create++;
    std::cout << "Carte creata: " << this->titlu << std::endl;
}

void Carte::afisare(std::ostream &out) const {
    out << "\n=== CARTE ===\n"
   << "Titlu: " << titlu << "\n"
   << "ISBN:"<<isbn<<"\n";
    if (autor){
        out << "Autor: " << autor->getNume() << " " << autor->getprenume() << "\n";
    } else {
        out << "Autor: Necunoscut\n";
    }

   out<< "Numar pagini:"<< numar_pagini<<"\n"
    << "Pret: " << pret_baza << " RON\n"
   << "Stoc: " << cantitate << "\n"
   << "Rating: " << calculeaza_raitingmin() << "/5\n"
   << "Vanzari: " << numar_vanzari << "\n"
   << "Status: " << (esteBestseller() ? "BESTSELLER" : "Standard") << "\n";

}

//operator<<
std::ostream& operator<<(std::ostream& out, const Carte& obj){
    obj.afisare(out);
    return out ;
};

// functii
[[nodiscard]] bool Carte::Scade_Stoc(int bucati_de_sters) {
    if (bucati_de_sters <= 0) return false;

    if (bucati_de_sters <= cantitate) {
        cantitate -= bucati_de_sters;
        numar_vanzari += bucati_de_sters;
        try {
            venituri_totale += bucati_de_sters * getPretFinal();
        } catch (...) {}

        return true;
    }
    return false;


}
double Carte::reducere(int discount) {
    if (discount < 0 || discount > 100) {
        throw DateInvalideException("Discount invalid (trebuie intre 0-100)!");
    }
    pret_baza *= (1.0 - (discount / 100.0));
    return pret_baza;
}

bool Carte::este_disponibila() const {
    return cantitate > 0;
}

double Carte::getpret_baza() const {return pret_baza;}

int Carte::getcantitate() const {return cantitate;}


const std::string & Carte::gettitlu() const {return titlu;}

bool Carte::esteBestseller() const{
    return numar_vanzari>=1000 && calculeaza_raitingmin()>=4.0;
}

void Carte::adauga_stoc(int nr_buc)  {
    if (nr_buc > 0)
        cantitate += nr_buc;
    else
        throw DateInvalideException("Nu poti adauga stoc negativ!");
}

void Carte::adauga_rating(int rating) {
    if (rating >= 1 && rating <= 5)
        rating_clienti.push_back(rating);
    else
        throw DateInvalideException("Rating trebuie sa fie intre 1 si 5!");
}

double Carte::calculeaza_raitingmin() const {
    if (rating_clienti.empty()) return 0;
    int suma = 0;
    for (const int r:rating_clienti) suma+=r;
    return static_cast<double>(suma)/rating_clienti.size();
}

int Carte::cumparaDupaPopularitate(const Carte &alta) const {
    double scor1=numar_vanzari*0.7+calculeaza_raitingmin()*100;
    double scor2=alta.numar_vanzari*0.7+alta.calculeaza_raitingmin()*100;
    if (scor1>scor2)
        return 1;
    else if (scor1<scor2)
        return -1;
    return 0;
}


int Carte::getAnPublicare() const {
    return an_publicare;
}

std::string Carte::getISBN() const {
    return isbn;
}







