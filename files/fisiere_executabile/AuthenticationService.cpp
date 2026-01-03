#include "../headere/AuthenticationService.h"
#include "../headere/Client.h"
#include "../headere/adresa.h"
#include <ctime>
#include <stdexcept>

std::shared_ptr<Client> AuthenticatorService::cautaClientIdentificator(const AppState &app, const std::string& input) {
    for (const auto& c : app.clienti) {
        if (!c) continue;

        if (c->getEmail() == input ||
            c->getTelefon() == input ||
            c->getUsername() == input) {
            return c;
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

std::shared_ptr<Client> AuthenticatorService::creeazaClient(AppState &app, const std::string &username,
                                                            const std::string &email, const Adresa &adresa, const std::string &telefon, std::string &parola) {
    if (existaClient(app, username, email, telefon)) {
        throw std::runtime_error("Exista deja un client cu aceste date!");
    }

    auto client = std::make_shared<Client>(
        username, email, adresa, telefon, parola
    );

    client->seteazaParola(parola);

    app.clienti.push_back(client);

    app.logs.push_back({time(nullptr),Tiplog::CONT_CREAT,email,"Cont client creat",username});

    return client;
}

std::shared_ptr<Client> AuthenticatorService::autentifica(AppState &app, const std::string &identificator,
    const std::string &parola) {
    auto client = cautaClientIdentificator(app, identificator);
    if (!client)
        return nullptr;

    if (client->verificaParola(parola)) {
        return client;
    }

    app.logs.push_back({time(nullptr),Tiplog::AUTENTIFICARE_ESUATA,identificator,"Parola incorecta",
        client->getUsername()});

    return nullptr;
}
