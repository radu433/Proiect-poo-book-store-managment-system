#ifndef OOP_CARTE_H
#define OOP_CARTE_H
#include <string>
#include "Autor.h"
#include <ostream>
#include <utility>
#include <vector>
#include  <memory>
#include <ctime>

#include "Publicatie.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class Carte : public Publicatie {
private:
    std::shared_ptr<Autor> autor;
    std::string isbn;

    // atribute statice
    static int total_carti_create;
    static double venituri_totale;

    static bool vaidareisbn(const std::string &isbn);

public:
    // constructor cu parametrii

    Carte(const std::string &titlu, std::shared_ptr<Autor> autor, int cantitate, const std::string &data_publicatie,
          const std::string &isbn, double pret_baza, int numar_pagini, const std::string &editura);

    // destructor virtual
    virtual ~Carte() override;

    // opertorul << +afisare
    virtual void afisare(std::ostream &out) const override;

    friend std::ostream &operator<<(std::ostream &out, const Carte &obj);

    //clone
    [[nodiscard]] std::shared_ptr<Publicatie> clone() const override;


    // functii


    void adauga_stoc(int nr_buc) override;

    void adauga_rating(int rating) override;

    [[nodiscard]] int cumparaDupaPopularitate(const Carte &alta) const;

    bool este_disponibila() const override;


    virtual double calculeazaPrioritateRestoc() const override;


    // functii virtuale
    [[nodiscard]] double getPretFinal() const override;

    [[nodiscard]] std::string getTip() const override;

    [[nodiscard]] double calculeaza_valoarea_academica() const override;

    [[nodiscard]] int timp_estimat_lecturii() const override;

    [[nodiscard]] int getcantitate() const override;

    [[nodiscard]] double getpretbaza() const override;

    void seteazaReducere(int procent, int durata_zilei) override;

    std::string getIdentificator() const override;

    // gettere
    [[nodiscard]] const std::string &getISBN() const;

    [[nodiscard]] Autor &getAutor() const;

    bool areAutor(int idautor) const override;
};
#endif //OOP_CARTE_H