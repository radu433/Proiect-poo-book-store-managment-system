

#ifndef OOP_CARTE_H
#define OOP_CARTE_H
#include <string>
#include "Autor.h"
#include <ostream>
#include <utility>
#include <vector>
#include  <memory>
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class Carte {
private:
    std::string titlu;
    std::shared_ptr<Autor> autor;
    int cantitate;
    int numar_vanzari;
    std::vector <int> rating_clienti;

    // atribute statice
    static int total_carti_create;
    static double venituri_totale;

protected:
    int an_publicare;
    std::string isbn;
    double pret_baza;
    int numar_pagini;

    // constructor cu parametrii

    Carte(const std::string& titlu, std::shared_ptr<Autor> autor, int cantitate, int an_publicare,
       const  std::string& isbn,double pret_baza, const int numar_pagini);
 // opertorul <<
    virtual void afisare(std::ostream& out)const;
public:

    friend std::ostream& operator<<(std::ostream& out, const Carte& obj);
    //operator =
    Carte& operator=(const Carte&)=delete;

    // constructor de copiere
    Carte(const Carte&)=delete;

    // destructor virtual
    virtual ~Carte()=default;
    // functii

    bool Scade_Stoc(int bucati_de_sters);

    double reducere(int discont );

    [[nodiscard]] bool este_disponibila() const;

    [[nodiscard]] double getpret_baza()const ;

    [[nodiscard]] int getcantitate()const ;

    [[nodiscard]] const std::string& gettitlu()const;

    [[nodiscard]] bool esteBestseller() const;

    void adauga_stoc(int nr_buc);

    void adauga_rating(int rating);

    [[nodiscard]] double calculeaza_raitingmin()const;

    [[nodiscard]] int cumparaDupaPopularitate(const Carte& alta) const;

    int getAnPublicare() const;

    [[nodiscard]] std::string getISBN()const;

// functii virtuale
    [[nodiscard]] virtual double getPretFinal()const=0;

    [[nodiscard]] virtual std::string getTip() const =0;

    [[nodiscard]] virtual double calculeaza_valoarea_academica() const=0;

    [[nodiscard]] virtual int timp_estimat_lecturii() const=0;
};
#endif //OOP_CARTE_H