

#ifndef OOP_MANUAL_H
#define OOP_MANUAL_H
#include  "Carte.h"
#include <string>
class Manual:public Carte {
private:
    std::string materie;
    int clasa;

public:
    // cnstructor pt load
    Manual(const std::string& titlu, const Autor* autor,double pret_baza,int cantitate,const std::string& data_publicatie,
        const std::string& isbn,const int numar_pagini,const std::string& editura, const std::string& materie,int clasa,
        int nr_vanzari, const std::vector<int>& ratinguri);
    // constructor parametrii
    Manual(const std::string& titlu, const Autor* autor,double pret_baza,int cantitate,const std::string& data_publicatie,
        const std::string& isbn,const int numar_pagini,const std::string& editura, const std::string& materie,int clasa);

    // destructor
    ~Manual() override =default;

    // functii


    [[nodiscard]] std::string  StareAprobare() const;

    [[nodiscard]] double calculeazaPrioritateRestoc() const override;

    // functii virtuale
    [[nodiscard]] double calculeaza_valoarea_academica()  const override;

    [[nodiscard]]  std::string getTip() const override;

    [[nodiscard]]  double getPretFinal()const override;
    TipPublicatie getTipPub() const override;

    std::string serializare() const override;
    static std::shared_ptr<Manual> deserializare(const std::string& line, const std::vector<std::shared_ptr<Autor>>& autori);

    // operator <<
    void afisare (std::ostream& out) const override;


};


#endif //OOP_MANUAL_H