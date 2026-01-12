//
// Created by sindicat on 1/9/2026.
//

#ifndef OOP_UNITATEVANZAREFACTORY_H
#define OOP_UNITATEVANZAREFACTORY_H
#include <memory>

#include "CarteIndividuala.h"
#include "CarteStiintifica.h"
#include "Manual.h"
#include "RevistaIndividuala.h"
#include "UnitateVanzare.h"

class UnitateVanzareFactory {
    public:
    static std::shared_ptr<UnitateVanzare> creeaza(const std::shared_ptr<Publicatie>& pub);

};

inline std::shared_ptr<UnitateVanzare> UnitateVanzareFactory::creeaza(const std::shared_ptr<Publicatie> &pub) {
    switch (pub->getTipPub()) {
        case TipPublicatie::Revista: {
            auto revista = std::dynamic_pointer_cast<Revista>(pub);
            if (!revista)
                throw DateInvalideException("Revista invalida");
            return std::make_shared<RevistaIndividuala>(revista);
        }
        case TipPublicatie::Carte: {
            auto carte =std::dynamic_pointer_cast<Carte>(pub);
            if (!carte)
                throw DateInvalideException("Carte invalida");
            return std::make_shared<CarteIndividuala>(carte);
        }
        case TipPublicatie::Manual: {
            auto manual=std::dynamic_pointer_cast<Manual>(pub);
            if (!manual)
                throw DateInvalideException("Manual invalida");
            return std::make_shared<CarteIndividuala>(manual);
        }
        case TipPublicatie::Cartestiintifica: {
            auto carte=std::dynamic_pointer_cast<CarteStiintifica>(pub);
            if (!carte)
                throw DateInvalideException("CarteStiintifica invalida");
            return std::make_shared<CarteIndividuala>(carte);
        }

            default: {
            throw DateInvalideException("Tip publicatie neacceptat");
        }
    }
}
#endif //OOP_UNITATEVANZAREFACTORY_H
