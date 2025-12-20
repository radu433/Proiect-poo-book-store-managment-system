#include "../headere/Reducere.h"

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