//
// Created by sindicat on 10/27/2025.
//

#ifndef OOP_CARTE_H
#define OOP_CARTE_H
#include <string>
#include "Autor.h"
#include <ostream>
#include <utility>
#include <vector>

class Carte {
private:
    std::string titlu;
    Autor autor;
    double pret;
    int cantitate;
    int an_publicare;
    int numar_vanzari;
    std::vector <int> rating_clienti;
    std::string isbn;
    double rating;
public:
    // constructor cu parametrii

    Carte(const std::string& titlu, const Autor& autor, double pret, int cantitate, int an_publicare);


    //constructor de copiere
    Carte(const Carte& obj);

    // operator de atribuire

    void swap(Carte& other) noexcept;

    Carte& operator=( Carte obj) noexcept;

    // opertorul <<
    friend std::ostream& operator<<(std::ostream& out, const Carte& obj);

    // destructor
    ~Carte();
// functii

     bool Scade_Stoc(int bucdesters);

    double reducere(int discont );

    [[nodiscard]] bool este_disponibila() const;

    [[nodiscard]] double getpret()const ;

    [[nodiscard]] double getstoc()const ;

    [[nodiscard]] double getrating()const ;

    [[nodiscard]] const std::string& gettitlu()const;

    [[nodiscard]] bool esteBestseller() const;

    void adauga_stoc(int nr_buc);

    void adauga_rating(int raiting);

    [[nodiscard]] double calculeaza_raitingmin()const;

    [[nodiscard]] int cumparaDupaPopularitate(const Carte& alta) const;



};
#endif //OOP_CARTE_H