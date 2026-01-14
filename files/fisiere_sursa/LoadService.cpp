// #include "../headere/LoadService.h"
// #include "../headere/Carte.h"
// #include "../headere/Manual.h"
// #include "../headere/CarteStiintifica.h"
// #include "../headere/Revista.h"
// #include "../headere/Client.h"
// #include "../headere/Comanda.h"
//
// #include <fstream>
// #include <ranges>
// #include <sstream>
//
// #include "../headere/AutorService.h"
// // healper pt factory
// static std::vector<std::string> split (const std::string& s, const char delim) {
//     std::vector<std::string> elems;
//     std::stringstream ss(s);
//     std::string item;
//     while (std::getline(ss, item, delim)) {
//         elems.push_back(item);
//     }
//     return elems;
//
// }
// std::string join (const std::vector<std::string>& v, char delim) {
//     std::ostringstream oss;
//     for (size_t i = 0; i < v.size(); ++i) {
//         oss<<v[i];
//         if (i+1<v.size())
//             oss<<delim;
//     }
//     return oss.str();
// }
// std::string Client::serialize() const {
//     std::ostringstream out;
//     out << email << "|"
//         << username << "|"
//         << sold;
//     return out.str();
// }
// Client Client::deserialize(const std::string& line) {
//     auto v = split(line, '|');
//     Client c();
//     c->setSold(std::stod(v[2]));
//     return c;
// }
// void LoadService::incarca(AppState &app) {
//     incarcaAutori(app);
//     incarcaPublicatii(app);
//     incarcaClienti(app);
//     incarcaStocSH(app);
//     incarcaPachete(app);
//     incarcaComenzi(app);
//     incarcaReducerile(app);
//     incarcaReviews(app);
//     incarcaLogs(app);
// }
// void LoadService::salveaza( const AppState &app) {
//     salveazaAutori(app);
//     salveazaPublicatii(app);
//     salveazaClienti(app);
//     salveazaStocSH(app);
//     salveazaPachete(app);
//     salveazaComenzi(app);
//     salveazaReducerile(app);
//     salveazaReviews(app);
//     salveazaLogs(app);
// }
//
// // autori
// void LoadService::salveazaAutori(const AppState &app) {
//     std::ofstream out("data/autori.txt");
//     if (!out)
//         return;
//     for (const auto& autor:app.getAutor()) {
//         out << autor.getidAutor() << "|"
//             << autor.getNume() << "|"
//             << autor.getprenume() << "|"
//             << autor.getvarsta() << "|";
//
//         const auto& isbnuri = autor.getCartiScrise();
//         for (size_t i = 0; i < isbnuri.size(); ++i) {
//             out << isbnuri[i];
//             if (i + 1 < isbnuri.size())
//                 out << ",";
//         }
//         out << "\n";
//     }
// }
//
// void LoadService::incarcaAutori(AppState &app) {
//     std::ifstream in("data/autori.txt");
//     if (!in) return;
//
//     std::string line;
//     while (std::getline(in, line)) {
//         std::stringstream ss(line);
//
//         std::string id_s, nume, prenume, varsta_s, isbnuri_s;
//         std::getline(ss, id_s, '|');
//         std::getline(ss, nume, '|');
//         std::getline(ss, prenume, '|');
//         std::getline(ss, varsta_s, '|');
//         std::getline(ss, isbnuri_s);
//
//         int id = std::stoi(id_s);
//         int varsta = std::stoi(varsta_s);
//
//         auto autor = std::make_shared<Autor>(id, nume, prenume, varsta);
//
//         std::stringstream ss_isbn(isbnuri_s);
//         std::string isbn;
//         while (std::getline(ss_isbn, isbn, ',')) {
//             if (!isbn.empty())
//                 autor->adauga_carte(isbn);
//         }
//
//         app.autor.push_back(autor);
//     }
// }
// // publicatii
// void LoadService::salveazaPublicatii(const AppState &app) {
//     std::ofstream out("data/publicatii.txt");
//     if (!out) return;
//
//     for (const auto& p : app.getPublicatii()) {
//         out << p->getTip() << "|"
//             << p->getIdentificator() << "|"
//             << p->getTitlu() << "|"
//             << p->getpretbaza() << "|"
//             << p->getcantitate() << "|"
//             << p->getdata_publicatie() << "|"
//             << p->getnr_pagini() << "|"
//             << p->geteditura();
//
//         // de tip carte
//         if (p->getTip() != "Revista") {
//             auto& c = dynamic_cast<const Carte&>(*p);
//             out << "|" << c.getAutor()->getidAutor();
//         }
//
//        //manual
//         if (p->getTip() == "Manual") {
//             auto& m = dynamic_cast<const Manual&>(*p);
//             out << "|" << m.getmaterie()
//                 << "|" << m.getclasa();
//         }
//
//         // carte s
//         if (p->getTip() == "Carte_stiintifica") {
//             auto& s = dynamic_cast<const CarteStiintifica&>(*p);
//             out << "|" << s.getDomeniu()
//                 << "|" << s.getTip()
//                 << "|" << s.getNr_referinte()
//                 << "|" << s.areFormule();
//         }
//
//         // revista
//         if (p->getTip() == "Revista") {
//             auto& r = dynamic_cast<const Revista&>(*p);
//             out << "|" << r.getFrecventa()
//                 << "|" << r.getNrE()
//                 << "|" << r.areC()
//                 << "|" << r.getTip();
//         }
//
//         out << "|" << (p->esteActiva() ? 1 : 0);
//
//         out << "\n";
//     }
// }
// static std::shared_ptr<Publicatie> creeazaPubL (const std::string& line,const AppState& app) {
//     auto v = split(line, '|');
//     const std::string& tip = v[0];
//
//
//     auto cautaAutor = [&](const int id) {
//         for (auto& a : app.getAutor())
//             if (a.getidAutor() == id)
//                 return a;
//         throw std::runtime_error("Autor inexistent!");
//     };
//     if (tip == "Carte") {
//         return std::make_shared<Carte>(v[2], cautaAutor(std::stoi(v[8])),std::stoi(v[4]), v[5],v[1], std::stod(v[3]),
//             std::stoi(v[6]), v[7]);
//     }
//
//     if (tip == "Manual") {
//         return std::make_shared<Manual>(
//             v[2],cautaAutor(std::stoi(v[8])),std::stod(v[3]), std::stoi(v[4]), v[5],v[1], std::stoi(v[6]), v[7],
//             v[9], std::stoi(v[10]));
//     }
//
//     if (tip == "Carte_stiintifica") {
//         return std::make_shared<CarteStiintifica>(v[2], cautaAutor(std::stoi(v[8])),std::stoi(v[4]), v[5], v[1],
//             std::stod(v[3]), std::stoi(v[6]), v[7],v[9], v[10],
//             std::stoi(v[11]), v[12] == "1");
//     }
//
//     if (tip == "Revista") {
//         return std::make_shared<Revista>(v[2], std::stoi(v[4]), v[5], std::stoi(v[6]),std::stod(v[3]), v[8], std::stoi(v[9]),
//             v[10] == "1", v[11], v[1], v[7]);
//     }
//
//     throw std::runtime_error("Tip publicatie necunoscut!");
// }
//
// void LoadService::incarcaPublicatii(AppState &app) {
//     std::ifstream in("data/publicatii.txt");
//     if (!in) return;
//
//     std::string line;
//     while (std::getline(in, line)) {
//         auto pub = creeazaPubL(line, app);
//         app.getPublicatii().push_back(pub);
//     }
// }
// // clienti
// void LoadService::salveazaClienti(const AppState &app) {
//     std::ofstream out("data/clienti.txt");
//     if (!out) return;
//
//     for (const auto& c : app.clienti) {
//         out << c.getEmail() << "|"
//             << c.getUsername() << "|"
//             << c.getTelefon() << "|"
//             << c.getP() << "|"
//             << c.getSold() << "|"
//             << c.getNumarComenzi() << "|"
//             << c.getTotalCumparaturi() << "|"
//             << c.getPunctedeFideliate() << "|"
//             << c.getReducereLaUrm() << "|"
//             << c.getAdresa().serialize() << "|"
//             << join(c.getIstoricIdentificatori(), ',')
//             << "\n";
//     }
// }
// void LoadService::incarcaClienti(AppState& app) {
//     std::ifstream in("data/clienti.txt");
//     if (!in) return;
//
//     std::string line;
//     while (std::getline(in, line)) {
//         auto v = split(line, '|');
//
//         auto client = std::make_shared<Client>(
//             v[1],                 // username
//             v[0],                 // email
//             Adresa::deserialize(v[9]),
//             v[2],                 // telefon
//             v[3]                  // parola
//         );
//
//         client->setSold(std::stod(v[4]));
//         client->setNumarComenzi(std::stoi(v[5]));
//         client->setTotalCumparaturi(std::stod(v[6]));
//         client->setPctFidelitate(std::stoi(v[7]));
//         client->setReducereLaUrm(std::stod(v[8]));
//
//         // istoric identificatori
//         auto ids = split(v[10], ',');
//         for (const auto& id : ids)
//             if (!id.empty())
//                 client->adaugaIdentificatorIstoric(id);
//
//         app.clienti.push_back(*client);
//     }
// }
//
//
