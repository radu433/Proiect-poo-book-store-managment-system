

#ifndef OOP_REDUCERESERVICE_H
#define OOP_REDUCERESERVICE_H
#include <random>

#include "AppState.h"
#include "Carte.h"
#include "CarteStiintifica.h"
#include "Manual.h"
#include "Reducere.h"
#include "Revista.h"

class ReducereService {
    public:
    static void aplicaReducerePublicatii(AppState& app);
    static double calculeazaPretFinalCuReduceri(const AppState& app,const std::shared_ptr<Publicatie>& p);
};

inline void ReducereService::aplicaReducerePublicatii(AppState &app) {
    if (app.publicatii.empty())
        return;

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    int saptamana = now->tm_yday / 7;
    int procent = std::vector<int>{5, 10, 15}[saptamana % 3];

    enum Tip { CARTE, MANUAL, REVISTA, STIINTIFICA };
    Tip tip = static_cast<Tip>(saptamana % 4);

    std::vector<std::shared_ptr<Publicatie>> candidati;

    for (const auto& p : app.publicatii) {
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

    if (candidati.empty())
        return;

    std::mt19937 gen(saptamana);
    std::ranges::shuffle(candidati, gen);

    const int nr = std::min(4, static_cast<int>(candidati.size()));

    app.reduceri.clear();

    std::time_t start = t;
    std::time_t sfarsit = t + 24 * 60 * 60;

    for (int i = 0; i < nr; ++i) {
        app.reduceri.emplace_back(
            candidati[i],
            procent,
            start,
            sfarsit
        );
    }
}

inline double ReducereService::
calculeazaPretFinalCuReduceri(const AppState &app, const std::shared_ptr<Publicatie> &p) {
    if (!p)
        return 0.0;

    double pret = p->getPretFinal();

    for (const auto& r : app.reduceri) {
        if (r.seAplicaLa(p)) {
            pret -= pret * r.getProcent() / 100.0;
            break; // o singura reducere
        }
    }
    return pret;
}
#endif //OOP_REDUCERESERVICE_H
