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

void AppState::adaugaLogs( const Tiplog tip, const std::string &email,const std::string& user, const std::string &detalii) {
    logs.push_back({
        std::time(nullptr), tip, email, detalii,user
    });
}

