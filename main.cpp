#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "files/headere/adresa.h"
#include "files/headere/Autor.h"
#include "files/headere/Carte.h"
#include "files/headere/Manual.h"
#include "files/headere/CarteStiintifica.h"
#include "files/headere/Revista.h"
#include "files/headere/Client.h"
#include "files/headere/Comanda.h"
#include "files/headere/UnitateVanzare.h"
#include "files/headere/CarteIndividuala.h"
#include "files/headere/PachetSerie.h"
// exceptii
#include "files/exceptii/exceptii_headere/LibrarieException.h"
#include "files/exceptii/exceptii_headere/DateInvalideException.h"
#include "files/exceptii/exceptii_headere/StocException.h"
#include "files/exceptii/exceptii_headere/ClientExceptions.h"
#include "files/exceptii/exceptii_headere/ComandaExceptions.h"

void Titlu(const std::string& titlu) {
    std::cout<<"\n---"<<titlu<<"---"<<std::endl;
}
void ruleazaTestLibrarie() {
    Titlu("=== SCENARIU COMPLET LIBRARIE ===");

    // 1. CREARE AUTORI + CARTI
    Titlu("1. Autori si Carti – initializare");

    auto autor1 = std::make_shared<Autor>("Mihai", "Eminescu", 75);
    auto autor2 = std::make_shared<Autor>("Albert", "Einstein", 120);

    autor1->adauga_carte("ISBN-EMI-01");
    autor2->adauga_carte("ISBN-EIN-01");
    autor2->adauga_carte("ISBN-EIN-02");

    // apel explicit pentru getcarti_scrise (cppcheck)
    std::cout << "Autor1 - numar carti scrise: " << autor1->getcarti_scrise() << "\n";
    std::cout << "Productivitate Einstein: " << autor2->calcproductivitate() << "\n";
    std::cout << "Ierarhie autor1: " << autor1->ierarhie_a() << "\n";

    // carti concrete
    auto manual = std::make_shared<Manual>(
        "Matematica M1", autor1, 50, 100, 2023, "ISBN-MAN-1",
        300, "Matematica", 12
    );

    auto carte_stiinta = std::make_shared<CarteStiintifia>(
        "Teoria Relativitatii", autor2,
        200, 1920, "ISBN-FIZ-999", 175.5,
        500, "Fizica", "Cercetare", 200, true
    );

    auto revista = std::make_shared<Revista>(
        "Nature", nullptr, 30, 2025, "ISSN-NAT-01",
        20.0, 100, "Lunara", 200, true, "Stiintifica"
    );

    std::cout << "ISBN manual: " << manual->getISBN() << "\n";
    manual->reducere(10);
    manual->adauga_stoc(15);

    std::cout << "Comparatie popularitate: " << carte_stiinta->cumparaDupaPopularitate(*revista) << "\n";

    std::cout << "Compatibilitate liceu carte stiintifica: "
              << (carte_stiinta->CopatibilaCuNivel("liceu") ? "Da" : "Nu") << "\n";

    revista->adauga_rating(4);
    std::cout << "Frecventa citire revista: "
              << revista->calculeaza_frecventa_citire() << "\n";

    std::cout << "Colectionabila? " << revista->este_colectionabila() << "\n";


    // 2. UNITATI VANZARE
    Titlu("2. Unitati de vanzare & pachete");

    auto unit_nou = std::make_shared<CarteIndividuala>(manual, false, "Noua");
    auto unit_sh  = std::make_shared<CarteIndividuala>(carte_stiinta, true, "Buna");

    std::vector<std::shared_ptr<Carte>> volum;
    volum.push_back(manual);

    auto pachet = std::make_shared<PachetSerie>(volum, "Pachet Bac", "Edu", true);
    pachet->adauga_volum(manual); // functie neutilizata folosita acum

    // 3. CLIENT, ADRESA, SOLD
    Titlu("3. Client + Adresa + Fidelitate");

    Adresa a("Bucuresti", "Sector 3", "Str. X", "123456");
    std::cout << "Adresa initiala: " << a.getAdresaCompleta() << "\n";

    a.modificaAdreasa("Iasi", "Copou", "Str. Universitatii", "700500");

    auto client = std::make_shared<Client>("Radu", "radu@email.com", a, "0777");
    client->alimenteazaCont(3000);
    client->adaugacumparaturi(2500);
    std::cout << "Discount personalizat: " << client->calcdiscountpersonalizat() << "\n";

    try { client->foloseste_pct_fidelitate(10); } catch(...) {}

    // 4. COMANDA COMPLETA
    Titlu("4. Comanda completa + teste exceptii + stare");

    try {
        Comanda c(client);

        c.adaugaArticol(unit_nou, 2);
        c.adaugaArticol(unit_sh, 1);
        c.adaugaArticol(pachet, 1);

        std::cout << "Stare comanda inainte: " << c.getStare() << "\n";

        // extragere ISBN – functie neutilizata
        auto lista_isbn = c.extrageISBN();
        std::cout << "ISBN-uri in comanda: ";
        for ( const auto& i : lista_isbn) std::cout << i << " ";
        std::cout << "\n";

        double total = c.finalizareComanda();
        std::cout << "Total platit: " << total << "\n";

        c.adaugaArticol(pachet, 1);

        std::cout << "Stare comanda inainte: " << c.getStare() << "\n";
        std::cout << "Numar articole in cos: " << c.getNumarArticole() << "\n";



        // testare anulare
        Comanda c2(client);
        c2.adaugaArticol(unit_nou, 1);
        c2.anuleazaComanda();
        std::cout << "Stare dupa anulare: " << c2.getStare() << "\n";

    } catch(const std::exception& e) {
        std::cout << "[Eroare comanda] " << e.what() << "\n";
    }

    // 5. Trade-in + logici extra
    Titlu("5. Trade-in + logici extra");

    if (unit_sh->esteEligibilaPtTrade(*unit_nou))
        std::cout << "Trade-in acceptat!\n";
    else
        std::cout << "Trade-in respins!\n";

    std::cout << "ID unitate vanzare: " << unit_nou->getId() << "\n";
    std::cout << "Zile detinere unitate SH: " << unit_sh->calculeazaZileDetinere() << "\n";
    std::cout << "Data cumparare formatata: " << unit_sh->getDataCumparareFormatata() << "\n";

    // 6. Scor relevanta + prioritati
    Titlu("6. Relevanta & prioritati");

    std::vector<std::string> pref = {"Matematica", "Fizica"};
    std::cout << "Scor relevanta manual: "
              << manual->CalculeazaRelevanta(12, pref, 2025) << "\n";

    // apel explicit pentru getCicluScolar (cppcheck)
    std::cout << "Ciclu scolar manual: " << manual->getCicluScolar() << "\n";

    std::cout << "Prioritate restoc manual: "
              << manual->calculeazaPrioritateRestoc(2025, 4) << "\n";

    // 7. Exceptii directe
    Titlu("7. Testare exceptii directe");

    try {
        Manual m_gresit("x", autor1, 10, 10, 2020, "ISBN", 123, "xx", -2);
    } catch (const DateInvalideException& e) {
        std::cout << "[Exceptie prinsa] " << e.what() << "\n";
    }

    try {
        throw StocInsuficientException("Test", 5, 10);
    } catch (const StocInsuficientException& e) {
        std::cout << "Stoc lipsa: " << e.getDiferenta() << "\n";
    }

    Titlu("=== FINAL SCENARIU LIBRARIE ===");
}


int main() {
try {
    ruleazaTestLibrarie();

}catch (const std::exception& e) {
    std::cerr<<"Eroare critica main:"<<e.what() << std::endl;
    return 1;
}
    std::cout << "\n\n program finalizat cu suuces!\n";
    return 0;
};
