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
    out << "Pachet: " << nume_pachet << " [" << transforma(tip_pachet) << "]\n";
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
PachetSerie::PachetSerie(const std::vector<std::shared_ptr<UnitateVanzare>>& continut, const std::string &nume_pachet,
    const TipPachet tip, const bool este_complet) : UnitateVanzare(),
nume_pachet(nume_pachet),continut(continut), tip_pachet(tip),este_complet(este_complet){
    if (continut.empty()) {
        throw DateInvalideException("Nu se poate crea un pachet gol (fara continut)!");
    }
    verificaCompletitudine();
    }

PachetSerie::PachetSerie(const std::vector<std::shared_ptr<UnitateVanzare>> &continutSH) : continut(continutSH),
    tip_pachet(TipPachet::Second_Hand),
    este_complet(false) {
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

    switch (tip_pachet) {
        case TipPachet::Bacalaureat:{reducere_baza+=0.05;break;}
        case TipPachet::Trilogie:{
        if (continut.size()>3)
            reducere_baza+=0.05;
        break;
        }
          default: break;
    }
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
        if (!carte->valideazaDisponibilitate()){
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

    }
    switch (tip_pachet) {
        case TipPachet::Bacalaureat: {
            if (nr_manuale < 5) {
                throw DateInvalideException(
                    "Pachet Bacalaureat INVALID: Are " + std::to_string(nr_manuale) +" manuale (minim necesar: 5).");

            }
            break;
        }
        case TipPachet::Trilogie: {
            if (continut.size() != 3) {
                throw DateInvalideException("Trilogie INVALIDA: Are " + std::to_string(continut.size()) +
                    " continut (trebuie exact 3).");
            }
            break;
        }
        case TipPachet::Educativ: {
            if ( nr_stiintifice < 5) {
                throw DateInvalideException("Educativ INVALID: Prea putine carti stiintifice (" +
                    std::to_string( nr_stiintifice) + ").");
            }
            break;
        }
        default:break;
    }
    if (!este_complet) {
        throw DateInvalideException("Pachetul este marcat manual ca fiind INCOMPLET in sistem.");
    }
        return true;

}

const std::vector<std::shared_ptr<UnitateVanzare>> & PachetSerie::getContinut() const {
    return this->continut;
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

        if (vol->getCantitate() < cantitate_ceruta) {
            return false;
        }
    }

    return true;
}

void PachetSerie::scadeStoc( const int cantitate)  {
    if (cantitate <= 0)
        throw StocInvalidException(cantitate);
    if (continut.empty())
        throw DateInvalideException("Pachet gol - nu se poate scadea stoc!");
    for (const auto& u : continut) {
        if (!u)
            throw DateInvalideException("Unitate invalida in pachet!");
        if (!u->verificaStocSuficient(cantitate)) {
            throw StocInsuficientException(u->getDescriere(),0,cantitate);
        }
    }
    for (const auto& u : continut) {
        u->scadeStoc(cantitate);
    }

}

std::string PachetSerie::getDescriere() const {
    std::stringstream ss;
    ss << "PACHET (" << transforma(tip_pachet) << "): " << nume_pachet;
    if (este_complet) ss << " [COMPLET]";
    ss << " - contine " << continut.size() << " continut";
    return ss.str();
}

double PachetSerie::getPretcomanda() const {
       double total=0.0;
    for (const auto& u : continut) {
        if (u)
            total += u->getPretcomanda();

    }
    return total*(1-calculeazaReducerePachet());
}

std::string PachetSerie::transforma(const TipPachet tip_pachet) {
    switch (tip_pachet) {
        case TipPachet::Bacalaureat: return "Bacalaureat";
        case TipPachet::Trilogie: return "Trilogie";
        case TipPachet::Stiintific: return "Stiintific";
        case TipPachet::Literatura: return "Literatura";
        case TipPachet::Educativ: return "Educativ";
        case TipPachet::Mixt: return "Mixt";
        case TipPachet::Personalizat: return "Personalizat";
        case TipPachet::Second_Hand: return "Second_Hand";
    }
    return "Necunoscut";
}

double PachetSerie::getPretFinUnitate() const {
    double total = 0.0;
    for (const auto& u : continut){
        if (u)
            total += u->getPretcomanda();
    }
    return total * (1 - calculeazaReducerePachet());
}

// operator <<
std::ostream & operator<<(std::ostream &out, const PachetSerie &p) {
    p.afisare(out);
    return out;
}
