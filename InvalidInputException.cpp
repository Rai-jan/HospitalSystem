#include "InvalidInputException.h"
InvalidInputException::InvalidInputException(const char* msg)
    : HospitalException(msg) {}
const char* InvalidInputException::what() {
    std::cout << "InvalidInputException: " << message << std::endl;
    return message;
}
void InvalidInputException::setMessage(const char* msg) {
    HospitalException::setMessage(msg);
}
