

#ifndef OOP_APPSTATE_H
#define OOP_APPSTATE_H
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <ostream>

#include "Review.h"
class Publicatie;
class Client;
class Comanda;
class Autor;
class Reducere;
class PachetSerie;

enum class Tiplog {
    CONT_CREAT, CONT_STERS, LOGIN, COMANDA_FINALIZATA, COMANDA_ANULATA, INCERCARI_MULTIPLE_AUTENTIFICARE_ESUATE,REVIEW_ADAUGAT,
    AUTENTIFICARE_ESUATA
};

struct LogEntry {
    std::time_t timestamp;
    Tiplog tip;
    std::string email_client;
    std::string detalii;
    std::string user;
    friend std::ostream& operator<<(std::ostream& os, const LogEntry& log){
        const std::tm* tm = std::localtime(&log.timestamp);

        os << "[" << std::put_time(tm, "%Y-%m-%d %H:%M:%S") << "] "
           << log.user << ": "
           << log.detalii;

        return os;
    }
};

struct AppState {
    std::vector<std::shared_ptr<Publicatie> > publicatii;
    std::vector<std::shared_ptr<Comanda> > comenzi;
    std::vector<std::shared_ptr<Client> > clienti;
    std::vector<std::shared_ptr<Autor> > autor;
    std::vector<Reducere> reduceri;
    std::vector<LogEntry> logs;
    std::vector<std::shared_ptr<PachetSerie>> pachetePredefinite;
    std::vector<Review> reviews;
// functii pt reviewuri
   bool existaReview(const std::string& username, const std::string& identificator_publicatie) const;

    void adaugaReview(const std::string& username, const std::string& identificator_publicatie,int rating, const std::string& text,
        bool verificat);

    std::vector<Review>getReviewPublicatie(const std::string& identificator_publicatie) const;

    double calculeazaRatingPublicatie(const std::string& identificator_publicatie) const;

    bool stergeReviewNeverificat(const std::string& username,const std::string& identificator_publicatie);

    // funtii pt loguri


    static void adaugaLogs( const Tiplog tip, const std::string &email, const std::string &detalii = "",const std::string &user);
};
#endif //OOP_APPSTATE_H
