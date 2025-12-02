

#ifndef OOP_PACHETSERIE_H
#define OOP_PACHETSERIE_H
#include "UnitateVanzare.h"

class PachetSerie: public UnitateVanzare {
private:
    std::string nume_pachet;
    std::vector<std::shared_ptr<Carte>> volume;
    std::string tip_pachet; // "Bacalaureat", "Trilogie", "Opere Complete"
    bool este_complet;
    protected:

    void afisare(std::ostream &out) const override;

    public:
    // constructor cu parametrii

    PachetSerie(const std::vector<std::shared_ptr<Carte>> &volume, const std::string &nume_pachet,
                const std::string &tip_pachet, bool este_complet);

    // constrcutor de copiere
    PachetSerie(const PachetSerie& other);

    // clone
    std::shared_ptr<UnitateVanzare> clone() const override;

    // destructor
    ~PachetSerie() override= default;

    // operator <<
    friend std::ostream& operator<<(std::ostream &out, const PachetSerie &p);

    // functii

    double calculeazaReducerePachet() const;

    bool verificaCompletitudine()const;

    void adauga_volum(const std::shared_ptr<Carte> &carte);

    // functii virtuale
    [[nodiscard]] std::vector<std::string> getListaISBN() const override;

    [[nodiscard]] bool verificaStocSuficient(int cantitate_ceruta) const override;

    void scadeStoc(int cantitate) override ;

    [[nodiscard]] std::string getDescriere() const override;

    [[nodiscard]] double getPretcomanda() const override;
};


#endif //OOP_PACHETSERIE_H
