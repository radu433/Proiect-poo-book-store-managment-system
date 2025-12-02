#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "files/headere/Adresa.h"
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
void testProiect() {
    Titlu("1. Crearea Autori si Carti (Polimorfism Ierarhia 1)");

    // Cream autori
    auto autor1 = std::make_shared<Autor>("Mihai", "Eminescu", 75);
    auto autor2 = std::make_shared<Autor>("Albert", "Einstein", 120);

    std::cout << *autor1 << std::endl;
    // Cream Carti Concrete
    // 1. Manual
    auto manual = std::make_shared<Manual>(
        "Matematica M1", autor1,
        50.0, 100, 2023, "ISBN-MAN-001",
        300, "Matematica", 12
    );

    // 2. Carte Stiintifica
    auto carte_stiinta = std::make_shared<CarteStiintifia>(
        "Teoria Relativitatii", autor2,
        200, 1920, "ISBN-FIZ-999",175.50 ,
        500, "Fizica", "Cercetare", 5000, true
    );

    // 3. Revista
    auto revista = std::make_shared<Revista>(
        "Nature", nullptr,
        30, 2025, "ISSN-NAT-01", 20.0,
        100, "Lunara", 200, true, "Stiintifica"
    );

    // Afisare Polimorfica (NVI)
    std::cout << *manual << std::endl;
    std::cout << *carte_stiinta << std::endl;


    Titlu("2. Test Functii Carte & Logica Specifica");

    std::cout << "Titlu: " << manual->gettitlu() << "\n";
    std::cout << "Pret Baza: " << manual->getpret_baza() << " RON\n";
    std::cout << "Pret Final (cu adaos): " << manual->getPretFinal() << " RON\n"; // Polimorfism

    // Functii specifice Manual
    std::cout << "Stare Aprobare: " << manual->StareAprobare(2025) << "\n";

    // Functii specifice Carte Stiintifica
    std::cout << "Factor Impact: " << carte_stiinta->calculeazaFactorImpact() << "\n";

    // Modificare stoc manual
    manual->adauga_stoc(20);
    std::cout << "Stoc dupa adaugare: " << manual->getcantitate() << "\n";


    Titlu("3. Crearea Unitatilor de Vanzare (Ierarhia 2)");

    // Carte Individuala (Noua)
    auto unitate_manual = std::make_shared<CarteIndividuala>(manual, false, "Noua");

    // Carte Individuala (Second Hand)
    auto unitate_sh = std::make_shared<CarteIndividuala>(carte_stiinta, true, "Buna");

    // Pachet Serie
    std::vector<std::shared_ptr<Carte>> lista_pachet;
    lista_pachet.push_back(manual);
    lista_pachet.push_back(manual); // Pachet de 2 manuale

    auto pachet = std::make_shared<PachetSerie>(
        lista_pachet, "Kitul Elevului", "Bacalaureat", true
    );

    std::cout << "Pret Unitate SH: " << unitate_sh->getPretcomanda() << " RON (Redus)\n";
    std::cout << "Pret Pachet: " << pachet->getPretcomanda() << " RON\n";
    std::cout << "Descriere Pachet: " << pachet->getDescriere() << "\n";


    Titlu("4. Test Client si Adresa");

    Adresa adresa1("Bucuresti", "Sector 3", "Str. Exemplu nr. 10", "011010");
    // Validare adresa prin constructor
    std::cout << "Adresa: " << adresa1.getAdresaCompleta() << "\n";

    auto client = std::make_shared<Client>("Radu", "radu@email.com", adresa1, "0766123456");

    // Alimentam contul
    client->alimenteazaCont(2000.0);
    std::cout << *client << "\n";


    Titlu("5. Procesare Comanda (Flux Complet)");

    try {
        Comanda comanda(client);

        // Adaugam articole
        std::cout << ">> Adaugare Manual (2 buc)...\n";
        comanda.adaugaArticol(unitate_manual, 2);

        std::cout << ">> Adaugare Pachet...\n";
        comanda.adaugaArticol(pachet, 1);

        // Afisam comanda inainte de finalizare
        std::cout << comanda;

        // Finalizare
        std::cout << ">> Finalizare...\n";
        double total = comanda.finalizareComanda();

        std::cout << "Comanda finalizata! Total platit: " << total << " RON\n";

    } catch (const StocInsuficientException& e) {
        std::cout << "[EROARE STOC] " << e.what() << "\n";

    } catch (const SoldInsuficientException& e) {
        std::cout << "[EROARE BANI] " << e.what() << "\n";

    } catch (const ComandaException& e) {
        std::cout << "[EROARE COMANDA] " << e.what() << "\n";

    } catch (const ClientException& e) {
        std::cout << "[EROARE CLIENT] " << e.what() << "\n";

    } catch (const LibrarieException& e) {
        std::cout << "[EROARE LIBRARIE] " << e.what() << "\n";

    } catch (const std::exception& e) {
        std::cout << "[EROARE NECUNOSCUTA] " << e.what() << "\n";
    }

    Titlu("6. Test Trade-In");

    // Testam daca putem da la schimb cartea SH pe una Noua
    if (unitate_sh->esteEligibilaPtTrade(*unitate_manual)) {
        std::cout << "Trade-In Acceptat!\n";
    } else {
        std::cout << "Trade-In Respins.\n";
    }

    Titlu("7. Testare Exceptii");

    try {
        // Incercam sa cream un manual cu clasa negativa
        Manual manual_gresit("Gresit", autor1, 10, 10, 2020, "ISBN", 100, "X", -5);
    } catch (const DateInvalideException& e) {
        std::cout << "[PRINS] " << e.what() << "\n";
    }

    std::cout << "\nSold ramas client: " << client->getSold() << " RON\n";
}
void testfunctiiavansate() {
    Titlu("8. TESTARE FUNCTIONALITATI AVANSATE & ADMINISTRARE");

    auto autor = std::make_shared<Autor>("Demo", "Autor", 89);
    Adresa adresa("Iasi", "Copou", "Str. Universitatii", "700500");
    auto client = std::make_shared<Client>("Student Eminescu", "student@edu.ro", adresa, "0799123456");
    client->alimenteazaCont(5000);

    auto manual = std::make_shared<Manual>("Info Intensiv", autor, 80.0, 50, 2024, "ISBN-INFO", 400, "Informatica", 11);

    auto revista = std::make_shared<Revista>("National Geographic", nullptr, 20,
        2025, "ISSN-NG", 40.0, 120, "Lunara", 300, true, "Stiintifica");


    auto unitate_manual = std::make_shared<CarteIndividuala>(manual, false, "Noua");
    auto unitate_revista = std::make_shared<CarteIndividuala>(revista, false, "Noua");

   // test sugestii este declansat la cumparare
    Comanda comanda_recomandare(client);
    comanda_recomandare.adaugaArticol(unitate_manual, 1);
    comanda_recomandare.adaugaArticol(unitate_revista, 1);

    comanda_recomandare.finalizareComanda();

    // scor relevanta
    std::vector<std::string> preferinte = {"Informatica", "Fizica"};
    double scor = manual->CalculeazaRelevanta(11, preferinte, 2025);

    std::cout << "Scor Relevanta Manual (Cls 11, Info): " << scor << "/100\n";
    std::cout << "Este materie de examen? " << (manual->getTip().find("Informatica") != std::string::npos ? "Da" : "Nu") << "\n";

    // Test revista
    std::cout << "Scor Frecventa Citire Revista: " << revista->calculeaza_frecventa_citire() << "\n";

    auto revista_veche = std::make_shared<Revista>("Times Vechi", nullptr, 100, 2025, "ISSN-OLD" ,10.0, 50, "Lunara", 500, false, "Istorie");
    revista_veche->adauga_rating(5);
    std::cout << "Revista veche este colectionabila? " << (revista_veche->este_colectionabila() ? "Da" : "Nu") << "\n";

    // Testare anulare comanda
    std::cout << "\n>> Testare Anulare Comanda...\n";
    Comanda cmd_anulare(client);
    cmd_anulare.adaugaArticol(unitate_manual, 1);
    std::cout << "Stare initiala: " << cmd_anulare.getStare() << "\n";

    cmd_anulare.anuleazaComanda();
    std::cout << "Stare dupa anulare: " << cmd_anulare.getStare() << "\n";
}


int main() {
try {
    testProiect();
    testfunctiiavansate();

}catch (const std::exception& e) {
    std::cerr<<"Eroare critica main:"<<e.what() << std::endl;
    return 1;
}
    std::cout << "\n\n program finalizat cu suuces!\n";
    return 0;
};
