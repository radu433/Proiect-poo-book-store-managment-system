
#ifndef OOP_BOOK_STORE_MANAGER_H
#define OOP_BOOK_STORE_MANAGER_H
#include "AppState.h"

class BookStoreManager {
private:
    AppState app;
    std::shared_ptr<Client> clientCurent;
    // meniuri principale
    void afisareMeniuPrincipal(AppState& app);
    void CLIAdministrator(AppState& app);
    void CLIClient(AppState& app);

    // submeniuri Administrator
    void afisazaPublicatii();// clasa de publicatii
    void meniuPublicatiii();
    void meniuAdaugaPublicatie();
    void meniuAutor();
    void meniuClientA();
    void meniuComenzi();
    void meniuStatistici();

    //submeniuri Client
    void meniuCautaPublicatii();
    void meniuCosCumparaturi();
    void adaugaReview_Rating();// clasa de publicatii
    void meniuDetaliiCont();

    // functie de afisare reducere activa
    void afisazaReducere();
    //functii din meniu clienti
    void listaIstoricComenzi();// clasa client
    void alimentareCont();//clasa client


public:
   void run(AppState& app);

};

#endif //OOP_BOOK_STORE_MANAGER_H