
#ifndef OOP_BOOK_STORE_MANAGER_H
#define OOP_BOOK_STORE_MANAGER_H
#include "AppState.h"
#include "ReducereService .h"
#include "AutorService.h"
#include "CosService.h"


class BookStoreManager {
private:
    std::shared_ptr<Client> clientCurent;
    std::shared_ptr<Comanda> comandaActiva;

    // afisarea reducerilor active
    static void afiseazaReduceriActive(const AppState& app);
    // meniuri principale
    void afisareMeniuPrincipal(AppState& app);

    static void CLIAdministrator(AppState& app);
    void CLIClient(AppState& app);

    // submeniuri Administrator
    static void meniuPublicatiiA(AppState& app);
    void meniuAdaugaPublicatie();

    static void meniuReviewuri(AppState& app);

    static void meniuAutorA(AppState& app);

    static void meniuClientA(AppState& app);

    static void meniuComenziA( const AppState& app);

    static void meniuStatisticiA( AppState& app);

    void aplicaReducerePublicatii();

    //submeniuri Client
    void meniuCautaPublicatii(AppState& app);

     void meniuSH_Tradein(AppState& app);
    static void meniuCosCumparaturi(AppState& app,std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva);
    void adaugaReview_Rating(AppState& app) const;// clasa de publicatii
    void meniuDetaliiCont(AppState& app);

    void afiseazaReducere();
    //functii din meniu clienti

    void listaIstoricComenzi();// clasa client
    void alimentareCont();//clasa client

    // autentificare client+reare cont
    std::shared_ptr<Client> autentificareClientUI(AppState& app);
    void creareContClientUI(AppState& app);

    //motoare de cautare pub/autori/clienti pt administartor
    static int selecteazaPub(const AppState& app);

    static std::shared_ptr<Autor> selecteazaAutor(AppState& app);

    static int filtreazaClienti(const AppState& app);

    static int filtreComanda(const AppState& app);

    // motor de cautare client+functie de vizualizare review+adauga in cos
    std::shared_ptr<Publicatie> selecteazaPubClient( AppState& app);

    static void afiseazaReviewuriClient(const AppState& app, const std::string& idP);

    // meniu adaugare publicatii+ functiile lor
    static void adaugaPub(AppState& app);
    static void adaugaCarte(AppState& app);
    static void adaugaManual(AppState& app);
    static void adaugaCarteStiintifica(AppState& app);
    static void adaugaRevista(AppState& app);

    // meniu adauga pachete(predefinite + create pe loc)
    static void meniuAdaugaPachete(const AppState& app,std::shared_ptr<Client>& clientCurent,std::shared_ptr<Comanda>& comandaActiva);

    static void afisazaContinutPachet(const AppState& app, int idx);
    //meniu stoc

    static void meniuStocA(AppState& app);

    //meniu statistici admin
    static void meniuStatisticaDistributieTip(const AppState& app);
   static void meniuTopPublicatiiPopularitate(const AppState& app);
    static void meniuTopPopularitatePeTip(const AppState& app);
    static void topPopulariatateoverall(const AppState& app);
    static void meniuStatisticiPublicatii(const AppState &app);
    static void meniuTop( AppState& app);

public:
   void run(AppState& app);

};

#endif //OOP_BOOK_STORE_MANAGER_H