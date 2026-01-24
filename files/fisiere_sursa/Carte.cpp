#include "../headere/Carte.h"
#include "../headere/Autor.h"
#include <iostream>
#include <utility>
#include <ctime>
#include <set>
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../headere/CarteIndividuala.h"

static const std::set<std::string> editura_top = {"Polirom", "Humanitas", "Oxford", "Teora", "Nemira"};

int Carte::total_carti_create = 0;
double Carte::venituri_totale = 0.0;

bool Carte::validareisbn(const std::string &isbn_raw) {
    std::string isbn;
    for (char c: isbn_raw) {
        if (c != '-' && c != ' ') {
            isbn += c;
        }
    }


    if (isbn.length() != 10 && isbn.length() != 13) {
        return false;
    }

    // --- CAZUL ISBN-10 ---
    if (isbn.length() == 10) {
        int suma = 0;
        for (int i = 0; i < 9; i++) {
            if (!isdigit(isbn[i])) return false;
            suma += (isbn[i] - '0') * (10 - i);
        }

        char last = isbn[9];
        int lastDigit;
        if (last == 'X' || last == 'x') {
            lastDigit = 10;
        } else if (isdigit(last)) {
            lastDigit = last - '0';
        } else {
            return false;
        }

        suma += lastDigit;
        return (suma % 11 == 0);
    }

    // --- CAZUL ISBN-13 ---
    if (isbn.length() == 13) {
        int suma = 0;
        for (int i = 0; i < 12; i++) {
            if (!isdigit(isbn[i])) return false;
            int digit = isbn[i] - '0';

            if (i % 2 == 0) {
                suma += digit * 1;
            } else {
                suma += digit * 3;
            }
        }

        int rest = suma % 10;
        int checkDigit = (rest == 0) ? 0 : (10 - rest);

        if (!isdigit(isbn[12])) return false;
        return (isbn[12] - '0') == checkDigit;
    }

    return false;
}
#include <sstream>

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

// constructor pt load
Carte::Carte(const std::string &titlu, const Autor* autor, int cantitate, const std::string &data_publicatie,
             const std::string &isbn, double pret_baza, int numar_pagini, const std::string &editura,
             int nr_vanzari, const std::vector<int>& ratinguri)
    : Publicatie(titlu, pret_baza, cantitate, data_publicatie, numar_pagini, nr_vanzari, editura, ratinguri),
      autor(autor), isbn(isbn)
{
    total_carti_create++;
}

// constructor cu parametrii
Carte::Carte(const std::string &titlu,  const  Autor* autor, int cantitate, const std::string &data_publicatie,
             const std::string &isbn,
             double pret_baza, const int numar_pagini, const std::string &editura) : Publicatie(titlu, pret_baza,
    cantitate, data_publicatie, numar_pagini, editura), autor(autor), isbn(isbn) {
    if (pret_baza < 0) {
        throw DateInvalideException("Pretul de baza nu poate fi negativ!");
    }
    if (titlu.empty()) {
        throw DateInvalideException("Titlul cartii nu poate fi gol!");
    }
    if (cantitate < 0) {
        throw DateInvalideException("Stocul initial nu poate fi negativ!");
    }
    if (!validareisbn(isbn)) {
        throw DateInvalideException("ISBN invalid: " + isbn);
    }
    if (!autor) {
        throw DateInvalideException("Autor nullptr la creare Carte!");
    }
    const_cast<Autor*>(autor)->adauga_carte(isbn);
    if (editura.empty())
        throw DateInvalideException("Orice carte trebuie sa aiba o editura!");

    total_carti_create++;
    std::cout << "Carte creata: " << this->titlu << std::endl;
}

Carte::~Carte()=default;


void Carte::afisare(std::ostream &out) const {
    Publicatie::afisare(out);
    out << "Autor: "
            << autor->getNume() << " " << autor->getprenume() << "\n"
            << "ISBN: " << isbn << "\n"
            << "Bestseller: " << (esteBesteller() ? "DA" : "NU") << "\n";
}

//operator<<
std::ostream &operator<<(std::ostream &out, const Carte &obj) {
    obj.afisare(out);
    return out;
};

// functii

std::shared_ptr<Publicatie> Carte::clone() const {
    return std::make_shared < Carte > (*this);
}

Carte & Carte::operator=(const Carte &other) {
    if (this!=&other) {
        Publicatie::operator=(other);
        this->autor = other.autor;
        this->isbn = other.isbn;
        this->editura = other.editura;
        this->data_publicatie = other.data_publicatie;
    }

    return *this;
}


void Carte::adauga_stoc(const int nr_buc) {
    if (nr_buc >= 0)
        cantitate += nr_buc;
    else
        throw DateInvalideException("Nu poti adauga stoc negativ!");
}


int Carte::cumparaDupaPopularitate(const Carte &alta) const {
    const double scor1 = CalculeazaScorPopularitate();
    if (const double scor2 = alta.CalculeazaScorPopularitate(); scor1 > scor2)
        return 1;
    else if (scor1 < scor2)
        return -1;
    return 0;
}

bool Carte::este_disponibila() const {
    return getcantitate() > 0;
}

double Carte::calculeazaPrioritateRestoc() const {
    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

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
        prioritate += 20.0; // carte nou lansată
    } else if (vechime <= 20000) {
        prioritate += 10.0;
    } else if (vechime >= 150000) {
        prioritate *= 0.7; // carte foarte veche
    }
    static const std::set<std::string> edituri_top =
            {"Polirom", "Humanitas", "Oxford", "Teora", "Nemira"};

    if (edituri_top.contains(editura))
        prioritate += 10.0;

    return prioritate;
}

double Carte::getPretFinal() const {
    double pret = pret_baza;

    const std::time_t t = std::time(nullptr);
    const std::tm *tm_now = std::localtime(&t);

    const int data_actuala = (tm_now->tm_year + 1900) * 10000
                             + (tm_now->tm_mon + 1) * 100
                             + tm_now->tm_mday;

    const int data_apritie = Data::parse(data_publicatie).toNumeric();

    if (data_actuala - data_apritie < 10000)
        pret *= 1.20;
    if (data_actuala - data_apritie > 25000)
        pret *= 0.85;
    if (esteBesteller())
        pret *= 1.30;
    if (getRatingMediu() < 2)
        pret *= 0.7;


    return pret;
}

std::string Carte::getTip() const {
    return "Carte";
}

double Carte::calculeaza_valoarea_academica() const {
    double scor_final = static_cast<double>(nr_pagini) / 50;

    scor_final += getRatingMediu() * 10;

    if (esteBesteller())
        scor_final += 15;
    const std::time_t t = std::time(nullptr);
    const std::tm *now = std::localtime(&t);

    const long data_curenta = (now->tm_year + 1900) * 10000 + (now->tm_mon + 1) * 100 + now->tm_mday;

    if (const long data_pub = Data::parse(data_publicatie).toNumeric(); data_curenta - data_pub > 200000) {
        scor_final += 20.0;
    }
    if (editura_top.contains(editura)) {
        scor_final += 10;
    }
    return scor_final;
}

int Carte::timp_estimat_lecturii() const {
    return nr_pagini * 2;
}


void Carte::seteazaReducere(int procent, int durata_zilei) {
    reducere_procent = procent;
    reducere_expirare = std::time(nullptr) + durata_zilei * 86400;
}

std::string Carte::getIdentificator() const {
    return isbn;
}

const Autor *Carte::getAutor() const {
    return autor;
}

bool Carte::areAutor(int idautor) const {
    return autor && autor->getidAutor() == idautor;
}

TipPublicatie Carte::getTipPub() const {
    return TipPublicatie::Carte;
}

std::string Carte::serializare() const {
    std::stringstream ss;
    ss << Publicatie::serializare() << autor->getidAutor();
    return ss.str();
}

std::shared_ptr<Carte> Carte::dinString(const std::string &line, const std::vector<std::shared_ptr<Autor>> &autori) {
    const auto v = split(line, '|');
    if (v.size() < 11) {
        throw LibrarieException("Linie invalida pentru deserializare Carte (campuri insuficiente)!");
    }

    std::string titlu = v[2];
    double pret = std::stod(v[3]);
    int cantitate = std::stoi(v[4]);
    std::string data = v[5];
    int pagini = std::stoi(v[6]);
    std::string editura = v[7];
    int vanzari = std::stoi(v[8]);
    
    // Parsare ratinguri (v[9])
    std::vector<int> ratings;
    if (!v[9].empty()) {
        for (auto r_str = split(v[9], ','); const auto& s : r_str) {
            if(!s.empty()) ratings.push_back(std::stoi(s));
        }
    }

    const int idAutor = std::stoi(v[10]);

    // Cautam autorul in lista
    const Autor* ptrAutor = nullptr;
    for (const auto& a : autori) {
        if (a->getidAutor() == idAutor) {
            ptrAutor = a.get();
            break;
        }
    }

    if (!ptrAutor) {
        throw LibrarieException("Autorul cu ID " + std::to_string(idAutor) + " nu a fost gasit la deserializarea cartii!");
    }

    return std::make_shared<Carte>(titlu, ptrAutor, cantitate, data, v[1], pret, pagini, editura, vanzari, ratings);
}


