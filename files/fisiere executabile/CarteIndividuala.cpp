#include "../headere/CarteIndividuala.h"
#include "../headere/Carte.h"
#include "../headere/Manual.h"
#include "../headere/Revista.h"
#include "../headere/CarteStiintifica.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <algorithm>

// Constructor
CarteIndividuala::CarteIndividuala(std::shared_ptr<Carte> carte, const bool este_second_hand, const std::string &conditie_fizica)
    : UnitateVanzare(std::move(carte)),
      este_second_hand(este_second_hand),
      conditie_fizica(conditie_fizica),
      data_cumparare(std::time(nullptr))
{
    if (conditie_fizica != "Noua" && conditie_fizica != "Buna" &&
        conditie_fizica != "Acceptabila" && conditie_fizica != "Uzata") {
        throw DateInvalideException("Conditie fizica necunoscuta: " + conditie_fizica);
        }
}

// Constructor de Copiere
CarteIndividuala::CarteIndividuala(const CarteIndividuala &other)
    : UnitateVanzare(other),
      este_second_hand(other.este_second_hand),
      conditie_fizica(other.conditie_fizica),
      data_cumparare(std::time(nullptr))
{
}

// Clone
std::shared_ptr<UnitateVanzare> CarteIndividuala::clone() const {
    return std::make_shared<CarteIndividuala>(*this);
}

// Afișare
void CarteIndividuala::afisare(std::ostream &out) const {
    UnitateVanzare::afisare(out);
    if (este_second_hand) {
        out << " [Stare: Second-Hand (" << conditie_fizica << ")]";
    } else {
        out << " [Stare: Nou]";
    }
}

//Logica de Preț
double CarteIndividuala::getPretcomanda() const {
    const auto carte = getProdusPrincipal();
    if (!carte) return 0.0;

    double pret = carte->getPretFinal();

    if (este_second_hand) {
        const double reducere = calculeazaReducereSecondHand();
        pret *= (1.0 - reducere);
    }
    return pret;
}

double CarteIndividuala::calculeazaReducereSecondHand() const {
    if (!este_second_hand) return 0.0;
    if (conditie_fizica == "Ca noua") return 0.15;
    if (conditie_fizica == "Buna") return 0.30;
    if (conditie_fizica == "Acceptabila") return 0.50;
    if (conditie_fizica == "Uzata") return 0.70;
    return 0.20;
}

// Logica de Stoc
bool CarteIndividuala::verificaStocSuficient(const int cantitate_ceruta) const {
    const auto carte = getProdusPrincipal();
    if (!carte) return false;
    return carte->getcantitate() >= cantitate_ceruta;
}

void CarteIndividuala::scadeStoc(const int cantitate) {
    const auto carte = getProdusPrincipal();
    if (!carte) {
        throw DateInvalideException("Nu se poate scadea stocul: Produs invalid!");
    }
    if (auto manual=std::dynamic_pointer_cast<Manual>(carte)) {
    int an_curent=2025;
    if (manual->StareAprobare(an_curent)=="Expirat") {
        throw DateInvalideException(
                "Blocare vanzare: Manualul '" + manual->gettitlu() +
                "' este EXPIRAT si nu mai este aprobat de minister!"
            );
    }
}
    if (auto revista=std::dynamic_pointer_cast<Revista>(carte)) {
        if (revista->esteEditieSpeciala()) {
            std::cout<<"Nu se vinde o editie de colectie!";
        }
    }
    else if (const auto stiintifica = std::dynamic_pointer_cast<CarteStiintifia>(carte)) {
        if (!stiintifica->esteDeActualitate(2025)) {
            std::cout << " Carte științifică veche. Verificați dacă există ediții noi.\n";
        }
    }

    if (!carte->Scade_Stoc(cantitate)) {
        throw StocInsuficientException(carte->gettitlu(), carte->getcantitate(), cantitate);
    }
}

// Informații
std::vector<std::string> CarteIndividuala::getListaISBN() const {
    if (const auto carte = getProdusPrincipal()) {
        return { carte->getISBN() };
    }
    return {};
}

std::string CarteIndividuala::getDescriere() const {
    const auto carte = getProdusPrincipal();
    return carte ? carte->gettitlu() : "Indisponibil";
}

//  Trade-In și Revânzare
bool CarteIndividuala::esteEligibilaPtTrade(const CarteIndividuala &carte_noua) const {
    // 1. Verificare Second Hand
    if (!este_second_hand) {
        std::cout << "Trade-in doar pentru carti Second-hand!\n";
        return false;
    }
    if (carte_noua.este_second_hand) {
        std::cout << "Trade-in necesita o carte noua la schimb!\n";
        return false;
    }

    // 2. Obținere Pointeri
    const auto carte_mea = getProdusPrincipal();
    const auto carte_target = carte_noua.getProdusPrincipal();

    if (!carte_mea || !carte_target)
        throw DateInvalideException("Nu se poate verifica trade-in: Carti invalide (NULL)!");

    // 3. Verificare ISBN
    if (carte_mea->getISBN() != carte_target->getISBN()) {
        std::cout << "Trade-in disponibil doar pentru aceeasi editie (ISBN diferit)\n";
        return false;
    }

    // 4. Verificare Condiție
    if (conditie_fizica == "Uzata") {
        std::cout << "Conditie prea slaba pentru trade-in\n";
        return false;
    }

    // 5. Calcul
    const int luni = calculeazaLuniDetinere();
    const double trade = calculeazaValoareRevanzare(luni);

    if (const double pret_noua = carte_noua.getPretcomanda(); trade < pret_noua * 0.30) {
        std::cout << "Valoare trade-in prea mica (" << trade << " RON).\n";
        return false;
    }

    std::cout << "Trade-In ACCEPTAT! Valoare: " << trade << " RON\n";
    return true;
}

double CarteIndividuala::calculeazaValoareRevanzare(const int luni_folosite) const {
    const auto carte = getProdusPrincipal();
    if (!carte) return 0.0;

    const double val_init = carte->getPretFinal();
    double factor_depreciere = 1.0;

    // Factor Condiție
    if (conditie_fizica == "Noua") factor_depreciere = 0.80;
    else if (conditie_fizica == "Buna") factor_depreciere = 0.65;
    else if (conditie_fizica == "Acceptabila") factor_depreciere = 0.50;
    else factor_depreciere = 0.25;

    // Factor Timp
    const double depreciere_timp = std::max(0.0, 1.0 - (luni_folosite * 0.01));
    factor_depreciere *= depreciere_timp;

    if (carte->calculeaza_valoarea_academica() >= 80.0)
        factor_depreciere *= 1.15;


    if (const auto manual = std::dynamic_pointer_cast<Manual>(carte)) {
        if (manual->StareAprobare(2025) == "APROBAT") {
            factor_depreciere *= 1.20;
        }
    }

    // Factor Rating (Nume corect)
    if (carte->calculeaza_raitingmin() >= 4.0)
        factor_depreciere *= 1.1;

    double valoare_revanzare = val_init * factor_depreciere;

    // Plafonare
    valoare_revanzare = std::max(valoare_revanzare, val_init * 0.15);
    valoare_revanzare = std::min(valoare_revanzare, val_init * 0.9);

    return valoare_revanzare;
}

// Helpers Dată
int CarteIndividuala::calculeazaLuniDetinere() const {
    const time_t acum = std::time(nullptr);
    const double secunde = std::difftime(acum, data_cumparare);
    const int zile = static_cast<int>(secunde / (60 * 60 * 24));
    return zile / 30;
}

int CarteIndividuala::calculeazaZileDetinere() const {
    const time_t acum = std::time(nullptr);
    const double secunde = std::difftime(acum, data_cumparare);
    return static_cast<int>(secunde / (60 * 60 * 24));
}

std::string CarteIndividuala::getDataCumparareFormatata() const {
    const std::tm* tm_info = std::localtime(&data_cumparare);
    if (!tm_info) return "Data invalida";
    std::stringstream ss;
    ss << std::put_time(tm_info, "%d/%m/%Y %H:%M:%S");
    return ss.str();
}