

#ifndef OOP_SERCHSERVICE_H
#define OOP_SERCHSERVICE_H
#include <vector>
#include <memory>
#include <string>

class AppState;
class Publicatie;
class Autor;
class Client;
class Comanda;

class SerchService {
    public:
    // publicatii
    static int filtrePublicatie(const AppState &app);

    static int Serchbar(const AppState &app);

    // autori

    static std::shared_ptr<Autor> cautaAutor(const AppState &app, const std::string &nume, const std::string &prenume);

    static std::shared_ptr<Autor> selecteazaAutor(AppState &app);

    //client
    static int filtreazaClient(const AppState &app);
    // comanda
    static int filtrareComanda(const AppState &app);
};


#endif //OOP_SERCHSERVICE_H
