

#ifndef OOP_REVISTA_H
#define OOP_REVISTA_H
#include "Carte.h"

class Revista : public Carte {
private:
    std::string frecventa;
    int numar_editie;
    bool are_cadou_suplimentar;
    std::string tip;
protected:
    void afisare(std::ostream &out) const override;
public:
    Revista(const std::string& titlu, const std::shared_ptr<Autor> &autor, int cantitate,
        int an_publicare,const  std::string& isbn,double pret_baza,const int numar_pagini,const std::string& frecventa,
        const int nr_editie,const bool are_cadou_suplimentar, const std::string& tip);
// functii virtuale

    [[nodiscard]] double getPretFinal() const override;

    [[nodiscard]] std::string getTip() const override;

    [[nodiscard]] double calculeaza_valoarea_academica() const override;

    [[nodiscard]] virtual int timp_estimat_lecturii() const override;

// functii
    bool esteEditieSpeciala() const;

    double calculeaza_frecventa_citire() const;

    bool este_colectionabila() const;

};

#endif //OOP_REVISTA_H