
#ifndef OOP_PUBLICATIE_H
#define OOP_PUBLICATIE_H
#include <ctime>
#include <memory>
#include <iostream>
#include <string>
#include <vector>
struct Data {
    int zi;
    int luna;
    int an;
    static Data parse(const std::string& s ) {
        return {
            std::stoi(s.substr(0,2)),
            std::stoi(s.substr(3,2)),
            std::stoi(s.substr(6,4))
        };
    }
    int toNumeric() const {
        return an * 10000 + luna * 100 + zi;
    }

};

class Publicatie {
private:
    static bool este_valida_data( const std::string& data);
protected:

    std::string titlu;
    double pret_baza;
    int cantitate;
    std::string data_publicatie;
    int nr_pagini;

    int numar_vanzari;
    std::vector<int> rating_clienti;
    std::string editura;
    bool activa=true;


    // membru static
    static int numar_total_publicatii;

    // constructor cu parametrii
    Publicatie(const std::string &titlu, double pret_baza, int cantitate, const std::string& data_publicatie, const int nr_pagini,
        const std::string& editura);

    int reducere_procent=0;
    std::time_t reducere_expirare=0;

public:

    // destructor virtual
    virtual ~Publicatie();

    //clone
    [[nodiscard]] virtual std::shared_ptr<Publicatie > clone() const=0;

    // oprerator <<
    virtual void afisare(std::ostream& out) const;

    friend std::ostream& operator<<(std::ostream& out, const Publicatie& obj);

    // operator =
    Publicatie& operator=(const Publicatie&)=delete;

    // functii

    std::time_t convertesteDatainSecunde() const;

    std::string determinaStatutStoc() const;


    // functii pt tote derivatele

    virtual void adauga_rating(int rating);

    [[nodiscard]] double getRatingMediu() const;

    [[nodiscard]] double CalculeazaScorPopularitate() const;

    [[nodiscard]]  virtual bool esteBesteller()const;

    virtual void adauga_stoc(int nr_bucati);



    // functii virtuale

    virtual std::string getTip() const =0;\

    virtual std::string getdata_publicatie()const  {return data_publicatie;}

    virtual std::string geteditura()const {return editura;}

    virtual int getnr_pagini()const {return nr_pagini;}

    [[nodiscard]] virtual double getPretFinal()const=0;

    [[nodiscard]] virtual double calculeaza_valoarea_academica() const=0;

    [[nodiscard]] virtual int timp_estimat_lecturii() const=0;

    virtual void seteazaReducere(int procent, int durata_zilei)=0;

    [[nodiscard]] virtual int getcantitate()const;

    [[nodiscard]] virtual double getpretbaza()const;

    virtual double calculeazaPrioritateRestoc() const;

    virtual std::string getIdentificator()const=0;

    virtual bool este_disponibila() const;

    //functii pt toate publicatiile

    virtual bool Scade_stoc(int bucati);

    void cresteCantitatea(int cantitate);

    virtual double reducere(int procent);

    virtual std::string getTitlu()const {
        return titlu;
    }
    virtual bool areAutor(int idautor)const;

    bool esteActiva() const {return activa;}
    void dezactiveaza() { activa = false; }
    void activeaza() { activa = true; }




};
#endif //OOP_PUBLICATIE_H