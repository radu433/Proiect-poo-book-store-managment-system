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
CarteIndividuala::CarteIndividuala(std::shared_ptr<Carte> carte)
    : UnitateVanzare(std::move(carte))
{
    if (conditie_fizica != "Noua" && conditie_fizica != "Buna" &&
        conditie_fizica != "Acceptabila" && conditie_fizica != "Uzata") {
        throw DateInvalideException("Conditie fizica necunoscuta: " + conditie_fizica);
        }
}

// Constructor de Copiere
CarteIndividuala::CarteIndividuala(const CarteIndividuala &other)
    : UnitateVanzare(other)

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

    return carte->getPretFinal();
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
        if (manual->StareAprobare() == "Expirat") {
            throw DateInvalideException(
                "Blocare vanzare: Manualul '" + manual->getTitlu() +
                "' este EXPIRAT si nu mai este aprobat de minister!"
            );
    }
}
    if (auto revista=std::dynamic_pointer_cast<Revista>(carte)) {
        if (revista->esteEditieSpeciala()) {
            throw DateInvalideException("Nu se poate vinde o editie de colectie (Revista)!");
        }
    } else if (const auto stiintifica = std::dynamic_pointer_cast<CarteStiintifica>(carte)) {
        if (!stiintifica->esteDeActualitate()) {
            std::cout << " Carte stiintifica veche. Verificati daca exista editii noi.\n";
        }
    }

    if (!carte->Scade_stoc(cantitate)) {
        throw StocInsuficientException(carte->getTitlu(), carte->getcantitate(), cantitate);
    }
}

std::string CarteIndividuala::getIdentificator() const {
    return produs_principal->getIdentificator();
}


// Informații
std::vector<std::string> CarteIndividuala::getListaIdentificatori() const {
    if (const auto carte = getProdusPrincipal()) {
        return {carte->getIdentificator()};
    }
    return {};
}

std::string CarteIndividuala::getDescriere() const {
    const auto carte = getProdusPrincipal();
    return carte ? carte->getTitlu() : "Indisponibil";
}

