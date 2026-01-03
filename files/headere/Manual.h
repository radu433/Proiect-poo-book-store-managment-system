

#ifndef OOP_MANUAL_H
#define OOP_MANUAL_H
#include  "Carte.h"
#include <string>
class Manual:public Carte {
private:
    std::string materie;
    int clasa;

public:
    // constructor parametrii
    Manual(const std::string& titlu, const std::shared_ptr<Autor> &autor,double pret_baza,int cantitate,const std::string& data_publicatie,
        const std::string& isbn,const int numar_pagini,const std::string& editura, const std::string& materie,int clasa);

    // destructor
    virtual ~Manual()=default;

    // functii

    std::string getCicluScolar()const;

    [[nodiscard]] int getclasa()const;

    [[nodiscard]] std::string  StareAprobare() const;

    [[nodiscard]] double calculeazaPrioritateRestoc() const override;

    [[nodiscard]] double CalculeazaRelevanta(int clasa_elev,const std::vector<std::string>& materii_preferate) const;
    std::string getmaterie()const{return materie;};
    // functii virtuale
    [[nodiscard]] double calculeaza_valoarea_academica()  const override;

    [[nodiscard]] virtual int timp_estimat_lecturii() const override;

    [[nodiscard]] virtual std::string getTip() const override;

    [[nodiscard]] virtual double getPretFinal()const override;

    virtual void seteazaReducere(int procent, int durata_zilei) override;

    bool areAutor(int idautor)const override;

    // operator <<
    void afisare (std::ostream& out) const override;
};


#endif //OOP_MANUAL_H