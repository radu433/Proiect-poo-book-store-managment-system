#ifndef OOP_UNITATEVANZARE_H
#define OOP_UNITATEVANZARE_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Publicatie.h"
#include "Revista.h"

#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class UnitateVanzare {
private:
    const int id_unic;
    static int id_contor_global;


protected:
    bool este_second_hand = false;
    std::string conditie_fizica = "Noua";
    time_t data_cumparare{};

    std::shared_ptr<Publicatie> produs_principal;

    // Constructor pt derivate de tip carte
    explicit UnitateVanzare(std::shared_ptr<Publicatie> produs);

    // constructor pt derivate de tip revista
    explicit UnitateVanzare(std::shared_ptr<Revista> revista);

    // Operator=
    UnitateVanzare& operator=(const UnitateVanzare&) = delete;

    // Helper afișare NVI
    virtual void afisare(std::ostream& out) const;

    // Constructor de copiere protected



public:
    UnitateVanzare(const UnitateVanzare& other);
    // getter
    [[nodiscard]] std::shared_ptr<Publicatie> getProdusPrincipal() const;

    // Destructor virtual
    virtual ~UnitateVanzare() = default;

    // getteri
    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getNumeProdus() const;
    [[nodiscard]] int getCantitate() const;

    // operator<<
    friend std::ostream& operator<<(std::ostream& out, const UnitateVanzare& obj);

    // functii virtuale

    // 1. Clone
    [[nodiscard]] virtual std::shared_ptr<UnitateVanzare> clone() const = 0;

    // Informații
    [[nodiscard]] virtual std::string getDescriere() const = 0;
    [[nodiscard]] virtual double getPretcomanda() const = 0;
    virtual  std::string getIdentificator() const=0;
    [[nodiscard]] virtual std::vector<std::string> getListaIdentificatori() const = 0;


    [[nodiscard]] virtual bool verificaStocSuficient(int cantitate_ceruta) const = 0;
    virtual void scadeStoc( const int cantitate) = 0;

    //functii
    [[nodiscard]] int calculeazaLuniDetinere() const;

    // Wrapper peste verificaStocSuficient(1)
    [[nodiscard]] bool valideazaDisponibilitate() const;
    std::string getTitlu() const {return produs_principal->getTitlu();}
    // Helper pentru ISBN (doar primul ISBN

    //functii t trade in
    virtual std::string getConditieFizica() const {return conditie_fizica;}
    virtual bool esteSecondHand() const {return este_second_hand; }
    virtual void marcheazaSecondHand(const std::string& conditie,int luni_vechime);

    // healpere pt data
    [[nodiscard]] int calculeazaZileDetinere() const;
    [[nodiscard]] std::string getDataCumparareFormatata() const;
};

#endif //OOP_UNITATEVANZARE_H