//
// Created by sindicat on 10/28/2025.
//
#include "Autor.h"
#include<iostream>
// constructor cu parametrii
Autor::Autor(const std::string &nume, const std::string &prenume, int varsta):nume(nume),
    prenume(prenume), varsta(varsta),numar_premii(0) {}
// getteri
const std::string& Autor::getNume() const {
    return nume;
}

const std::string & Autor::getprenume() const {
    return prenume;
}

int Autor::getvarsta() const {
    return varsta;
}


// supraincarcarea operatorului <<
std::ostream & operator<<(std::ostream &out, const Autor &obj) {

        out<<"Autor:"<<obj.getNume()<<" "<<obj.getprenume()<<" Varsta:"<<obj.getvarsta();
        return out;

};



// fuctii

size_t Autor::getcarti_scrise() const { return carti_scrise.size();
}

void Autor::adauga_carte(const std::string &isbn) {
    for (const auto& c :carti_scrise) {
        if (c==isbn) {
            std::cout<<"Cartea deja exista !\n ";
            return;
        }
    }
    carti_scrise.push_back(isbn);
}

double Autor::calcproductivitate() const {
    if (varsta<18) return 0.0;
    int ani_c=varsta-18;
    if (ani_c==0) return 0.0;
    return static_cast<double>(carti_scrise.size())/ani_c;
}

int Autor::calcscorrenume() const {
    return carti_scrise.size() * 10 +numar_premii*50;
}

std::string Autor::ierarhie_a() const {
    int scor=calcscorrenume();
    if (scor > 500) return "Legendar";
    if (scor > 200) return "Renumit";
    if (scor > 100) return "Consacrat";
    if (scor > 50) return "In ascensiune";
    return "Incepator";

}
