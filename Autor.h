//
// Created by sindicat on 10/27/2025.
//

#ifndef OOP_AUTOR_H
#define OOP_AUTOR_H
#include <string>
#include <ostream>
#include <vector>

class Autor {
private:
    std::string nume;
    std::string prenume;
    int varsta;
    std::vector<std::string> carti_scrise;
    int numar_premii;

public:
    Autor(  const std::string& nume, const std::string& prenume, int varsta);

    [[nodiscard]] const std::string& getNume() const;

    [[nodiscard]] const std::string& getprenume() const;

    [[nodiscard]] int getvarsta()const;

    //destructor

    ~Autor()=default;
    // supraincarcarea operatorului<<
    friend std::ostream& operator<<(std::ostream& out, const Autor& obj);

    //functii

    [[nodiscard]] size_t getcarti_scrise()const;

    void adauga_carte(const std::string& isbn);

    double calcproductivitate()const;

    [[nodiscard]] int calcscorrenume()const;

    std::string ierarhie_a()const;




};
#endif //OOP_AUTOR_H