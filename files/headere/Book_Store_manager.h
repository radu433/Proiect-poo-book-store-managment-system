
#ifndef OOP_BOOK_STORE_MANAGER_H
#define OOP_BOOK_STORE_MANAGER_H
#include "AppState.h"
#include "ReducereService.h"
#include "AutorService.h"
#include "CosService.h"


class BookStoreManager {
private:
    std::shared_ptr<Client> clientCurent;
    std::shared_ptr<Comanda> comandaActiva;
    AppState& app;

 template<typename T>
   void topPopularitatePeTip(const std::string &numeTip);

    // afisarea reducerilor active
    void afiseazaReduceriActive() const;
    // meniuri principale
    void afisareMeniuPrincipal();

     void CLIAdministrator();
    void CLIClient();

    // submeniuri Administrator
     void meniuPublicatiiA() const;
    void meniuAdaugaPublicatie();
     void meniuReviewuri() const;
     void meniuAutorA() const;
     void meniuClientA() const;
    void meniuComenziA() const;
    void meniuStatisticiA();
    void aplicaReducerePublicatii();

    //submeniuri Client
    void meniuCautaPublicatii();

     void meniuSH_Tradein();
    void meniuCosCumparaturi();
    void adaugaReview_Rating() const;// clasa de publicatii
    void meniuDetaliiCont() const;

    void afiseazaReducere();
    //functii din meniu clienti

    void listaIstoricComenzi();// clasa client
    void alimentareCont();//clasa client

    // autentificare client+reare cont
    std::shared_ptr<Client> autentificareClientUI() const;

     void creareContClientUI() const;

    //motoare de cautare pub/autori/clienti pt administartor
      int selecteazaPub() const ;

    const Autor* selecteazaAutor() const;

     int filtreazaClienti() const;

     int filtreComanda() const;

    // motor de cautare client+functie de vizualizare review+adauga in cos
     std::shared_ptr<Publicatie> selecteazaPubClient() const;

    void afiseazaReviewuriClient( const std::string& idP) const;

    // meniu adaugare publicatii+ functiile lor
    void adaugaPub() const;
     void adaugaCarte() const;
    void adaugaManual() const;
     void adaugaCarteStiintifica() const;
     void adaugaRevista() const;
      const Autor* gasesteSauCreeazaAutorDupaISBN(const std::string& isbn) const;

    // meniu adauga pachete(predefinite + create pe loc)
     void meniuAdaugaPachete();

     void afisazaContinutPachet(int idx) const;
    //meniu stoc

     void meniuStocA() const;

    //meniu statistici admin
     void meniuStatisticaDistributieTip() const;
    void meniuTopPublicatiiPopularitate();
     void meniuTopPopularitatePeTip();
     void topPopulariatateoverall() const;
    void meniuStatisticiPublicatii();
     void meniuTop();

public:
 explicit BookStoreManager(AppState& app);
   void run();

};

#endif //OOP_BOOK_STORE_MANAGER_H