#include "../headere/AuthenticationService.h"
#include "../headere/Client.h"
#include "../headere/adresa.h"
#include <ctime>
#include <stdexcept>

const Client* AuthenticatorService::cautaClientIdentificator(const AppState &app, const std::string& input) {
    for (const auto& c : app.getClient()) {
        if (c.getEmail() == input ||
            c.getTelefon() == input ||
            c.getUsername() == input) {
            return &c;
            }
    }
    return nullptr;
}

bool AuthenticatorService::existaClient(const AppState &app, const std::string& username, const std::string& email,
    const std::string& telefon) {
    return cautaClientIdentificator(app, username) ||
           cautaClientIdentificator(app, email) ||
           cautaClientIdentificator(app, telefon);
}

const Client* AuthenticatorService::creeazaClient(AppState &app, const std::string &username,
                                                            const std::string &email, const Adresa &adresa, const std::string &telefon, std::string &parola) {
    if (existaClient(app, username, email, telefon)) {
        throw std::runtime_error("Exista deja un client cu aceste date!");
    }

    const Client client(username,email,adresa,telefon,parola);
    client.seteazaParola(parola);
    app.adaugaClient(client);
    app.adaugaLogs(Tiplog::CONT_CREAT,email,username,"Cont client creat");

    return &app.getClient().back();
}

const Client* AuthenticatorService::autentifica(AppState &app, const std::string &identificator,
    const std::string &parola) {
    const auto client = cautaClientIdentificator(app, identificator);
    if (!client)
        return nullptr;

    if (client->verificaParola(parola)) {
        return client;
    }

    app.adaugaLogs(Tiplog::AUTENTIFICARE_ESUATA,client->getEmail(),client->getUsername(),"Parola incorecta");

    return nullptr;
}
