//
// Created by sindicat on 11/3/2025.
//

#ifndef OOP_ADRESA_H
#define OOP_ADRESA_H
#include <string>
#include <ostream>
class Adresa {
    std::string judet;
    std::string oras;
    std::string strada;
    std::string cod_postal;
    public:
    Adresa(const std::string &judet, const std::string &oras, const std::string &strada, const std::string &cond_postal);


    friend std::ostream& operator<<(std::ostream& out, const Adresa& a);

    void modificaAdreasa(const std::string& judet, const std::string& oras,
                      const std::string& strada, const std::string& cod_postal);

    bool esteValida() const;

    [[nodiscard]] std::string getAdresaCompleta() const;

};

#endif //OOP_ADRESA_H
