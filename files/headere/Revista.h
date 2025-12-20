

#ifndef OOP_REVISTA_H
#define OOP_REVISTA_H
#include "Carte.h"
#include "Publicatie.h"

class Revista : public Publicatie {
private:
    std::string frecventa;
    int numar_editie;
    bool are_cadou_suplimentar;
    std::string tip;
    std::string ISSN;
    static bool verificareISSN( const std::string &ISSN);
protected:
    void afisare(std::ostream &out) const override;
public:
    Revista(const std::string& titlu,  int cantitate,
         const std::string& data_publicatie,const int nr_pagini,double pret_baza,const std::string& frecventa,
        const int nr_editie,const bool are_cadou_suplimentar, const std::string& tip, const std::string& ISSN, const std::string& editura);


//clone
    [[nodiscard]] std::shared_ptr<Publicatie> clone() const override;

// destructor virtual
    ~Revista() override=default;

// functii virtuale

    [[nodiscard]] double getPretFinal() const override;

    [[nodiscard]] std::string getTip() const override;

    [[nodiscard]] double calculeaza_valoarea_academica() const override;

    [[nodiscard]] virtual int timp_estimat_lecturii() const override;

    std::string getFrecventa()const{return frecventa;}

    virtual void seteazaReducere(int procent, int durata_zilei) override;

 double calculeazaPrioritateRestoc() const override;

// functii
    bool esteEditieSpeciala() const;

    double calculeaza_frecventa_citire() const;

    bool este_colectionabila() const;

  std::string getIdentificator() const override;

};

#endif //OOP_REVISTA_H