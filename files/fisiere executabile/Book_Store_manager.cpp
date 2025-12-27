#include "../headere/Book_Store_manager.h"
#include "../headere/AuthenticationService.h"
#include "../headere/adresa.h"
#include "../headere/Client.h"
#include "../headere/ReviewService.h"

#include <iomanip>
#include <iostream>
#include <limits>

#include "../headere/AutorService.h"
#include "../headere/ClientService.h"
#include "../headere/PublicatieService.h"
#include "../headere/SerchService.h"


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

int BookStoreManager::filtreazaClienti(AppState &app) {
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
// meniu publicatii administrator
void BookStoreManager::meniuPublicatiiA(AppState &app) {
    int opt = -1;
    while (opt!=0) {
        std::cout<<"\n=== MENIU PUBLICATII ===\n";
        std::cout << "1. Adauga Publicatii\n";
        std::cout << "2. Dezactiveaza publicatie\n";
        std::cout << "3. Sterge Publicatie\n";
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
                    return;
                }

                std::cout << "\n=== LOG-URI SISTEM ===\n";
                for (const auto& log : app.logs) {
                    std::cout << log << "\n";
                }
                break;
            }

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
            case 5: meniuClientA();
                break;
            case 6: meniuComenzi();
                break;
            case 7:meniuStatistici();break;
            case 0: break;
            default:
                std::cout << "Optiune invalida!\n";
        }
    }
}

void BookStoreManager::CLIClient(AppState &app) {
    clientCurent = autentificareClientUI(app);
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
        std::cout << "Optiune: ";

        std::cin >> opt;
        switch (opt) {
            case 1: afiseazaReduceriActive(app);
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
                break;
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
