#include <iostream>
#include <array>
#include <iomanip>
#include "Autor.h"
#include "Carte.h"
#include "Client.h"
#include  "adresa.h"

void Titlu(const std::string& titlu) {
    std::cout<<"\n---"<<titlu<<"---"<<std::endl;
}

int main() {
    Titlu("1. Crearea autori si carti");

    // capelarea constructorilor / creare de obiecte
    Autor autor1("Mihai", "Ion",1945);
    Autor autor2("Popescu", "Mihnea",1955);

    // operatorul <<
    std::cout<<autor1<<std::endl;
    std::cout<<autor2<<std::endl;

    // creare obiect carete
    Carte carte1("morometii",autor1, 65,1200,1966);
    Carte carte2("Baltagul",autor2, 85,200,1976);
    Carte carte3("amintiri din copilarie",autor1,55,13,1999);
    // operator <<
    std::cout<<carte1<<std::endl;
    std::cout<<carte2<<std::endl;

    Titlu("Test functii Carte");

    std::cout << "Titlul cartii 1: " << carte1.gettitlu() << std::endl;
    std::cout << "Pretul cartii 1: " << carte1.getpret() << " RON" << std::endl;

    carte1.reducere(10);
    std::cout<<"pretul cartii redus este: " << carte1.getpret() << std::endl;

    carte1.adauga_stoc(20);
    std::cout<<"Dupa adaugarea de stoc:"<<carte1.getstoc()<<std::endl;

    if (carte1.Scade_Stoc(45)==true)
    std::cout<<"Dupa reducerea stocului: "<<carte1.getstoc()<<std::endl;
    else
        std::cout<<"iposibil\n";
    if (carte1.este_disponibila()) {
        std::cout << "Cartea este inca disponibila." << std::endl;
    }

    // constructor de copiere si opertor=
    Carte carte_copiere=carte1;
    std::cout<<"copia creata"<<carte_copiere<<std::endl;

    Carte carte_atribuire("Test",autor1,150,44,1999);
    std::cout<<"Inainte de atribuire:"<< carte_atribuire<<std::endl;
    carte_atribuire=carte2;
    std::cout<<"dupa atribuire:"<<carte_atribuire<<std::endl;

    Titlu("test client si functii");
    // constructor de adresa
    Adresa adresa1("Bucuresti", "Sector 3", "Str. Exemplu nr. 10","011010");
    //functii pt clasa adresa
    std::cout << "Adresa inainte de modificare:\n" << adresa1 << std::endl;

    adresa1.modificaAdreasa("Ilfov", "Otopeni", "Str. Aeroportului 1", "077190");

    std::cout << "Adresa dupa modificare:\n" << adresa1 << std::endl;

    std::cout << "Test adresa valida: "
              << (adresa1.esteValida() ? "Valid" : "Invalid") << std::endl;

    std::cout << "--- Eticheta de livrare ---" << std::endl;
    std::cout << adresa1.getAdresaCompleta() << std::endl;
    std::cout << "--------------------------" << std::endl;

    //constructor client
    Client client("radu43", "mradu43@test.ro", adresa1,"07666658");

    //opretaor <<
    std::cout << "Stare initiala client:\n" << client << std::endl;

    double suma1=carte1.getpret()*2;
    carte1.Scade_Stoc(2);

    client.adaugacumparaturi(suma1);
    std::cout << "\nDupa prima comanda (" << suma1 << " RON):\n" << client << std::endl;

    client.adaugacumparaturi(600.0);
    std::cout << "\nDupa a doua comanda (600 RON):\n" << client << std::endl;

    int discount=client.calcdiscountpersonalizat();
    std::cout << "Discountul calculat al clientului: " << discount << "%" << std::endl;
    if (client.esteVIP())
        std::cout << "VIP este!" << std::endl;
    else
        std::cout << "Nu este VIP!" << std::endl;

    std::cout<<"Clientul se afla pe scara ierarhica :"<<client.ierarhie_clienti()<<std::endl;

    Titlu("Test functii neutilizate");

    std::cout << "Productivitate autor: " << autor1.calcproductivitate() << std::endl;
    std::cout << "Ierarhie autor: " << autor1.ierarhie_a() << std::endl;

    carte1.adauga_rating(5);
    std::cout << "Rating nou carte1: " << carte1.getrating() << std::endl;

    client.adaugaComanda(150.0, {"ISBN123", "ISBN456"});
    client.foloseste_pct_fidelitate(10);

    std::cout << "Inainte de adaugare carte, autorul 1 avea: " << autor1.getcarti_scrise() << " carti." << std::endl;
    std::cout << "Autorul 1 avea " << autor1.getcarti_scrise() << " carti inainte." << std::endl;
    autor1.adauga_carte("978-0-306-40615-7");
    std::cout << "Autorul 1 are " << autor1.getcarti_scrise() << " carti acum." << std::endl;

    std::cout << "\nComparare popularitate (carte1 vs carte2):" << std::endl;
    int rezultat_comparare = carte1.cumparaDupaPopularitate(carte2);

    if (rezultat_comparare == 1) {
        std::cout << carte1.gettitlu() << " e mai populara." << std::endl;
    } else if (rezultat_comparare == -1) {
        std::cout << carte2.gettitlu() << " e mai populara." << std::endl;
    } else {
        std::cout << "Cartile au popularitate egala." << std::endl;
    }

    std::cout << "Soldul initial al clientului: " << client.getSold() << " RON" << std::endl;

    return 0;
};
