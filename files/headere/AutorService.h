

#ifndef OOP_AUTORSERVICE_H
#define OOP_AUTORSERVICE_H
#include <vector>
#include <memory>
#include <string>
#include "../headere/AppState.h"

class Autor;

enum class TipTopAutor{PRODUCTIVITATE,SCOR_RENUME,IERARHIE};

struct AutorStat {
    std::shared_ptr<Autor> autor;
    double productivitate;
    double scor_renume;
    std::string ierarhie;
};
class AutorService {
public:
    static AutorStat getDetaliiAutor(const AppState& app, const std::shared_ptr<Autor>& autor);

    static std::vector<AutorStat> getTopAutori(AppState& app, TipTopAutor tip);

    static void adaugaAutor(AppState& app, const std::shared_ptr<Autor>& autor);
};
#endif //OOP_AUTORSERVICE_H