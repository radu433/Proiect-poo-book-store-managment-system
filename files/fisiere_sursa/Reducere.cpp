#include "../headere/Reducere.h"

#include <sstream>
#include <vector>

Reducere::Reducere( std::shared_ptr<Publicatie> pub,
                    int procent,
                    std::time_t start,
                     std::time_t sfarsit)
    : publicatie(std::move(pub)), procent(procent), start(start), sfarsit(sfarsit) {}

bool Reducere::esteActiva() const {
    const std::time_t now = std::time(nullptr);
    return now >= start && now <= sfarsit;
}

bool Reducere::seAplicaLa(const std::shared_ptr<Publicatie>& p) const {
    return p == publicatie && esteActiva();
}

int Reducere::getProcent() const {
    return procent;
}

std::shared_ptr<Publicatie> Reducere::getPublicatie() const {
    return publicatie;
}

std::string Reducere::serializare() const {
    std::stringstream ss;
    ss << publicatie->getIdentificator() << "|" << procent << "|" << start << "|" << sfarsit;
    return ss.str();
}
Reducere Reducere::deserializare(const std::string& line, const std::vector<std::shared_ptr<Publicatie>>& publicatii) {
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> v;
    while (std::getline(ss, item, '|')) {
        v.push_back(item);
    }

    if (v.size() < 4) {
        throw std::runtime_error("Linie invalida pentru deserializare Reducere");
    }

    const std::string id_pub = v[0];
    const int proc = std::stoi(v[1]);
    const std::time_t st = std::stoll(v[2]);
    const std::time_t sf = std::stoll(v[3]);
    std::shared_ptr<Publicatie> p_gasit = nullptr;
    for (const auto& p : publicatii) {
        if (p->getIdentificator() == id_pub) {
            p_gasit = p;
            break;
        }
    }
    if (!p_gasit) {
        throw std::runtime_error("Publicatia " + id_pub + " nu a fost gasita pentru reducere!");
    }
    return Reducere(p_gasit, proc, st, sf);
}
