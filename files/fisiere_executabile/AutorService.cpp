#include "../headere/AutorService.h"
#include "../headere/AppState.h"
#include "../headere/Autor.h"

#include <algorithm>

static int prioritateIerarhie(const std::string& i) {
    if (i == "Legendar") return 5;
    if (i == "Renumit") return 4;
    if (i == "Consacrat") return 3;
    if (i == "In ascensiune") return 2;
    return 1; // Incepator
}

AutorStat AutorService::getDetaliiAutor(const AppState&, const std::shared_ptr<Autor> &autor) {
    AutorStat stat;
    stat.autor = autor;
    stat.productivitate = autor->calcproductivitate();
    stat.scor_renume = autor->calcscorrenume();
    stat.ierarhie = autor->ierarhie_a();

    return stat;
}



std::vector<AutorStat> AutorService::getTopAutori( AppState &app, TipTopAutor tip) {
    std::vector<AutorStat> lista;

    for (const auto& a : app.autor) {
        AutorStat stat;
        stat.autor = a;
        stat.productivitate = a->calcproductivitate();
        stat.scor_renume = a->calcscorrenume();
        stat.ierarhie = a->ierarhie_a();
        lista.push_back(stat);
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

void AutorService::adaugaAutor(AppState &app, const std::shared_ptr<Autor> &autor) {
    app.autor.push_back(autor);
}

