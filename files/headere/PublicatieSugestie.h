

#ifndef OOP_PUBLICATIESUGESTIE_H
#define OOP_PUBLICATIESUGESTIE_H
#include <string>

class PublicatieSugestie {
private:
    std::string titlu;
    std::string descriere;
    std::string categorie;

public:
    PublicatieSugestie(
        std::string titlu,
        std::string descriere,
        std::string categorie
        ): titlu(std::move(titlu)),
          descriere(std::move(descriere)),
          categorie(std::move(categorie)) {}

    const std::string& getTitlu() const{return titlu;}
    const std::string& getDescriere() const{return descriere;}
    const std::string& getCategorie() const{return categorie;}
};
#endif //OOP_PUBLICATIESUGESTIE_H