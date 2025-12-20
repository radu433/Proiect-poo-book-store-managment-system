

#ifndef OOP_APPSTATE_H
#define OOP_APPSTATE_H
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

#include "Review.h"
class Publicatie;
class Client;

enum class Tiplog {
    CONT_CREAT, CONT_STERS, LOGIN, COMANDA_FINALIZATA, COMANDA_ANULATA, INCERCARI_MULTIPLE_AUTENTIFICARE_ESUATE
};

struct LogEntry {
    std::time_t timestamp;
    Tiplog tip;
    std::string email_client;
    std::string detalii;
};

struct AppState {
    std::vector<std::shared_ptr<Publicatie> > publicatii;
    std::vector<std::shared_ptr<Comanda> > comenzi;
    std::vector<std::shared_ptr<Client> > clienti;
    std::vector<std::shared_ptr<Autor> > autor;
    std::vector<Reducere> reduceri;
    std::vector<LogEntry> logs;
    std::vector<std::shared_ptr<PachetSerie> > pachetePredefinite;
    std::vector<Review> reviews;

    bool getReviewuriPublicatie(const std::string &username, const std::string &identificator_publicatie) const;

    void getReviewuri(const std::string &username, const std::string &identificator_publicatie, int rating,
                      const std::string &text,
                      bool verificat);

    std::vector<Review> getReviewPublicatie(const std::string &identificator_publicatie) const;

    bool stergeReviewNeverificat(
        const std::string &username,
        const std::string &identificator_publicatie);
};
#endif //OOP_APPSTATE_H
