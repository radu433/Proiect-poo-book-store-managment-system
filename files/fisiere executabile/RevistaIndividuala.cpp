#include "../headere/RevistaIndividuala.h"
#include "../headere/UnitateVanzare.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
// contrsuctor
RevistaIndividuala::RevistaIndividuala(const std::shared_ptr<Revista> &revista) : UnitateVanzare(revista){
    if (!revista)
        throw DateInvalideException("Revista invalida(nullptr)");
}
//constructor de copiere
RevistaIndividuala::RevistaIndividuala(const RevistaIndividuala &other):UnitateVanzare(other) {
}
//clone
std::shared_ptr<UnitateVanzare> RevistaIndividuala::clone() const {
  return   std::make_shared<RevistaIndividuala>(*this);
}
// functii virtuale
bool RevistaIndividuala::verificaStocSuficient(int cantitate_ceruta) const {
    if (cantitate_ceruta <= 0) {
        throw DateInvalideException("Cantitate invalida pentru revista!");
    }

    return produs_principal->getcantitate() >= cantitate_ceruta;
}
void RevistaIndividuala::scadeStoc(int cantitate) {
    if (!verificaStocSuficient(cantitate)) {
        throw LibrarieException("Stoc insuficient pentru revista!");
    }

    produs_principal->Scade_stoc(cantitate);
}

double RevistaIndividuala::getPretcomanda() const {
    return produs_principal->getPretFinal();
}

std::vector<std::string> RevistaIndividuala::getListaIdentificatori() const {
    return {produs_principal->getISBN()};
}
std::string RevistaIndividuala::getDescriere() const {
    return "Revista: " + produs_principal->getTitlu();
}

std::string RevistaIndividuala::getIdentificator() const {
    return produs_principal->getIdentificator();
}

// afisare
void RevistaIndividuala::afisare(std::ostream& out) const {
    out << "[Revista individuala]\n";
    out << "Titlu: " << produs_principal->getTitlu() << "\n";
    out << "Identificator (ISSN): " << produs_principal->getISBN() << "\n";
    out << "Pret: " << getPretcomanda() << " lei\n";
}
