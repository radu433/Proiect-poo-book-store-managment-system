#include "../headere/InventoryService.h"

#include <iostream>

#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../headere/Publicatie.h"

void InventoryService::adaugaStoc(Publicatie &p, int cantitate) {
    if (cantitate<0)
        throw DateInvalideException("Cantitate invalida!");
    p.adauga_stoc(cantitate);
}

void InventoryService::scadeStoc(Publicatie &p, int cantitate) {
    if (cantitate <= 0)
        throw DateInvalideException("Cantitate invalida!");

    if (!p.Scade_stoc(cantitate))
        throw DateInvalideException("Stoc insuficient.");
}

std::vector<PrioritateRestoc> InventoryService::calculeazaPrioritateRestoc(const AppState &app) {
    std::vector<PrioritateRestoc> rezultat;

    for (const auto& p : app.publicatii) {
        const double popularitate = p->CalculeazaScorPopularitate();
        const int stoc = p->getcantitate();

        const double scor = popularitate / (stoc + 1);

        rezultat.push_back({p, scor});
    }

    std::ranges::sort(rezultat,
                      [](const auto& a, const auto& b) {
                          return a.scor > b.scor;
                      });

    return rezultat;
}
