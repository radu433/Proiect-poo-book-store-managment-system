#include "../exceptii_headere/ClientExceptions.h"
// constructor clasa de baza
ClientException::ClientException(const std::string &msg):LibrarieException("Eroare Client: " + msg) {
}

//client invalid
ClientInvalidException::ClientInvalidException(const std::string &detalii): ClientException("Date intrare: " + detalii) {

}
// sold induficient
SoldInsuficientException::SoldInsuficientException(double sold_curent, double total_necesar):
ClientException("Fonduri insuficiente! Sold curent: " + std::to_string(sold_curent) +
                      " RON, Total necesar: " + std::to_string(total_necesar) + " RON") {
}





