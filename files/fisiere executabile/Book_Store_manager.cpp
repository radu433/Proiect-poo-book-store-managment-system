#include "../headere/Book_Store_manager.h"

#include <iomanip>
#include <iostream>
#include <limits>

v

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

void BookStoreManager::CLIClient(AppState &app) {
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
