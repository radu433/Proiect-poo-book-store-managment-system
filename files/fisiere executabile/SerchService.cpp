#include "../headere/SerchService.h"
#include "../headere/AppState.h"
#include "../headere/Autor.h"
#include "../headere/Revista.h"
#include "../headere/CarteStiintifica.h"
#include "../headere/Carte.h"
#include "../headere/Manual.h"
#include "../headere/Client.h"
#include "../headere/Comanda.h"
#include <iostream>
#include <limits>
// publicatii
int SerchService::filtrePublicatie(const AppState &app) {
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

int SerchService::Serchbar(const AppState &app) {
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

    for (std::size_t i = 0; i < rezultat.size(); ++i) {
        std::cout << i << ". "
                << app.publicatii[rezultat[i]]->getTitlu()
                << "\n";
    }
    int idx;
    std::cout << "Selecteaza publicatia (-1 inapoi): ";
    std::cin >> idx;

    if (idx < 0 || idx >= static_cast<int>(rezultat.size()))
        return -1;

    return rezultat[idx];
}

// autori

std::shared_ptr<Autor> SerchService::cautaAutor(const AppState &app, const std::string &nume,
    const std::string &prenume) {
    for (const auto &a: app.autor) {
        if (a->getNume() == nume && a->getprenume() == prenume)
            return a;
    }
    return nullptr;
}

std::shared_ptr<Autor> SerchService::selecteazaAutor(AppState &app) {
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

            if (idx < 0 || idx > static_cast<int>(app.autor.size()))
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

int SerchService::filtreazaClient(const AppState &app) {
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
                        rezultate.push_back(static_cast<int>(i));
                }
                break;
            }
            case 2: {
                std::string email;
                std::cout << "Introduceti email-ul pe care il cautati:\n";
                std::getline(std::cin, email);
                for (size_t i = 0; i < app.clienti.size(); ++i) {
                    if (app.clienti[i]->getEmail().find(email) != std::string::npos)
                        rezultate.push_back(static_cast<int>(i));
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
// comanda
int SerchService::filtrareComanda(const AppState &app) {
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

// clienti


