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
void ruleazaTesteleLibrarie() {

    Titlu("1. INITIALIZARE SISTEM  Autori, Carti, Clienti");

    auto autor1 = std::make_shared<Autor>("Mihai", "Eminescu", 75);
    auto autor2 = std::make_shared<Autor>("Albert", "Einstein", 120);

    Adresa adr_client("Bucuresti", "Sector 3", "Str. Exemplu 10", "011010");
    auto client = std::make_shared<Client>("Radu", "radu@email.com", adr_client, "0766123456");
    client->alimenteazaCont(5000);

    auto manual = std::make_shared<Manual>("Matematica M1", autor1,
        50.0, 100, 2023, "ISBN-MAN-001", 300, "Matematica", 12);

    auto carte_stiinta = std::make_shared<CarteStiintifia>("Relativitate", autor2,
        200, 1920, "ISBN-FIZ-001", 175.50, 500, "Fizica", "Cercetare", 10000, true);

    auto revista = std::make_shared<Revista>("Nature", nullptr,
        30, 2025, "ISSN-NAT", 20.0, 100, "Lunara", 200, true, "Stiintifica");



    Titlu("2. TESTARE UNITATI VANZARE Manual / SH / Pachet");

    auto unitate_manual = std::make_shared<CarteIndividuala>(manual, false, "Noua");
    auto unitate_stiintifica = std::make_shared<CarteIndividuala>(carte_stiinta, true, "Buna");

    std::vector<std::shared_ptr<Carte>> lista_pachet = { manual, manual };
    auto pachet = std::make_shared<PachetSerie>(lista_pachet, "Kitul Elevului", "Bacalaureat", true);



    Titlu("3. PROCESARE COMANDA  Adaugare, Finalizare, Recomandari");

    try {
        Comanda comanda(client);

        comanda.adaugaArticol(unitate_manual, 2);
        comanda.adaugaArticol(pachet, 1);

        std::cout << comanda;

        double total = comanda.finalizareComanda();
        std::cout << "Comanda finalizata! Total: " << total << " RON\n";

    } catch (const std::exception& e) {
        std::cout << "Exceptie comanda: " << e.what() << "\n";
    }



    Titlu("4. FUNCTIONALITATI AVANSATE  Relevanta, Popularitate, Colectibilitate");

    std::vector<std::string> preferinte = {"Informatica", "Fizica"};
    double scor = manual->CalculeazaRelevanta(12, preferinte, 2025);

    std::cout << "Relevanta manual: " << scor << "/100\n";

    manual->adauga_rating(5);
    auto manual2 = std::make_shared<Manual>("Test2", autor1, 100, 10,
        2020, "ISBN2", 100, "Info", 10);

    std::cout << "Comparatie popularitate: "
              << manual->cumparaDupaPopularitate(*manual2) << "\n";

    auto revista_veche = std::make_shared<Revista>("Times Vechi", nullptr, 100,
        2025, "ISSN-OLD", 10.0, 50, "Lunara", 500, false, "Istorie");

    revista_veche->adauga_rating(5);
    std::cout << "Revista veche este colectionabila? "
              << (revista_veche->este_colectionabila() ? "Da" : "Nu") << "\n";



    Titlu("5. TESTARE EXCEPTII SI FUNCTII AUXILIARE");

    try {
        throw StocInsuficientException("Demo Carte", 5, 10);
    } catch (const StocInsuficientException& e) {
        std::cout << "Diferenta stoc: " << e.getDiferenta() << "\n";
    }

    Adresa adr("A", "B", "C", "123456");
    adr.modificaAdreasa("X", "Y", "Z", "654321");

    auto client_test = std::make_shared<Client>("User", "u@u.com", adr, "000");
    client_test->adaugacumparaturi(3000);

    std::cout << "Discount personalizat client: "
              << client_test->calcdiscountpersonalizat() << "\n";

    auto unitate_test = std::make_shared<CarteIndividuala>(manual, false, "Noua");

    std::cout << "Zile detinere: " << unitate_test->calculeazaZileDetinere() << "\n";
    std::cout << "Data cumparare: " << unitate_test->getDataCumparareFormatata() << "\n";

    Comanda cmd(client_test);
    cmd.adaugaArticol(unitate_test, 1);
    std::cout << "Nr articole comanda: " << cmd.getNumarArticole() << "\n";

    std::vector<std::shared_ptr<Carte>> list;
    list.push_back(manual);
    auto pachet_extins = std::make_shared<PachetSerie>(list, "Pachet", "Tip", true);
    pachet_extins->adauga_volum(manual2);

    std::cout << "\nTESTE FINALIZATE CU SUCCES.\n";
}

int main() {
try {
    ruleazaTesteleLibrarie();

}catch (const std::exception& e) {
    std::cerr<<"Eroare critica main:"<<e.what() << std::endl;
    return 1;
}
    std::cout << "\n\n program finalizat cu suuces!\n";
    return 0;
};
