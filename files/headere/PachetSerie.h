

#ifndef OOP_PACHETSERIE_H
#define OOP_PACHETSERIE_H
#include "UnitateVanzare.h"

class PachetSerie: public UnitateVanzare {
private:
    std::string nume_pachet;
    std::vector<std::shared_ptr<Publicatie>> continut;
    std::vector<std::shared_ptr<UnitateVanzare>> continutSH;
    std::string tip_pachet; // "Bacalaureat", "Trilogie", "Opere Complete"
    bool este_complet;
    bool esteSH=false;


protected:

    void afisare(std::ostream &out) const override;

    public:
    // constructor cu parametrii pachete noi

    PachetSerie(const std::vector<std::shared_ptr<Publicatie>> &continut, const std::string &nume_pachet,
                const std::string &tip_pachet, bool este_complet);

    // constructor cu parametrii pachete sh
    PachetSerie( const std::vector<std::shared_ptr<UnitateVanzare>>& continutSH);
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

    const std::vector<std::shared_ptr<Publicatie>>& getContinut() const;


    // functii virtuale
    std::string getIdentificator() const override;

    [[nodiscard]] std::vector<std::string> getListaIdentificatori() const override;

    [[nodiscard]] bool verificaStocSuficient(int cantitate_ceruta) const override;

    void scadeStoc(int cantitate) override ;

    [[nodiscard]] std::string getDescriere() const override;

    [[nodiscard]] double getPretcomanda() const override;
};


#endif //OOP_PACHETSERIE_H
