#include "../headere/InventoryService.h"

#include <iostream>

void InventoryService::modificaStocPublicatie(AppState& app, int idx) {
    if (app.publicatii.empty()) {
        std::cout << "Nu exista publicatii in sistem.\n";
        return;
    }

    std::cout << "\n=== DEZACTIVARE PUBLICATIE ===\n";
    std::cout << "1. Cauta publicatia\n";
    std::cout << "2. Filtreaza publicatiile\n";
    std::cout << "0. Anulare\n";
    std::cout << "Optiune: ";

    int opts;
    std::cin >> opts;

    idx = -1;
    switch (opts) {
        case 1:
            idx = SerchService::Serchbar(app);
            break;
        case 2:
            idx = SerchService::filtrePublicatie(app);
            break;
        case 0:
            return;
        default:
            std::cout << "Optiune invalida.\n";
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
