
#ifndef OOP_INVENTORYSERVICE_H
#define OOP_INVENTORYSERVICE_H
#include <vector>
#include <map>
#include <memory>
#include "../headere/SerchService.h"
#include "../headere/AppState.h"

class Publicatie;
class InventoryService {
public:
    void modificaStocPublicatie(AppState& app, int idx);
    // functii pt stoc
    void modificaStocPublicatie(const AppState &app, int idx);
    void restocheazaDinPrioritati(const AppState &app);
};
#endif //OOP_INVENTORYSERVICE_H