#include "../headere/LoadService.h"
#include "../headere/Carte.h"
#include "../headere/Manual.h"
#include "../headere/CarteStiintifica.h"
#include "../headere/Revista.h"
#include "../headere/Client.h"
#include "../headere/Comanda.h"

#include <fstream>
#include <ranges>
#include <sstream>
#include <iostream>

#include "../headere/AutorService.h"
#include "../exceptii/exceptii_headere/DateInvalideException.h"
#include "../exceptii/exceptii_headere/LibrarieException.h"

void LoadService::incarca(AppState &app) {
    incarcaAutori(app);
    incarcaPublicatii(app);
    incarcaClienti(app);
    incarcaStocSH(app);
    incarcaPachete(app);
    incarcaComenzi(app);
    incarcaReducerile(app);
    incarcaReviews(app);
    incarcaLogs(app);
}
void LoadService::salveaza( const AppState &app) {
    salveazaAutori(app);
    salveazaPublicatii(app);
    salveazaClienti(app);
    salveazaStocSH(app);
    salveazaPachete(app);
    salveazaComenzi(app);
    salveazaReducerile(app);
    salveazaReviews(app);
    salveazaLogs(app);
}

// autori
void LoadService::salveazaAutori(const AppState &app) {
    std::ofstream out("data/autori.txt");
    if (!out) return;
    for (const auto& autor : app.getAutor()) {
        out << autor.serializare() << "\n";
    }
}

void LoadService::incarcaAutori(AppState &app) {
    std::ifstream in("data/autori.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            Autor a = Autor::deserializare(line);
            app.adaugaAutor(a);
        } catch (const DateInvalideException& e) {
             std::cerr << "[Eroare Autori] Linie invalida: " << e.what() << "\n";
        } catch (const LibrarieException& e) {
             std::cerr << "[Eroare Autori] Problema librarie: " << e.what() << "\n";
        } catch (const std::exception& e) {
             std::cerr << "[Eroare Autori] Exceptie: " << e.what() << "\n";
        }
    }
}
// publicatii
void LoadService::salveazaPublicatii(const AppState &app) {
    std::ofstream out("data/publicatii.txt");
    if (!out) return;

    for (const auto& p : app.getPublicatii()) {
        out << p->serializare() << "\n";
    }
}

void LoadService::incarcaPublicatii(AppState &app) {
    std::ifstream in("data/publicatii.txt");
    if (!in) return;

    // Pregatim vectorul de shared_ptr<Autor> pentru deserializare
    std::vector<std::shared_ptr<Autor>> autoriPtrs;
    auto& autoriReali = app.getAutor1(); //
    autoriPtrs.reserve(autoriReali.size());

    for (auto& a : autoriReali) {
        std::shared_ptr<Autor> ptr(&a, [](Autor*){});
        autoriPtrs.push_back(ptr);
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            // Aici apelam Factory-ul centralizat
            if (auto pub = Publicatie::fabricare(line, autoriPtrs)) {
                app.adaugaPublicatie(pub);
            }
        } catch (const DateInvalideException& e) {
             std::cerr << "[Eroare Publicatii] Linie invalida: " << e.what() << "\n";
        } catch (const LibrarieException& e) {
             std::cerr << "[Eroare Publicatii] Problema librarie: " << e.what() << "\n";
        } catch (const std::exception& e) {
             std::cerr << "[Eroare Publicatii] Exceptie: " << e.what() << "\n";
        }
    }
}
// clienti
void LoadService::salveazaClienti(const AppState &app) {
    std::ofstream out("data/clienti.txt");
    if (!out) return;

    for (const auto& c : app.getClient()) {
        out << c.serializare() << "\n";
    }
}

void LoadService::incarcaClienti(AppState& app) {
    std::ifstream in("data/clienti.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            app.adaugaClient(Client::deserializare(line));
        } catch (const DateInvalideException& e) {
             std::cerr << "[Eroare Clienti] Linie invalida: " << e.what() << "\n";
        } catch (const LibrarieException& e) {
             std::cerr << "[Eroare Clienti] Problema librarie: " << e.what() << "\n";
        } catch (const std::exception& e) {
             std::cerr << "[Eroare Clienti] Exceptie: " << e.what() << "\n";
        }
    }
}

// reduceri
void LoadService::salveazaReducerile(const AppState &app) {
    std::ofstream out("data/reduceri.txt");
    if (!out) return;

    for (const auto& r : app.getReducere()) {
        out << r.serializare() << "\n";
    }
}

void LoadService::incarcaReducerile(AppState& app) {
    std::ifstream in("data/reduceri.txt");
    if (!in) return;

    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        try {
            app.adaugaReduceri(Reducere::deserializare(line, app.getPublicatii()));
        } catch (const std::exception& e) {
             std::cerr << "[Eroare Reduceri] " << e.what() << "\n";
        }
    }
}

// Stoc SH
void LoadService::salveazaStocSH(const AppState &app) {
    std::ofstream out("data/stoc_sh.txt");
    if(!out) return;
    for(const auto& u : app.getStocSH()) {
        if(u) out << u->serializare() << "\n";
    }
}

void LoadService::incarcaStocSH(AppState &app) {
    std::ifstream in("data/stoc_sh.txt");
    if(!in) return;
    std::string line;
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        try {
            if(auto u = UnitateVanzare::fabricare(line, app.getPublicatii())) {
                app.adaugaSH(u);
            }
        } catch(const std::exception& e) {
            std::cerr << "[Eroare StocSH] " << e.what() << "\n";
        }
    }
}

// Pachete
void LoadService::salveazaPachete(const AppState &app) {
    std::ofstream out("data/pachete.txt");
    if(!out) return;
    for(const auto& p : app.getPachetePredefinite()) {
        out << p.serializare() << "\n";
    }
}

void LoadService::incarcaPachete(AppState &app) {
    std::ifstream in("data/pachete.txt");
    if(!in) return;
    std::string line;
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        try {
            if(auto p = PachetSerie::dinString(line, app.getPublicatii())) {
                app.adaugaPachet(*p);
            }
        } catch(const std::exception& e) {
             std::cerr << "[Eroare Pachete] " << e.what() << "\n";
        }
    }
}
// comenzi
void LoadService::salveazaComenzi(const AppState &app) {
    std::ofstream out("data/comenzi.txt");
    if(!out) return;
    for(const auto& c : app.getComanda()) {
        out << c.serializare() << "\n";
    }
}

void LoadService::incarcaComenzi(AppState &app) {
    std::ifstream in("data/comenzi.txt");
    if(!in) return;
    std::vector<std::shared_ptr<UnitateVanzare>> totStocul;
    for(const auto& u : app.getStocSH()) totStocul.push_back(u);
    for(const auto& p : app.getPachetePredefinite()) {
        totStocul.push_back(std::make_shared<PachetSerie>(p));
    }
    std::string line;
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        try {
            app.adaugaComanda(Comanda::deserializare(line, app.getClient(), totStocul));
        } catch(const std::exception& e) {
            std::cerr << "[Eroare Comenzi] " << e.what() << "\n";
        }
    }
}
// reviews
void LoadService::salveazaReviews(const AppState &app) {
    std::ofstream out("data/reviews.txt");
    if(!out) return;
    for(const auto& r : app.getReviews()) {
        out << r.serializare() << "\n";
    }
}

void LoadService::incarcaReviews(AppState &app) {
    std::ifstream in("data/reviews.txt");
    if(!in) return;
    std::string line;
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        try {
            app.incarcaReview(Review::deserializare(line));
        } catch(...) {}
    }
}
// logs
void LoadService::salveazaLogs(const AppState &app) {
    std::ofstream out("data/logs.txt");
    if(!out) return;
    for(const auto& log : app.getLogs()) {
        out << log.serializare() << "\n";
    }
}

void LoadService::incarcaLogs(AppState &app) {
    std::ifstream in("data/logs.txt");
    if(!in) return;
    std::string line;
    while(std::getline(in, line)) {
        if(line.empty()) continue;
        try {
            app.incarcaLog(LogEntry::deserializare(line));
        } catch(...) {}
    }
}