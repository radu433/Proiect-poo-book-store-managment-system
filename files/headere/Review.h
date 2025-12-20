

#ifndef OOP_REVIEW_H
#define OOP_REVIEW_H
#include <string>
#include <ctime>
struct Review {
    std::string username;
    std::string identificator_publicatie; // isbn sau issn
    int rating;
    std::string text;
    std::time_t data;
    bool verificat;

    Review(
       const std::string& username,const std::string& identificator_publicatie,int rating,
       const std::string& text,bool verificat
   )
       : username(username),identificator_publicatie(identificator_publicatie),rating(rating),
         text(text),data(std::time(nullptr)),verificat(verificat)
    {}

};
#endif //OOP_REVIEW_H