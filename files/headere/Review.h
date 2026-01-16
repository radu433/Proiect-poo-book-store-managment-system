

#ifndef OOP_REVIEW_H
#define OOP_REVIEW_H
#include <string>
#include <ctime>

#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/PublicatieException.h"

class Review {
private:
    std::string username;
    std::string identificator_publicatie; // isbn sau issn
    int rating;
    std::string text;
    std::time_t data;
    bool verificat;
public:
    Review(
       const std::string& username,const std::string& identificator_publicatie,int rating,
       const std::string& text,bool verificat): username(username),identificator_publicatie(identificator_publicatie)
        ,rating(rating),text(text),data(std::time(nullptr)),verificat(verificat) {
        if (rating<1 || rating>5 )
            throw DateInvalideException("Rating invalid!");
    }
    const std::string& getUsername() const {return username;}
    const std::string& getIdentificatorPublicatie() const {return identificator_publicatie;}
    int getRating() const {return rating;}
    const std::string& getText() const {return text;}
    bool getVerificat() const {return verificat;}
    std::time_t getData() const {return data;}
};

#endif //OOP_REVIEW_H