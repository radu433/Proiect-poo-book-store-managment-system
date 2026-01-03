#include "../exceptii_headere/PublicatieException.h"

PublicatieException::PublicatieException(const std::string& msg)
    : LibrarieException("Eroare Publicatie: " + msg) {}

PublicatieExpirata::PublicatieExpirata(const std::string& data, int zile)
    : PublicatieException("Publicatie expirata din " + data + 
                         " (" + std::to_string(zile) + " zile)"),
      data_expirare(data),
      zile_expirate(zile) {}

TitluInvalidException::TitluInvalidException(const std::string& motiv)
    : PublicatieException("Titlu invalid: " + motiv) {}

RatingInvalidException::RatingInvalidException(int rating)
    : PublicatieException("Rating invalid: " + std::to_string(rating) + 
                         " (trebuie intre 1-5)"),
      rating_invalid(rating) {}