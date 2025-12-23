#include "../headere/AppState.h"

#include <iostream>

bool AppState::existaReview(const std::string &username, const std::string &identificator_publicatie) const {
    for (const auto& r : reviews) {
        if (r.username == username && r.identificator_publicatie == identificator_publicatie) {
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
        if (r.identificator_publicatie == identificator_publicatie) {
            rezultat.push_back(r);
        }
    }

    std::ranges::sort(rezultat,
                      [](const Review& a, const Review& b) {
                          if (a.verificat != b.verificat)
                              return a.verificat > b.verificat; // verificate primele
                          return a.data > b.data;               // mai recente primele
                      });

    return rezultat;
}

double AppState::calculeazaRatingPublicatie(const std::string &identificator_publicatie) const {
        int suma = 0;
        int cnt = 0;

        for (const auto& r : reviews) {
            if (r.identificator_publicatie == identificator_publicatie &&
                r.verificat) {
                suma += r.rating;
                cnt++;
                }
        }

        return (cnt == 0) ? 0.0 : static_cast<double>(suma) / cnt;
}

bool AppState::stergeReviewNeverificat(const std::string &username, const std::string &identificator_publicatie) {

        for (auto it = reviews.begin(); it != reviews.end(); ++it) {
            if (it->username == username &&
                it->identificator_publicatie == identificator_publicatie &&
                !it->verificat) {
                reviews.erase(it);
                return true;
                }
        }
        return false;
}
// functii logs
void AppState::Logsactivitateclients(const AppState &app) {
    std::cout << "\n=== LOGURI SISTEM ===\n";
    for (const auto &l: app.logs) {
        std::cout << "[" << std::ctime(&l.timestamp) << "] ";

        switch (l.tip) {
            case Tiplog::CONT_CREAT:
                std::cout << "CONT CREAT";
                break;
            case Tiplog::CONT_STERS:
                std::cout << "CONT STERS";
                break;
            case Tiplog::LOGIN:
                std::cout << "LOGIN";
                break;
            case Tiplog::COMANDA_FINALIZATA:
                std::cout << "COMANDA FINALIZATA";
                break;
            case Tiplog::COMANDA_ANULATA:
                std::cout << "COMANDA ANULATA";
                break;
            case Tiplog::INCERCARI_MULTIPLE_AUTENTIFICARE_ESUATE:
                std::cout << "INCERCARI_MULTIPLE_AUTENTIFICARE_ESUATE";
                break;
            case Tiplog::REVIEW_ADAUGAT:
                std::cout << "REVIEW_ADAUGAT";
                break;
        }

        std::cout << " | " << l.email_client;
        if (!l.detalii.empty())
            std::cout << " | " << l.detalii;
        std::cout << "\n";
    }
}

void AppState::adaugaLogs(AppState &app, const Tiplog tip, const std::string &email, const std::string &detalii) {
    app.logs.push_back({
        std::time(nullptr), tip, email, detalii
    });
}

