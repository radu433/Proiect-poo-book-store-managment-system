#ifndef OOP_PUBLICATIEEXCEPTION_H
#define OOP_PUBLICATIEEXCEPTION_H

#include "LibrarieException.h"

class PublicatieException : public LibrarieException {
public:
    explicit PublicatieException(const std::string& msg);
};

class PublicatieExpirata : public PublicatieException {
private:
    std::string data_expirare;
    int zile_expirate;
public:
    PublicatieExpirata(const std::string& data, int zile);
    [[nodiscard]] int getZileExpirate() const { return zile_expirate; }
};

class TitluInvalidException : public PublicatieException {
public:
    explicit TitluInvalidException(const std::string& motiv);
};

class RatingInvalidException : public PublicatieException {
private:
    int rating_invalid;
public:
    explicit RatingInvalidException(int rating);
    [[nodiscard]] int getRating() const { return rating_invalid; }
};

#endif