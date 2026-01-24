#include "files/headere/AppState.h"
#include "files/headere/Book_Store_manager.h"
#include "files/headere/LoadService.h"
#include <iostream>

int main() {
    try {
        AppState app;
        LoadService::incarca(app);
        BookStoreManager manager(app);
        manager.run();
        LoadService::salveaza(app);
    }catch (const std::exception& e) {
        std::cout<<"Eroare critica: "<< e.what()<<std::endl;
    }
    return 0;
}