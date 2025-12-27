
#ifndef OOP_BOOK_STORE_MANAGER_H
#define OOP_BOOK_STORE_MANAGER_H
#include "AppState.h"
#include "ReducereService .h"
#include "AutorService.h"


class BookStoreManager {
private:
    std::shared_ptr<Client> clientCurent;

    // afisarea reducerilor active
    static void afiseazaReduceriActive(const AppState& app);
    // meniuri principale
    void afisareMeniuPrincipal(AppState& app);
    void CLIAdministrator(AppState& app);
    void CLIClient(AppState& app);

    // submeniuri Administrator
    static void meniuPublicatiiA(AppState& app);
    void meniuAdaugaPublicatie();

    static void meniuReviewuri(AppState& app);

    static void meniuAutorA(AppState& app);

    static void meniuClientA(AppState& app);
    void meniuComenzi();
    void meniuStatistici();

    void aplicaReducerePublicatii();

    //submeniuri Client
    void meniuCautaPublicatii();
    void meniuCosCumparaturi();
    void adaugaReview_Rating();// clasa de publicatii
    void meniuDetaliiCont();

    void afiseazaReducere();
    //functii din meniu clienti

    void listaIstoricComenzi();// clasa client
    void alimentareCont();//clasa client

    // autentificare client+reare cont
    std::shared_ptr<Client> autentificareClientUI(AppState& app);
    void creareContClientUI(AppState& app);

    //motoare de cautare pub/autori/clienti
    static int selecteazaPub(const AppState& app);

    static std::shared_ptr<Autor> selecteazaAutor(AppState& app);

    static int filtreazaClienti(AppState& app);

    // meniu adaugare publicatii+ functiile lor
    static void adaugaPub(AppState& app);
    static void adaugaCarte(AppState& app);
    static void adaugaManual(AppState& app);
    static void adaugaCarteStiintifica(AppState& app);
    static void adaugaRevista(AppState& app);
public:
   void run(AppState& app);

};

#endif //OOP_BOOK_STORE_MANAGER_H