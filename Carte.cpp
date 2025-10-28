//
// Created by sindicat on 10/28/2025.
//
#include "Carte.h"
#include <iostream>
#include <utility>
// constructor cu parametrii


Carte::Carte(const std::string& titlu, const Autor& autor, double pret, int cantitate, int an_publicare)
    : titlu(titlu), autor(autor), pret(pret),
      cantitate(cantitate), an_publicare(an_publicare), numar_vanzari(0), rating(0)
      {
    std::cout << "Carte creată: " << titlu << std::endl;
}

//constructor de copiere
    Carte::Carte(const Carte& obj):titlu(obj.titlu),autor(obj.autor),pret(obj.pret),
cantitate(obj.cantitate),an_publicare(obj.an_publicare), numar_vanzari(obj.numar_vanzari),rating(0){};
// operator de atribuire
    void Carte:: swap (Carte& other) noexcept {
            using std::swap; // Permite ADL (Argument-Dependent Lookup)
            swap(titlu, other.titlu);
            swap(autor, other.autor);
            swap(rating, other.rating);
            swap(pret, other.pret);
            swap(cantitate, other.cantitate);
            swap(an_publicare, other.an_publicare);
            swap(numar_vanzari, other.numar_vanzari);


    }
Carte& Carte::operator=(Carte obj) noexcept{
        swap(obj);
        return *this;
    }
// operatorul <<
std::ostream& operator<<(std::ostream& out, const Carte& obj){
        out << "\n=== CARTE ===\n"
       << "Titlu: " << obj.titlu << "\n"
       << obj.autor << "\n"
       << "Pret: " << obj.pret << " RON\n"
       << "Stoc: " << obj.cantitate << "\n"
       << "Rating: " << obj.calculeaza_raitingmin() << "/5\n"
       << "Vanzari: " << obj.numar_vanzari << "\n"
       << "Status: " << (obj.esteBestseller() ? "BESTSELLER" : "Standard") << "\n";
        return out;
    };
// destructor
Carte::~Carte() {
    std::cout<<"destructor:"<< titlu<<std::endl;
}

// functii
[[nodiscard]] bool Carte::Scade_Stoc(int bucdesters) {
    if (bucdesters<=0)
        return false;
    if (bucdesters<=cantitate) {
        cantitate-=bucdesters;
        return true;
    }

    return true;
}
double Carte::reducere(int discount) {
    pret*=(1-(discount/100.0));
    return pret;
}

bool Carte::este_disponibila() const {
    return cantitate > 0;
}

double Carte::getpret() const {return pret;}

double Carte::getstoc() const {return cantitate;}

double Carte::getrating() const { return rating;}


const std::string & Carte::gettitlu() const {return titlu;}

bool Carte::esteBestseller() const{
    return numar_vanzari>=1000 && calculeaza_raitingmin()>=4.0;
}

void Carte::adauga_stoc(int nr_buc)  {
    if (nr_buc>0)
        cantitate+=nr_buc;
}

double Carte::calculeaza_raitingmin() const {
    if (rating_clienti.empty()) return 0;
    int suma = 0;
    for (int r:rating_clienti) suma+=r;
    return static_cast<double>(suma)/rating_clienti.size();
}

int Carte::cumparaDupaPopularitate(const Carte &alta) const {
    double scor1=numar_vanzari*0.7+calculeaza_raitingmin()*100;
    double scor2=alta.numar_vanzari*0.7+calculeaza_raitingmin()*100;
    if (scor1>scor2)
        return 1;
    else if (scor1<scor2)
        return -1;
    return 0;
}



void Carte::adauga_rating(int raiting) {
    if (raiting>=1 && raiting<=5)
        rating_clienti.push_back(raiting);
}






