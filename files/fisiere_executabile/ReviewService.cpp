#include "../headere/ReviewService.h"
#include "../headere/AppState.h"
#include "../headere/Review.h"
#include "../headere/Comanda.h"

#include <algorithm>
#include <ranges>

bool ReviewService::verificaReview(const AppState &app, const std::string &username,
    const std::string &id_pub) {
    for (const auto& comanda : app.comenzi) {
        if (comanda->getUsernameClient() != username)
            continue;

        if (auto isbnuri = comanda->extrageISBN(); std::ranges::find(isbnuri, id_pub) != isbnuri.end()) {
            return true;
        }
    }
    return false;
}

std::vector<const Review *> ReviewService::getReviewuriSortate(const AppState &app, const std::string &id_pub) {
    std::vector<const Review*> rezultat;

    for (const auto& r : app.reviews) {
        if (r.identificator_publicatie == id_pub) {
            rezultat.push_back(&r);
        }
    }

    std::ranges::sort(rezultat,
                      [](const Review* a, const Review* b) {
                          if (a->verificat != b->verificat)
                              return a->verificat > b->verificat;
                          return a->data > b->data;
                      });

    return rezultat;
}

std::vector<const Review *>
ReviewService::getReviewuriNeverificate(
    const AppState &app,
    const std::string &id_pub
) {
    std::vector<const Review*> rezultat;

    for (const auto& r : app.reviews) {
        if (r.identificator_publicatie == id_pub && !r.verificat) {
            rezultat.push_back(&r);
        }
    }

    std::ranges::sort(rezultat,
                      [](const Review* a, const Review* b) {
                          return a->data > b->data;
                      });

    return rezultat;
}
void ReviewService::stergeReviewuriNeverificate(
    AppState& app,
    const std::string& id_pub,
    const std::vector<int>& indexuri
) {
    std::vector<int> pozitii;

    for (size_t i = 0; i < app.reviews.size(); ++i) {
        if (const auto& r = app.reviews[i]; r.identificator_publicatie == id_pub && !r.verificat) {
            pozitii.push_back(static_cast<int>(i));
        }
    }


    for (auto it = indexuri.rbegin(); it != indexuri.rend(); ++it) {
        int idx = *it;
        if (idx >= 0 && idx < (int)pozitii.size()) {
            app.reviews.erase(app.reviews.begin() + pozitii[idx]);
        }
    }
}
