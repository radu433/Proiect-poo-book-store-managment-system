

#ifndef OOP_APPSTATE_H
#define OOP_APPSTATE_H
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <memory>
#include <ostream>

#include "Review.h"
#include "UnitateVanzare.h"
#include "PachetSerie.h"
#include "Reducere.h"
#include "Client.h"
#include "Autor.h"
#include "Comanda.h"
#include "Publicatie.h"
#include "Review.h"

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

class AppState {
private:
    std::vector<std::shared_ptr<Publicatie> > publicatii;
    std::vector<Comanda > comenzi;
    std::vector<Client > clienti;
    std::vector<Autor > autor;
    std::vector<Reducere> reduceri;
    std::vector<LogEntry> logs;
    std::vector<PachetSerie> pachetePredefinite;
    std::vector<Review> reviews;
    std::vector<std::shared_ptr<UnitateVanzare>> stocSH;
public:
// functii pt reviewuri
   bool existaReview(const std::string& username, const std::string& identificator_publicatie) const;

    void adaugaReview(const std::string& username, const std::string& identificator_publicatie,int rating, const std::string& text,
        bool verificat);

    std::vector<Review>getReviewPublicatie(const std::string& identificator_publicatie) const;

    bool stergeReviewNeverificat(const std::string& username,const std::string& identificator_publicatie);

    // funtii pt loguri


     void adaugaLogs( const Tiplog tip, const std::string &email,const std::string &user, const std::string &detalii = "");

    // geteri appstate
    const std::vector<std::shared_ptr<Publicatie>>& getPublicatii() const {return publicatii;}
    const std::vector<Comanda>& getComanda() const{return comenzi;}
    const std::vector <Client>& getClient() const{return clienti;}
    std::vector<Autor>& getAutor1(){return autor;}
    const std::vector<Autor>& getAutor()const {return autor;}
    const std::vector<Reducere>& getReducere() const{return reduceri;}
    const std::vector<LogEntry>& getLogs() const{return logs;}
    const std::vector<PachetSerie>& getPachetePredefinite() const{return pachetePredefinite;}
    const std::vector<Review>& getReviews() const{return reviews;}
    const std::vector<std::shared_ptr<UnitateVanzare>>& getStocSH() const{return stocSH;}
    std::shared_ptr<Publicatie>gasestePublicatie(const std::string& identificator) const;
    // functii pt midificare si stergere
    void stergetoatReducerile() {reduceri.clear();}
    void adaugaReduceri(const Reducere& r){reduceri.push_back(r);}
    void adaugaPublicatie(const std::shared_ptr<Publicatie>& p){publicatii.push_back(p);}
    bool stergePublicatie(const std::string& isbn);
    void stergeReviewPublicatie(const std::string& isbn);
    std::shared_ptr<UnitateVanzare> stergeDinStoc(size_t idx);
    void adaugaSH(const std::shared_ptr<UnitateVanzare> &produs);
    void adaugaClient(const Client &client);
    void adaugaAutor(const Autor &autor1);
    Comanda& adaugaComanda(const  Client& client);



};
#endif //OOP_APPSTATE_H
