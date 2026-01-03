#ifndef OOP_CARTEINDIVIDUALA_H
#define OOP_CARTEINDIVIDUALA_H
#include "UnitateVanzare.h"
#include <ctime>
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class CarteIndividuala : public UnitateVanzare {

protected:
    void afisare(std::ostream &out) const override;

public:
    // constructor cu parametrii
    CarteIndividuala(std::shared_ptr<Carte> carte);

    // constructor de copiere
    CarteIndividuala(const CarteIndividuala &other);

    // clone
    std::shared_ptr<UnitateVanzare> clone() const override;

    // destructor
    ~CarteIndividuala() override = default;

    // functii virtuale
    [[nodiscard]] bool verificaStocSuficient(int cantitate_ceruta) const override;

    double getPretcomanda() const override;

    std::vector<std::string> getListaIdentificatori() const override;

    std::string getDescriere() const override;

    void scadeStoc(const int cantitate) override;

    std::string getIdentificator() const override;

};

#endif //OOP_CARTEINDIVIDUALA_H