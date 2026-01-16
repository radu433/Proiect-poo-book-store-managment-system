//
// Created by sindicat on 11/3/2025.
//

#ifndef OOP_ADRESA_H
#define OOP_ADRESA_H
#include <string>
#include <ostream>

class Adresa {
private:
    std::string judet;
    std::string oras;
    std::string strada;
    int numar;
    std::string cod_postal;



    // operator <<

    friend std::ostream &operator<<(std::ostream &out, const Adresa &a);

public:
    // constructor
    Adresa(const std::string &judet, const std::string &oras, const std::string &strada,
           const int numar, const std::string &cond_postal);

    void modificaAdreasa(const std::string &judet, const std::string &oras,
                        const std::string &strada,const int numar, const std::string &cod_postal);
    [[nodiscard]] std::string getAdresaCompleta() const;

    std::string getStrada() const;
    int getNumar() const;

    std::string getOras() const;
};

#endif //OOP_ADRESA_H