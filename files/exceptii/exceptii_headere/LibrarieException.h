

#ifndef OOP_LIBRARIEEXCEPTION_H
#define OOP_LIBRARIEEXCEPTION_H
#include <string>
#include <exception>
#include <stdexcept>

class LibrarieException : public std::runtime_error {
protected:
    std::string mesaj;
public:
    // constructor
    explicit LibrarieException(const std::string& mesaj);

    // destrcutor virtual
    ~LibrarieException() override = default;

    const char* what() const noexcept override;
};

#endif //OOP_LIBRARIEEXCEPTION_H