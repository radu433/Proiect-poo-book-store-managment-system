#include "../headere/ReviewService.h"
#include "../headere/AppState.h"
#include "../headere/Review.h"
#include "../headere/Comanda.h"

#include <algorithm>
#include <ranges>

bool ReviewService::verificaReview(const AppState &app, const std::string &username,
    const std::string &id_pub) {
    for (const auto& comanda : app.getComanda()) {
        if (comanda.getUser()!= username)
            continue;

        if (auto isbnuri = comanda.extrageIdentificatori(); std::ranges::find(isbnuri, id_pub) != isbnuri.end()) {
            return true;
        }
    }
    return false;
}

std::vector<const Review *> ReviewService::getReviewuriSortate(const AppState &app, const std::string &id_pub) {
    std::vector<const Review*> rezultat;

    for (const auto& r : app.getReviews()) {
        if (r.getIdentificatorPublicatie() == id_pub) {
            rezultat.push_back(&r);
        }
    }

    std::ranges::sort(rezultat,
                      [](const Review* a, const Review* b) {
                          if (a->getVerificat() != b->getVerificat())
                              return a->getVerificat() && !b->getVerificat();
                          return a->getData() > b->getData();
                      });

    return rezultat;
}

std::vector<const Review *>
ReviewService::getReviewuriNeverificate(
    const AppState &app,
    const std::string &id_pub
) {
    std::vector<const Review*> rezultat;

    for (const auto& r : app.getReviews()) {
        if (r.getIdentificatorPublicatie() == id_pub && !r.getVerificat()) {
            rezultat.push_back(&r);
        }
    }

    std::ranges::sort(rezultat,
                      [](const Review* a, const Review* b) {
                          return a->getData() > b->getData();
                      });

    return rezultat;
}
void ReviewService::stergeReviewuriNeverificate(
    AppState& app,
    const std::string& id_pub,
    const std::vector<int>& indexuri
) {
    const auto reviewuri = app.getReviewPublicatie(id_pub);

    for (const int idx : indexuri) {
        if (idx < 0 || idx >= static_cast<int>(reviewuri.size()))
            continue;

        if (const Review& r = reviewuri[idx]; !r.getVerificat()) {
            app.stergeReviewNeverificat(r.getUsername(),id_pub);
        }
    }
}

double ReviewService::calculeazaRatingPublicatieVerificate(const AppState& app,const std::string &identificator_publicatie) {
    int suma = 0;
    int cnt = 0;

    for (const auto& r :app.getReviews() ) {
        if (r.getIdentificatorPublicatie() == identificator_publicatie &&
            r.getVerificat()) {
            suma += r.getRating();
            cnt++;
            }
    }

    return (cnt == 0) ? 0.0 : static_cast<double>(suma) / cnt;
}

double ReviewService::calculeazaRatingPublicatieall(const AppState &app,const std::string& identificator_publicatie) {
    int suma=0;
    int cnt=0;

    for (const auto& r :app.getReviews() ) {
        if (r.getIdentificatorPublicatie() == identificator_publicatie) {
            suma += r.getRating();
            cnt++;
        }

    }
    return (cnt == 0) ? 0.0 : static_cast<double>(suma) / cnt;
}

