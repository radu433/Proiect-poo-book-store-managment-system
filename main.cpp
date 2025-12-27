#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include <algorithm>
#include <limits>
#include <ctime>

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
#include "files/headere/Publicatie.h"
#include"files/headere/Reducere.h"
#include "files/headere/RevistaIndividuala.h"
#include "files/headere/Review.h"
#include "files/headere/AppState.h"

// exceptii
#include <iomanip>
#include <random>

#include "files/exceptii/exceptii_headere/LibrarieException.h"
#include "files/exceptii/exceptii_headere/DateInvalideException.h"
#include "files/exceptii/exceptii_headere/StocException.h"
#include "files/exceptii/exceptii_headere/ClientExceptions.h"
#include "files/exceptii/exceptii_headere/ComandaExceptions.h"

struct StatPopularitate {
    double sumaCarte = 0, sumaManual = 0, sumaCS = 0;
    int nrCarte = 0, nrManual = 0, nrCS = 0;
};


// functii specifice pt review+ rating
void adaugaRaview(AppState &app, std::shared_ptr<Client> &clientCurent) {
}
// cautare publicatie cautare rapida(serchbar) + filtre
int filtrePublicatie(const AppState &app) {
    if (app.publicatii.empty()) {
        std::cout << "Nu exista publicatii.\n";
        return -1;
    }

    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- SELECTEAZA PUBLICATIE ---\n";
        std::cout << "1. Cauta dupa titlu\n";
        std::cout << "2. Filtreaza dupa tip\n";
        std::cout << "3. Filtreaza dupa autor\n";
        std::cout << "4. Afiseaza toate\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::vector<int> rezultate; // index real

        switch (opt) {
            case 1: {
                std::string cautare;
                std::cout << "Introdu titlu (sau parte din titlu): ";
                std::getline(std::cin, cautare);

                for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i) {
                    if (app.publicatii[i]->getTitlu().find(cautare) != std::string::npos) {
                        rezultate.push_back(i);
                    }
                }
                break;
            }
            case 2: {
                std::cout << "Tip (1-Carte 2-Manual 3-Stiintifica 4-Revista): ";
                int tip;
                std::cin >> tip;

                for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i) {
                    const auto &p = app.publicatii[i];
                    if (tip == 1 && std::dynamic_pointer_cast<Carte>(p) &&
                        !std::dynamic_pointer_cast<Manual>(p) &&
                        !std::dynamic_pointer_cast<CarteStiintifica>(p))
                        rezultate.push_back(i);
                    if (tip == 2 && std::dynamic_pointer_cast<Manual>(p))
                        rezultate.push_back(i);
                    if (tip == 3 && std::dynamic_pointer_cast<CarteStiintifica>(p))
                        rezultate.push_back(i);
                    if (tip == 4 && std::dynamic_pointer_cast<Revista>(p))
                        rezultate.push_back(i);
                }
                break;
            }
            case 3: {
                std::string nume;
                std::cout << "Introdu numele autorului: ";
                std::getline(std::cin, nume);

                for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i) {
                    if (auto c = std::dynamic_pointer_cast<Carte>(app.publicatii[i])) {
                        if (const Autor &autor = c->getAutor(); autor.getNume().find(nume) != std::string::npos ||
                                                                autor.getprenume().find(nume) != std::string::npos) {
                            rezultate.push_back(i);
                        }
                    }
                }
                break;
            }


            case 4: {
                for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i)
                    rezultate.push_back(i);
                break;
            }
            case 0:
                return -1;
            default:
                std::cout << "Optiune invalida.\n";
                continue;
        }

        if (rezultate.empty()) {
            std::cout << "Nu s-au gasit rezultate.\n";
            continue;
        }

        std::cout << "\n--- REZULTATE ---\n";
        for (int i = 0; i < static_cast<int>(rezultate.size()); ++i) {
            std::cout << "[" << i << "] "
                    << app.publicatii[rezultate[i]]->getTitlu() << "\n";
        }

        int alegere;
        std::cout << "Selecteaza index (-1 inapoi): ";
        std::cin >> alegere;

        if (alegere >= 0 && alegere < static_cast<int>(rezultate.size())) {
            return rezultate[alegere];
        }

        std::cout << "Index invalid.\n";
    }

    return -1;
}

int Serchbar(AppState &app) {
    std::string cautare;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "introduceti titlul cautat: \n";
    std::getline(std::cin, cautare);
    std::vector<int> rezultat;
    for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i) {
        if (app.publicatii[i]->getTitlu().find(cautare) != std::string::npos) {
            rezultat.push_back(i);
        }
    }
    if (rezultat.empty()) {
        std::cout << "Nu a fost gasita nici o publicatie!\n";
        return -1;
    }

    for (int i = 0; i < rezultat.size(); ++i) {
        std::cout << i << ". "
                << app.publicatii[rezultat[i]]->getTitlu()
                << "\n";
    }
    int idx;
    std::cout << "Selecteaza publicatia (-1 inapoi): ";
    std::cin >> idx;

    if (idx < 0 || idx >= (int) rezultat.size())
        return -1;

    return rezultat[idx];
}

void meniuCautaPulicatii(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== MENIU CAUTARE PUBLICATIE ===\n";
        std::cout << "1. SerchBar(catare rapida)\n ";
        std::cout << "2. Filtraza publicatie\n";
        std::cout << "0. Inapoi\n";
        std::cin >> opt;
        switch (opt) {
            case 1: Serchbar(app);
                break;
            case 2: filtrePublicatie(app);
                break;
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

// cautare autor
std::shared_ptr<Autor> cautaAutor(const AppState &app, const std::string &nume, const std::string &prenume) {
    for (const auto &a: app.autor) {
        if (a->getNume() == nume && a->getprenume() == prenume)
            return a;
    }
    return nullptr;
}

std::shared_ptr<Autor> selecteazaAutor(AppState &app) {
    int opt;
    std::cout << "\n--- AUTOR ---\n";
    std::cout << "1. Alege autor existent\n";
    std::cout << "2. Creeaza autor nou\n";
    std::cout << "0. Fara autor (Revista) \n";

    std::cin >> opt;

    if (opt == 1) {
        if (app.autor.empty()) {
            std::cout << "Nu exista in acest moment autori!\n";
            return nullptr;
        }
        for (size_t i = 0; i < app.autor.size(); ++i) {
            std::cout << i << "." << app.autor[i]->getNume() << " " << app.autor[i]->getprenume() << "\n";
        }
        std::cout << "Introduceti indexul autorului doretrit: \n";
        int idx;
        std::cin >> idx;

        if (idx < 0 || idx > app.autor.size())
            return nullptr;
        return app.autor[idx];
    }
    if (opt == 2) {
        std::string nume, prenume;
        int varsta;

        std::cin.ignore();
        std::cout << "Nume: ";
        std::getline(std::cin, nume);

        std::cout << "Prenume: ";
        std::getline(std::cin, prenume);

        if (const auto exista = cautaAutor(app, nume, prenume)) {
            std::cout << "Autorul exista deja! Indexul lui este :  " << std::distance(app.autor.begin(),
                std::ranges::find(app.autor, exista)) << "\n";
            return exista;
        }
        std::cout << "Varsta: ";
        std::cin >> varsta;

        auto autor = std::make_shared<Autor>(prenume, nume, varsta);
        app.autor.push_back(autor);
        return autor;
    }

    return nullptr;
}

// cautare client


int filtreazaClient(const AppState &app) {
    if (app.clienti.empty()) {
        std::cout << "Nu exista clienti.\n";
        return -1;
    }
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- CAUTARE CLIENT ---\n";
        std::cout << "1. Cauta dupa username\n";
        std::cout << "2. Cauta dupa email\n";
        std::cout << "3. Afiseaza toti clientii\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max());

        std::vector<int> rezultate;

        switch (opt) {
            case 1: {
                std::string user;
                std::cout << "Introduceti username-ul pe care il cautati:\n";
                std::getline(std::cin, user);
                for (size_t i = 0; i < app.clienti.size(); ++i) {
                    if (app.clienti[i]->getUsername().find(user) != std::string::npos)
                        rezultate.push_back(i);
                }
                break;
            }
            case 2: {
                std::string email;
                std::cout << "Introduceti email-ul pe care il cautati:\n";
                std::getline(std::cin, email);
                for (size_t i = 0; i < app.clienti.size(); ++i) {
                    if (app.clienti[i]->getEmail().find(email) != std::string::npos)
                        rezultate.push_back(i);
                }
                break;
            }
            case 3: {
                for (size_t i = 0; i < app.clienti.size(); ++i)
                    rezultate.push_back(i);
                break;
            }
            case 0: return -1;
            default:
                std::cout << "Optiune invalida!\n";
                continue;
        }
        if (rezultate.empty()) {
            std::cout << "Nu s-au gasit rezultate.\n";
            continue;
        }

        std::cout << "\n--- REZULTATE ---\n";
        for (size_t i = 0; i < rezultate.size(); ++i) {
            const auto &c = app.clienti[rezultate[i]];
            std::cout << "[" << i << "] "
                    << c->getUsername()
                    << " | " << c->getEmail()
                    << "\n";
        }

        int alegere;
        std::cout << "Selecteaza client (-1 inapoi): ";
        std::cin >> alegere;

        if (alegere >= 0 && alegere < static_cast<int>(rezultate.size()))
            return rezultate[alegere];

        std::cout << "Index invalid.\n";
    }
    return -1;
}

int filtrareComanda(const AppState &app) {
    if (app.comenzi.empty()) {
        std::cout << "Nu exista comenzi.\n";
        return -1;
    }

    std::cout << "\n=== LISTA COMENZI ===\n";
    for (size_t i = 0; i < app.comenzi.size(); ++i) {
        const auto &c = app.comenzi[i];
        std::cout << i << ". "
                << "ID: " << c->getId()
                << " | Data: " << c->getDataFormatata()
                << " | Articole: " << c->getNumarArticole()
                << " | Total: " << c->calculeazaTotal()
                << " lei\n";
    }

    int idx;
    std::cout << "Selecteaza comanda (-1 inapoi): ";
    std::cin >> idx;

    if (idx < 0 || idx >= static_cast<int>(app.comenzi.size()))
        return -1;

    return idx;
}


// functii pentru login si creare de cont + meniu autentificare
std::shared_ptr<Client> cautaClientIdentificator(
    const AppState &app,
    const std::string &input
) {
    for (const auto &c: app.clienti) {
        if (!c) continue;

        if (c->getEmail() == input ||
            c->getTelefon() == input ||
            c->getUsername() == input) {
            return c;
        }
    }
    return nullptr;
}

void creareContClient(AppState &app) {
    std::string username, email, telefon, parola;
    std::string judet, oras, strada, cod_postal;
    int numar = 0;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n=== CREARE CONT CLIENT ===\n";

    std::cout << "Username: ";
    std::getline(std::cin, username);

    std::cout << "Email: ";
    std::getline(std::cin, email);

    std::cout << "Telefon: ";
    std::getline(std::cin, telefon);

    // verificare unicitate
    if (cautaClientIdentificator(app, email) ||
        cautaClientIdentificator(app, telefon) ||
        cautaClientIdentificator(app, username)) {
        std::cout << "Exista deja un cont cu aceste date!\n";
        return;
    }

    std::cout << "Parola: ";
    std::getline(std::cin, parola);

    // Adresa
    std::cout << "Judet: ";
    std::getline(std::cin, judet);

    std::cout << "Oras: ";
    std::getline(std::cin, oras);

    std::cout << "Strada: ";
    std::getline(std::cin, strada);

    std::cout << "Numar: ";
    std::cin >> numar;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Cod postal: ";
    std::getline(std::cin, cod_postal);

    try {
        Adresa adresa(judet, oras, strada, numar, cod_postal);

        auto client = std::make_shared<Client>(username, email, adresa, telefon, parola);

        client->seteazaParola(parola);

        app.clienti.push_back(client);

        app.logs.push_back({
            time(nullptr),
            Tiplog::CONT_CREAT,
            email,
            "Cont client creat"
        });

        std::cout << "Cont creat cu succes! Va rugam sa va autentificati.\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}


std::shared_ptr<Client> autentificareClient(AppState &app) {
    int opt = -1;
    std::cout << "\n=== CLIENT ===\n";
    std::cout << "1. Login\n";
    std::cout << "2. Creeaza cont\n";
    std::cout << "0. Inapoi\n";
    std::cin >> opt;

    if (opt == 0) return nullptr;

    if (opt == 2) {
        creareContClient(app);
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string identificator;
    std::cout << "Intrpduceti Email/Nr. telefon/Username: \n";
    std::getline(std::cin, identificator);

    std::string parola;
    std::cout << "Inroduceti parola: \n";
    std::getline(std::cin, parola);

    auto client = cautaClientIdentificator(app, identificator);
    if (!client) {
        std::cout << "Client inexistent!\n";
        return nullptr;
    }
    int incercari = 0;
    while (incercari < 5) {
        std::string parola_introdusa;
        std::cout << "Parola (0 pentru iesire): ";
        std::cin >> parola_introdusa;

        if (parola_introdusa == "0")
            return nullptr;

        if (client->verificaParola(parola_introdusa)) {
            std::cout << "Autentificare reusita!\n";
            return client;
        }

        incercari++;
        std::cout << "Parola gresita. Reintroduceti parola!\n";
    }
    app.logs.push_back({
        time(nullptr), Tiplog::INCERCARI_MULTIPLE_AUTENTIFICARE_ESUATE, client->getEmail(),
        "5 incercari esuate de autentificare"
    });
    return nullptr;
}

// functii pt adaugat carte individuala + pachet serie in cos

void adaugaCarteIndividuala(AppState &app, std::shared_ptr<Publicatie> pubSelectata,
                            std::shared_ptr<Client> &clientCurent
                            , std::shared_ptr<Comanda> &comandaActiva) {
    if (!pubSelectata) {
        std::cout << "Publicatie invalida!\n";
        return;
    }

    int cantitate;
    std::cout << "Cantitate: ";
    std::cin >> cantitate;

    if (cantitate <= 0) {
        std::cout << "Cantitate invalida!\n";
        return;
    }

    std::shared_ptr<UnitateVanzare> unitate = nullptr;

    if (auto carte = std::dynamic_pointer_cast<Carte>(pubSelectata)) {
        int optSecondHand;
        std::cout << "Cartea este second-hand?\n";
        std::cout << "1. Da\n";
        std::cout << "2. Nu\n";
        std::cin >> optSecondHand;

        bool esteSecondHand = (optSecondHand == 1);
        std::string conditie = "Noua";

        if (esteSecondHand) {
            std::cout << "Conditie fizica (Noua / Buna / Acceptabila / Uzata): ";
            std::cin >> conditie;
        }

        unitate = std::make_shared<CarteIndividuala>(
            carte,
            esteSecondHand,
            conditie
        );
    } else if (auto revista = std::dynamic_pointer_cast<Revista>(pubSelectata)) {
        unitate = std::make_shared<RevistaIndividuala>(revista);
    } else {
        std::cout << "Publicatia selectata nu poate fi adaugata individual!\n";
        return;
    }
    if (!comandaActiva) {
        comandaActiva = std::make_shared<Comanda>(clientCurent);
    }
    try {
        comandaActiva->adaugaArticol(unitate, cantitate);
        std::cout << "Articol adaugat in cos.\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

void adaugaPachet(AppState &app, std::shared_ptr<Client> &clientCurent, std::shared_ptr<Comanda> &comandaActiva) {
    int opt = -1;
    std::cout << "\nDoriti sa adaugati un pachet prestabilit sau sa creati unul?\n";
    std::cout << "1. Creati un pachet\n";
    std::cout << "2. Pachet prestabilit\n";
    std::cout << "0. Inapoi\n";
    std::cin >> opt;
    std::shared_ptr<UnitateVanzare> unitate = nullptr;

    if (opt == 1) {
        int nrvolume = 0;
        std::cout << "Numarul de volume in pachet: \n";
        std::cin >> nrvolume;
        if (nrvolume <= 1) {
            std::cout << "Numar invalid de volume!(cel putin doua)\n";
            return;
        }
        std::vector<std::shared_ptr<Publicatie> > continut;

        for (int i = 0; i < nrvolume; ++i) {
            int idx = 0;
            std::cout << "\n Selecteaza publicatia " << (i + 1) << ":\n";
            int p = -1;
            std::cout << "Motor de cautare:\n1.Serchbar\n2.FiltrePublicatie\n";
            std::cin >> p;
            if (p == 1)
                idx = Serchbar(app);
            if (p == 2)
                idx = filtrePublicatie(app);

            if (idx < 0 || idx >= static_cast<int>(app.publicatii.size())) {
                std::cout << "Selectie invalida!\n";
                return;
            }

            continut.push_back(app.publicatii[idx]);
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::string numePachet, tipPachet;
        std::cout << "Nume pachet: ";
        std::cin >> numePachet;
        std::cout << "Tip pachet: ";
        std::cin >> tipPachet;

        bool esteComplet = true;

        unitate = std::make_shared<PachetSerie>(
            continut,
            numePachet,
            tipPachet,
            esteComplet
        );
    }

    if (opt == 2) {
        if (app.pachetePredefinite.empty()) {
            std::cout << "Nu exista pachete prestabilite!\n";
            return;
        }

        std::cout << "\n=== PACHETE DISPONIBILE ===\n";
        for (size_t i = 0; i < app.pachetePredefinite.size(); ++i) {
            std::cout << i << ". "
                    << app.pachetePredefinite[i]->getDescriere()
                    << "\n";
        }

        int idx;
        std::cout << "Alege pachet: ";
        std::cin >> idx;

        if (idx < 0 || idx >= (int) app.pachetePredefinite.size()) {
            std::cout << "Index invalid!\n";
            return;
        }

        unitate = app.pachetePredefinite[idx]->clone();
    } else {
        return;
    }
    if (!comandaActiva) {
        comandaActiva = std::make_shared<Comanda>(clientCurent);
    }
    try {
        comandaActiva->adaugaArticol(unitate, 1);
        std::cout << "Pachet adaugat in cos.\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

// stergere din cos a unui produs sau pachetet
void StergeDinCos(AppState &app, std::shared_ptr<Client> &clientCurent, std::shared_ptr<Comanda> &comandaActiva) {
    if (!comandaActiva || comandaActiva->getNumarArticole() == 0) {
        std::cout << "Cosul este gol!\n";
        return;
    }
    std::cout << "=== COS CURENT ===\n";
    std::cout << *comandaActiva << "\n";

    int idx = 0;
    std::cout << "Introduceti indexul articolului de sters :\n";
    std::cin >> idx;
    try {
        comandaActiva->stergeArticol(idx);
        std::cout << "Articolul a fost sters  din cos.\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

// functie pt review si rating
void AdaugaReview_Rating();

double calculeazaPretFinalCuReduceri(const AppState &app, const std::shared_ptr<Publicatie> &p) {
    double pret = p->getPretFinal();

    for (const auto &r: app.reduceri) {
        if (r.seAplicaLa(p)) {
            pret -= pret * r.getProcent() / 100.0;
            break; // o singură reducere
        }
    }
    return pret;
}

void afiseazaReduceri(const AppState &app) {
    std::cout << "\n--- REDUCERI ACTIVE ---\n";

    for (const auto &r: app.reduceri) {
        if (r.esteActiva()) {
            std::cout << r.getProcent() << "% reducere\n";
        }
    }
}

void topClienti(const AppState &app) {
    if (app.clienti.empty()) {
        std::cout << "Nu exista clienti.\n";
        return;
    }

    auto lista = app.clienti;

    std::ranges::sort(lista, [](const auto &a, const auto &b) {
        return a->getTotalCumparaturi() > b->getTotalCumparaturi();
    });

    std::cout << "\n=== TOP CLIENTI DUPA SUMA CHELTUITA ===\n";
    int poz = 1;
    for (const auto &c: lista) {
        std::cout << poz++ << ". "
                << c->getEmail()
                << " | Total cheltuit: "
                << c->getTotalCumparaturi()
                << " lei | Ierarhie: "
                << c->ierarhie_clienti()
                << "\n";
    }
}

void aplicareReducerePublicatie(AppState &app) {
    if (app.publicatii.empty()) return;

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    int saptamana = now->tm_yday / 7;
    int zi_reducere = saptamana % 7;

    int procent = std::vector<int>{5, 10, 15}[saptamana % 3];

    enum Tip { CARTE, MANUAL, REVISTA, STIINTIFICA };
    const auto tip = static_cast<Tip>(saptamana % 4);

    std::vector<std::shared_ptr<Publicatie> > candidati;

    for (const auto &p: app.publicatii) {
        if (tip == CARTE &&
            std::dynamic_pointer_cast<Carte>(p) &&
            !std::dynamic_pointer_cast<Manual>(p) &&
            !std::dynamic_pointer_cast<CarteStiintifica>(p))
            candidati.push_back(p);

        else if (tip == MANUAL &&
                 std::dynamic_pointer_cast<Manual>(p))
            candidati.push_back(p);

        else if (tip == REVISTA &&
                 std::dynamic_pointer_cast<Revista>(p))
            candidati.push_back(p);

        else if (tip == STIINTIFICA &&
                 std::dynamic_pointer_cast<CarteStiintifica>(p))
            candidati.push_back(p);
    }

    if (candidati.empty()) return;

    std::mt19937 gen(saptamana);
    std::ranges::shuffle(candidati, gen);

    const int nr = std::min(4, static_cast<int>(candidati.size()));

    app.reduceri.clear(); // doar campania curentă

    std::time_t start = t;
    std::time_t sfarsit = t + 24 * 60 * 60; // 1 zi
    for (int i = 0; i < nr; ++i) {
        app.reduceri.emplace_back(
            candidati[i], procent, start, sfarsit
        );
    }
}

void adaugaRatingPublicatie(const AppState &app, int idx) {
    idx = filtrePublicatie(app);
    if (idx == -1) {
        std::cout << "Publicatia nu exista!\n";
        return;
    }
    int rating;
    while (true) {
        std::cout << "Introducei ratin (1-5): \n";
        std::cin >> rating;

        if (rating >= 1 && rating >= 5)
            break;
        std::cout << "Rating invalid. Introduceti din nou!\n";
    }
    app.publicatii[idx]->adauga_rating(rating);
    std::cout << "Rating adaugat cu succes!\n";
    std::cout << "Rating mediu actual: "
            << app.publicatii[idx]->getRatingMediu() << "\n";
}


void topPopularitateoverall(const AppState &app) {
    std::vector<std::shared_ptr<Carte> > carti;

    for (const auto &p: app.publicatii) {
        if (auto c = std::dynamic_pointer_cast<Carte>(p))
            carti.push_back(c);
        if (carti.empty()) {
            std::cout << "Nu exista carti " << std::endl;
            return;
        }
    }
    std::ranges::sort(carti,
                      [](const auto &a, const auto &b) {
                          return a->CalculeazaScorPopularitate() >
                                 b->CalculeazaScorPopularitate();
                      });

    std::cout << "\n--- TOP 3 PUBLICATII ---\n";

    for (size_t i = 0; i < std::min<size_t>(3, carti.size()); ++i) {
        std::cout << i + 1 << ". ";
        carti[i]->afisare(std::cout);
        std::cout << " | Scor: "
                << carti[i]->CalculeazaScorPopularitate() << "\n";
    }
}

template<typename T>
void topPopularitatePeTip(const AppState &app, const std::string &numeTip) {
    std::vector<std::shared_ptr<T> > lista;

    for (const auto &p: app.publicatii) {
        if (auto t = std::dynamic_pointer_cast<T>(p))
            lista.push_back(t);
    }

    if (lista.empty()) {
        std::cout << "Nu exista publicatii de tip " << numeTip << ".\n";
        return;
    }

    std::sort(lista.begin(), lista.end(),
              [](const auto &a, const auto &b) {
                  return a->CalculeazaScorPopularitate() >
                         b->CalculeazaScorPopularitate();
              });

    std::cout << "\n--- TOP 5 " << numeTip << " ---\n";

    for (size_t i = 0; i < std::min<size_t>(5, lista.size()); ++i) {
        std::cout << i + 1 << ". ";
        lista[i]->afisare(std::cout);
        std::cout << " | Scor: "
                << lista[i]->CalculeazaScorPopularitate() << "\n";
    }
}

void meniuTopPopularitatePeTip(const AppState &app) {
    int opt = -1;

    while (opt != 0) {
        std::cout << "\n--- TOP PE TIP ---\n";
        std::cout << "1. Carte\n";
        std::cout << "2. Manual\n";
        std::cout << "3. Carte stiintifica\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1:
                topPopularitatePeTip<Carte>(app, "Carte");
                break;
            case 2:
                topPopularitatePeTip<Manual>(app, "Manual");
                break;
            case 3:
                topPopularitatePeTip<CarteStiintifica>(app, "Carte stiintifica");
                break;
            case 0:
                return;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void statisticaDistributiePopularitate(const AppState &app) {
    int carti = 0, manuale = 0, cs = 0, reviste = 0;
    for (const auto &p: app.publicatii) {
        if (std::dynamic_pointer_cast<Carte>(p)) carti++;
        if (std::dynamic_pointer_cast<CarteStiintifica>(p)) cs++;
        if (std::dynamic_pointer_cast<Manual>(p)) manuale++;
        if (std::dynamic_pointer_cast<Revista>(p)) reviste++;
    }
    std::cout << "Carti:" << carti << std::endl;
    std::cout << "Manuale:" << manuale << std::endl;
    std::cout << "Carti stiintifice:" << cs << std::endl;
    std::cout << "Reviste:" << reviste << std::endl;
}

StatPopularitate statisticaDetaliata(const AppState &app) {
    StatPopularitate s;
    for (const auto &p: app.publicatii) {
        if (auto m = std::dynamic_pointer_cast<Manual>(p)) {
            s.sumaManual += m->CalculeazaScorPopularitate();
            s.nrManual++;
        } else if (auto cs = std::dynamic_pointer_cast<CarteStiintifica>(p)) {
            s.sumaCS += cs->CalculeazaScorPopularitate();
            s.nrCS++;
        } else if (auto c = std::dynamic_pointer_cast<Carte>(p)) {
            s.sumaCarte += c->CalculeazaScorPopularitate();
            s.nrCarte++;
        }
    }

    return s;
}

void meniuStatisticaDistributieTip(const AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- STATISTICI PUBLICATII ---\n";
        std::cout << "1. Statistica popularitate generala \n";
        std::cout << "2. Statistica popularitate impartita pe tipuri\n";
        std::cout << "0. Inapoi\n";
        std::cin >> opt;
        StatPopularitate s = statisticaDetaliata(app);
        int total = s.nrCarte + s.nrManual + s.nrCS;

        if (total == 0) {
            std::cout << "Nu exista carti.\n";
            continue;
        }
        switch (opt) {
            case 1: {
                const double medie =
                        (s.sumaCarte + s.sumaManual + s.sumaCS) / total;

                std::cout << "Scor mediu global: " << medie << "\n";
                break;
            }
            case 2: {
                if (s.nrCarte > 0)
                    std::cout << "Carte: "
                            << s.sumaCarte / s.nrCarte << "\n";
                else
                    std::cout << "Carte: N/A\n";

                if (s.nrManual > 0)
                    std::cout << "Manual: "
                            << s.sumaManual / s.nrManual << "\n";
                else
                    std::cout << "Manual: N/A\n";

                if (s.nrCS > 0)
                    std::cout << "Carte stiintifica: "
                            << s.sumaCS / s.nrCS << "\n";
                else
                    std::cout << "Carte stiintifica: N/A\n";

                break;
            }
            case 0: {
                break;
            }
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void meniuTopPublicatiiPopularitate(const AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n --- PREZENTAREA TOPURILOR ---/n";
        std::cout << "1. Top overall\n";
        std::cout << "2. Top publicatii pe tip\n";
        std::cout << "0. Inapoi\n";
        std::cin >> opt;
        switch (opt) {
            case 1: {
                topPopularitateoverall(app);
                break;
            }
            case 2: {
                meniuTopPopularitatePeTip(app);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "optiune Invalida!\n";
        }
    }
}


void afisareDetaliiPublicatie(const AppState &app, int idx) {
    if (idx < 0 || idx >= static_cast<int>(app.publicatii.size())) {
        std::cout << "Index invalid.\n";
        return;
    }

    auto p = app.publicatii[idx];
    std::cout << "\n=============================\n";


    if (auto m = std::dynamic_pointer_cast<Manual>(p)) {
        std::cout << "====Manual==== \n";
        std::cout << "Titlu: " << m->getTitlu() << "\n";
        std::cout << "Tip: Manual\n\n";

        std::cout << "Materie: " << m->getmaterie() << "\n";
        std::cout << "Clasa: " << m->getclasa() << "\n";
        std::cout << "Ciclu scolar: " << m->getCicluScolar() << "\n";
        std::cout << "Prioritate restoc: "
                << m->calculeazaPrioritateRestoc() << "\n";

        std::cout << "Autor: "
                << m->getAutor().getNume() << " "
                << m->getAutor().getprenume() << "\n";

        std::cout << "Editura: " << m->geteditura() << "\n";
        std::cout << "Data aparitie: " << m->getdata_publicatie() << "\n";
        std::cout << "Numar pagini: " << m->getnr_pagini() << "\n";

        double pretFinal = calculeazaPretFinalCuReduceri(app, p);
        std::cout << "Pret final: " << pretFinal << " lei\n";
        std::cout << "Rating mediu: " << m->getRatingMediu() << "\n";
        std::cout << "Scor popularitate: "
                << m->CalculeazaScorPopularitate() << "\n";
        std::cout << "Timp estimat lectura: "
                << m->timp_estimat_lecturii() << " minute\n";
        std::cout << "Statut stoc: " << m->determinaStatutStoc() << "\n";
    } else if (auto cs = std::dynamic_pointer_cast<CarteStiintifica>(p)) {
        std::cout << "====Carte Stiintifica==== \n";
        std::cout << "Titlu: " << cs->getTitlu() << "\n";
        std::cout << "Tip: Carte stiintifica\n\n";

        std::cout << "Domeniu: " << cs->getDomeniu() << "\n";
        std::cout << "Nivel academic: " << cs->getnr_pagini() << "\n";
        std::cout << "Factor impact: "
                << cs->calculeazaFactorImpact() << "\n";
        std::cout << "Este de actualitate: "
                << (cs->esteDeActualitate() ? "Da" : "Nu") << "\n";

        std::cout << "Autor: "
                << cs->getAutor().getNume() << " "
                << cs->getAutor().getprenume() << "\n";

        std::cout << "Editura: " << cs->geteditura() << "\n";
        std::cout << "Data aparitie: " << cs->getdata_publicatie() << "\n";
        std::cout << "Numar pagini: " << cs->getnr_pagini() << "\n";

        std::cout << "Pret final: " << cs->getPretFinal() << " lei\n";
        std::cout << "Rating mediu: " << cs->getRatingMediu() << "\n";
        std::cout << "Scor popularitate: "
                << cs->CalculeazaScorPopularitate() << "\n";
        std::cout << "Timp estimat lectura: "
                << cs->timp_estimat_lecturii() << " minute\n";
        std::cout << "Statut stoc: " << cs->determinaStatutStoc() << "\n";
    } else if (auto r = std::dynamic_pointer_cast<Revista>(p)) {
        std::cout << "====Revista==== \n";
        std::cout << "Titlu: " << r->getTitlu() << "\n";
        std::cout << "Tip: Revista\n\n";

        std::cout << "ISSN: " << r->getIdentificator() << "\n";
        std::cout << "Frecventa: " << r->getFrecventa() << "\n";
        std::cout << "Editie speciala: "
                << (r->esteEditieSpeciala() ? "Da" : "Nu") << "\n";
        std::cout << "Colectionabila: "
                << (r->este_colectionabila() ? "Da" : "Nu") << "\n";

        std::cout << "Editura: " << r->geteditura() << "\n";
        std::cout << "Data aparitie: " << r->getdata_publicatie() << "\n";

        std::cout << "Pret final: " << r->getPretFinal() << " lei\n";
        std::cout << "Rating mediu: " << r->getRatingMediu() << "\n";
        std::cout << "Scor popularitate: "
                << r->CalculeazaScorPopularitate() << "\n";
        std::cout << "Statut stoc: " << r->determinaStatutStoc() << "\n";
    } else if (auto c = std::dynamic_pointer_cast<Carte>(p)) {
        std::cout << "====Carte==== \n";
        std::cout << "Titlu: " << c->getTitlu() << "\n";
        std::cout << "Tip: Carte\n\n";

        std::cout << "Valoare academica: "
                << c->calculeaza_valoarea_academica() << "\n";

        std::cout << "Autor: "
                << c->getAutor().getNume() << " "
                << c->getAutor().getprenume() << "\n";

        std::cout << "Editura: " << c->geteditura() << "\n";
        std::cout << "Data aparitie: " << c->getdata_publicatie() << "\n";
        std::cout << "Numar pagini: " << c->getnr_pagini() << "\n";

        std::cout << "Pret final: " << c->getPretFinal() << " lei\n";
        std::cout << "Rating mediu: " << c->getRatingMediu() << "\n";
        std::cout << "Scor popularitate: "
                << c->CalculeazaScorPopularitate() << "\n";
        std::cout << "Timp estimat lectura: "
                << c->timp_estimat_lecturii() << " minute\n";
        std::cout << "Statut stoc: " << c->determinaStatutStoc() << "\n";
    }

    std::cout << "=============================\n";
}


void meniuStatisticiPublicatii(const AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- STATISTICI PUBLICTII ---\n";
        std::cout << "1. Numar total de publictii\n";
        std::cout << "2. Distributie pe tip\n";
        std::cout << "3. Top publicatii populare\n";
        std::cout << "0. Inapoi\n";
        std::cin >> opt;

        switch (opt) {
            case 1:
                std::cout << "1. Numar total de publicatii: " << app.publicatii.size() << std::endl;
                break;

            case 2: {
                meniuStatisticaDistributieTip(app);
                break;
            }

            case 3: {
                meniuTopPublicatiiPopularitate(app);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void afiseazaPrioritatiRestoc(const AppState &app) {
    if (app.publicatii.empty()) {
        std::cout << "Nu exista publicatii in sistem.\n";
        return;
    }

    // Copiem lista ca sa o putem sorta
    std::vector<std::shared_ptr<Publicatie> > lista = app.publicatii;

    std::ranges::sort(lista,
                      [](const std::shared_ptr<Publicatie> &a,
                         const std::shared_ptr<Publicatie> &b) {
                          return a->calculeazaPrioritateRestoc() >
                                 b->calculeazaPrioritateRestoc();
                      });

    std::cout << "\n=== PRIORITATI DE RESTOC ===\n\n";

    int poz = 1;
    for (const auto &p: lista) {
        std::cout << poz++ << ". "
                << p->getTitlu()
                << " | Tip: " << p->getTip()
                << " | Stoc: " << p->getcantitate()
                << " | Prioritate: "
                << std::fixed << std::setprecision(2)
                << p->calculeazaPrioritateRestoc()
                << "\n";
    }



    std::vector<std::shared_ptr<Publicatie> > lista = app.publicatii;

    std::ranges::sort(lista,
                      [](const auto &a, const auto &b) {
                          return a->calculeazaPrioritateRestoc() >
                                 b->calculeazaPrioritateRestoc();
                      });

    int n;
    std::cout << "Cate publicatii vrei sa restochezi (top N): ";
    std::cin >> n;

    int cant;
    std::cout << "Cantitate adaugata per publicatie: ";
    std::cin >> cant;

    for (int i = 0; i < n && i < static_cast<int>(lista.size()); ++i) {
        lista[i]->adauga_stoc(cant);
        std::cout << "Restocat: " << lista[i]->getTitlu() << "\n";
    }
}

void meniuPrioritatiRestoc(const AppState &app) {
    int opt = -1;

    while (opt != 0) {
        std::cout << "\n=== PRIORITATI RESTOC ===\n";
        std::cout << "1. Afiseaza lista prioritati\n";
        std::cout << "2. Restocheaza publicatie\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1:
                afiseazaPrioritatiRestoc(app);
                break;

            case 2:
                restocheazaDinPrioritati(app);
                break;

            case 0: { break; }
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void modificaStocPublicatie(const AppState &app, int idx) {
    idx = filtrePublicatie(app);
    if (idx == -1) {
        std::cout << "Publicatia nu exista!";
        return;
    }
    auto pub = app.publicatii[idx];
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- MODIFICARE STOC ---\n";
        std::cout << "Titlu: " << pub->getTitlu() << "\n";
        std::cout << "Stoc curent: " << pub->getcantitate() << "\n";
        std::cout << "1. Adauga stoc\n";
        std::cout << "2. Scade stoc\n";
        std::cout << "3. Statistica prioritate restoc\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                int cantitate;

                std::cout << "Cantitate de adaugat:\n";
                std::cin >> cantitate;
                try {
                    pub->adauga_stoc(cantitate);
                    std::cout << "Stoc actualizat! Nou stoc: "
                            << pub->getcantitate() << "\n";
                } catch (const std::exception &e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }
                break;
            }
            case 2: {
                int cantitate;
                std::cout << "Cantitate de scazut: ";
                std::cin >> cantitate;

                try {
                    if (!pub->Scade_stoc(cantitate)) {
                        std::cout << "Stoc insuficient! Disponibil: "
                                << pub->getcantitate() << "\n";
                    } else {
                        std::cout << "Stoc actualizat! Nou stoc: "
                                << pub->getcantitate() << "\n";
                    }
                } catch (const std::exception &e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }
                break;
            }
            case 3: {
                meniuPrioritatiRestoc(app);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void cloneazaPublicatie(AppState &app, int idx) {
    idx = filtrePublicatie(app);
    if (idx == -1) {
        std::cout << "Publicatia nu exista!\n";
        return;
    }

    auto original = app.publicatii[idx];

    std::cout << "\n--- CLONARE PUBLICATIE ---\n";
    std::cout << "Se cloneaza: " << original->getTitlu() << "\n";
    std::cout << "Tip: " << original->getTip() << "\n";
    std::cout << "Stoc original: " << original->getcantitate() << "\n";

    char confirm;
    std::cout << "Confirmi clonarea? (y/n): ";
    std::cin >> confirm;

    if (confirm != 'y' && confirm != 'Y') {
        std::cout << "Clonare anulata.\n";
        return;
    }

    auto copie = original->clone();

    // Reset logic stoc
    int stoc_nou;
    std::cout << "Introdu stocul initial pentru clona: ";
    std::cin >> stoc_nou;

    if (stoc_nou > 0) {
        copie->adauga_stoc(stoc_nou);
    }

    app.publicatii.push_back(copie);

    std::cout << "Publicatie clonata cu succes!\n";
    std::cout << "Total publicatii in sistem: "
            << app.publicatii.size() << "\n";
}

void meniuOperatiiPublicatie(AppState &app, int idx) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- OPERATII PUBLICATIE ---\n";
        std::cout << "1. Afisare detalii\n"
        std::cout << "2.Adauga rating\n";
        std::cout << "3.Modifica stoc\n";
        std::cout << "4.Cloneaza publicatie\n";
        std::cout << "0.Iesire\n";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                afisareDetaliiPublicatie(app, idx);
                break;
            }

            case 2: {
                adaugaRatingPublicatie(app, idx);
                break;
            }

            case 3: {
                modificaStocPublicatie(app, idx);
                break;
            }
            case 4: {
                cloneazaPublicatie(app, idx);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}


void adaugaCarte(AppState &app) {
    try {
        std::string titlu, isbn, editura, data_publicatie;
        int nr_pagini, cantitate;
        double pret_baza;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Titlu: ";
        std::getline(std::cin, titlu);

        std::cout << "ISBN: ";
        std::getline(std::cin, isbn);

        std::cout << "Editura: ";
        std::getline(std::cin, editura);

        std::cout << "Numar pagini: ";
        std::cin >> nr_pagini;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Data aparitie (dd.mm.yyyy): ";
        std::getline(std::cin, data_publicatie);

        std::cout << "Pret de baza: ";
        std::cin >> pret_baza;

        std::cout << "Cantitate: ";
        std::cin >> cantitate;

        auto autor = selecteazaAutor(app);

        const auto carte = std::make_shared<Carte>(titlu, autor, cantitate, data_publicatie, isbn, pret_baza, nr_pagini,
                                                   editura);

        app.publicatii.push_back(carte);

        if (autor) {
            autor->adauga_carte(isbn);
        }

        std::cout << "Carte adaugata cu succes!\n";
        std::cout << "Pret final calculat: "
                << carte->getPretFinal() << "\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

void adaugaManula(AppState &app) {
    try {
        std::string titlu, isbn, editura, data_publicatie, materie;
        int clasa, nr_pagini, cantitate;
        double pret_baza;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Titlu: ";
        std::getline(std::cin, titlu);

        std::cout << "ISBN: ";
        std::getline(std::cin, isbn);

        std::cout << "Editura: ";
        std::getline(std::cin, editura);

        std::cout << "Numar pagini: ";
        std::cin >> nr_pagini;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Data aparitie (dd.mm.yyyy): ";
        std::getline(std::cin, data_publicatie);

        std::cout << "Pret de baza: ";
        std::cin >> pret_baza;

        std::cout << "Cantitate: ";
        std::cin >> cantitate;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Materie: ";
        std::getline(std::cin, materie);

        std::cout << "Clasa: ";
        std::cin >> clasa;
        auto autor = selecteazaAutor(app);

        const auto carte = std::make_shared<Manual>(titlu, autor, pret_baza, cantitate, data_publicatie,
                                                    isbn, nr_pagini, editura, materie, clasa);

        app.publicatii.push_back(carte);

        if (autor) {
            autor->adauga_carte(isbn);
        }

        std::cout << "Manual adaugat cu succes!\n";
        std::cout << "Pret final calculat: "
                << carte->getPretFinal() << "\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

void adaugaCarteStiintifica(AppState &app) {
    try {
        std::string titlu, isbn, editura, data_publicatie, domeniu, nivel_academic;
        int cantitate, nr_pagini, nr_referinte;
        double pret_baza;
        std::string raspuns;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Titlu: ";
        std::getline(std::cin, titlu);

        std::cout << "Domeniul: ";
        std::getline(std::cin, domeniu);

        std::cout << "Nivel academic (  Liceu / Universitar / Cercetare): ";
        std::getline(std::cin, nivel_academic);

        std::cout << "Numar Referinte: ";
        std::cin >> nr_referinte;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Contine formule/grafice? (da/nu): ";
        std::getline(std::cin, raspuns);

        std::ranges::transform(raspuns,
                               raspuns.begin(), ::tolower);

        bool are_formule_grafice = (raspuns == "da");

        std::cout << "ISBN: ";
        std::getline(std::cin, isbn);

        std::cout << "Editura: ";
        std::getline(std::cin, editura);

        std::cout << "Numar pagini: ";
        std::cin >> nr_pagini;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Data aparitie (dd.mm.yyyy): ";
        std::getline(std::cin, data_publicatie);

        std::cout << "Pret de baza: ";
        std::cin >> pret_baza;

        std::cout << "Cantitate: ";
        std::cin >> cantitate;

        auto autor = selecteazaAutor(app);

        const auto carte = std::make_shared<CarteStiintifica>(
            titlu, autor, cantitate, data_publicatie, isbn,
            pret_baza, nr_pagini, editura,
            domeniu, nivel_academic, nr_referinte, are_formule_grafice
        );

        app.publicatii.push_back(carte);

        if (autor) {
            autor->adauga_carte(isbn);
        }

        std::cout << "Cartea stiintifica adaugata cu succes!\n";
        std::cout << "Pret final calculat: "
                << carte->getPretFinal() << "\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

void adaugaRevista(AppState &app) {
    try {
        std::string titlu, data_publicatie, frecventa, tip, ISSN, editura, raspuns;
        int cantitate, nr_pagini, numar_editie;
        double pret_baza;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Titlu: ";
        std::getline(std::cin, titlu);

        std::cout << "Data aparitie (dd.mm.yyyy): ";
        std::getline(std::cin, data_publicatie);

        std::cout << "Numar pagini: ";
        std::cin >> nr_pagini;

        std::cout << "Pret de baza: ";
        std::cin >> pret_baza;

        std::cout << "Cantitate: ";
        std::cin >> cantitate;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Frecventa (Zilnica / Saptamanala / Lunara): ";
        std::getline(std::cin, frecventa);

        std::cout << "Numar editie: ";
        std::cin >> numar_editie;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Are cadou suplimentar? (da/nu): ";
        std::getline(std::cin, raspuns);

        std::ranges::transform(raspuns, raspuns.begin(), ::tolower);
        bool are_cadou_suplimentar = (raspuns == "da");

        std::cout << "Tip revista: ";
        std::getline(std::cin, tip);

        std::cout << "ISSN: ";
        std::getline(std::cin, ISSN);

        std::cout << "Editura: ";
        std::getline(std::cin, editura);

        auto revista = std::make_shared<Revista>(titlu, cantitate, data_publicatie, nr_pagini, pret_baza, frecventa,
                                                 numar_editie,
                                                 are_cadou_suplimentar, tip, ISSN, editura);

        app.publicatii.push_back(revista);

        std::cout << "Revista adaugata cu succes!\n";
        std::cout << "Pret final calculat: "
                << revista->getPretFinal() << "\n";
    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

void meniuAdaugaPublicatii(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- MENIU ADAUGARE PUBLICATII ---\n";
        std::cout << "1. Adauga Carte\n";
        std::cout << "2. Adauga Manual\n";
        std::cout << "3. Adauga Carte Stiintifica\n";
        std::cout << "4. Adauga Revista\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        switch (opt) {
            case 1: {
                adaugaCarte(app);
                break;
            }
            case 2: {
                adaugaManula(app);
                break;
            }
            case 3: {
                adaugaCarteStiintifica(app);
                break;
            }
            case 4: {
                adaugaRevista(app);
                break;
            }
            case 0: {
                break;
            }
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void meniuPublicatii(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- MENIU PUBLICATII ---\n";
        std::cout << "1. Lista publicatii\n";
        std::cout << "2. Adauga publicatie\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        switch (opt) {
            case 1: {
                if (const int idx = filtrePublicatie(app); idx != -1)
                    meniuOperatiiPublicatie(app, idx);
                break;
            }
            case 2: {
                meniuAdaugaPublicatii(app);
                break;
            }
            case 0: break;

            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void DetaliiAutor(AppState &app) {
    auto autor = selecteazaAutor(app);
    if (!autor) {
        std::cout << "Autorul nu exista!\n";
        return;
    }
    std::cout << "\n=== AUTOR ===\n";
    std::cout << autor->getNume() << " "
            << autor->getprenume() << "\n";
    std::cout << "Varsta: " << autor->getvarsta() << "\n";
    std::cout << "Carti scrise: "
            << autor->getcarti_scrise() << "\n";
    std::cout << "Ierarhie: "
            << autor->ierarhie_a() << "\n";
    std::cout << "Productivitate: "
            << autor->calcproductivitate() << "\n";
    std::cout << "Scor renume: "
            << autor->calcscorrenume() << "\n";
}

int prioritateIerarhie(const std::string &i) {
    if (i == "Legendar") return 5;
    if (i == "Renumit") return 4;
    if (i == "Consacrat") return 3;
    if (i == "In ascensiune") return 2;
    return 1; // Incepator
}

bool cmpAutoriIerarhie(const std::shared_ptr<Autor> &a,
                       const std::shared_ptr<Autor> &b) {
    int pa = prioritateIerarhie(a->ierarhie_a());
    int pb = prioritateIerarhie(b->ierarhie_a());

    if (pa != pb)
        return pa > pb;

    return a->calcscorrenume() > b->calcscorrenume();
}

void topAutori(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- TOP AUTORI ---\n";
        std::cout << "1. Top productivitate \n";
        std::cout << "2. Top scor renume \n";
        std::cout << "3. Afisare ierarhie";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                if (app.autor.emplace_back()) {
                    std::cout << "Nu exista autori!\n";
                    return;
                }
                auto lista = app.autor;
                std::ranges::sort(lista, [](const auto &a, const auto &b) {
                    return a->calcproductivitate() > b->calcproductivitate();
                });
                std::cout << "\n=== TOP AUTORI DUPA PRODUCTIVITATE ===\n";
                int poz = 1;
                for (const auto &a: lista) {
                    std::cout << poz++ << ". "
                            << a->getNume() << " "
                            << a->getprenume()
                            << " | Productivitate: "
                            << a->calcproductivitate()
                            << "\n";
                }
                break;
            }
            case 2: {
                if (app.autor.emplace_back()) {
                    std::cout << "Nu exista autori!\n";
                    return;
                }
                auto lista = app.autor;
                std::ranges::sort(lista, [](const auto &a, const auto &b) {
                    return a->calcscorrenume() > b->calcscorrenume();
                });
                std::cout << "\n=== TOP AUTORI DUPA SCOR RENUME ===\n";
                int poz = 1;
                for (const auto &a: lista) {
                    std::cout << poz++ << ". "
                            << a->getNume() << " "
                            << a->getprenume()
                            << " | Scor: "
                            << a->calcscorrenume()
                            << "\n";
                }
                break;
            }
            case 3: {
                if (app.autor.empty()) {
                    std::cout << "Nu exista autori.\n";
                    return;
                }

                auto lista = app.autor;
                std::ranges::sort(lista, cmpAutoriIerarhie);

                std::cout << "\n=== AUTORI IERARHIZATI ===\n";

                std::string curent = "";
                for (const auto &a: lista) {
                    std::string i = a->ierarhie_a();

                    if (i != curent) {
                        curent = i;
                        std::cout << "\n[" << curent << "]\n";
                    }

                    std::cout << "- "
                            << a->getNume() << " "
                            << a->getprenume()
                            << " | Scor: " << a->calcscorrenume()
                            << " | Productivitate: "
                            << std::fixed << std::setprecision(2)
                            << a->calcproductivitate()
                            << "\n";
                }
            }
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void meniuAutor(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- MENIU AUTOR---\n";
        std::cout << "1. Detalii auotr\n";
        std::cout << "2. Top autori.\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        switch (opt) {
            case 1: {
                DetaliiAutor(app);
                break;
            }
            case 2: {
                topAutori(app);
                break;
            }

            case 0: {
                break;
            }
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void statisticiClienti(const AppState &app) {
    if (app.clienti.empty()) {
        std::cout << "Nu exista clienti.\n";
        return;
    }
    int total = app.clienti.size();
    int activi = 0;
    int vip = 0, tcomenzi = 0;
    double tvanzari = 0.0;

    std::shared_ptr<Client> maxComenzi = nullptr;
    std::shared_ptr<Client> maxCheltuieli = nullptr;

    for (const auto &c: app.clienti) {
        int nrC = c->getNumarComenzi();
        double totalV = c->getTotalCumparaturi();

        tcomenzi += nrC;
        tvanzari += totalV;

        if (nrC >= 1)
            activi++;

        if (c->esteVIP())
            vip++;
        if (!maxComenzi || nrC > maxComenzi->getNumarComenzi())
            maxComenzi = c;

        if (!maxCheltuieli || totalV > maxCheltuieli->getTotalCumparaturi())
            maxCheltuieli = c;
    }
    std::cout << "\n=== STATISTICI CLIENTI ===\n";
    std::cout << "Clienti total: " << total << "\n";
    std::cout << "Clienti activi: " << activi << "\n";
    std::cout << "Clienti inactivi: " << total - activi << "\n";
    std::cout << "Clienti VIP: " << vip << "\n\n";

    std::cout << "Comenzi totale: " << tcomenzi << "\n";
    std::cout << "Valoare totala vanzari: "
            << std::fixed << std::setprecision(2)
            << tvanzari << " lei\n\n";

    if (maxComenzi) {
        std::cout << "Client cu cele mai multe comenzi: "
                << maxComenzi->getUsername()
                << " (" << maxComenzi->getNumarComenzi() << ")\n";
    }

    if (maxCheltuieli) {
        std::cout << "Client cu cele mai mari cheltuieli: "
                << maxCheltuieli->getUsername()
                << " (" << maxCheltuieli->getTotalCumparaturi()
                << " lei)\n";
    }
}

void detaliiClient(const AppState &app) {
    if (app.autor.empty()) {
        std::cout << "Nu exista clienti.\n";
        return;
    }
    int idx = filtreazaClient(app);
    if (idx == -1) return;

    const auto &c = app.clienti[idx];
    std::cout << "\n=== DETALII CLIENT ===\n";
    std::cout << "Username: " << c->getUsername() << "\n";
    std::cout << "Email: " << c->getEmail() << "\n";
    std::cout << "Sold: " << c->getSold() << " lei\n";
    std::cout << "Numar comenzi: " << c->getNumarComenzi() << "\n";
    std::cout << "Total cumparaturi: "
            << c->getTotalCumparaturi() << " lei\n";
    std::cout << "Puncte fidelitate: "
            << c->getPunctedeFideliate() << "\n";
    std::cout << "Ierarhie client: "
            << c->ierarhie_clienti() << "\n";
}

void meniuClientA(const AppState &app) {
    int opt = -1;

    while (opt != 0) {
        std::cout << "\n--- MENIU CLIENT ---\n";
        std::cout << "1. Statistici clienti\n";
        std::cout << "2. Detalii client\n";
        std::cout << "3. Logs activitate clienti\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1:
                statisticiClienti(app);
                break;
            case 2: {
                detaliiClient(app);
                break;
            }
            case 3: {
                Logsactivitateclients(app);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida.\n";
        }
    }
}

void meniuComenziA(const AppState &app) {
    int opt = -1;

    while (opt != 0) {
        std::cout << "\n--- MENIU COMENZI ---\n";
        std::cout << "1. Lista comenzi\n";
        std::cout << "2. Datalii comanda\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                std::cout << "\n=== LISTA COMENZI ===\n";
                for (size_t i = 0; i < app.comenzi.size(); ++i) {
                    const auto &c = app.comenzi[i];
                    std::cout << i << ". "
                            << "ID: " << c->getId()
                            << " | Data: " << c->getDataFormatata()
                            << " | Articole: " << c->getNumarArticole()
                            << " | Total: " << c->calculeazaTotal()
                            << " lei\n";
                    break;
                }
            case 2: {
                int idx = filtrareComanda(app);

                if (idx == -1) {
                    std::cout << "Comanda nu a fost gasita!\n";
                    return;
                }
                const auto &cmd = app.comenzi[idx];

                std::cout << "\n===== DETALII COMANDA =====\n";
                std::cout << "ID comanda: " << cmd->getId() << "\n";
                std::cout << "Data: " << cmd->getDataFormatata() << "\n";
                std::cout << "Stare: " << cmd->getStare() << "\n";
                std::cout << "Numar articole: " << cmd->getNumarArticole() << "\n";

                std::cout << "\n--- Articole ---\n";
                std::cout << *cmd;

                std::cout << "Total comanda: "
                        << cmd->calculeazaTotal() << " lei\n";

                std::cout << "===========================\n";
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida.\n";
            }
        }
    }
}

void meniuTopuri(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== TOPURI ===\n";
        std::cout << "1. Top clienti (dupa suma cheltuita)\n";
        std::cout << "2. Top autori (ierarhie)\n";
        std::cout << "3. Top publicatii\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                topClienti(app);
                break;
            }
            case 2: {
                topAutori(app);
                break;
            }
            case 3: {
                meniuTopPublicatiiPopularitate(app);
            }
            case 0: break;
            default:
                std::cout << "Optiune invalida.\n";
        }
    }
}

void meniustatistici(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << " === MENIU STATISTICI GENERALE ===\n";
        std::cout << "1. Statistici publicatii\n";
        std::cout << "2. Statistici comenzi\n";
        std::cout << "3. Statistici clienti\n";
        std::cout << "4. Topuri\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        switch (opt) {
            case 1: meniuStatisticiPublicatii(app);
                break;
            case 2: {
                if (app.comenzi.empty()) {
                    std::cout << "Nu exista comenzi.\n";
                    return;
                }

                int finalizate = 0;
                int anulate = 0;
                double total = 0.0;

                for (const auto &c: app.comenzi) {
                    if (c->getStare() == "Finalizata")
                        finalizate++;
                    else if (c->getStare() == "Anulata")
                        anulate++;

                    total += c->calculeazaTotal();
                }

                std::cout << "\n=== STATISTICI COMENZI ===\n";
                std::cout << "Comenzi totale: "
                        << app.comenzi.size() << "\n";
                std::cout << "Finalizate: "
                        << finalizate << "\n";
                std::cout << "Anulate: "
                        << anulate << "\n";
                std::cout << "Valoare totala: "
                        << total << " lei\n";
                std::cout << "Valoare medie comanda: "
                        << total / app.comenzi.size()
                        << " lei\n";
                break;
            }
            case 3: {
                if (app.clienti.empty()) {
                    std::cout << "Nu exista clienti in sistem.\n";
                    return;
                }

                int activi = 0;
                double total_incasari = 0.0;

                std::shared_ptr<Client> fidel = nullptr;
                double max_cheltuit = -1;

                for (const auto &c: app.clienti) {
                    if (c->getNumarComenzi() > 0)
                        activi++;

                    total_incasari += c->getTotalCumparaturi();

                    if (c->getTotalCumparaturi() > max_cheltuit) {
                        max_cheltuit = c->getTotalCumparaturi();
                        fidel = c;
                    }
                }

                std::cout << "\n=== STATISTICI CLIENTI ===\n";
                std::cout << "Clienti total: " << app.clienti.size() << "\n";
                std::cout << "Clienti activi: " << activi << "\n";
                std::cout << "Clienti fara comenzi: "
                        << app.clienti.size() - activi << "\n";
                std::cout << "Incasari totale: "
                        << total_incasari << " lei\n";

                if (fidel) {
                    std::cout << "\nCel mai fidel client:\n";
                    std::cout << *fidel << "\n";
                    std::cout << "Total cheltuit: "
                            << fidel->getTotalCumparaturi() << " lei\n";
                    std::cout << "Ierarhie: "
                            << fidel->ierarhie_clienti() << "\n";
                }
                break;
            }
            case 4: meniuTopuri(app);
                break;
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void adaugaInCos(AppState &app, std::shared_ptr<Client> &clientCurent, std::shared_ptr<Comanda> &comandaActiva) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== CE ATI DORI SA ADAUGATI IN COS ===\n";
        std::cout << "1. Carte individuala\n";
        std::cout << "2. Pachet serie\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;
        switch (opt) {
            case 1: {
                int idx = 0;
                std::cout << "\n Selecteaza publicatia\n ";
                int p = -1;
                std::cout << "Motor de cautare:\n1.Serchbar\n2.FiltrePublicatie\n";
                std::cin >> p;
                if (p == 1)
                    idx = Serchbar(app);
                if (p == 2)
                    idx = filtrePublicatie(app);

                if (idx < 0 || idx >= static_cast<int>(app.publicatii.size())) {
                    std::cout << "Selectie invalida!\n";
                    return;
                }
                const auto pub = app.publicatii[idx];
                adaugaCarteIndividuala(app, pub, clientCurent, comandaActiva);
                break;
            }
            case 2: {
                adaugaPachet(app, clientCurent, comandaActiva);
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void meniuCosCumparaturi(AppState &app, std::shared_ptr<Client> &clientCurent) {
    static std::shared_ptr<Comanda> comandaActiva = nullptr;
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== MENIU COMENZI ===\n";
        std::cout << "1. Adauga publicatie in cos\n";
        std::cout << "2. Sterge publicatie din cos\n";
        std::cout << "3. Vezi cosul\n";
        std::cout << "4. Finalizeaza comanda\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1: adaugaInCos(app, clientCurent, comandaActiva);
                break;
            case 2: StergeDinCos(app, clientCurent, comandaActiva);
                break;
            case 3: {
                if (comandaActiva)
                    std::cout << *comandaActiva;
                else
                    std::cout << "Cosul este gol!\n";
            }
            case 4: finalizeazaCos(app, clientCurent, comandaActiva);
                break;
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n ";
        }
    }
}

void CLIAdministrator(AppState &app) {
    std::string parola;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Introduceti parola pentru a va loga in contul de administrator: ";
    std::getline(std::cin, parola);
    if (parola != "admin123") {
        std::cout << "Parola incorecta!\n";
        return;
    }
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- MENIU ADMINISTRATOR ---\n";
        std::cout << "1. !Reduceri Active!\n";
        std::cout << "2.Meniu Publicatii\n";
        std::cout << "3.Meniu Autor\n";
        std::cout << "4.Meniu Client\n";
        std::cout << "5.Meniu Comenzi\n";
        std::cout << "6.Statistici\n";
        std::cout << "0. Logout\n";

        std::cin >> opt;
        switch (opt) {
            case 1: afiseazaReduceri(app);
                break;
            case 2: meniuPublicatii(app);
                break;
            case 3: meniuAutor(app);
                break;
            case 4: meniuClientA(app);
                break;
            case 5: meniuComenziA(app);
                break;
            case 6: meniustatistici(app);
                break;
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void CLIClient(AppState &app) {
    std::shared_ptr<Client> clientCurent = autentificareClient(app);
    if (!clientCurent) {
        return;
    }

    int opt = -1;
    while (opt != 0) {
        std::cout << "=== MENIU CLIENT ===\n";
        std::cout << "1. Reduceri active!\n";
        std::cout << "2. Cauta Publicatii\n";
        std::cout << "3. Cos de cumparaturi\n";
        std::cout << "4. Review-uri si rating\n";
        std::cout << "5. Istoric comenzi\n";
        std::cout << "6. Detalii cont\n";
        std::cout << "7. Alimentare cont\n";
        std::cout << "0. Logout\n";

        std::cin >> opt;
        switch (opt) {
            case 1: afiseazaReduceri(app);
                break;
            case 2: meniuCautaPulicatii(app);
                break;
            case 3: meniuCosCumparaturi(app, clientCurent);
                break;
            case 4: AdaugaReview_Rating(app);
                break;
            case 5: {
                bool gasit = false;
                std::cout << "\n === ISTORIC COMENZI ===\n";
                for (const auto &cmd: app.comenzi) {
                    if (cmd && cmd->getClient() == clientCurent) {
                        gasit = true;
                        std::cout
                                << "ID: " << cmd->getId()
                                << " | Data: " << cmd->getDataFormatata()
                                << " | Stare: " << cmd->getStare()
                                << " | Articole: " << cmd->getNumarArticole()
                                << " | Total: " << std::fixed << std::setprecision(2)
                                << cmd->calculeazaTotal()
                                << "\n";
                    }
                }
                if (!gasit)
                    std::cout << "Nu ai comenzi inregistrate!\n";
            }
            case 6: meniuDetaliiCont(app);
                break;
            case 7: {
                double suma = 0;
                std::cout <<
                        "introduceti suma pe care vreti sa o adaugati soldului(Pot fi adaugate si sume zecimale) : \n";
                std::cin >> suma;
                try {
                    clientCurent->alimenteazaCont(suma);
                    std::cout << "Sold nou: "
                            << clientCurent->getSold()
                            << " lei\n";
                } catch (const std::exception &e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }
                break;
            }
            case 0:
                break;

            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void CLI(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== LIBRARIE  ===\n";
        std::cout << "1. Administrator\n";
        std::cout << "2. Client\n";
        std::cout << "0. Iesire\n";

        std::cin >> opt;
        switch (opt) {
            case 1: {
                CLIAdministrator(app);
                break;
            }
                case 2: {
                CLIClient(app);
                break;

            }
            case 0: {
                break;
            }
                default:
                    std::cout<<"Optiune invalida!\n";
        }
    }
}
int main() {

}
