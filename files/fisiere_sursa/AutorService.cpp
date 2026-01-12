#include "../headere/AutorService.h"
#include "../headere/AppState.h"
#include "../headere/Autor.h"

#include <algorithm>

#include "../exceptii/exceptii_headere/AutorException.h"

static int prioritateIerarhie(const std::string& i) {
    if (i == "Legendar") return 5;
    if (i == "Renumit") return 4;
    if (i == "Consacrat") return 3;
    if (i == "In ascensiune") return 2;
    return 1; // Incepator
}

AutorStat AutorService::getDetaliiAutor(const AppState&, const Autor* autor) {
    AutorStat stat;
    stat.autor = autor;
    stat.productivitate = autor->calcproductivitate();
    stat.scor_renume = autor->calcscorrenume();
    stat.ierarhie = autor->ierarhie_a();

    return stat;
}



std::vector<AutorStat> AutorService::getTopAutori( const AppState &app, const TipTopAutor tip) {
    std::vector<AutorStat> lista;

    for ( auto& a : app.getAutor()) {
        lista.push_back({&a,a.calcproductivitate(),static_cast<double>(a.calcscorrenume()),
            a.ierarhie_a()});
    }


    switch (tip) {
        case TipTopAutor::PRODUCTIVITATE:
            std::ranges::sort(lista, [](const auto& a, const auto& b) {
                return a.productivitate > b.productivitate;
            });
            break;

        case TipTopAutor::SCOR_RENUME:
            std::ranges::sort(lista, [](const auto& a, const auto& b) {
                return a.scor_renume > b.scor_renume;
            });
            break;

        case TipTopAutor::IERARHIE:
            std::ranges::sort(lista, [](const auto& a, const auto& b) {
                const int pa = prioritateIerarhie(a.ierarhie);
                if (const int pb = prioritateIerarhie(b.ierarhie); pa != pb)
                    return pa > pb;
                return a.scor_renume > b.scor_renume;
            });
            break;
    }

    return lista;

}

void AutorService::adaugaAutor(AppState &app, const Autor* autor) {
    app.adaugaAutor(*autor);
}

void AutorService::asociazaCarte( AppState& app, const std::string& isbn, const std::string& numeA, const std::string& prenumeA) {
    for (auto& a : app.getAutor1()) {
        if (a.getNume() == numeA && a.getprenume() == prenumeA) {
            a.adauga_carte(isbn);
            return;
        }
    }
    throw AutorException("Autorul nu a fost gasit");

}

