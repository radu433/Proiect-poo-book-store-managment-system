#include "../headere/SerchService.h"
#include "../headere/Comanda.h"
#include "../headere/Client.h"
#include "../headere/Manual.h"
#include "../headere/CarteStiintifica.h"
#include "../headere/Revista.h"
// publicatii
std::vector<int> SerchService::filtreazaPublicatiiDupaTip(
    const std::vector<std::shared_ptr<Publicatie>>& publicatii,
    const int tip
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(publicatii.size()); ++i) {
        const auto& p = publicatii[i];
        if (!p) continue;

        if (tip == 1 && std::dynamic_pointer_cast<Carte>(p) &&
            !std::dynamic_pointer_cast<Manual>(p) &&
            !std::dynamic_pointer_cast<CarteStiintifica>(p))
            rez.push_back(i);

        if (tip == 2 && std::dynamic_pointer_cast<Manual>(p))
            rez.push_back(i);

        if (tip == 3 && std::dynamic_pointer_cast<CarteStiintifica>(p))
            rez.push_back(i);

        if (tip == 4 && std::dynamic_pointer_cast<Revista>(p))
            rez.push_back(i);
    }
    return rez;
}

std::vector<int> SerchService::filtreazaPublicatiiDupaAutor(
    const std::vector<std::shared_ptr<Publicatie>>& publicatii,
    const std::string& nume
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(publicatii.size()); ++i) {
        if (const auto c = std::dynamic_pointer_cast<Carte>(publicatii[i])) {
            if (const auto& a = c->getAutor(); a.getNume().find(nume) != std::string::npos ||
                                               a.getprenume().find(nume) != std::string::npos)
                rez.push_back(i);
        }
    }
    return rez;
}
// autor
std::vector<int> SerchService::filtreazaAutoriNume(
    const std::vector<std::shared_ptr<Autor>>& autori,
    const std::string& nume
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(autori.size()); ++i) {
        if (autori[i]->getNume().find(nume) != std::string::npos ||
            autori[i]->getprenume().find(nume) != std::string::npos)
            rez.push_back(i);
    }
    return rez;
}
// clienti
std::vector<int> SerchService::filtreazaClientiUsername(
    const std::vector<std::shared_ptr<Client>>& clienti,
    const std::string& username
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(clienti.size()); ++i) {
        if (clienti[i]->getUsername().find(username) != std::string::npos)
            rez.push_back(i);
    }
    return rez;
}

std::vector<int> SerchService::filtreazaClientiEmail(
    const std::vector<std::shared_ptr<Client>>& clienti,
    const std::string& email
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(clienti.size()); ++i) {
        if (clienti[i]->getEmail().find(email) != std::string::npos)
            rez.push_back(i);
    }
    return rez;
}
// comenzi
std::vector<int> SerchService::toateComenzile(
    const std::vector<std::shared_ptr<Comanda>>& comenzi
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(comenzi.size()); ++i)
        rez.push_back(i);
    return rez;
}

std::vector<int> SerchService::comenziDupaClient(
    const std::vector<std::shared_ptr<Comanda>>& comenzi,
    const std::shared_ptr<Client>& client
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(comenzi.size()); ++i) {
        if (comenzi[i] && comenzi[i]->getClient() == client)
            rez.push_back(i);
    }
    return rez;
}

std::vector<int> SerchService::comenziDupaStare(
    const std::vector<std::shared_ptr<Comanda>>& comenzi,
    const std::string& stare
) {
    std::vector<int> rez;
    for (int i = 0; i < static_cast<int>(comenzi.size()); ++i) {
        if (comenzi[i] && comenzi[i]->getStare() == stare)
            rez.push_back(i);
    }
    return rez;
}