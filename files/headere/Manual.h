

#ifndef OOP_MANUAL_H
#define OOP_MANUAL_H
#include  "Carte.h"
#include <string>
class Manual:public Carte {
private:
    std::string materie;
    int clasa;
protected:
    // operator <<
    void afisare (std::ostream& out) const override;
public:
    // constructor parametrii
    Manual(const std::string& titlu, const std::shared_ptr<Autor> &autor,double pret_baza,int cantitae,int an_publicare,
        const std::string& isbn,const int numar_pagini, const std::string& materie,int clasa);


    // functii

    std::string getCicluScolar()const;

    [[nodiscard]] int getclasa()const;

    [[nodiscard]] std::string  StareAprobare(int an_curent)const;

    [[nodiscard]] double calculeazaPrioritateRestoc(int an_curent, int luna_cureta) const;

    [[nodiscard]] double CalculeazaRelevanta(int clasa_elev,const std::vector<std::string>& materii_preferate,
        int an_curent) const;
    // functii virtuale
    [[nodiscard]] double calculeaza_valoarea_academica()  const override;

    [[nodiscard]] virtual int timp_estimat_lecturii() const override;

    [[nodiscard]] virtual std::string getTip() const override;

    [[nodiscard]] virtual double getPretFinal()const override;
};


#endif //OOP_MANUAL_H