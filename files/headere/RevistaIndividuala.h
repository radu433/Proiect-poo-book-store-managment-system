

#ifndef OOP_REVISTAINDIVIDUALA_H
#define OOP_REVISTAINDIVIDUALA_H
#include "UnitateVanzare.h"
#include"Revista.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
class RevistaIndividuala: public UnitateVanzare {
protected:
    void afisare(std::ostream& out) const override;


public:
    // constructor
    explicit RevistaIndividuala(const std::shared_ptr<Revista> &revista);
    // constructor de copiere
    RevistaIndividuala(const RevistaIndividuala& other);
    // clone
    std::shared_ptr<UnitateVanzare> clone() const override;
    //destructor
    ~RevistaIndividuala() override = default;

    // functii virtuale
    bool verificaStocSuficient(int cantitate_ceruta) const override;

    void scadeStoc(const int cantitate) override;

    double getPretcomanda() const override;

    std::vector<std::string> getListaIdentificatori() const override;

    std::string getDescriere() const override;

    [[nodiscard]]  std::string getIdentificator() const override;


    void marcheazaSecondHand(const std::string &conditie, int luni_vechime) override;
};
#endif //OOP_REVISTAINDIVIDUALA_H