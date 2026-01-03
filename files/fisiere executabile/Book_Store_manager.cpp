#include "../headere/Book_Store_manager.h"
#include "../headere/AuthenticationService.h"
#include "../headere/adresa.h"
#include "../headere/Client.h"
#include "../headere/Comanda.h"
#include "../headere/PachetSerie.h"
#include "../headere/ComandaService.h"
#include "../headere/ReviewService.h"
#include "../headere/InventoryService.h"
#include "../headere/CosService.h"
#include "../headere/TradeInService.h"

#include <iomanip>
#include <iostream>
#include <limits>

#include "../headere/AutorService.h"
#include "../headere/ClientService.h"
#include "../headere/ComandaService.h"
#include "../headere/CosService.h"
#include "../headere/PublicatieService.h"
#include "../headere/SerchService.h"

#include "../exceptii/exceptii_headere/ComandaExceptions.h"
#include "../exceptii/exceptii_headere/StocException.h"
#include "../exceptii/exceptii_headere/ClientExceptions.h"
#include "../headere/CarteIndividuala.h"
#include "../headere/RevistaIndividuala.h"
#

enum class TipPublicatie {
    Necunoscut,
    Carte,
    Revista,
    Manual,
    CarteStiintifica
};
TipPublicatie determinaTipPublicatie(const std::shared_ptr<Publicatie>& pub) {
    if (std::dynamic_pointer_cast<CarteStiintifica>(pub))
        return TipPublicatie::CarteStiintifica;
    if (std::dynamic_pointer_cast<Manual>(pub))
        return TipPublicatie::Manual;
    if (std::dynamic_pointer_cast<Carte>(pub))
        return TipPublicatie::Carte;
    if (std::dynamic_pointer_cast<Revista>(pub))
        return TipPublicatie::Revista;

    return TipPublicatie::Necunoscut;
}


void BookStoreManager::afiseazaReduceriActive(const AppState &app) {
    std::cout << "\n--- REDUCERI ACTIVE ---\n";

    if (app.reduceri.empty()) {
        std::cout << "Nu exista reduceri active.\n";
        return;
    }

    bool gasit = false;

    for (const auto& r : app.reduceri) {
        if (!r.esteActiva())
            continue;

        gasit = true;

        auto pub = r.getPublicatie();
        if (!pub)
            continue;

        double pretInitial = pub->getPretFinal();
        double pretRedus = ReducereService::calculeazaPretFinalCuReduceri(app, pub);

        std::cout
            << "Publicatie: " << pub->getTitlu() << "\n"
            << "Reducere: " << r.getProcent() << "%\n"
            << "Pret initial: " << pretInitial << " lei\n"
            << "Pret redus:   " << pretRedus << " lei\n"
            << "-----------------------------\n";
    }

    if (!gasit) {
        std::cout << "Nu exista reduceri active in acest moment.\n";
    }
}

// motor cautare publicatii
int BookStoreManager::selecteazaPub(const AppState &app) {
    if (app.publicatii.empty()) {
        std::cout << "Nu exista publicatii.\n";
        return -1;
    }

    int opt = -1;
    std::vector<int> rezultate;

    std::cout << "\n=== SELECTARE PUBLICATIE ===\n";
    std::cout << "1. Cautare rapida (SearchBar)\n";
    std::cout << "2. Cautare pas cu pas (Filtre)\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Optiune: ";
    std::cin >> opt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    switch (opt) {
        case 1: {

            std::string text;
            std::cout << "Introdu titlu (sau parte din titlu): ";
            std::getline(std::cin, text);

            rezultate = SerchService::search<Publicatie>(
                app.publicatii,
                [&](const std::shared_ptr<Publicatie>& p) {
                    return p->getTitlu().find(text) != std::string::npos;
                }
            );
            break;
        }

        case 2: {

            int filtru = -1;
            std::cout << "\n--- FILTRE PUBLICATII ---\n";
            std::cout << "1. Filtru dupa tip\n";
            std::cout << "2. Filtru dupa autor\n";
            std::cout << "3. Afiseaza toate\n";
            std::cout << "Optiune: ";
            std::cin >> filtru;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            switch (filtru) {
                case 1: {
                    int tip;
                    std::cout << "Tip (1-Carte 2-Manual 3-Stiintifica 4-Revista): ";
                    std::cin >> tip;

                    rezultate = SerchService::filtreazaPublicatiiDupaTip(
                        app.publicatii, tip
                    );
                    break;
                }

                case 2: {
                    std::string nume;
                    std::cout << "Nume autor: ";
                    std::getline(std::cin, nume);

                    rezultate = SerchService::filtreazaPublicatiiDupaAutor(
                        app.publicatii, nume
                    );
                    break;
                }

                case 3:
                    for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i)
                        rezultate.push_back(i);
                    break;

                default:
                    std::cout << "Optiune invalida.\n";
                    return -1;
            }
            break;
        }

        case 0:
            return -1;

        default:
            std::cout << "Optiune invalida.\n";
            return -1;
    }
    if (rezultate.empty()) {
        std::cout << "Nu s-au gasit rezultate.\n";
        return -1;
    }

    std::cout << "\n--- REZULTATE ---\n";
    for (size_t i = 0; i < rezultate.size(); ++i) {
        std::cout << i << ". "
                  << app.publicatii[rezultate[i]]->getTitlu()
                  << "\n";
    }

    int alegere;
    std::cout << "Selecteaza publicatia (-1 inapoi): ";
    std::cin >> alegere;

    if (alegere < 0 || alegere >= static_cast<int>(rezultate.size()))
        return -1;

    return rezultate[alegere];
}

std::shared_ptr<Autor> BookStoreManager::selecteazaAutor(AppState &app) {
    if (app.autor.empty()) {
        std::cout << "Nu exista autori. Continuam fara autor.\n";
        return nullptr;
    }

    int opt = -1;
    std::cout << "\n=== SELECTARE AUTOR ===\n";
    std::cout << "1. Cautare rapida (SearchBar)\n";
    std::cout << "2. Afiseaza toti autorii\n";
    std::cout << "0. Fara autor\n";
    std::cout << "Optiune: ";
    std::cin >> opt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<int> rezultate;

    switch (opt) {
        case 1: {
            std::string text;
            std::cout << "Introdu nume/prenume: ";
            std::getline(std::cin, text);

            rezultate = SerchService::search<Autor>(
                app.autor,
                [&](const std::shared_ptr<Autor>& a) {
                    return a->getNume().find(text) != std::string::npos ||
                           a->getprenume().find(text) != std::string::npos;
                }
            );
            break;
        }

        case 2:
            for (int i = 0; i < static_cast<int>(app.autor.size()); ++i)
                rezultate.push_back(i);
            break;

        case 0:
            return nullptr;

        default:
            std::cout << "Optiune invalida.\n";
            return nullptr;
    }

    if (rezultate.empty()) {
        std::cout << "Nu s-au gasit autori.\n";
        return nullptr;
    }

    std::cout << "\n--- AUTORI ---\n";
    for (size_t i = 0; i < rezultate.size(); ++i) {
        const auto& a = app.autor[rezultate[i]];
        std::cout << i << ". "
                  << a->getNume() << " "
                  << a->getprenume() << "\n";
    }

    int alegere;
    std::cout << "Selecteaza autor (-1 inapoi): ";
    std::cin >> alegere;

    if (alegere < 0 || alegere >= static_cast<int>(rezultate.size()))
        return nullptr;

    return app.autor[rezultate[alegere]];
}

int BookStoreManager::filtreazaClienti(const AppState &app) {
    if (app.clienti.empty()) {
        std::cout << "Nu exista clienti.\n";
        return -1;
    }

    int opt;
    std::cout << "\n=== SELECTARE CLIENT ===\n";
    std::cout << "1. Cautare rapida (username / email)\n";
    std::cout << "2. Afiseaza toti clientii\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Optiune: ";
    std::cin >> opt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<int> rezultate;

    switch (opt) {
        case 1: {
            std::string text;
            std::cout << "Introdu username sau email: ";
            std::getline(std::cin, text);

            rezultate = SerchService::search<Client>(
                app.clienti,
                [&](const std::shared_ptr<Client>& c) {
                    return c->getUsername().find(text) != std::string::npos ||
                           c->getEmail().find(text) != std::string::npos;
                }
            );
            break;
        }

        case 2:
            for (int i = 0; i < static_cast<int>(app.clienti.size()); ++i)
                rezultate.push_back(i);
            break;

        case 0:
            return -1;

        default:
            std::cout << "Optiune invalida.\n";
            return -1;
    }

    if (rezultate.empty()) {
        std::cout << "Nu s-au gasit clienti.\n";
        return -1;
    }

    std::cout << "\n--- CLIENTI ---\n";
    for (size_t i = 0; i < rezultate.size(); ++i) {
        const auto& c = app.clienti[rezultate[i]];
        std::cout << i << ". "
                  << c->getUsername()
                  << " | " << c->getEmail()
                  << "\n";
    }

    int alegere;
    std::cout << "Selecteaza client (-1 inapoi): ";
    std::cin >> alegere;

    if (alegere < 0 || alegere >= static_cast<int>(rezultate.size()))
        return -1;

    return rezultate[alegere];
}

int BookStoreManager::filtreComanda(const AppState &app) {
    if (app.comenzi.empty()) {
        std::cout << "Nu exista comenzi.\n";
        return -1;
    }

    std::cout << "\n=== LISTA COMENZI ===\n";
    for (size_t i = 0; i < app.comenzi.size(); ++i) {
        const auto& c = app.comenzi[i];
        std::cout << i << ". "
                  << "ID: " << c->getId()
                  << " | Data: " << c->getDataFormatata()
                  << " | Articole: " << c->getNumarArticole()
                  << " | Total: " << c->calculeazaTotal()
                  << " lei\n";
    }

    int idx;
    std::cout << "Selecteaza comanda (-1 inapoi): ";

    if (!(std::cin >> idx))
        return -1;

    if (idx < 0 || idx >= static_cast<int>(app.comenzi.size()))
        return -1;

    return idx;
}
// motor de cautare client+functie de vizualizare review+adauga in cos
std::shared_ptr<Publicatie> BookStoreManager::selecteazaPubClient( AppState &app) {
    std::vector<int> rezultate;
    if (app.publicatii.empty()) {
        std::cout<<"Nu exista publicatii!\n";
        return nullptr;
    }

   int opt = -1;

    while (opt != 0) {
        rezultate.clear();

        std::cout << "\n=== SELECTARE PUBLICATIE ===\n";
        std::cout << "1. Cautare rapida (SearchBar)\n";
        std::cout << "2. Afiseaza publicatii (filtre)\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";
        std::cin >> opt;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opt) {


            case 1: {
                std::string text;
                std::cout << "Introdu text (titlu): ";
                std::getline(std::cin, text);

                rezultate = SerchService::search<Publicatie>(
                    app.publicatii,
                    [&](const std::shared_ptr<Publicatie> &p) {
                        return p->esteActiva() &&
                               p->getTitlu().find(text) != std::string::npos;
                    }
                );
                break;
            }


            case 2: {
                int sub = -1;
                while (sub != 0) {
                    rezultate.clear();

                    std::cout << "\n--- FILTRARE PUBLICATII ---\n";
                    std::cout << "1. Toate publicatiile active\n";
                    std::cout << "2. Publicatii dupa tip\n";
                    std::cout << "3. Publicatii dupa autor\n";
                    std::cout << "0. Inapoi\n";
                    std::cout << "Optiune: ";
                    std::cin >> sub;

                    switch (sub) {
                        case 1:
                            for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i)
                                if (app.publicatii[i]->esteActiva())
                                    rezultate.push_back(i);
                            break;

                        case 2: {
                            int t;
                            std::cout << "Tip (1 Carte, 2 Manual, 3 CS, 4 Revista): ";
                            std::cin >> t;

                            for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i) {
                                const auto& p = app.publicatii[i];
                                if (!p->esteActiva()) continue;

                                if ((t == 1 && dynamic_cast<Carte*>(p.get())) ||
                                    (t == 2 && dynamic_cast<Manual*>(p.get())) ||
                                    (t == 3 && dynamic_cast<CarteStiintifica*>(p.get())) ||
                                    (t == 4 && dynamic_cast<Revista*>(p.get())))
                                    rezultate.push_back(i);
                            }
                            break;
                        }

                        case 3: {
                            const auto autor = selecteazaAutor(app);
                            if (!autor) break;

                            for (int i = 0; i < static_cast<int>(app.publicatii.size()); ++i) {
                                if (const auto& p = app.publicatii[i]; p->esteActiva() &&
                                                                       p->areAutor(autor->getidAutor()))
                                    rezultate.push_back(i);
                            }
                            break;
                        }

                        case 0:
                            break;

                        default:
                            std::cout << "Optiune invalida!\n";
                    }

                    if (!rezultate.empty())
                        break;

                    std::cout << "Nu exista rezultate pentru filtrul ales.\n";
                }
                break;
            }

            case 0:
                return nullptr;

            default:
                std::cout << "Optiune invalida!\n";
        }

        if (rezultate.empty())
            continue;

        std::cout << "\n--- PUBLICATII ---\n";
        for (size_t i = 0; i < rezultate.size(); ++i) {
            const auto& p = app.publicatii[rezultate[i]];
            std::cout << i << ". " << p->getTitlu()
                      << " | Pret: " << p->getPretFinal()
                      << " | Stoc: " << p->getcantitate() << "\n";
        }

        int alegere;
        std::cout << "Selecteaza (-1 inapoi): ";
        std::cin >> alegere;

        if (alegere < 0 || alegere >= (int)rezultate.size())
            continue;

        return app.publicatii[rezultate[alegere]];
    }

    return nullptr;
}

void BookStoreManager::afiseazaReviewuriClient(const AppState &app, const std::string &idP) {
    auto reviewuri=ReviewService::getReviewuriSortate(app,idP);
    if (reviewuri.empty()) {
        std::cout<<"Nu sunt review-uri!";
        return;
    }
    std::cout << "\n--- REVIEW-URI (primele 5) ---\n";
    int afisate = 0;
    for (const Review* r : reviewuri) {
        if (!r->verificat)
            break;

        std::cout << r->username
                  << " | Rating: " << r->rating << "\n";
        std::cout << "  \"" << r->text << "\"\n";

        if (++afisate == 5)
            break;
    }
    int opt;
    std::cout << "\n1. Vezi toate review-urile verificate\n";
    std::cout << "2. Vezi toate review-urile (inclusiv neverificate)\n";
    std::cout << "0. Continua\n";
    std::cout << "Optiune: ";
    std::cin >> opt;

    if (opt == 1) {
        std::cout << "\n--- TOATE REVIEW-URILE VERIFICATE ---\n";
        for (const Review* r : reviewuri) {
            if (!r->verificat)
                break;

            std::cout << r->username
                      << " | Rating: " << r->rating << "\n";
            std::cout << "  \"" << r->text << "\"\n";
            std::cout << "-------------------------\n";
        }
    }
    else if (opt == 2) {
        std::cout << "\n--- TOATE REVIEW-URILE ---\n";
        for (const Review* r : reviewuri) {
            std::cout << (r->verificat ? "[VERIFICAT] " : "[NEVERIFICAT] ")
                      << r->username
                      << " | Rating: " << r->rating << "\n";
            std::cout << "  \"" << r->text << "\"\n";
            std::cout << "-------------------------\n";
        }
    }
}

// meniu pt adaugare publicatii+ functiile aferente
void BookStoreManager::adaugaCarte(AppState &app) {
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

        auto carte = std::make_shared<Carte>(
            titlu, autor, cantitate, data_publicatie,
            isbn, pret_baza, nr_pagini, editura
        );

        PublicatieService::adaugarePublicatie(app, carte);

        if (autor) {
            autor->adauga_carte(isbn);
        }

        double pretFinal = ReducereService::calculeazaPretFinalCuReduceri(app, carte);

        std::cout << "Carte adaugata cu succes!\n";
        std::cout << "Pret final: " << pretFinal << " lei\n";

    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}
void BookStoreManager::adaugaManual(AppState &app) {
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

        auto manual = std::make_shared<Manual>(
            titlu, autor, pret_baza, cantitate, data_publicatie,
            isbn, nr_pagini, editura, materie, clasa
        );

        PublicatieService::adaugarePublicatie(app, manual);

        if (autor) {
            autor->adauga_carte(isbn);
        }

        double pretFinal = ReducereService::calculeazaPretFinalCuReduceri(app, manual);

        std::cout << "Manual adaugat cu succes!\n";
        std::cout << "Pret final: " << pretFinal << " lei\n";

    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}
void BookStoreManager::adaugaCarteStiintifica(AppState &app) {
    try {
        std::string titlu, isbn, editura, data_publicatie;
        std::string domeniu, nivel_academic, raspuns;
        int cantitate, nr_pagini, nr_referinte;
        double pret_baza;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Titlu: ";
        std::getline(std::cin, titlu);

        std::cout << "Domeniu: ";
        std::getline(std::cin, domeniu);

        std::cout << "Nivel academic: ";
        std::getline(std::cin, nivel_academic);

        std::cout << "Numar referinte: ";
        std::cin >> nr_referinte;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Contine formule/grafice? (da/nu): ";
        std::getline(std::cin, raspuns);

        std::ranges::transform(raspuns, raspuns.begin(), ::tolower);
        bool are_formule = (raspuns == "da");

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

        auto carte = std::make_shared<CarteStiintifica>(
            titlu, autor, cantitate, data_publicatie, isbn,
            pret_baza, nr_pagini, editura,
            domeniu, nivel_academic, nr_referinte, are_formule
        );

        PublicatieService::adaugarePublicatie(app, carte);

        if (autor) {
            autor->adauga_carte(isbn);
        }

        double pretFinal = ReducereService::calculeazaPretFinalCuReduceri(app, carte);

        std::cout << "Carte stiintifica adaugata cu succes!\n";
        std::cout << "Pret final: " << pretFinal << " lei\n";

    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}
void BookStoreManager::adaugaRevista(AppState &app) {
    try {
        std::string titlu, data_publicatie, frecventa, tip, issn, editura, raspuns;
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

        std::cout << "Frecventa: ";
        std::getline(std::cin, frecventa);

        std::cout << "Numar editie: ";
        std::cin >> numar_editie;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Are cadou suplimentar? (da/nu): ";
        std::getline(std::cin, raspuns);

        std::ranges::transform(raspuns, raspuns.begin(), ::tolower);
        bool are_cadou = (raspuns == "da");

        std::cout << "Tip revista: ";
        std::getline(std::cin, tip);

        std::cout << "ISSN: ";
        std::getline(std::cin, issn);

        std::cout << "Editura: ";
        std::getline(std::cin, editura);

        auto revista = std::make_shared<Revista>(
            titlu, cantitate, data_publicatie, nr_pagini,
            pret_baza, frecventa, numar_editie,
            are_cadou, tip, issn, editura
        );

        PublicatieService::adaugarePublicatie(app, revista);

        double pretFinal = ReducereService::calculeazaPretFinalCuReduceri(app, revista);

        std::cout << "Revista adaugata cu succes!\n";
        std::cout << "Pret final: " << pretFinal << " lei\n";

    } catch (const std::exception &e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}



void BookStoreManager::adaugaPub(AppState& app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== ADAUGA PUBLICATIE ===\n";
        std::cout << "1. Carte\n";
        std::cout << "2. Manual\n";
        std::cout << "3. Carte stiintifica\n";
        std::cout << "4. Revista\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin >> opt;

        switch (opt) {
            case 1:
                adaugaCarte(app);
                break;
            case 2:
                adaugaManual(app);
                break;
            case 3:
                adaugaCarteStiintifica(app);
                break;
            case 4:
                adaugaRevista(app);
                break;
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

// statististici admin
template<typename T>
   void topPopularitatePeTip(const AppState& app,const std::string &numeTip) {
    std::vector<std::shared_ptr<T>> lista;

    for (const auto &p : app.publicatii) {
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
void BookStoreManager::meniuTopPopularitatePeTip(const AppState &app) {
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
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}
void BookStoreManager::meniuStatisticaDistributieTip(const AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- STATISTICI PUBLICATII ---\n";
        std::cout << "1. Statistica popularitate generala\n";
        std::cout << "2. Statistica popularitate impartita pe tipuri\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;


        StatPopularitate s = PublicatieService::statisticaDetaliata(app);

        const int total = s.nrCarte + s.nrManual + s.nrCS;
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
                    std::cout << "Carte: " << s.sumaCarte / s.nrCarte << "\n";
                else
                    std::cout << "Carte: N/A\n";

                if (s.nrManual > 0)
                    std::cout << "Manual: " << s.sumaManual / s.nrManual << "\n";
                else
                    std::cout << "Manual: N/A\n";

                if (s.nrCS > 0)
                    std::cout << "Carte stiintifica: " << s.sumaCS / s.nrCS << "\n";
                else
                    std::cout << "Carte stiintifica: N/A\n";
                break;
            }
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}
void BookStoreManager::topPopulariatateoverall(const AppState &app) {
    std::vector<std::shared_ptr<Carte>> carti;

    for (const auto &p : app.publicatii) {
        if (auto c = std::dynamic_pointer_cast<Carte>(p))
            carti.push_back(c);
    }

    if (carti.empty()) {
        std::cout << "Nu exista carti.\n";
        return;
    }

    std::ranges::sort(carti, [](const auto &a, const auto &b) {
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

void BookStoreManager::meniuTopPublicatiiPopularitate(const AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- PREZENTAREA TOPURILOR ---\n";
        std::cout << "1. Top overall\n";
        std::cout << "2. Top publicatii pe tip\n";
        std::cout << "0. Inapoi\n";

        std::cin >> opt;

        switch (opt) {
            case 1:
                topPopulariatateoverall(app);
                break;
            case 2:
                meniuTopPopularitatePeTip(app);
                break;
            case 0:
                break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }

}
void BookStoreManager::meniuStatisticiPublicatii(const AppState &app) {
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


void creareContClientUI(AppState& app) {
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

    std::cout << "Parola: ";
    std::getline(std::cin, parola);

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

        AuthenticatorService::creeazaClient(
            app, username, email, adresa, telefon, parola
        );

        std::cout << "Cont creat cu succes! Va rugam sa va autentificati.\n";
    }
    catch (const std::exception& e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}
std::shared_ptr<Client> BookStoreManager::autentificareClientUI(AppState &app) {
    int opt = -1;

    std::cout << "\n=== CLIENT ===\n";
    std::cout << "1. Login\n";
    std::cout << "2. Creeaza cont\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Optiune: ";
    std::cin >> opt;

    if (opt == 0)
        return nullptr;

    if (opt == 2) {
        creareContClientUI(app);
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string identificator;
    std::cout << "Introduceti Email / Telefon / Username: ";
    std::getline(std::cin, identificator);

    int incercari = 0;
    while (incercari < 5) {
        std::string parola;
        std::cout << "Parola (0 pentru iesire): ";
        std::getline(std::cin, parola);

        if (parola == "0")
            return nullptr;

        auto client = AuthenticatorService::autentifica(
            app, identificator, parola
        );

        if (client) {
            std::cout << "Autentificare reusita!\n";
            return client;
        }

        incercari++;
        std::cout << "Parola gresita. Reincercati!\n";
    }

    std::cout << "Prea multe incercari esuate.\n";
    return nullptr;
}
// meniu stoc
void BookStoreManager::meniuStocA(AppState& app) {
    const int idx = selecteazaPub(app);
    if (idx == -1) {
        std::cout << "Publicatia nu exista!\n";
        return;
    }

    const auto pub = app.publicatii[idx];

    int opt = -1;
    while (opt != 0) {
        std::cout << "\n--- MENIU STOC ---\n";
        std::cout << "Titlu: " << pub->getTitlu() << "\n";
        std::cout << "Stoc curent: " << pub->getcantitate() << "\n";
        std::cout << "1. Adauga stoc\n";
        std::cout << "2. Scade stoc\n";
        std::cout << "3. Prioritate restoc\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                int cant;
                std::cout<<"Introduceti cantitatea dorita: ";
                std::cin>>cant;
                InventoryService::adaugaStoc(*pub,cant);
                break;
            }
            case 2: {
                int cant;
                std::cout<<"Introduceti cantitatea dorita de scazut: ";
                std::cin>>cant;
                InventoryService::scadeStoc(*pub,cant);
                break;
            }
            case 3: {
                auto lista = InventoryService::calculeazaPrioritateRestoc(app);

                if (lista.empty()) {
                    std::cout << "Nu exista publicatii.\n";
                    break;
                }

                std::cout << "\n=== PRIORITATI RESTOC ===\n";
                int poz = 1;
                for (const auto& p : lista) {
                    std::cout << poz++ << ". "
                              << p.pub->getTitlu()
                              << " | Stoc: " << p.pub->getcantitate()
                              << " | Scor: " << p.scor << "\n";
                }
                break;
            }
            case 0: break;
            default:
                std::cout<<"Optiune invalida!";
        }
    }
}
// meniu publicatii administrator
void BookStoreManager::meniuPublicatiiA(AppState &app) {
    int opt = -1;
    while (opt!=0) {
        std::cout<<"\n=== MENIU PUBLICATII ===\n";
        std::cout << "1. Adauga Publicatii\n";
        std::cout << "2. Dezactiveaza publicatie\n";
        std::cout << "3. Sterge Publicatie\n";
        std::cout << "4. Meniu Stoc ";
        std::cout << "4. Cloneaza Publicatie\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin>>opt;
        switch (opt) {
            case 1:adaugaPub(app); break;
            case 2: {
                const int idx=selecteazaPub(app);
                if (idx<0)
                    break;
                try {
                    PublicatieService::dezactivarePublicatie(app.publicatii[idx]);
                    std::cout << "Publicatie dezactivata cu succes.\n";
                } catch (const std::exception& e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }
                break;
            }
            case 3: {
                const int idx =selecteazaPub(app);
                if (idx < 0)
                    break;

                std::string id = app.publicatii[idx]->getIdentificator();

                try {
                    PublicatieService::stergePublicatie(app, id);
                    std::cout << "Publicatie stearsa cu succes.\n";
                } catch (const std::exception& e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }
                break;
            }
            case 4: {
                    meniuStocA(app); break;
            }
            case 5: {
                int idx = selecteazaPub(app);
                if (idx < 0)
                    break;

                int stoc;
                std::cout << "Stoc initial: ";
                std::cin >> stoc;

                try {
                    const auto copie = PublicatieService::cloneazaPublicatie(
                        app,
                        app.publicatii[idx],
                        stoc
                    );
                    std::cout << "Publicatie clonata: "
                              << copie->getTitlu() << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }
                break;
            }
            case 0:break;
            default: std::cout<<"Optiune invalida!";
        }
    }
}

void BookStoreManager::meniuReviewuri(AppState &app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== MENIU REVIEW ===\n";
        std::cout << "1. Vezi Review-uri\n";
        std::cout << "2. Sterge Review-uri\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";
        std::cin >> opt;

        switch (opt) {
            case 1: {
                const int idx = selecteazaPub(app);
                if (idx < 0) break;

                const auto pub = app.publicatii[idx];

                auto reviewuri = ReviewService::getReviewuriSortate(
                    app,
                    pub->getIdentificator()
                );

                if (reviewuri.empty()) {
                    std::cout << "Nu exista review-uri pentru aceasta publicatie.\n";
                    break;
                }

                std::cout << "\n=== REVIEW-URI PENTRU: "
                          << pub->getTitlu() << " ===\n";

                for (size_t i = 0; i < reviewuri.size(); ++i) {
                    const Review* r = reviewuri[i];
                    std::cout << "[" << i << "] "
                              << (r->verificat ? "[VERIFICAT] " : "[NEVERIFICAT] ")
                              << r->username << " | Rating: " << r->rating << "\n";
                    std::cout << "   \"" << r->text << "\"\n";
                    std::cout << "---------------------------------\n";
                }
                break;
            }

            case 2: {
                const int idxPub = selecteazaPub(app);
                if (idxPub < 0) break;

                auto pub = app.publicatii[idxPub];

                auto reviewuri = ReviewService::getReviewuriNeverificate(
                    app,
                    pub->getIdentificator()
                );

                if (reviewuri.empty()) {
                    std::cout << "Nu exista review-uri NEVERIFICATE pentru aceasta publicatie.\n";
                    break;
                }

                std::cout << "\n=== REVIEW-URI NEVERIFICATE ===\n";

                for (size_t i = 0; i < reviewuri.size(); ++i) {
                    const Review* r = reviewuri[i];
                    std::cout << "[" << i << "] "
                              << r->username << " | Rating: " << r->rating << "\n";
                    std::cout << "   \"" << r->text << "\"\n";
                    std::cout << "---------------------------------\n";
                }

                std::cout << "\nIntrodu indexurile review-urilor de sters (-1 pentru stop):\n";

                std::vector<int> indexuri;
                int idx;
                while (std::cin >> idx && idx != -1) {
                    indexuri.push_back(idx);
                }

                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (!indexuri.empty()) {
                    ReviewService::stergeReviewuriNeverificate(
                        app,
                        pub->getIdentificator(),
                        indexuri
                    );
                    std::cout << "Review-urile selectate au fost sterse.\n";
                } else {
                    std::cout << "Nu s-a selectat niciun review.\n";
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
void BookStoreManager::meniuAutorA(AppState& app) {
    int opt=-1;
    while (opt != 0) {
        std::cout << "\n--- MENIU AUTORI ---\n";
        std::cout<<"1. Adauga autori\n";
        std::cout<<"2. Detalii autor\n";
        std::cout<< "3.Topuri autor\n";
        std::cout<<"0. Inapoi\n";
        std::cout<<" Optiunea:";

        std::cin>>opt;
        switch (opt) {
            case 1: {
                std::string nume,prenume;
                int varsta;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout<<" === Adauga Autor ===\n";
                std::cout<<"Nume: ";
                std::getline(std::cin, nume);

                std::cout<<"Prenume: ";
                std::getline(std::cin, prenume);

                std::cout<<"Varsta: ";
                std::cin>>varsta;

                if (nume.empty() || prenume.empty() || varsta <= 0) {
                    std::cout << "Date invalide. Autorul nu a fost adaugat.\n";
                    break;
                }
                try {
                    auto autor = std::make_shared<Autor>(nume, prenume, varsta);

                    AutorService::adaugaAutor(app, autor);

                    std::cout << "Autor adaugat cu succes: "
                              << nume << " " << prenume << "\n";
                } catch (const std::exception& e) {
                    std::cout << "Eroare la adaugarea autorului: "
                              << e.what() << "\n";
                }
                    break;
            }
            case 2: {
                auto autor = selecteazaAutor(app);
                if (!autor) {
                    std::cout << "Autor inexistent sau selectie anulata.\n";
                    break;
                }

                auto det = AutorService::getDetaliiAutor(app, autor);

                std::cout << "\n=== AUTOR ===\n";
                std::cout << autor->getNume() << " " << autor->getprenume() << "\n";
                std::cout << "Varsta: " << autor->getvarsta() << "\n";
                std::cout << "Carti scrise: " << autor->getcarti_scrise() << "\n";
                std::cout << "Ierarhie: " << det.ierarhie << "\n";
                std::cout << "Productivitate: " << det.productivitate << "\n";
                std::cout << "Scor renume: " << det.scor_renume << "\n";
                break;
            }
            case 3: {
                int opt1 = -1;
                while (opt1 != 0) {
                    std::cout << "\n--- TOP AUTORI ---\n";
                    std::cout << "1. Productivitate\n";
                    std::cout << "2. Scor renume\n";
                    std::cout << "3. Ierarhie\n";
                    std::cout << "0. Inapoi\n";
                    std::cout << "Optiune: ";
                    std::cin >> opt1;

                    TipTopAutor tip;

                    switch (opt1) {
                        case 1:
                            tip = TipTopAutor::PRODUCTIVITATE;
                            break;
                        case 2:
                            tip = TipTopAutor::SCOR_RENUME;
                            break;
                        case 3:
                            tip = TipTopAutor::IERARHIE;
                            break;
                        case 0:
                            break;
                        default:
                            std::cout << "Optiune invalida!\n";
                            continue;
                    }

                    auto top = AutorService::getTopAutori(app, tip);

                    if (top.empty()) {
                        std::cout << "Nu exista autori.\n";
                        continue;
                    }

                    std::cout << "\n=== TOP AUTORI ===\n";
                    int poz = 1;

                    for (const auto&[autor, productivitate, scor_renume, ierarhie] : top) {
                        std::cout << poz++ << ". "
                                  << autor->getNume() << " "
                                  << autor->getprenume()
                                  << " | Prod: " << productivitate
                                  << " | Scor: " << scor_renume
                                  << " | Ierarhie: " << ierarhie
                                  << "\n";
                    }

                }
                break;
            }
            case 0: break;
                default:
                std::cout << "Optiune invalida!\n";
        }


    }
}

void BookStoreManager::meniuClientA(AppState& app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== MENIU CLIENTI (ADMIN) ===\n";
        std::cout << "1. Statistici clienti\n";
        std::cout << "2. Detalii client\n";
        std::cout << "3. Afiseaza log-uri\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin >> opt;
        switch (opt) {
            case 1: {
                if (app.clienti.empty()) {
                    std::cout << "Nu exista clienti.\n";
                    break;
                }

                auto stats = ClientService::calculeazaStatistici(app);

                std::cout << "\n=== STATISTICI CLIENTI ===\n";
                std::cout << "Clienti total: " << stats.totalClienti << "\n";
                std::cout << "Clienti activi: " << stats.activi << "\n";
                std::cout << "Clienti inactivi: " << stats.inactivi << "\n";
                std::cout << "Clienti VIP: " << stats.vip << "\n\n";

                std::cout << "Comenzi totale: " << stats.totalComenzi << "\n";
                std::cout << "Valoare totala vanzari: "
                          << std::fixed << std::setprecision(2)
                          << stats.totalVanzari << " lei\n\n";

                if (stats.maxComenzi) {
                    std::cout << "Client cu cele mai multe comenzi: "
                              << stats.maxComenzi->getUsername()
                              << " (" << stats.maxComenzi->getNumarComenzi() << " comenzi)\n";
                }

                if (stats.maxCheltuieli) {
                    std::cout << "Client cu cele mai mari cheltuieli: "
                              << stats.maxCheltuieli->getUsername()
                              << " (" << stats.maxCheltuieli->getTotalCumparaturi()
                              << " lei)\n";
                }
                break;
            }
            case 2: {
                if (app.clienti.empty()) {
                    std::cout << "Nu exista clienti.\n";
                    break;
                }

                const int idx = filtreazaClienti(app);
                if (idx < 0 || idx >= static_cast<int>(app.clienti.size()))
                    break;

                auto det = ClientService::getDetaliiClient(*app.clienti[idx]);

                std::cout << "\n=== DETALII CLIENT ===\n";
                std::cout << "Username: " << det.username << "\n";
                std::cout << "Email: " << det.email << "\n";
                std::cout << "Sold: " << det.sold << " lei\n";
                std::cout << "Numar comenzi: " << det.nrComenzi << "\n";
                std::cout << "Total cumparaturi: " << det.totalCumparaturi << " lei\n";
                std::cout << "Puncte fidelitate: " << det.puncteFidelitate << "\n";
                std::cout << "Ierarhie client: " << det.ierarhie << "\n";
                break;
            }
                case 3: {
                if (app.logs.empty()) {
                    std::cout << "Nu exista log-uri.\n";
                    break;
                }

                std::cout << "\n=== LOGURI SISTEM ===\n";

                for (const auto&[timestamp, tip, email_client, detalii, user] : app.logs) {
                    const std::tm* tm = std::localtime(&timestamp);

                    std::cout << "[" << std::put_time(tm, "%Y-%m-%d %H:%M:%S") << "] ";

                    switch (tip) {
                        case Tiplog::CONT_CREAT: std::cout << "CONT CREAT"; break;
                        case Tiplog::CONT_STERS: std::cout << "CONT STERS"; break;
                        case Tiplog::LOGIN: std::cout << "LOGIN"; break;
                        case Tiplog::AUTENTIFICARE_ESUATA: std::cout << "AUTENTIFICARE ESUATA"; break;
                        case Tiplog::INCERCARI_MULTIPLE_AUTENTIFICARE_ESUATE:
                            std::cout << "INCERCARI MULTIPLE AUTENTIFICARE ESUATE"; break;
                        case Tiplog::COMANDA_FINALIZATA: std::cout << "COMANDA FINALIZATA"; break;
                        case Tiplog::COMANDA_ANULATA: std::cout << "COMANDA ANULATA"; break;
                        case Tiplog::REVIEW_ADAUGAT: std::cout << "REVIEW ADAUGAT"; break;
                    }

                    std::cout << " | user: " << user
                              << " | email: " << email_client;

                    if (!detalii.empty())
                        std::cout << " | " << detalii;

                    std::cout << "\n";
                }
                break;
            }
            case 0:break;
            default:
                    std::cout << "Optiune invalida.\n";


        }
    }
}

void BookStoreManager::meniuComenziA( const AppState& app) {
    int opt = -1;

    while (opt != 0) {
        std::cout << "\n--- MENIU COMENZI ---\n";
        std::cout << "1. Lista comenzi\n";
        std::cout << "2. Detalii comanda\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                if (app.comenzi.empty()) {
                    std::cout << "Nu exista comenzi.\n";
                    break;
                }

                std::cout << "\n=== LISTA COMENZI ===\n";
                for (size_t i = 0; i < app.comenzi.size(); ++i) {
                    const auto& c = app.comenzi[i];
                    std::cout << i << ". "
                              << "ID: " << c->getId()
                              << " | Data: " << c->getDataFormatata()
                              << " | Articole: " << c->getNumarArticole()
                              << " | Total: " << c->calculeazaTotal()
                              << " lei\n";
                    break;
                }
                case 2: {
                    const int idx = filtreComanda(app);
                    if (idx < 0 || idx >= static_cast<int>(app.comenzi.size()))
                        break;

                    const auto& cmd = app.comenzi[idx];

                    std::cout << "\n===== DETALII COMANDA =====\n";
                    std::cout << "ID comanda: " << cmd->getId() << "\n";
                    std::cout << "Data: " << cmd->getDataFormatata() << "\n";
                    std::cout << "Stare: " << cmd->getStare() << "\n";
                    std::cout << "Numar articole: " << cmd->getNumarArticole() << "\n\n";

                    std::cout << "--- Articole ---\n";
                    std::cout << *cmd << "\n";

                    std::cout << "Total comanda: "
                              << cmd->calculeazaTotal() << " lei\n";
                    std::cout << "===========================\n";
                    break;
                }
                case 0: break;
                default:
                    std::cout<<"Optiune invalida!\n";
            }

        }
    }
}

void BookStoreManager::meniuTop(  AppState &app) {
    int opt = -1;

    while (opt != 0) {
        std::cout << "\n=== TOPURI ===\n";
        std::cout << "1. Top clienti (dupa suma cheltuita)\n";
        std::cout << "2. Top autori (ierarhie)\n";
        std::cout << "3. Top publicatii (popularitate)\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                if (app.clienti.empty()) {
                    std::cout << "Nu exista clienti.\n";
                    break;
                }

                auto lista = app.clienti;

                std::ranges::sort(lista, [](const auto &a, const auto &b) {
                    return a->getTotalCumparaturi() > b->getTotalCumparaturi();
                });

                std::cout << "\n=== TOP CLIENTI DUPA SUMA CHELTUITA ===\n";
                int poz = 1;
                for (const auto &c : lista) {
                    std::cout << poz++ << ". "
                              << c->getEmail()
                              << " | Total cheltuit: "
                              << c->getTotalCumparaturi()
                              << " lei | Ierarhie: "
                              << c->ierarhie_clienti()
                              << "\n";
                }
                break;
            }

            case 2: {
                int opt1 = -1;
                while (opt1 != 0) {
                    std::cout << "\n--- TOP AUTORI ---\n";
                    std::cout << "1. Productivitate\n";
                    std::cout << "2. Scor renume\n";
                    std::cout << "3. Ierarhie\n";
                    std::cout << "0. Inapoi\n";
                    std::cout << "Optiune: ";
                    std::cin >> opt1;

                    TipTopAutor tip;

                    switch (opt1) {
                        case 1:
                            tip = TipTopAutor::PRODUCTIVITATE;
                            break;
                        case 2:
                            tip = TipTopAutor::SCOR_RENUME;
                            break;
                        case 3:
                            tip = TipTopAutor::IERARHIE;
                            break;
                        case 0:
                            break;
                        default:
                            std::cout << "Optiune invalida!\n";
                            continue;
                    }

                    auto top = AutorService::getTopAutori(app,tip);

                    if (top.empty()) {
                        std::cout << "Nu exista autori.\n";
                        continue;
                    }

                    std::cout << "\n=== TOP AUTORI ===\n";
                    int poz = 1;

                    for (const auto&[autor, productivitate, scor_renume, ierarhie] : top) {
                        std::cout << poz++ << ". "
                                  << autor->getNume() << " "
                                  << autor->getprenume()
                                  << " | Prod: " << productivitate
                                  << " | Scor: " << scor_renume
                                  << " | Ierarhie: " << ierarhie
                                  << "\n";
                    }

                }
                break;
            }

            case 3:
                meniuTopPublicatiiPopularitate(app);
                break;

            case 0:
                break;

            default:
                std::cout << "Optiune invalida.\n";
        }
    }
}

void BookStoreManager::meniuStatisticiA( AppState& app) {
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
            case 1: meniuStatisticiPublicatii(app);break;

            case 2: {
                auto s = ComandaService::calculeazaStatistici(app);

                std::cout << "\n=== STATISTICI COMENZI ===\n";
                std::cout << "Comenzi totale: " << s.total << "\n";
                std::cout << "Finalizate: " << s.finalizate << "\n";
                std::cout << "Anulate: " << s.anulate << "\n";
                std::cout << "Valoare totala: " << s.valoareTotala << " lei\n";
                std::cout << "Valoare medie comanda: " << s.valoareMedie << " lei\n";
                break;
            }
            case 3: {
                if (app.clienti.empty()) {
                    std::cout << "Nu exista clienti in sistem.\n";
                    break;
                }

                auto stats = ClientService::calculeazaStatistici(app);

                std::cout << "\n=== STATISTICI CLIENTI ===\n";
                std::cout << "Clienti total: " << stats.totalClienti << "\n";
                std::cout << "Clienti activi: " << stats.activi << "\n";
                std::cout << "Clienti inactivi: " << stats.inactivi << "\n";
                std::cout << "Clienti VIP: " << stats.vip << "\n\n";

                std::cout << "Comenzi totale: " << stats.totalComenzi << "\n";
                std::cout << "Incasari totale: "
                          << std::fixed << std::setprecision(2)
                          << stats.totalVanzari << " lei\n\n";

                if (stats.maxComenzi) {
                    std::cout << "Client cu cele mai multe comenzi: "
                              << stats.maxComenzi->getUsername()
                              << " (" << stats.maxComenzi->getNumarComenzi()
                              << " comenzi)\n";
                }

                if (stats.maxCheltuieli) {
                    std::cout << "Client cu cele mai mari cheltuieli: "
                              << stats.maxCheltuieli->getUsername()
                              << " (" << stats.maxCheltuieli->getTotalCumparaturi()
                              << " lei)\n";
                }
                break;

            }
            case 4:meniuTop(app);
            case 0: break;
            default:
                    std::cout<<"Optiune invalida!\n";


        }
    }

}



void BookStoreManager::CLIAdministrator(AppState &app) {
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
        std::cout << "3.Meniu Review-uri\n";
        std::cout << "4.Meniu Autor\n";
        std::cout << "5.Meniu Client\n";
        std::cout << "6.Meniu Comenzi\n";
        std::cout << "7.Statistici\n";
        std::cout << "0. Logout\n";
        std::cout << "Optiune: ";

        std::cin >> opt;
        switch (opt) {
            case 1: afiseazaReduceriActive(app);
                break;
            case 2: meniuPublicatiiA(app);
                break;
            case 3: meniuReviewuri(app);
                break;
            case 4: meniuAutorA(app);
                break;
            case 5: meniuClientA(app);
                break;
            case 6: meniuComenziA(app);
                break;
            case 7:meniuStatisticiA(app);break;
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}
void BookStoreManager::afisazaContinutPachet(const AppState &app, int idx) {
    auto pachet = app.pachetePredefinite[idx];

    std::cout << "\n=== CONTINUT PACHET ===\n";
    std::cout << "Identificator pachet: "
              << pachet->getIdentificator() << "\n";

    const auto& continut = pachet->getContinut();

    if (continut.empty()) {
        std::cout << "(Pachet gol)\n";
        return;
    }

    for (size_t i = 0; i < continut.size(); ++i) {
        if (continut[i]) {
            std::cout << "  " << i << ". "
                      << continut[i]->getTitlu()
                      << " | ID: "
                      << continut[i]->getIdentificator()
                      << "\n";
        }
    }
}
void BookStoreManager::meniuCautaPublicatii(AppState &app) {
    if (!clientCurent) {
        std::cout<<"Trebuie sa fii autentificat!\n";
        return;
    }
    auto pub=selecteazaPubClient(app);
    if (!pub)
        return;
    std::cout << "\n=== DETALII PUBLICATIE ===\n";
    pub->afisare(std::cout);
    std::cout << "Pret: " << pub->getPretFinal() << " lei\n";
    std::cout << "Stoc disponibil: " << pub->getcantitate() << "\n";

    afiseazaReviewuriClient(app, pub->getIdentificator());

    int opt;
    std::cout << "\n1. Adauga in cos\n";
    std::cout << "0. Inapoi\n";
    std::cout << "Optiune: ";
    std::cin >> opt;

    if (opt != 1)
        return;

    int cantitate;
    std::cout << "Cantitate dorita: ";
    std::cin >> cantitate;

    if (cantitate <= 0) {
        std::cout << "Cantitate invalida.\n";
        return;
    }

    if (cantitate > pub->getcantitate()) {
        std::cout << "Stoc insuficient! Disponibil: "
                  << pub->getcantitate() << "\n";
        return;
    }

    try {
        clientCurent->adaugaComanda(pub,cantitate);
        std::cout << "Publicatia a fost adaugata in cos!\n";
    } catch (const std::exception& e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }
}

void BookStoreManager::meniuAdaugaPachete(const AppState &app,std::shared_ptr<Client>& clientCurent,
    std::shared_ptr<Comanda>& comandaActiva) {
    int opt=-1;
    while (opt!=0) {
        std::cout<<"\n === MENIU ADAUGA PACHETE ===\n";
        std::cout <<"1. Adauga pachet predefinit\n ";
        std::cout<<"2. Creaza pachet si adauga \n";
        std::cout<<"0. Inapoi\n";
        std::cout<<"Optiune:";

        std::cin>>opt;
        switch (opt) {
            case 1: {
                if (app.pachetePredefinite.empty()) {
                    std::cout<<" Nu exista momentan pachete predefinite!";
                    break;
                }

                std::cout << "\n=== PACHETE PREDEFINITE ===\n";
                for (size_t i = 0; i < app.pachetePredefinite.size(); ++i) {
                    std::cout << i << ". "
                              << app.pachetePredefinite[i]->getDescriere()
                              << "\n";
                }
                int idx;
                std::cout << "\nAlege indexul pachetului (sau -1 pentru inapoi): ";
                std::cin >> idx;

                if (idx == -1)
                    break;

                if (idx < 0 || idx >= static_cast<int>(app.pachetePredefinite.size())) {
                    std::cout << "Index invalid!\n";
                    break;
                }
                std::cout << "\n=== CONTINUT PACHET ===\n";
                afisazaContinutPachet(app, idx);
                int opt2;
                std::cout << "\n1. Adauga pachetul in cos\n";
                std::cout << "0. Inapoi\n";
                std::cout << "Optiune: ";
                std::cin >> opt2;

                if (opt2 == 1) {
                    try {
                        CosService::adaugaPachetPredefinit(app,clientCurent,comandaActiva,idx);
                        std::cout << "Pachet adaugat in cos.\n";
                    }
                    catch (const std::exception& e) {
                        std::cout << "Eroare: " << e.what() << "\n";
                    }
                }

                break;

            }
                case 2: {
                 std::vector<std::shared_ptr<Publicatie>> continut;

    TipPublicatie tipAcceptat = TipPublicatie::Necunoscut;
    bool continua = true;

    std::cout << "\n=== CREARE PACHET PERSONALIZAT ===\n";
    std::cout << "Selecteaza publicatii (toate trebuie sa fie de acelasi tip).\n";

    while (continua) {
        int idx = selecteazaPub(app);

        if (idx < 0 || idx >= static_cast<int>(app.publicatii.size())) {
            std::cout << "Selectie invalida!\n";
            continue;
        }

        auto pub = app.publicatii[idx];
        TipPublicatie tipCurent = determinaTipPublicatie(pub);

        if (tipCurent == TipPublicatie::Necunoscut) {
            std::cout << "Tip de publicatie neacceptat!\n";
            continue;
        }

        if (tipAcceptat == TipPublicatie::Necunoscut) {
            tipAcceptat = tipCurent;
            continut.push_back(pub);
            std::cout << "Publicatie adaugata.\n";
        }
        else if (tipCurent == tipAcceptat) {
            continut.push_back(pub);
            std::cout << "Publicatie adaugata.\n";
        }
        else {
            std::cout << "Pachetul permite doar publicatii de acelasi tip!\n";
            continue;
        }

        int opt1;
        std::cout << "\n1. Adauga alta publicatie\n";
        std::cout << "2. Finalizeaza pachet\n";
        std::cout << "0. Anuleaza\n";
        std::cout << "Optiune: ";
        std::cin >> opt1;

        if (opt1 == 1) {
            continua = true;
        }
        else if (opt1== 2) {
            continua = false;
        }
        else {
            std::cout << "Crearea pachetului a fost anulata.\n";
            continut.clear();
            break;
        }
    }

    if (continut.size() < 2) {
        std::cout << "Un pachet trebuie sa contina cel putin 2 publicatii.\n";
        break;
    }

    std::string numePachet, tipPachet;
    std::cout << "Nume pachet: ";
    std::cin >> numePachet;
    std::cout << "Tip pachet: ";
    std::cin >> tipPachet;

    auto pachetCreat = std::make_shared<PachetSerie>(
        continut,
        numePachet,
        tipPachet,
        true
    );

    try {
        CosService::adaugaPachetCreat(
            clientCurent,
            comandaActiva,
            pachetCreat
        );
        std::cout << "Pachetul personalizat a fost adaugat in cos.\n";
    }
    catch (const std::exception& e) {
        std::cout << "Eroare: " << e.what() << "\n";
    }

    break;
            }
            case 0:
                break;
            default:
                    std::cout<<"Optiune Invalida!\n";
        }
    }
}



void BookStoreManager::meniuCosCumparaturi(AppState& app,std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva) {
    int opt=-1;
    while (opt!=0) {
        std::cout<<"\n=== MENIU COS CUMPARATURI ===\n ";
        std::cout<<"1. Adauga articol idividual in cos \n";
        std::cout<<"2. Adauga articol pachet \n";
        std::cout<<"3. Sterge articol din cos \n";
        std::cout<<"4. Vezi cosul \n";
        std::cout<<"5. Finalizeaza comanda\n";
        std::cout<<"0. Inapoi\n";
        std::cout<<"Optiune: ";

        std::cin>>opt;

        switch (opt) {
            case 1: {
                int idx=-1;
                idx=selecteazaPub(app);
                if (idx==-1)
                    std::cout<<"Publicatia nu a fost gasita\n";
                if (idx < 0 || idx >= static_cast<int>(app.publicatii.size())) {
                    std::cout << "Index invalid.\n";
                    break;
                }
                int cantitate;
                std::cout<<"Introduceti cantitatea dorita:";
                std::cin>>cantitate;
                CosService::adaugaCarteIndividuala(clientCurent,comandaActiva,cantitate,app.publicatii[idx]);
                break;
            }
            case 2 : {
                meniuAdaugaPachete(app,clientCurent,comandaActiva); break;
            }
            case 3: {
                if (!comandaActiva || comandaActiva->getNumarArticole() == 0) {
                    std::cout << "Cosul este gol!\n";
                    break;
                }

                std::cout << "\n=== COS CURENT ===\n";

                const auto& articole = comandaActiva->getArticole();
                for (size_t i = 0; i < articole.size(); ++i) {
                    std::cout << i << ". "
                              << articole[i].getUnitate()->getDescriere()
                              << " x" << articole[i].getCantitate()
                              << " = " << articole[i].getSubtotal()
                              << " lei\n";
                }

                int idx;
                std::cout << "Index articol de sters (-1 pentru inapoi): ";
                std::cin >> idx;

                if (idx == -1)
                    break;

                try {
                    comandaActiva->stergeArticol(idx);
                    std::cout << "Articol sters din cos.\n";
                }
                catch (const std::exception& e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }

                break;
            }
            case 4: {
                if (!comandaActiva || comandaActiva->getNumarArticole() == 0) {
                    std::cout << "Cosul este gol!\n";
                    break;
                }

                std::cout << "\n=== COS CUMPARATURI ===\n";

                const auto& articole = comandaActiva->getArticole();
                for (size_t i = 0; i < articole.size(); ++i) {
                    std::cout << i << ". "
                              << articole[i].getUnitate()->getDescriere()
                              << " x" << articole[i].getCantitate()
                              << " = " << articole[i].getSubtotal()
                              << " lei\n";
                }

                std::cout << "-----------------------------\n";
                std::cout << "TOTAL: " << comandaActiva->calculeazaTotal() << " lei\n";

                break;
            }
            case 5: {
               if (!comandaActiva || comandaActiva->getNumarArticole() == 0) {
    std::cout << "Nu exista o comanda activa sau cosul este gol!\n";
    break;
}

try {

    double total = comandaActiva->calculeazaTotal();
    std::cout << "Total comanda: " << total << " lei\n";


    int pctDisponibile = clientCurent->getPunctedeFideliate();

    if (pctDisponibile > 0) {
        std::cout << "Aveti " << pctDisponibile << " puncte de fidelitate.\n";
        std::cout << "Doriti sa folositi puncte? (1 = Da, 0 = Nu): ";

        int opt3;
        std::cin >> opt3;

        if (opt3 == 1) {
            int pct;
            std::cout << "Cate puncte doriti sa folositi? ";
            std::cin >> pct;

            double reducere = clientCurent->foloseste_pct_fidelitate(pct);
            total -= reducere;

            if (total < 0)
                total = 0;

            std::cout << "Total dupa reducere: " << total << " lei\n";
        }
    }


    if (clientCurent->getSold() < total) {
        throw SoldInsuficientException(clientCurent->getSold(), total);
    }


    comandaActiva->finalizareComanda();

    std::cout << "\nComanda a fost finalizata cu succes!\n";
    std::cout << "Total platit: " << total << " lei\n";


    comandaActiva.reset();

} catch (const ComandaGoalaException&) {
    std::cout << "Eroare: comanda este goala!\n";
} catch (const ComandaAnulataException&) {
    std::cout << "Eroare: comanda este anulata!\n";
} catch (const ComandaFinalizataException&) {
    std::cout << "Eroare: comanda este deja finalizata!\n";
} catch (const SoldInsuficientException&) {
    std::cout << "Fonduri insuficiente pentru finalizarea comenzii!\n";
} catch (const StocInsuficientException&) {
    std::cout << "Stoc insuficient pentru unul dintre produse!\n";
} catch (const DateInvalideException& e) {
    std::cout << "Date invalide: " << e.what() << "\n";
} catch (const std::exception& e) {
    std::cout << "Eroare neasteptata: " << e.what() << "\n";
}

break;
            }
            case 0: break;
                default:
                std::cout<<"Optiune Invalida!\n";
        }
    }
}

void BookStoreManager::meniuSH_Tradein(AppState& app) {
    int opt=-1;
    while (opt!=0) {
        std::cout<<" \n === MENIU TRADE-IN \n===";
        std::cout<<"1. Adauga carte pt trade-in\n";
        std::cout<<"2. Cumpara carte Second-Hand\n";
        std::cout<<"3. Creeaza si cumpara pachet sh\n";
        std::cout<<"0.Inapoi\n";
        std::cout<<"Opriune:";

        std::cin>>opt;
        switch (opt) {
            case 1: {
                int idx=-1;
                idx=selecteazaPub(app);
                if (idx==-1) {
                    std::cout<<"Publicatia nu a fost gasita!";
                    break;
                }
                auto pub = app.publicatii[idx];
                std::shared_ptr<UnitateVanzare> unitate;

                if (auto carte = std::dynamic_pointer_cast<Carte>(pub)) {
                    unitate = std::make_shared<CarteIndividuala>(carte);
                }
                else if (auto revista = std::dynamic_pointer_cast<Revista>(pub)) {
                    unitate = std::make_shared<RevistaIndividuala>(revista);
                }
                else {
                    std::cout << "Acest tip de publicatie nu este acceptat la trade-in.\n";
                    break;
                }

                std::string conditie;
                std::cout<<"Conditie (Noua/Buna/Acceptabila/Uzata): ";
                int vechime;
                std::cout << "Vechime:\n";
                std::cout << "1. < 6 luni\n";
                std::cout << "2. 6-12 luni\n";
                std::cout << "3. 1-2 ani\n";
                std::cout << "4. > 2 ani\n";
                std::cout << "Optiune: ";
                std::cin >> vechime;
                int luni = 0;
                switch (vechime) {
                    case 1: luni = 3; break;
                    case 2: luni = 9; break;
                    case 3: luni = 18; break;
                    case 4: luni = 30; break;
                    default: luni = 6; break;
                }
                unitate->marcheazaSecondHand(conditie, luni);
                if (!TradeInService::esteEligibila(*unitate)) {
                    std::cout << "Publicatia NU este eligibila pentru trade-in.\n";
                    break;
                }
                double estimare =TradeInService::calculeazaValoareTradeIn(*unitate, luni);
                std::cout << "Valoare estimata trade-in: "<< estimare << " lei\n";
                std::string optiune;
                std::cout << "Alege recompensa (BANI / REDUCERE / PUNCTE): ";
                std::cin >> optiune;
                try {
                    // 9. Procesare trade-in
                    double valoare_finala =
                        TradeInService::proceseazaTradeIn(
                            *clientCurent,*unitate,app,optiune);

                    std::cout << "Trade-In realizat cu succes!\n";
                    std::cout << "Valoare acordata: "
                              << valoare_finala << " lei\n";

                } catch (const std::exception& e) {
                    std::cout << "Eroare la trade-in: "
                              << e.what() << "\n";
                }

                break;
            }
            case 2: {
                if (app.stocSH.empty()) {
                    std::cout << "Nu exista produse SH disponibile.\n";
                    break;
                }

                std::cout << "\n=== STOC SECOND-HAND ===\n";
                for (size_t i = 0; i < app.stocSH.size(); ++i) {
                    std::cout << i << ". " << *app.stocSH[i] << "\n";
                }

                std::cout << "-1. Inapoi\n";
                std::cout << "Selecteaza produsul SH: ";

                int idx;
                std::cin >> idx;

                if (idx == -1)
                    break;

                if (idx < 0 || idx >= static_cast<int>(app.stocSH.size())) {
                    std::cout << "Index invalid.\n";
                    break;
                }

                auto unitateSH = app.stocSH[idx];

                try {
                    CosService::adaugaUnitataVanzare(clientCurent,comandaActiva,unitateSH);

                    app.stocSH.erase(app.stocSH.begin() + idx);

                    std::cout << "Produs SH adaugat in cos.\n";

                } catch (const std::exception& e) {
                    std::cout << "Eroare: " << e.what() << "\n";
                }

                break;
            }
            case 3: {

    if (app.stocSH.size() < 2) {
        std::cout << "Nu exista suficiente produse SH pentru a crea un pachet.\n";
        break;
    }

    std::vector<std::shared_ptr<UnitateVanzare>> continutSH;
     bool continua = true;

    std::cout << "\n=== CREARE PACHET SECOND-HAND ===\n";
    std::cout << "Selecteaza produse SH. Cand esti gata, apasa 0.\n";

    while (continua) {

        std::cout << "\n--- STOC SH DISPONIBIL ---\n";
        for (size_t i = 0; i < app.stocSH.size(); ++i) {
            std::cout << i << ". " << *app.stocSH[i] << "\n";
        }

        std::cout << "-1. Finalizeaza pachet\n";
        std::cout << "Selectie: ";

        int idx;
        std::cin >> idx;

        if (idx == 0) {
            break;
        }

        if (idx < 0 || idx >= static_cast<int>(app.stocSH.size())) {
            std::cout << "Index invalid.\n";
            continue;
        }

        // adaugam produsul in pachet
        continutSH.push_back(app.stocSH[idx]);

        // il scoatem din stoc SH
        app.stocSH.erase(app.stocSH.begin() + idx);

        std::cout << "Produs adaugat in pachet. Total produse: "
                  << continutSH.size() << "\n";
    }

    if (continutSH.size() < 2) {
        std::cout << "Un pachet SH trebuie sa contina minim 2 produse.\n";

        // punem produsele inapoi
        for (auto& u : continutSH)
            app.stocSH.push_back(u);

        break;
    }

    try {
        // cream pachetul SH
        auto pachetSH = std::make_shared<PachetSerie>(continutSH);


        CosService::adaugaUnitataVanzare(
            clientCurent,
            comandaActiva,
            pachetSH
        );

        std::cout << "Pachetul SH a fost creat si adaugat in cos!\n";

    } catch (const std::exception& e) {
        std::cout << "Eroare la creare pachet SH: " << e.what() << "\n";


        for (auto& u : continutSH)
            app.stocSH.push_back(u);
    }

    break;
            }
            case 0: break;
            default:
                    std::cout<<"Optiune invalida!\n";
        }
    }

}
void BookStoreManager::adaugaReview_Rating(AppState& app) const {
    int opt=-1;
    while (opt!=0)  {
        std::cout<<"\n === MENIU REVIEW-URI === \n";
        std:: cout<<"1. Adauga review si rating unei publicatii\n";
        std::cout<< "2. Vezi review-uri si ratinguri ale unei publicatii\n";
        std::cout<<"0. Inapoi\n";
        std::cout<<"optiunea:";
        std::cin>>opt;
        switch (opt) {
            case 1: {
                int idx = selecteazaPub(app);
                if (idx < 0 || idx >= static_cast<int>(app.publicatii.size())) {
                    std::cout << "Selectie invalida.\n";
                    break;
                }

                auto pub = app.publicatii[idx];
                std::string identificator = pub->getIdentificator();
                bool esteVerificat =clientCurent->aCumparatPub(identificator);

                if (!esteVerificat) {
                    std::cout
                        << "Review NEVERIFICAT (nu ati cumparat aceasta publicatie)\n";
                }
                int rating;
                std::cout << "Rating (1 - 5): ";
                std::cin >> rating;

                if (rating < 1 || rating > 5) {
                    std::cout << "Rating invalid!\n";
                    break;
                }

                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                std::string text;
                std::cout << "Review: ";
                std::getline(std::cin, text);

                Review r(clientCurent->getUsername(),identificator,rating,text,esteVerificat);

                app.reviews.push_back(r);

                std::cout << "Review adaugat cu succes!\n";

                break;
            }
            case 2: {
                const int idx = selecteazaPub(app);
                if (idx < 0) break;

                const auto pub = app.publicatii[idx];

                auto reviewuri = ReviewService::getReviewuriSortate(
                    app,
                    pub->getIdentificator()
                );

                if (reviewuri.empty()) {
                    std::cout << "Nu exista review-uri pentru aceasta publicatie.\n";
                    break;
                }

                std::cout << "\n=== REVIEW-URI PENTRU: "
                          << pub->getTitlu() << " ===\n";

                for (size_t i = 0; i < reviewuri.size(); ++i) {
                    const Review* r = reviewuri[i];
                    std::cout << "[" << i << "] "
                              << (r->verificat ? "[VERIFICAT] " : "[NEVERIFICAT] ")
                              << r->username << " | Rating: " << r->rating << "\n";
                    std::cout << "   \"" << r->text << "\"\n";
                    std::cout << "---------------------------------\n";
                }
                break;
            }
            case 0: break;
            default:
                std::cout<<"Optiune invalida!\n";
        }
    }

}
void BookStoreManager::meniuDetaliiCont(AppState& app) {
    int opt = -1;
    while (opt != 0) {

        std::cout << "\n=== DETALII CONT ===\n";
        std::cout << "1. Informatii generale\n";
        std::cout << "2. Sold si puncte fidelitate\n";
        std::cout << "3. Statistici comenzi\n";
        std::cout << "4. Review-urile mele\n";
        std::cout << "0. Inapoi\n";
        std::cout << "Optiune: ";

        std::cin >> opt;

        switch (opt) {
            case 1: {
                std::cout << "\n--- DATE CLIENT ---\n";
                std::cout << "Username: " << clientCurent->getUsername() << "\n";
                std::cout << "Email: " << clientCurent->getEmail() << "\n";
                std::cout << "Telefon: " << clientCurent->getTelefon() << "\n";
                std::cout << "Status: " << clientCurent->ierarhie_clienti() << "\n";
                break;
            }
            case 2: {
                std::cout << "\n--- SOLD & FIDELITATE ---\n";
                std::cout << "Sold curent: "
                          << clientCurent->getSold() << " lei\n";
                std::cout << "Puncte fidelitate: "
                          << clientCurent->getPunctedeFideliate() << "\n";
                break;
            }
            case 3: {
                std::cout << "\n--- STATISTICI COMENZI ---\n";
                std::cout << "Numar comenzi: "
                          << clientCurent->getNumarComenzi() << "\n";
                std::cout << "Total cheltuit: "
                          << clientCurent->getTotalCumparaturi()
                          << " lei\n";
                break;
            }
            case 4: {
                std::cout << "\n--- REVIEW-URILE MELE ---\n";

                bool gasit = false;
                for (const auto& r : app.reviews) {
                    if (r.username == clientCurent->getUsername()) {
                        gasit = true;

                        std::cout << "\nPublicatie: "
                                  << r.identificator_publicatie << "\n";

                        std::cout << "Rating: "
                                  << r.rating << "/5 ";

                        if (r.verificat)
                            std::cout << "[VERIFICAT]\n";
                        else
                            std::cout << "[NEVERIFICAT]\n";

                        std::cout << "Review: " << r.text << "\n";
                    }
                }

                if (!gasit) {
                    std::cout << "Nu ati scris niciun review.\n";
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
void BookStoreManager::CLIClient(AppState &app) {
    clientCurent = autentificareClientUI(app);
    comandaActiva=nullptr;
    if (!clientCurent) {
        return;
    }

    int opt = -1;
    while (opt != 0) {
        std::cout << "=== MENIU CLIENT ===\n";
        std::cout << "1. Reduceri active!\n";
        std::cout << "2. Cauta Publicatii\n";
        std::cout<<  "3. Afisare sugestii dupa cuparataturi\n";
        std::cout<<  "4. Publiatii Secon-hand/ Trade-in pulicatii\n";
        std::cout << "5. Cos de cumparaturi\n";
        std::cout << "6. Review-uri si rating\n";
        std::cout << "7. Istoric comenzi\n";
        std::cout << "8. Detalii cont\n";
        std::cout << "9. Alimentare cont\n";
        std::cout << "0. Logout\n";
        std::cout << "Optiune: ";

        std::cin >> opt;
        switch (opt) {
            case 1: afiseazaReduceriActive(app);
                break;
            case 2: {
                    meniuCautaPublicatii(app);break;
            }
            case 3: {
                auto sugestii = clientCurent->genereazaSugestii();

                if (sugestii.empty()) {
                    std::cout << "Momentan nu exista sugestii personalizate.\n";
                    break;
                }

                std::cout << "\n=== SUGESTII PENTRU TINE ===\n";
                for (const auto& s : sugestii) {
                    std::cout << "- [" << s.getCategorie() << "] "
                              << s.getTitlu() << "\n"
                              << "  " << s.getDescriere() << "\n";
                }
                break;
            }
            case 4: meniuSH_Tradein(app);break;
            case 5: meniuCosCumparaturi(app,clientCurent,comandaActiva);
                break;
            case 6:adaugaReview_Rating(app);
                break;
            case 7: {
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
                break;
            }
            case 8: meniuDetaliiCont(app);
                break;
            case 9: {
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




void BookStoreManager::afisareMeniuPrincipal(AppState&app) {
    int opt = -1;
    while (opt != 0) {
        std::cout << "\n=== LIBRARIE  ===\n";
        std::cout << "1. Administrator\n";
        std::cout << "2. Client\n";
        std::cout << "0. Iesire\n";
        std::cout << "Optiune: ";

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



void BookStoreManager::run(AppState& app) {
    afisareMeniuPrincipal(app);
}
