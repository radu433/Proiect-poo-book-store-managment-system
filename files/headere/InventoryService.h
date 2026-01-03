
#ifndef OOP_INVENTORYSERVICE_H
#define OOP_INVENTORYSERVICE_H
#include <vector>
#include <map>
#include <memory>
#include "../headere/SerchService.h"
#include "../headere/AppState.h"

class Publicatie;
struct PrioritateRestoc {
    std::shared_ptr<Publicatie> pub;
    double scor;
};
class InventoryService {
public:
  static void adaugaStoc(Publicatie& p, int cantitate);
  static void scadeStoc (Publicatie& p, int cantitate);

    static std::vector <PrioritateRestoc> calculeazaPrioritateRestoc(const AppState& app);
};
#endif //OOP_INVENTORYSERVICE_H