#include "InsufficientFundsException.h"
InsufficientFundsException::InsufficientFundsException(const char* msg)
    : HospitalException(msg) {}
const char* InsufficientFundsException::what() {
    std::cout << "InsufficientFundsException: " << message << std::endl;
    return message;
}
void InsufficientFundsException::setMessage(const char* msg) {
    HospitalException::setMessage(msg);
}
