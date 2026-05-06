#include "InsufficientFundsException.h"

InsufficientFundsException::InsufficientFundsException(const char* msg)
    : HospitalException("Insufficient funds") {
    setMessage(msg);
}
const char* InsufficientFundsException::what() {
    HospitalException::what();
    std::cout<<std::endl;
    return message;
}
void InsufficientFundsException::setMessage(const char* msg) {
    int length = getLength(msg);
    if(length >= 200) {
        length = 199; // Truncate if message is too long
    }
    copyarray(message, msg, length);
}
