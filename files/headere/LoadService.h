

#ifndef OOP_LOADSERVICE_H
#define OOP_LOADSERVICE_H
#include "AppState.h"
class LoadService {
    public:

    static void salveaza(const AppState& app);
    static void incarca( AppState& app);
private:
    // autori
    static void salveazaAutori(const AppState& app);
    static void incarcaAutori(AppState& app);
    // publciatii
    static void salveazaPublicatii(const AppState& app);
    static void incarcaPublicatii(AppState& app);
    //clienti
    static void salveazaClienti(const AppState& app);
    static void incarcaClienti(AppState& app);
    // stoc sh
    static void salveazaStocSH(const AppState& app);
    static void incarcaStocSH(AppState& app);
    //pachete
    static void salveazaPachete(const AppState& app);
    static void incarcaPachete(AppState& app);
    //comenzi
    static void salveazaComenzi(const AppState& app);
    static void incarcaComenzi(AppState& app);
    // reduceri
    static void salveazaReducerile(const AppState& app);
    static void incarcaReducerile(AppState& app);
    // review
    static void salveazaReviews(const AppState& app);
    static void incarcaReviews(AppState& app);
    //logs
    static void salveazaLogs(const AppState& app);
    static void incarcaLogs(AppState& app);


};
#endif //OOP_LOADSERVICE_H