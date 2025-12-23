
#ifndef OOP_PUBLICATIESERVICE_H
#define OOP_PUBLICATIESERVICE_H
#include <memory>
#include <vector>
#include <string>

#include "Client.h"
#include "Reducere.h"

class AppState;
class Publicatie;
class Review;

class PublicatieService {
public :
    // functii pt adaugat publicatii(admin)
 static void adaugarePublicatie(AppState &app, const std::shared_ptr<Publicatie> &pub);

    // stergere+ dezactivare publicatie
    static void stergePublicatie(AppState &app, const std::string& identificator);// de scris si adaugat in meniupulicatii
    static void dezactivarePublicatie(const std::shared_ptr<Publicatie> &pub);

    // functii pt adaugarea review si rating
    static void adaugaReview_Rating(AppState &app,const Client& client, const std::shared_ptr<Publicatie> &pub,const std::string &text
        ,int rating);


    // clonare
    static std::shared_ptr<Publicatie> cloneazaPublicatie(AppState &app, const std::shared_ptr<Publicatie>& original,int stocInitial);

    // reduceri active
    static void aplicareReducerePublicatie(std::shared_ptr<Publicatie> pub,
                                           const Reducere &reducere) {
    }
};
#endif //OOP_PUBLICATIESERVICE_H