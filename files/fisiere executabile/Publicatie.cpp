#include "../headere/Publicatie.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/PaginareException.h"
#include "../exceptii/exceptii_headere/PublicatieException.h"

#include <iomanip>
#include <numeric>
#include <ctime>
#include <string>
#include <cstdio>

int Publicatie::numar_total_publicatii=0;


bool Publicatie::este_valida_data(const std::string& data) {
    int zi, luna, an;

    if (sscanf(data.c_str(), "%d.%d.%d", &zi, &luna, &an) != 3)
        return false;

    if (an < 1500 || an > 2100)
        return false;

    if (luna < 1 || luna > 12)
        return false;

    if (zi < 1 || zi > 31)
        return false;

    // februarie
    if (luna == 2) {
        bool an_bisect = (an % 4 == 0 && an % 100 != 0) || (an % 400 == 0);
        if (zi > (an_bisect ? 29 : 28))
            return false;

    }
    // luni cu 30 de zile
    if ((luna == 4 || luna == 6 || luna == 9 || luna == 11) && zi > 30)
        return false;

    return true;
}

// constructor
Publicatie::Publicatie( const std::string &titlu,  double pret_baza, int cantitate, const std::string &data_publicatie  ,
    const int nr_pagini, const std::string& editura) : titlu(
                                                           titlu), pret_baza(pret_baza), cantitate(cantitate),
                                                       data_publicatie(data_publicatie), nr_pagini(nr_pagini),
                                                       numar_vanzari(0), editura(editura){
    if (nr_pagini <= 0) {
        throw NumarPaginiInvalidException(nr_pagini);
    }

    if (nr_pagini > 10000) {
        throw PaginareException("Numar pagini nerealist: " + std::to_string(nr_pagini));
    }
    if (titlu.empty()) {
        throw TitluInvalidException("Titlul nu poate fi gol!");
    }

    if (titlu.length() > 200) {
        throw TitluInvalidException("Titlu prea lung (max 200 caractere)");
    }
    if (!este_valida_data(data_publicatie))
        throw DateInvalideException("Data invalida: " + data_publicatie + " (format corect: ZZ.LL.AAAA)");

    if (editura.empty())
        throw DateInvalideException("O publicatie trebuie sa aiba o editura!");
    numar_total_publicatii++;
}

// destructor
Publicatie::~Publicatie() {
numar_total_publicatii--;
}

// afisare
void Publicatie::afisare(std::ostream &out) const {
    const std::string tip=getTip();
    out <<"\n===========================\n"<<
        "TIP: "<< tip<<"\n"<<
            "\n===========================\n"<<"Titlu:        " << titlu << "\n"
        <<"Editura:"<<editura<<"\n"
        << "Data Publicare: " << data_publicatie << "\n"
        << "Nr. Pagini:   " << nr_pagini << "\n"
        << "Pret Baza:    " << std::fixed << std::setprecision(2) << pret_baza << " RON\n"
        << "Stoc:         " << cantitate << " buc.\n"
    << "Rating mediu: " << getRatingMediu() << "/5\n";
}

double Publicatie::CalculeazaScorPopularitate() const {
    double media_rating = 0.0;
    if (!rating_clienti.empty()) {
        const double suma = std::accumulate(rating_clienti.begin(), rating_clienti.end(), 0.0);
        media_rating = suma / rating_clienti.size();
    }
    double scor_rating = media_rating;


    double puncte_vanzare = (numar_vanzari / 100.0) * 40.0;
    if (puncte_vanzare > 40.0) puncte_vanzare = 40.0;


    std::time_t now = std::time(nullptr);
    std::time_t pub_time = convertesteDatainSecunde();


    double secunde_vechime = std::difftime(now, pub_time);
    double puncte_vechime = 0.0;


    double zile_vechime = secunde_vechime / (60 * 60 * 24);


    if (getTip() == "Revista") {

        if (zile_vechime < 30) {
            puncte_vechime = 20.0;
        } else if (zile_vechime < 60) {
            puncte_vechime = 10.0;
        }
    } else {

        if (zile_vechime < 365) {
            puncte_vechime = 10.0;
        } else {
            puncte_vechime = 5.0;
        }
    }

    return scor_rating + puncte_vanzare + puncte_vechime;
}

bool Publicatie::esteBesteller() const {
    return numar_vanzari>=1000 && getRatingMediu()>=4.0;
}

void Publicatie::adauga_stoc(int nr_buc) {
    if (nr_buc <= 0)
        throw DateInvalideException("Cantitate invalida!");

    cantitate += nr_buc;
}


int Publicatie::getcantitate() const {
    return cantitate;
}

double Publicatie::getpretbaza() const {
    return pret_baza;
}

bool Publicatie::Scade_stoc(int bucati) {
    if (bucati <= 0) {
        throw DateInvalideException("Numarul de bucati trebuie sa fie pozitiv!");
    }

    if (bucati > cantitate) {
        return false; // nu exista destul stoc
    }

    cantitate -= bucati;
    numar_vanzari += bucati;

    if (cantitate < 0)
        throw DateInvalideException("Eroare interna: cantitate negativa!");

    return true;
}

void Publicatie::cresteCantitatea(int cantitate) {
    if (cantitate<= 0)
        throw DateInvalideException("Cantitate invalida!");
    this->cantitate+=cantitate;
}

double Publicatie::reducere(int procent) {
    if (procent < 0 || procent > 100) {
        throw DateInvalideException("Discount invalid (0–100%)!");
    }

    pret_baza *= (1.0 - procent / 100.0);
    return pret_baza;
}

std::time_t Publicatie::convertesteDatainSecunde() const {
    std::tm tm_pub = {};
    int zi, luna, an;


  sscanf(data_publicatie.c_str(), "%d.%d.%d", &zi, &luna, &an);

    tm_pub.tm_year = an - 1900;
    tm_pub.tm_mon = luna - 1;
    tm_pub.tm_mday = zi;
    tm_pub.tm_isdst = -1;

    return std::mktime(&tm_pub);
}

std::string Publicatie::determinaStatutStoc() const {
    if (cantitate == 0) {
        return "EPUIZAT";
    } else if (cantitate <= 5) {
        return "STOC CRITIC";
    } else if (cantitate <= 20) {
        return "STOC REDUS";
    } else if (cantitate <= 50) {
        return "DISPONIBIL";
    } else {
        return "STOC ABUNDENT";
    }
}

void Publicatie::adauga_rating(int rating) {
    if (rating<1 || rating>5) {
        throw DateInvalideException("Eroare interna: rating invalid!");
    }
    rating_clienti.push_back(rating);
}

double Publicatie::getRatingMediu() const {
    if (rating_clienti.empty()) {
        return 0.0;
    }

    const double suma = std::accumulate(rating_clienti.begin(), rating_clienti.end(), 0.0);
    return static_cast<double>(suma) / rating_clienti.size();
}

std::ostream& operator<<(std::ostream& out, const Publicatie& obj) {
    obj.afisare(out);
    return out ;
}


