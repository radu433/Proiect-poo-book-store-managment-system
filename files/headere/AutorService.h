

#ifndef OOP_AUTORSERVICE_H
#define OOP_AUTORSERVICE_H
#include <vector>
#include <memory>
#include <string>
#include "../headere/AppState.h"

class Autor;

enum class TipTopAutor{PRODUCTIVITATE,SCOR_RENUME,IERARHIE};

struct AutorStat {
    const Autor* autor;
    double productivitate;
    double scor_renume;
    std::string ierarhie;
};
class AutorService {
public:
    static AutorStat getDetaliiAutor(const AppState& app, const Autor* autor);

    static std::vector<AutorStat> getTopAutori(const AppState& app,  TipTopAutor tip);

    static void adaugaAutor(AppState& app, const Autor* autor);

    static void asociazaCarte( AppState& app, const std::string& isbn, const std::string& numeA, const std::string& prenumeA);
};
#endif //OOP_AUTORSERVICE_H