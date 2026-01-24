

#ifndef OOP_REVIEW_H
#define OOP_REVIEW_H
#include <string>
#include <ctime>
#include <sstream>
#include <vector>

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
    
    // Constructor Load
    Review(const std::string& user, const std::string& idPub, int rat, const std::string& txt, std::time_t dt, bool ver)
        : username(user), identificator_publicatie(idPub), rating(rat), text(txt), data(dt), verificat(ver) {}

    const std::string& getUsername() const {return username;}
    const std::string& getIdentificatorPublicatie() const {return identificator_publicatie;}
    int getRating() const {return rating;}
    const std::string& getText() const {return text;}
    bool getVerificat() const {return verificat;}
    std::time_t getData() const {return data;}

    std::string serializare() const {
        return username + "|" + identificator_publicatie + "|" + std::to_string(rating) + "|" + text + "|" + std::to_string(data) + "|" + (verificat ? "1" : "0");
    }

    static Review deserializare(const std::string& line) {
        std::stringstream ss(line);
        std::string item;
        std::vector<std::string> v;
        while (std::getline(ss, item, '|')) {
            v.push_back(item);
        }
        if (v.size() < 6) throw std::runtime_error("Linie Review invalida");
        
        return Review(v[0], v[1], std::stoi(v[2]), v[3], std::stoll(v[4]), v[5] == "1");
    }
};


#endif //OOP_REVIEW_H