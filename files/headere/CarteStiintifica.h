

#ifndef OOP_CARTESTIINTIFICA_H
#define OOP_CARTESTIINTIFICA_H
#include "Carte.h"
#include <string>

class CarteStiintifia:public Carte {
private:
    std::string domeniu;
    std::string nivel_academic;
    int nr_referinte;
    bool are_formule_diagrame;
private:
    //operator <<
    void afisare (std::ostream& out) const override;
public:
    // constructor cu parametrii
    CarteStiintifia(const std::string& titlu, const std::shared_ptr<Autor> &autor, int cantitae,
    int an_publicare,const  std::string& isbn,double pret_baza, const int numar_pagini,const std::string& domeniu,const std::string& nivel_academic,
    int nr_referinte, const bool are_formule_diagrame);
    // functii virtuale

    [[nodiscard]] virtual int timp_estimat_lecturii() const override;

    [[nodiscard]] double getPretFinal() const override;

    [[nodiscard]] std::string getTip() const override;

    [[nodiscard]] double calculeaza_valoarea_academica() const override;

    // functii
    [[nodiscard]] const  std::string getDomeniu()const;

    [[nodiscard]] bool esteDeActualitate(int an_curent) const;


    [[nodiscard]] double calculeazaFactorImpact() const;

    [[nodiscard]] bool CopatibilaCuNivel(const std::string& nivel_studii) const;



};
#endif //OOP_CARTESTIINTIFICA_H