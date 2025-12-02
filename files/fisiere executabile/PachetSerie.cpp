#include "../headere/PachetSerie.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../headere/Manual.h"
#include "../headere/Revista.h"
#include "../headere/CarteStiintifica.h"
#include <iostream>
#include <sstream>

#include "../headere/Carte.h"
// afisare
void PachetSerie::afisare(std::ostream &out) const {
    UnitateVanzare::afisare(out);
    out << "Pachet: " << nume_pachet << " [" << tip_pachet << "]\n";
    out << "Volume: " << volume.size();
    if (este_complet) {
        out << " (Complet)";
    } else {
        out << " (Incomplet)";
    }
    out << "\n";

    out << "Continut:\n";
    int index = 1;
    for (const auto& carte : volume) {
        if (carte) {
            out << "  " << index++ << ". " << carte->gettitlu() << "\n";
        }
    }

}

// constructor cu parametrii
PachetSerie::PachetSerie(const std::vector<std::shared_ptr<Carte>>& volume, const std::string &nume_pachet,
    const std::string &tip_pachet, const bool este_complet) : UnitateVanzare(volume.empty()? nullptr: volume[0]),
nume_pachet(nume_pachet),volume(volume), tip_pachet(tip_pachet),este_complet(este_complet){
    if (volume.empty()) {
        throw DateInvalideException("Nu se poate crea un pachet gol (fara volume)!");
    }
    }


PachetSerie::PachetSerie(const PachetSerie &other):UnitateVanzare(other), nume_pachet(other.nume_pachet),
volume(other.volume), tip_pachet(other.tip_pachet),este_complet(other.este_complet){
}

std::shared_ptr<UnitateVanzare> PachetSerie::clone() const {
    return std::make_shared<PachetSerie>(*this);

}


double PachetSerie::calculeazaReducerePachet() const {
    double reducere_baza=0.05;

    if (const int nr_vol=static_cast<int>(volume.size()); nr_vol>=15)
        reducere_baza+=0.15;
    else if (nr_vol>=10 && nr_vol<=15)
        reducere_baza+=0.1;
    else if (nr_vol>=5 && nr_vol<=10)
        reducere_baza+=0.05;

    if (este_complet) {
        reducere_baza+=0.05;
    }

    if (tip_pachet=="Bacalaureat")
        reducere_baza+=0.05;
    else if (tip_pachet=="Opere complete")
        reducere_baza+=0.05;
    return  std::max(reducere_baza,0.3);
}

bool PachetSerie::verificaCompletitudine() const {
    if (volume.empty())
    {throw DateInvalideException("Pachetul '" + nume_pachet + "' este gol (fara volume)!");}
    [[maybe_unused]] int nr_manuale=0;
    [[maybe_unused]] int nr_reviste=0;
    [[maybe_unused]] int nr_carti=0;
    for (const auto& carte : volume) {
        if (!carte)
            throw DateInvalideException("Eroare critica: Pachetul contine un pointer NULL!");
        if (!carte->este_disponibila()){
            throw StocInsuficientException(carte->gettitlu(), 0, 1);
        }
        if (std::dynamic_pointer_cast<Manual>(carte)) {
            nr_manuale++;
        } else if (std::dynamic_pointer_cast<CarteStiintifia>(carte)) {
            nr_carti++;
        } else if (std::dynamic_pointer_cast<Revista>(carte)) {
            nr_reviste++;
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
        if (volume.size() != 3) {
            throw DateInvalideException(
                "Trilogie INVALIDA: Are " + std::to_string(volume.size()) +
                " volume (trebuie exact 3)."
            );
        }
    }
    else if (tip_pachet == "Opere Complete") {
        if ( nr_carti < 5) {
            throw DateInvalideException(
                "Opere Complete INVALID: Prea putine carti stiintifice (" +
                std::to_string( nr_carti) + ")."
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
    for (const auto& vol : volume) {
        if (vol && vol->getISBN() == carte->getISBN()) {
            std::cout << "Cartea deja exista in pachet: " << carte->gettitlu() << "\n";
            return;
        }
    }

    volume.push_back(carte);
    std::cout << "Volum adaugat la pachet: " << carte->gettitlu() << "\n";
    std::cout << "Total volume: " << volume.size() << "\n";
}

std::vector<std::string> PachetSerie::getListaISBN() const {
    std::vector<std::string> lista;
    // volume este vectorul de shared_ptr<Carte> din PachetSerie
    for (const auto& vol : volume) {
        lista.push_back(vol->getISBN());
    }
    return lista;
}

bool PachetSerie::verificaStocSuficient(int cantitate_ceruta) const {


    for (const auto& vol : volume) {

        if (vol->getcantitate() < cantitate_ceruta) {
            return false;
        }
    }

    return true;
}

void PachetSerie::scadeStoc( const int cantitate)  {
    if (volume.empty()) {
        throw DateInvalideException("Pachet gol - nu se poate scadea stoc!");
    }

    if (cantitate<=0)
        throw StocInvalidException(cantitate);

    for (const auto& vol:volume) {
        if (!vol)
            throw DateInvalideException("Carte invalida in pachet!");
        if (vol->getcantitate()<cantitate) {
            std::stringstream ss;
            ss<< "Stoc insuficient pentru pachetul " << nume_pachet << "!\n"
               << "Cartea '" << vol->gettitlu() << "' are doar "
               << vol->getcantitate() << " exemplare.\n"
               << "Pachetul necesita " << cantitate << " din fiecare carte.";
            throw StocInsuficientException(vol->gettitlu(), vol->getcantitate(), cantitate);;
        }

    }
    std::cout << "Scadere stoc pentru pachetul: " << nume_pachet << "\n";
    for (const auto& vol : volume) {
        if (!vol->Scade_Stoc(cantitate))
            throw std::runtime_error("Eroare critica la scaderea stocului in pachet!");

        std::cout<< " - "<< vol->gettitlu() << " x"<<cantitate<<"(Ramas:"<<vol->getcantitate()<<")\n";
    }
    std::cout << "Pachetul " << nume_pachet << " a fost vandut x" << cantitate << "\n";

}

std::string PachetSerie::getDescriere() const {
    std::stringstream ss;
    ss << "PACHET (" << tip_pachet << "): " << nume_pachet;
    if (este_complet) ss << " [COMPLET]";
    ss << " - contine " << volume.size() << " volume";
    return ss.str();
}

double PachetSerie::getPretcomanda() const {
    double total_brut = 0.0;

    // Suma prețurilor cărților
    for (const auto& vol : volume) {
        if (vol) {
            total_brut += vol->getPretFinal();
        }
    }

    // Calculăm reducerea pe baza tipului
    double reducere_baza = 0.05;


    if (static_cast<int>(volume.size()) >= 15) reducere_baza += 0.15;
    else if (volume.size() >= 10) reducere_baza += 0.1;


    // Returnăm prețul final
    return total_brut * (1.0 - reducere_baza);
}

// operator <<
std::ostream & operator<<(std::ostream &out, const PachetSerie &p) {
    p.afisare(out);
    return out;
}
