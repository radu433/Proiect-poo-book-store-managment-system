

#ifndef OOP_AUTHENTICATIONSERVICE_H
#define OOP_AUTHENTICATIONSERVICE_H
#include "adresa.h"
#include "AppState.h"
class AuthenticatorService {
public:
    //cauta dupa email username telefon
    static const Client* cautaClientIdentificator(const AppState& app,const std::string &input);

    //verifica daca exista
    static bool existaClient(const AppState& app,const std::string& username,const std::string& email,const std::string& telefon);

    //creaza client
    static const Client* creeazaClient(AppState &app, const std::string &username, const std::string &email,
                                                 const Adresa &adresa, const std::string &telefon, std::string &parola);

    // autentificare
    static const Client* autentifica(AppState& app,const std::string& identificator,const std::string& parola);
};
#endif //OOP_AUTHENTICATIONSERVICE_H