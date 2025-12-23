#include "../headere/PublicatieService.h"
#include "../headere/AppState.h"
#include "../headere/Publicatie.h"
#include "../headere/Client.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

#include <algorithm>
#include <stdexcept>

void PublicatieService::adaugarePublicatie(AppState &app, const std::shared_ptr<Publicatie> &pub) {
    if (!pub) {
        throw DateInvalideException("Publicatie invalida");
    }

    app.publicatii.push_back(pub);
}

void PublicatieService::stergePublicatie(AppState &app, const std::string& identificator) {

       const auto it = std::ranges::remove_if(app.publicatii
                                        ,
                                        [&](const std::shared_ptr<Publicatie>& p) {
                                            return p && p->getIdentificator() == identificator;
                                        }).begin();

       if (it == app.publicatii.end()) {
           throw std::runtime_error("Publicatia nu exista");
       }

       app.publicatii.erase(it, app.publicatii.end());


       std::erase_if(
           app.reviews,
           [&](const Review& r) {
               return r.identificator_publicatie == identificator;
           });
}

void PublicatieService::dezactivarePublicatie(const std::shared_ptr<Publicatie> &pub) {
    if (!pub) {
        throw DateInvalideException("Publicatie invalida");
    }

    if (!pub->esteActiva()) {
        throw std::runtime_error("Publicatia este deja dezactivata");
    }

    pub->dezactiveaza();
}

void PublicatieService::adaugaReview_Rating(AppState &app, const Client &client, const std::shared_ptr<Publicatie> &pub,
    const std::string &text, const int rating) {
    if (!pub || !pub->esteActiva()) {
        throw DateInvalideException("Publicatia nu este activa");
    }

    if (rating < 1 || rating > 5) {
        throw DateInvalideException("Rating invalid");
    }

    if (text.empty()) {
        throw DateInvalideException("Text review gol");
    }

    const std::string& username = client.getUsername();
    const std::string& identificator = pub->getIdentificator();

    if (app.existaReview(username, identificator)) {
        throw std::runtime_error("Review deja existent");
    }

    bool verificat = false;

    if (const auto& istoric = client.getIstoricIdentificatori(); std::ranges::find(istoric, identificator) != istoric.end()) {
        verificat = true;
    }

    app.adaugaReview(
        username,
        identificator,
        rating,
        text,
        verificat
    );

    app.adaugaLogs(
        app,
        Tiplog::REVIEW_ADAUGAT,
        client.getEmail(),
        "Publicatie: " + pub->getTitlu()
    );
}

std::shared_ptr<Publicatie> PublicatieService::cloneazaPublicatie(AppState &app, const std::shared_ptr<Publicatie> &original,
    int stocInitial) {
    if (!original) {
        throw DateInvalideException("Publicatie invalida");
    }

    if (stocInitial < 0) {
        throw DateInvalideException("Stoc initial invalid");
    }

    auto copie = original->clone();

    if (stocInitial > 0) {
        copie->adauga_stoc(stocInitial);
    }

    app.publicatii.push_back(copie);

    return copie;
}



