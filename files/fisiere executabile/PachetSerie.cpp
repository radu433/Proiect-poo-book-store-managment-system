#include "../headere/PachetSerie.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../headere/Manual.h"
#include "../headere/Revista.h"
#include "../headere/CarteStiintifica.h"
#include "../headere/Carte.h"
#include <iostream>
#include <sstream>

// afisare
void PachetSerie::afisare(std::ostream &out) const {
    UnitateVanzare::afisare(out);
    out << "Pachet: " << nume_pachet << " [" << tip_pachet << "]\n";
    out << "continut: " << continut.size();
    if (este_complet) {
        out << " (Complet)";
    } else {
        out << " (Incomplet)";
    }
    out << "\n";

    out << "Continut:\n";
    int index = 1;
    for (const auto& carte : continut) {
        if (carte) {
            out << "  " << index++ << ". " << carte->getTitlu() << "\n";
        }
    }

}

// constructor cu parametrii
PachetSerie::PachetSerie(const std::vector<std::shared_ptr<Publicatie>>& continut, const std::string &nume_pachet,
    const std::string &tip_pachet, const bool este_complet) : UnitateVanzare(continut.empty()? nullptr: continut[0]),
nume_pachet(nume_pachet),continut(continut), tip_pachet(tip_pachet),este_complet(este_complet){
    if (continut.empty()) {
        throw DateInvalideException("Nu se poate crea un pachet gol (fara continut)!");
    }
    }


PachetSerie::PachetSerie(const PachetSerie &other):UnitateVanzare(other), nume_pachet(other.nume_pachet),
continut(other.continut), tip_pachet(other.tip_pachet),este_complet(other.este_complet){
}

std::shared_ptr<UnitateVanzare> PachetSerie::clone() const {
    return std::make_shared<PachetSerie>(*this);

}


double PachetSerie::calculeazaReducerePachet() const {
    double reducere_baza=0.05;

    if (const int nr_vol=static_cast<int>(continut.size()); nr_vol>=15)
        reducere_baza+=0.15;
    else if (nr_vol>=10 )
        reducere_baza+=0.1;
    else if (nr_vol>=5 )
        reducere_baza+=0.05;

    if (este_complet) {
        reducere_baza+=0.05;
    }

    if (tip_pachet=="Bacalaureat")
        reducere_baza+=0.05;
    else if (tip_pachet=="Opere complete")
        reducere_baza+=0.05;
    return  std::min(reducere_baza,0.3);
}

bool PachetSerie::verificaCompletitudine() const {
    if (continut.empty())
    {throw DateInvalideException("Pachetul '" + nume_pachet + "' este gol (fara continut)!");}
    [[maybe_unused]] int nr_manuale=0;
    [[maybe_unused]] int nr_reviste=0;
    [[maybe_unused]] int nr_carti=0;
    [[maybe_unused]] int nr_stiintifice=0;
    for (const auto& carte : continut) {
        if (!carte)
            throw DateInvalideException("Eroare critica: Pachetul contine un pointer NULL!");
        if (!carte->este_disponibila()){
            throw StocInsuficientException(carte->getTitlu(), 0, 1);
        }
        if (std::dynamic_pointer_cast<Manual>(carte)) {
            nr_manuale++;
        } else if (std::dynamic_pointer_cast<CarteStiintifica>(carte)) {
            nr_stiintifice++;
        } else if (std::dynamic_pointer_cast<Revista>(carte)) {
            nr_reviste++;
        }else if (std::dynamic_pointer_cast<Carte>(carte)) {
            nr_carti++;
        }

    }if (tip_pachet == "Bacalaureat") {
        if (nr_manuale < 5) {
            throw DateInvalideException(
                "Pachet Bacalaureat INVALID: Are " + std::to_string(nr_manuale) +
                " manuale (minim necesar: 5)."
            );
        }
    }
    else if (tip_pachet == "Trilogie") {
        if (continut.size() != 3) {
            throw DateInvalideException(
                "Trilogie INVALIDA: Are " + std::to_string(continut.size()) +
                " continut (trebuie exact 3)."
            );
        }
    }
    else if (tip_pachet == "Opere Complete") {
        if ( nr_stiintifice < 5) {
            throw DateInvalideException(
                "Opere Complete INVALID: Prea putine carti stiintifice (" +
                std::to_string( nr_stiintifice) + ")."
            );
        }
    }
    if (!este_complet) {
        throw DateInvalideException("Pachetul este marcat manual ca fiind INCOMPLET in sistem.");
    }
        return true;
    }


void PachetSerie::adauga_volum(const std::shared_ptr<Carte> &carte) {
    if (!carte) {
        std::cout << "Eroare: Carte invalida!\n";
        return;
    }

    // verifică dacă cartea deja exista în pachet
    for (const auto& vol : continut) {
        if (vol && vol->getIdentificator() == carte->getISBN()) {
            std::cout << "Cartea deja exista in pachet: " << carte->getTitlu() << "\n";
            return;
        }
    }

    continut.push_back(carte);
    std::cout << "Volum adaugat la pachet: " << carte->getTitlu() << "\n";
    std::cout << "Total continut: " << continut.size() << "\n";
}

const std::vector<std::shared_ptr<Publicatie>> & PachetSerie::getContinut() const {
    return continut;
}

std::string PachetSerie::getIdentificator() const {
    return produs_principal->getIdentificator();
}

std::vector<std::string> PachetSerie::getListaIdentificatori() const {
    std::vector<std::string> identificatori;
    // continut este vectorul de shared_ptr<Carte> din PachetSerie
    for (const auto& volum : continut) {
        if (volum) {
            identificatori.push_back(volum->getIdentificator());
        }
    }

    return identificatori;
}

bool PachetSerie::verificaStocSuficient(int cantitate_ceruta) const {


    for (const auto& vol : continut) {

        if (vol->getcantitate() < cantitate_ceruta) {
            return false;
        }
    }

    return true;
}

void PachetSerie::scadeStoc( const int cantitate)  {
    if (continut.empty()) {
        throw DateInvalideException("Pachet gol - nu se poate scadea stoc!");
    }

    if (cantitate<=0)
        throw StocInvalidException(cantitate);

    for (const auto& vol:continut) {
        if (!vol)
            throw DateInvalideException("Carte invalida in pachet!");
        if (vol->getcantitate()<cantitate) {
            std::stringstream ss;
            ss<< "Stoc insuficient pentru pachetul " << nume_pachet << "!\n"
               << "Cartea '" << vol->getTitlu() << "' are doar "
               << vol->getcantitate() << " exemplare.\n"
               << "Pachetul necesita " << cantitate << " din fiecare carte.";
            throw StocInsuficientException(vol->getTitlu(), vol->getcantitate(), cantitate);;
        }

    }
    std::cout << "Scadere stoc pentru pachetul: " << nume_pachet << "\n";
    for (const auto& vol : continut) {
        if (!vol->Scade_stoc(cantitate))
            throw std::runtime_error("Eroare critica la scaderea stocului in pachet!");

        std::cout<< " - "<< vol->getTitlu() << " x"<<cantitate<<"(Ramas:"<<vol->getcantitate()<<")\n";
    }
    std::cout << "Pachetul " << nume_pachet << " a fost vandut x" << cantitate << "\n";

}

std::string PachetSerie::getDescriere() const {
    std::stringstream ss;
    ss << "PACHET (" << tip_pachet << "): " << nume_pachet;
    if (este_complet) ss << " [COMPLET]";
    ss << " - contine " << continut.size() << " continut";
    return ss.str();
}

double PachetSerie::getPretcomanda() const {
    double total_brut = 0.0;

    // Suma prețurilor cărților
    for (const auto& vol : continut) {
        if (vol) {
            total_brut += vol->getPretFinal();
        }
    }

    // Calculăm reducerea pe baza tipului
    double reducere_baza = calculeazaReducerePachet();

    return total_brut * (1.0 - reducere_baza);
}

// operator <<
std::ostream & operator<<(std::ostream &out, const PachetSerie &p) {
    p.afisare(out);
    return out;
}
