#include "../headere/AppState.h"

#include <iostream>

bool AppState::existaReview(const std::string &username, const std::string &identificator_publicatie) const {
    for (const auto& r : reviews) {
        if (r.getUsername() == username && r.getIdentificatorPublicatie() == identificator_publicatie) {
            return true;
            }
    }
    return false;
}

void AppState::adaugaReview(const std::string &username, const std::string &identificator_publicatie, int rating,
    const std::string &text, bool verificat) {
    reviews.emplace_back(username,identificator_publicatie,rating,text,verificat);
}

std::vector<Review> AppState::getReviewPublicatie(const std::string &identificator_publicatie) const {
    std::vector<Review> rezultat;

    for (const auto& r : reviews) {
        if (r.getIdentificatorPublicatie() == identificator_publicatie) {
            rezultat.push_back(r);
        }
    }

    std::ranges::sort(rezultat,
                      [](const Review& a, const Review& b) {
                          if (a.getVerificat() != b.getVerificat())
                              return a.getVerificat() && !b.getVerificat();
                          return a.getData() > b.getData();
                      });

    return rezultat;
}

bool AppState::stergeReviewNeverificat(const std::string &username, const std::string &identificator_publicatie) {

        for (auto it = reviews.begin(); it != reviews.end(); ++it) {
            if (it->getUsername() == username &&
                it->getIdentificatorPublicatie() == identificator_publicatie &&
                !it->getVerificat()) {
                reviews.erase(it);
                return true;
                }
        }
        return false;
}
// functii logs

void AppState::adaugaLogs( const Tiplog tip, const std::string &email,const std::string& user, const std::string &detalii) {
    logs.push_back({std::time(nullptr), tip, email, detalii,user});
}

std::shared_ptr<Publicatie> AppState::gasestePublicatie(const std::string &identificator) const {
    for (const auto& p : publicatii) {
        if (p && p->getIdentificator() == identificator) {
            return p;
        }
    }
    return nullptr;
}

bool AppState::stergePublicatie(const std::string &isbn) {
    size_t removed = std::erase_if(publicatii, [&](const std::shared_ptr<Publicatie>& p) {
        return p && p->getIdentificator() == isbn;
    });
    return removed > 0;
}

void AppState::stergeReviewPublicatie(const std::string &isbn) {
    std::erase_if(reviews,[&](const Review& r) {
        return r.getIdentificatorPublicatie() == isbn;
    }
    );
}

std::shared_ptr<UnitateVanzare> AppState::stergeDinStoc(size_t idx) {
    if (idx>=stocSH.size())
        throw DateInvalideException("index invalid!");
    auto u = stocSH[idx];
    stocSH.erase(stocSH.begin()+idx);
    return u;
}

void AppState::adaugaSH(const std::shared_ptr<UnitateVanzare> &produs)  {
    stocSH.push_back(produs);
}



void AppState::adaugaClient(const Client &client) {
    clienti.push_back(client);
}

void AppState::adaugaAutor(const Autor &autor1) {
    autor.push_back(autor1);
}

Comanda & AppState::adaugaComanda(const Client &client) {
    comenzi.emplace_back(client);
    return comenzi.back();
}










