#ifndef OOP_UNITATEVANZARE_H
#define OOP_UNITATEVANZARE_H

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include "Carte.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class UnitateVanzare {
private:
    const int id_unic;
    static int id_contor_global;

protected:
    std::shared_ptr<Carte> produs_principal;

    // Constructor protected (doar pentru derivate)
    explicit UnitateVanzare(std::shared_ptr<Carte> produs);

    // Operator= șters
    UnitateVanzare& operator=(const UnitateVanzare&) = delete;

    // Helper afișare NVI
    virtual void afisare(std::ostream& out) const;

    // Constructor de copiere protected
    UnitateVanzare(const UnitateVanzare& other);


public:
    // getter
    [[nodiscard]] std::shared_ptr<Carte> getProdusPrincipal() const;

    // Destructor virtual
    virtual ~UnitateVanzare() = default;

    // getteri
    [[nodiscard]] int getId() const;
    [[nodiscard]] std::string getNumeProdus() const;
    [[nodiscard]] int getCantitate() const; // Corectat typo (era getCantitae)

    // operator<<
    friend std::ostream& operator<<(std::ostream& out, const UnitateVanzare& obj);

    // functii virtuale

    // 1. Clone
    [[nodiscard]] virtual std::shared_ptr<UnitateVanzare> clone() const = 0;

    // Informații
    [[nodiscard]] virtual std::string getDescriere() const = 0;
    [[nodiscard]] virtual double getPretcomanda() const = 0;
    [[nodiscard]] virtual std::vector<std::string> getListaISBN() const = 0;

    [[nodiscard]] virtual bool verificaStocSuficient(int cantitate_ceruta) const = 0;
    virtual void scadeStoc( const int cantitate) = 0;

    //functii

    // Wrapper peste verificaStocSuficient(1)
    [[nodiscard]] bool valideazaDisponibilitate() const;

    // Helper pentru ISBN (doar primul ISBN
    [[nodiscard]] std::string getISBN() const;
};

#endif //OOP_UNITATEVANZARE_H