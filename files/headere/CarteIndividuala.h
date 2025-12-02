#ifndef OOP_CARTEINDIVIDUALA_H
#define OOP_CARTEINDIVIDUALA_H
#include "UnitateVanzare.h"
#include <ctime>
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class CarteIndividuala  : public UnitateVanzare {
private:
    bool este_second_hand;
    std::string conditie_fizica; // Noua , Buna, Acceptabila , Uzata
    time_t data_cumparare;
    
protected:
    void afisare(std::ostream &out) const override;
    
public:
    // constructor cu parametrii
    CarteIndividuala(std::shared_ptr<Carte> carte, bool este_second_hand, const std::string &conditie_fizica);

    // constructor de copiere
    CarteIndividuala(const CarteIndividuala& other);

    // clone
    std::shared_ptr<UnitateVanzare> clone() const override;

    // destructor
    ~CarteIndividuala() override = default;

    // functii virtuale
    [[nodiscard]] bool verificaStocSuficient(int cantitate_ceruta) const override;
    double getPretcomanda() const override;
    std::vector<std::string> getListaISBN() const override;
    std::string getDescriere() const override;
    void scadeStoc( const int cantitate) override;

    // functii
    [[nodiscard]] int calculeazaLuniDetinere() const;
    [[nodiscard]] int calculeazaZileDetinere() const;
    [[nodiscard]] std::string getDataCumparareFormatata() const;
    bool esteEligibilaPtTrade(const CarteIndividuala& carte_noua) const;
    double calculeazaValoareRevanzare(int luni_folosite) const;
    double calculeazaReducereSecondHand() const;
};

#endif //OOP_CARTEINDIVIDUALA_H
