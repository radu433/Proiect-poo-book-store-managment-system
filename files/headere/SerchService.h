

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
    // tamplate pt folosirea serch barului pt publicatii autori si clienti
    template <typename T, typename Predicate>
   static std::vector<int> search(const std::vector<std::shared_ptr<T>>& lista,Predicate pred) {
        std::vector<int> rezultate;
        for (int i = 0; i < static_cast<int>(lista.size()); ++i) {
            if (lista[i] && pred(lista[i])) {
                rezultate.push_back(i);
            }
        }
        return rezultate;
    }

    // publicatii
    static std::vector<int> filtreazaPublicatiiDupaTip(
        const std::vector<std::shared_ptr<Publicatie>>& publicatii,
        int tip
    );

    static std::vector<int> filtreazaPublicatiiDupaAutor(
        const std::vector<std::shared_ptr<Publicatie>>& publicatii,
        const std::string& nume
    );

    // autori

    static std::vector<int> filtreazaAutoriNume(
        const std::vector<std::shared_ptr<Autor>>& autori,
        const std::string& nume
    );

    //client
    static std::vector<int> filtreazaClientiUsername(
        const std::vector<std::shared_ptr<Client>>& clienti,
        const std::string& username
    );

    static std::vector<int> filtreazaClientiEmail(
        const std::vector<std::shared_ptr<Client>>& clienti,
        const std::string& email
    );
    // comenzi
    static std::vector<int> toateComenzile(
    const std::vector<std::shared_ptr<Comanda>>& comenzi
);

    static std::vector<int> comenziDupaClient(
        const std::vector<std::shared_ptr<Comanda>>& comenzi,
        const std::shared_ptr<Client>& client
    );

    static std::vector<int> comenziDupaStare(
        const std::vector<std::shared_ptr<Comanda>>& comenzi,
        const std::string& stare
    );
};


#endif //OOP_SERCHSERVICE_H
