#include "../exceptii_headere/ComandaExceptions.h"
// clasa de baza
ComandaException::ComandaException(const std::string &mesaj)
    : LibrarieException("Eroare Flux Comanda: " + mesaj)
{
}
// comanda invalida
ComandaInvalidaException::ComandaInvalidaException(const std::string& actiune)
    : ComandaException("Nu se poate efectua '" + actiune ){}
// comnda finalizata
ComandaFinalizataException::ComandaFinalizataException(int id_comanda)
    : ComandaException("Comanda #" + std::to_string(id_comanda) +
                       " este deja FINALIZATA. Nu se mai pot aduce modificari.")
{
}
// comanda anulata
ComandaAnulataException::ComandaAnulataException(int id_comanda)
    : ComandaException("Comanda #" + std::to_string(id_comanda) +
                       " este ANULATA. Operatiune respinsa.")
{
}
// clasa goala
ComandaGoalaFinalizareException::ComandaGoalaFinalizareException() :
ComandaException("Nu se poate finaliza o comanda fara produse (cos gol)!") {
}

ComandaGoalaException::ComandaGoalaException():
     ComandaException("Cosul este gol !"){}

