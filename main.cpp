#include "files/headere/AppState.h"
#include "files/headere/Book_Store_manager.h"
#include <iostream>

int main() {
    try {
        AppState app;
        BookStoreManager manager(app);
        manager.run();
    }catch (const std::exception& e) {
        std::cout<<"Eroare critica: "<< e.what()<<std::endl;
    }
    return 0;
}