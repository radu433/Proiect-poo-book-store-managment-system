

#ifndef OOP_PACHETSERIE_H
#define OOP_PACHETSERIE_H
#include "UnitateVanzare.h"
enum class TipPachet {Bacalaureat,Trilogie, Stiintific,Literatura,Educativ,Mixt,Personalizat, Second_Hand};
class PachetSerie: public UnitateVanzare {
private:
    std::string nume_pachet;
    std::vector<std::shared_ptr<UnitateVanzare>> continut;
    TipPachet tip_pachet;
    bool este_complet;

    friend class PachetSerieService;

protected:

    void afisare(std::ostream &out) const override;

    public:
    // constructor cu parametrii pachete noi

    explicit PachetSerie(const std::vector<std::shared_ptr<UnitateVanzare>> &continut, const std::string &nume_pachet,
                const TipPachet tip, bool este_complet);

    // constructor cu parametrii pachete sh
   explicit  PachetSerie( const std::vector<std::shared_ptr<UnitateVanzare>>& continutSH);
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


    const std::vector<std::shared_ptr<UnitateVanzare>>& getContinut() const;


    // functii virtuale
    std::string getIdentificator() const override;

    [[nodiscard]] std::vector<std::string> getListaIdentificatori() const override;

    [[nodiscard]] bool verificaStocSuficient(int cantitate_ceruta) const override;

    void scadeStoc(int cantitate) override ;

    [[nodiscard]] std::string getDescriere() const override;

    [[nodiscard]] double getPretcomanda() const override;
    static std::string transforma (TipPachet tip_pachet);
    double getPretFinUnitate() const override;

    bool estePPredefinit() const override {return true;}

    std::string serializare() const override;
    static std::shared_ptr<PachetSerie> deserializare(const std::string& line, const std::vector<std::shared_ptr<Publicatie>>& publicatii);
};


#endif //OOP_PACHETSERIE_H
