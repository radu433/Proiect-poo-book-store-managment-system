#ifndef OOP_CARTESTIINTIFICA_H
#define OOP_CARTESTIINTIFICA_H
#include "Carte.h"
#include <string>

class CarteStiintifica : public Carte {
private:
    std::string domeniu;
    std::string nivel_academic;
    int nr_referinte;
    bool are_formule_diagrame;

public:
    // constructor cu parametrii
    CarteStiintifica(const std::string &titlu, const std::shared_ptr<Autor> &autor, int cantitate,
                     const std::string &data_publicatie, const std::string &isbn, double pret_baza,
                     const int numar_pagini, const std::string &editura, const std::string &domeniu,
                     const std::string &nivel_academic,
                     int nr_referinte, const bool are_formule_diagrame);

    // functii virtuale

    [[nodiscard]] virtual int timp_estimat_lecturii() const override;

    [[nodiscard]] double getPretFinal() const override;

    [[nodiscard]] std::string getTip() const override;

    [[nodiscard]] double calculeaza_valoarea_academica() const override;

    virtual void seteazaReducere(int procent, int durata_zilei) override;

    // functii
    [[nodiscard]] const std::string &getDomeniu() const;

    [[nodiscard]] bool esteDeActualitate() const;


    [[nodiscard]] double calculeazaFactorImpact() const;

    [[nodiscard]] bool CompatibilaCuNivel(const std::string &nivel_studii) const;

    [[nodiscard]] double calculeazaPrioritateRestoc() const override;

    bool areAutor(int idautor)const override;

    //operator <<
    void afisare(std::ostream &out) const override;
};
#endif //OOP_CARTESTIINTIFICA_H