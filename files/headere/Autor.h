//
// Created by sindicat on 10/27/2025.
//

#ifndef OOP_AUTOR_H
#define OOP_AUTOR_H
#include <string>
#include <ostream>
#include <vector>
#include "../exceptii/exceptii_headere/LibrarieException.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"

class Autor {
private:
    std::string nume;
    std::string prenume;
    int varsta;
    std::vector<std::string> carti_scrise;
    int numar_premii;
    int idautor;


    static int nextidautor;

public:
    //constructor fara parametrii
    Autor();
    // constructor cu parametrii
    Autor(  const std::string& nume, const std::string& prenume, int varsta);
    // constructor pt load
    Autor(int id, const std::string& nume, const std::string& prenume, int varsta, int numar_premii, const std::vector<std::string>& carti);


    [[nodiscard]] const std::string& getNume() const;

    [[nodiscard]] const std::string& getprenume() const;

    [[nodiscard]] int getvarsta()const;

    //destructor

    ~Autor()=default;
    // supraincarcarea operatorului<<
    friend std::ostream& operator<<(std::ostream& out, const Autor& obj);

    //functii

    [[nodiscard]] size_t getcarti_scrise()const;

    void adauga_carte(const  std::string& isbn);

    double calcproductivitate()const;

    [[nodiscard]] int calcscorrenume()const;

    std::string ierarhie_a()const;
    int getidAutor()const;
    bool areCarte(const std::string& idx) const;
    std::string serializare() const;
    static Autor deserializare(const std::string& line);





};
#endif //OOP_AUTOR_H