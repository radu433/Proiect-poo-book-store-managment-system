# Book Store Management System

Acest proiect reprezinta o aplicatie orientata pe obiect care gestioneaza activitatea unui magazin de carti. Sistemul permite administrarea informatiilor despre publicatii (titlu, autor, categorie, stoc, pret) si a operatiunilor specifice unui bookstore, precum cautarea cartilor, actualizarea stocurilor si procesarea vanzarilor.

---

## Componentele sistemului

### Publicatie
Clasa de baza pentru:
- Carte
- Manual
- Revista
- Carte stiintifica  

Reprezinta obiectele ce pot fi cumparate din magazin.

### Autor
Gestioneaza informatiile despre autorii publicatiilor si statisticile asociate acestora.

### Unitate de vanzare
Include urmatoarele tipuri:
- Carte individuala
- Revista individuala
- Pachet serie  

Aceasta clasa reprezinta puntea dintre publicatie si comanda, deoarece clientul adauga in cos fie un singur obiect, fie un pachet de obiecte.

### AppState
Componenta in care sunt stocate toate datele sistemului.

### Client
Reprezinta utilizatorul aplicatiei si gestioneaza informatiile despre cont, comenzi si interactiunile cu sistemul.

### Adresa
Contine datele de livrare asociate unui client.

### Reducere
Gestioneaza tipurile de reduceri aplicabile publicatiilor.

### Comanda
Reprezinta o comanda plasata de un client.

### Articol comanda
Elementele individuale dintr-o comanda (produsele comandate).

### Review
Gestioneaza recenziile adaugate de clienti pentru publicatii.

---

## Functionalitatile sistemului

Sistemul ofera o interfata de tip CLI care leaga toate componentele aplicatiei. Exista doua moduri de operare:

### Modul administrator
Administratorul poate:
- adauga, edita si sterge publicatii
- vizualiza si sterge review-uri pentru orice publicatie
- adauga autori si vizualiza statistici despre acestia
- vizualiza statistici despre clienti
- consulta log-urile cu toate actiunile din sistem
- vizualiza toate comenzile si statistici aferente
- accesa un meniu dedicat statisticilor detaliate pentru toate componentele sistemului

### Modul client
Clientul poate:
- cauta publicatii folosind bara de cautare si diverse filtre
- adauga publicatii sau pachete predefinite in cos
- crea propriile pachete, cu restrictia ca acestea sa contina un singur tip de publicatie (Carte, Manual, Revista sau Carte stiintifica)
- primi sugestii de publicatii pe baza cumparaturilor anterioare
- utiliza un sistem de trade-in pentru publicatii, primind bani in cont, puncte de fidelitate sau reduceri
- accesa un marketplace second-hand pentru achizitionarea de publicatii sau pachete second-hand cu reduceri suplimentare
- adauga review-uri pentru publicatii (verificate sau neverificate, in functie de achizitie)
- vizualiza detalii despre cont, inclusiv istoricul comenzilor, punctele de fidelitate si review-urile proprii

---

## Sistem de reduceri

Reducerile sunt de tip „fulger”. Sistemul selecteaza aleator un tip de publicatie si patru publicatii din acel tip, carora le aplica intr-o zi aleatorie din saptamana o reducere suplimentara intre 5% si 15%.

Reducerea este activa timp de 24 de ore.

---
