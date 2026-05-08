#include "HospitalException.h"

const char* HospitalException::what() {
    std::cout << "HospitalException: " << message;
    return message;
}
void HospitalException::setMessage(const char* msg) {
    int length = getLength(msg);
    if(length >= 199) length = 199;
    copyarray(message, msg, length);
}
